"""Replay each step of the practical session and run what its CI runs.

The tests are named after the step they replay, so that `pytest -k step3`
replays a single step. Steps 1 to 5 install the locked environment of their
solution: they need a platform listed in the lockfiles (linux-64 in the CI).
"""

from pathlib import Path

from conftest import bash, git_commit, git_init, overlay, run, step_dir

# The commands of the `cppcheck` job, as given in the pre-commit chapter.
CPPCHECK = """
cmake -S . -B .build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_EXAMPLES=ON
cppcheck --enable=missingInclude,unusedFunction,warning,style,portability,performance -q --project=.build/compile_commands.json --suppressions-list=.cppcheck 2> cppcheck_err.txt
if [[ -s cppcheck_err.txt ]]; then
    cat cppcheck_err.txt
    exit 1
fi
"""

# The commands of the `clang-tidy-check` job, as given in the long-work chapter.
CLANG_TIDY = """
cmake . -Bbuild -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DBUILD_EXAMPLES=ON
compdb -p build list > compile_commands.json
run-clang-tidy -extra-arg=-isystem$CONDA_PREFIX/include
"""


def check_solution_ci(project: Path, branch: str = "replay") -> None:
    """Run the jobs of the `Run CI` workflow of steps 3 to 5 on `project`.

    The tests run before cppcheck, in the order of the learner: from step 1,
    `pixi run test` creates the `.build` directory that cppcheck reuses.
    """
    if not (project / ".git").exists():
        git_init(project, branch)
    run(["pixi", "install", "--locked"], project)
    run(["pixi", "run", "--locked", "pre-commit", "run", "--all-files", "--show-diff-on-failure"], project)
    run(["pixi", "run", "--locked", "test"], project)
    bash(CPPCHECK, project)


def test_step1_pixi(project: Path) -> None:
    """The pixi workspace of step 1 builds the project and its tests pass."""
    overlay(step_dir("0.initialization/required_files"), project)
    overlay(step_dir("1.pixi/solution"), project)

    run(["pixi", "install", "--locked"], project)
    run(["pixi", "run", "--locked", "test"], project)
    run(["pixi", "run", "--locked", "build"], project)


def test_step3_learner(project: Path) -> None:
    """pre-commit cleans the messy code of step 0, which then passes the CI of step 3."""
    solution = step_dir("3.pre-commit/solution")
    overlay(step_dir("0.initialization/required_files"), project)
    overlay(step_dir("1.pixi/solution"), project)
    overlay(step_dir("2.first-action/solution"), project)
    # The chapter asks to work on the `add-pre-commit` branch: the
    # `no-commit-to-branch` hook fails on `main`.
    git_init(project, "add-pre-commit")
    overlay(step_dir("3.pre-commit/required_files"), project)
    # Files the learner writes by hand during the step.
    for name in (".pre-commit-config.yaml", "pixi.toml", "pixi.lock", ".github/workflows/ci.yml"):
        (project / name).write_bytes((solution / name).read_bytes())
    git_commit(project, "add pre-commit")

    pre_commit = ["pixi", "run", "--locked", "pre-commit", "run", "--all-files"]
    first = run(pre_commit, project, check=False)
    assert first.returncode != 0, "pre-commit should reformat the messy code of step 0"
    git_commit(project, "format the code")

    # The chapter warns that cppcheck first finds an unused variable.
    run(["pixi", "install", "--locked"], project)
    run(pre_commit + ["--show-diff-on-failure"], project)
    run(["pixi", "run", "--locked", "test"], project)
    result = bash(CPPCHECK, project, check=False)
    assert result.returncode != 0, "cppcheck should find the unused variable of step 0"
    assert "Variable 'unused' is assigned a value that is never used" in result.stdout

    spline = project / "include" / "splinart" / "spline.hpp"
    lines = spline.read_text().splitlines(keepends=True)
    kept = [line for line in lines if line.strip() != "auto unused = 4;"]
    assert len(kept) == len(lines) - 1, "spline.hpp should declare `unused` once"
    spline.write_text("".join(kept))
    # The pre-commit hook realigns the assignments around the removed line.
    run(pre_commit, project, check=False)
    git_commit(project, "remove the unused variable")
    check_solution_ci(project)


def test_step3_solution(project: Path) -> None:
    """The CI of the step 3 solution passes."""
    overlay(step_dir("3.pre-commit/solution"), project)
    check_solution_ci(project)


def test_step4_solution(project: Path) -> None:
    """The CI of the step 4 solution passes and the documentation builds."""
    overlay(step_dir("4.documentation/solution"), project)
    check_solution_ci(project)

    run(["pixi", "run", "--locked", "doc"], project)
    html = project / "doc" / "build" / "html"
    for page in ("index.html", "api/index.html", "spline.html"):
        assert (html / page).is_file(), f"the documentation has no {page}"


def test_step5_learner_clang_tidy(project: Path) -> None:
    """clang-tidy rejects the step 4 code, as the long-work chapter warns."""
    solution = step_dir("5.long-run/solution")
    overlay(step_dir("4.documentation/solution"), project)
    overlay(step_dir("5.long-run/required_files"), project)
    for name in ("pixi.toml", "pixi.lock"):
        (project / name).write_bytes((solution / name).read_bytes())

    run(["pixi", "install", "--locked", "-e", "lint"], project)
    result = run(["pixi", "run", "--locked", "-e", "lint", "bash", "-eo", "pipefail", "-c", CLANG_TIDY], project, check=False)
    assert result.returncode != 0, "clang-tidy should reject the step 4 code"
    assert "drawPiXel" in result.stdout, "the chapter quotes `drawPiXel` as an example of error"


def test_step5_solution(project: Path) -> None:
    """The CI and the clang-tidy job of the step 5 solution pass."""
    overlay(step_dir("5.long-run/solution"), project)
    check_solution_ci(project)

    run(["pixi", "install", "--locked", "-e", "lint"], project)
    run(["pixi", "run", "--locked", "-e", "lint", "bash", "-eo", "pipefail", "-c", CLANG_TIDY], project)


def test_step6_package(project: Path, tmp_path: Path) -> None:
    """The recipe of step 6 builds a package that installs and runs."""
    overlay(step_dir("5.long-run/solution"), project)
    overlay(step_dir("6.packaging/required_files"), project)
    (project / "version.txt").write_text("0.0.1\n")

    run(
        ["pixi", "exec", "rattler-build", "build", "--recipe", "recipe", "--output-dir", "output"],
        project,
        env={"VERSION": "0.0.1"},
    )
    packages = list((project / "output").glob("*/splinart-cpp-0.0.1-*.conda"))
    assert len(packages) == 1, f"expected one splinart-cpp 0.0.1 package, found {packages}"

    # Install it in a new workspace, as the packaging chapter does.
    run(["pixi", "init", "test-splinart"], tmp_path)
    user = tmp_path / "test-splinart"
    run(["pixi", "workspace", "channel", "add", "--prepend", (project / "output").as_uri()], user)
    run(["pixi", "add", "splinart-cpp"], user)
    run(["pixi", "run", "splinart-cpp", "--help"], user)
