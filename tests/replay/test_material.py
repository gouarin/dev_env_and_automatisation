"""Fast checks on the files of `material/`: no environment to install."""

import pytest

from conftest import COURSE, MATERIAL, run, step_dir, tree_files

WORKFLOWS = sorted(MATERIAL.glob("**/.github/**/*")) + sorted((COURSE / ".github").glob("**/*"))


@pytest.mark.parametrize("step", ["3.pre-commit", "4.documentation", "5.long-run"])
def test_material_required_files_in_solution(step: str) -> None:
    """The files given to the learner are also in the solution of the step."""
    required = tree_files(step_dir(step) / "required_files")
    solution = tree_files(step_dir(step) / "solution")
    missing = sorted(path for path in required if path not in solution)
    assert not missing, f"missing from the solution: {missing}"
    different = sorted(path for path in required if required[path] != solution[path])
    assert not different, f"different in the solution: {different}"


def test_material_workflows_location() -> None:
    """GitHub only runs workflows stored in `.github/workflows/`."""
    misplaced = [
        path.relative_to(COURSE).as_posix()
        for path in WORKFLOWS
        if path.is_file() and (path.parent.name != "workflows" or path.suffix not in {".yml", ".yaml"})
    ]
    assert not misplaced, f"not in .github/workflows/: {misplaced}"


def test_material_actionlint() -> None:
    """actionlint, with shellcheck on the `run` scripts, accepts every workflow."""
    files = [str(path) for path in WORKFLOWS if path.is_file()]
    run(["actionlint", "-no-color", *files], COURSE, course_env=True)


def test_material_not_copied_in_chapters() -> None:
    """The chapters show the files of `material/` with {literalinclude}, not with copies that drift."""
    chapters = {path: path.read_text() for path in sorted(COURSE.glob("*/*.md"))}
    copies = []
    for path in sorted(MATERIAL.rglob("*")):
        if not path.is_file() or path.suffix not in {".yml", ".yaml", ".json", ".toml", ".py", ".sh", ".bat"}:
            continue
        content = path.read_text().strip()
        if content.count("\n") < 5:
            continue
        copies += [
            f"{chapter.relative_to(COURSE)} copies {path.relative_to(COURSE)}"
            for chapter, text in chapters.items()
            if content in text
        ]
    assert not copies, "use {literalinclude}: " + "; ".join(copies)
