# byeol-cmake

Shared CMake modules for byeol-family repositories.

## Purpose
This repository contains shared build conventions and helper modules:
- common options
- warnings policy
- changelog version parsing helper

## Consumer policy
Consumer repositories should vendor release tags only.
Do not track main directly.

## Custom commands
### by_parse_ver
This extracts the version only when the first version-like line in the changelog starts with `## vMAJOR.MINOR.PATCH`.
If an earlier `v1.2.3` appears in a different form such as `### v1.2.3`, parsing fails.

```cmake
by_parse_ver("../byeol/CHANGELOGS.md")
```

After calling it, these variables are available in the caller scope:
- `BY_VER_MAJOR`
- `BY_VER_MINOR`
- `BY_VER_PATCH`

Relative paths are resolved from the caller's current CMake list file.

### by_check_dependency
This checks whether at least one candidate executable exists and, when `MIN_VERSION`
is provided, whether the detected version is greater than or equal to it.

```cmake
by_check_dependency(
    NAMES python3 python
    MIN_VERSION 3.10.0
)
```

Supported arguments:
- `NAMES`: candidate executable names searched in order.
- `MIN_VERSION`: minimum accepted version. If omitted, only existence is checked.
- `VERSION_ARGS`: arguments used to query the version. Default: `--version`.

If the dependency check fails, `by_check_dependency()` stops the current CMake
run with `message(FATAL_ERROR ...)`.

Example with a custom version flag:

```cmake
by_check_dependency(
    NAMES msbuild.exe
    MIN_VERSION 17.0
    VERSION_ARGS /version
)
```
