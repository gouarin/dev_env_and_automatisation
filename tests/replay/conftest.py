"""Helpers to replay the practical session the way a learner does it.

Each step starts from the files a learner has at that point: the files given
in `material/<step>/required_files` and the solution of the previous steps.
Commands run in a scratch directory, with a clean environment, so that the
pixi workspace of the course does not leak into the learner project.
"""

import os
import shutil
import subprocess
from pathlib import Path

import pytest

COURSE = Path(__file__).resolve().parents[2]
MATERIAL = COURSE / "material"


def step_dir(name: str) -> Path:
    """Return `material/<name>`, e.g. `step_dir("3.pre-commit")`."""
    path = MATERIAL / name
    assert path.is_dir(), f"{path} does not exist"
    return path


def overlay(src: Path, dst: Path) -> None:
    """Copy the tree `src` into `dst`, replacing files that already exist."""
    shutil.copytree(src, dst, dirs_exist_ok=True)


def learner_env() -> dict[str, str]:
    """Return the environment without the variables of the course pixi workspace."""
    env = {
        key: value
        for key, value in os.environ.items()
        if not key.startswith(("PIXI_", "CONDA_", "INIT_CWD"))
    }
    prefix = os.environ.get("CONDA_PREFIX")
    if prefix:
        env["PATH"] = os.pathsep.join(
            entry for entry in env["PATH"].split(os.pathsep) if not entry.startswith(prefix)
        )
    return env


def run(
    cmd: list[str],
    cwd: Path,
    *,
    check: bool = True,
    env: dict[str, str] | None = None,
    course_env: bool = False,
) -> subprocess.CompletedProcess[str]:
    """Run `cmd` in `cwd` and fail the test with its output if `check` is set.

    The command runs outside the pixi environment of the course, unless
    `course_env` is set to use its tools (actionlint, shellcheck).
    """
    full_env = dict(os.environ) if course_env else learner_env()
    if env:
        full_env.update(env)
    print(f"$ {' '.join(cmd)}  (in {cwd})", flush=True)
    result = subprocess.run(
        cmd,
        cwd=cwd,
        env=full_env,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        check=False,
    )
    print(result.stdout, flush=True)
    if check and result.returncode != 0:
        pytest.fail(
            f"`{' '.join(cmd)}` exited with {result.returncode}:\n{result.stdout[-5000:]}",
            pytrace=False,
        )
    return result


def bash(script: str, cwd: Path, **kwargs) -> subprocess.CompletedProcess[str]:
    """Run a bash script in the default pixi environment, like `shell: pixi run bash {0}`."""
    return run(["pixi", "run", "--locked", "bash", "-eo", "pipefail", "-c", script], cwd, **kwargs)


def git_commit(path: Path, message: str) -> None:
    """Commit every file of `path`."""
    run(["git", "add", "-A"], path)
    identity = ["-c", "user.name=replay", "-c", "user.email=replay@example.invalid"]
    run(["git", *identity, "commit", "-q", "--no-verify", "--allow-empty", "-m", message], path)


def git_init(path: Path, branch: str) -> None:
    """Make `path` a git repository whose files are committed on `branch`.

    pre-commit only checks files known to git, and some hooks
    (`check-added-large-files`) look at the files staged for the next commit:
    committing first reproduces a checkout, as in the CI.
    """
    run(["git", "init", "-q", "-b", "main"], path)
    run(["git", "checkout", "-q", "-b", branch], path)
    git_commit(path, "replay")


def tree_files(root: Path) -> dict[str, bytes]:
    """Return the content of every file under `root`, keyed by its relative path."""
    ignored = {".git", ".pixi", ".build", "build"}
    files = {}
    for path in sorted(root.rglob("*")):
        relative = path.relative_to(root)
        if path.is_file() and not ignored & set(relative.parts):
            files[relative.as_posix()] = path.read_bytes()
    return files


@pytest.fixture
def project(tmp_path: Path) -> Path:
    """An empty directory for the learner project."""
    path = tmp_path / "splinart-cpp"
    path.mkdir()
    return path
