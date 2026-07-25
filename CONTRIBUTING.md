# Contributing to Byeol

Thanks for taking an interest in Byeol. This document covers what you need to know
before opening a pull request.

Byeol is a hobby project built around one idea: you should never have to restart a
program just to see whether your fix worked. Keep that in mind and most of the
conventions below will make sense.

---

## Before you start

Byeol is not a single repository. Several repositories combine into one product, and
which one you should touch depends on what you are changing.

| Repository                                            | What lives there                                             |
| ----------------------------------------------------- | ------------------------------------------------------------ |
| [byeol](https://github.com/byeolang/byeol)             | The interpreter itself. AST, parser, verifier, runtime.       |
| [launcher](https://github.com/byeolang/launcher)       | The `byeol` command users actually run. Resolves versions.    |
| [stela](https://github.com/byeolang/stela)             | Configuration language, plus logging, type and memory modules.|
| [indep](https://github.com/byeolang/indep)             | Platform abstraction. The only place `#ifdef` is allowed.     |
| [flagStacker](https://github.com/byeolang/flagStacker) | Command line flag parsing.                                    |
| [build-common](https://github.com/byeolang/build-common)| Shared CMake rules.                                          |

Dependencies flow in one direction only. `byeol` depends on `stela`, `stela` depends on
`indep`, and nothing ever points back upward. If your change seems to need a backward
dependency, that is a sign the change belongs somewhere else.

For how these fit together, read
[the architecture overview](doc/ref/ko/ae-architecture-overview.md).

---

## Setting up

You will need a C++ compiler (clang is preferred), CMake 3.16+, Flex 2.6+, Bison 3.8+,
and Python 3.8+. The full setup is described in
[the development environment guide](doc/ref/ko/ab-dev-env.md).

```bash
python3 build/builder.py prerequisites   # check your tools
python3 build/builder.py dbg             # debug build
python3 build/builder.py test            # run the test suite
```

Note that `builder.py` does not offer the same commands in every repository. In `stela`,
for example, there is no build command and you invoke CMake directly.

---

## Making your change

Read [the coding rules](doc/ref/ko/ad-convention-rules.md) before writing C++. Byeol
does not follow the C++ conventions you may be used to, and the reasons are explained
there. Formatting is applied automatically, so do not hand-format:

```bash
python3 build/builder.py format
```

Two rules matter more than the rest, because CI cannot catch either one.

Respect the layer boundaries. A module may only use modules below it. Adding a
dependency that skips or reverses the layering breaks the design even when it compiles.

Search before you write. Utilities in this codebase are easy to duplicate by accident,
since similar helpers live in `indep`, `memlite` and `core`. Grep the public headers
(`module/*.hpp`) before adding anything that sounds generic.

Tests are expected for behavioural changes. See
[the testing guide](doc/ref/ko/an-testing-guide.md).

---

## Commits

Write commit messages in English, in the form `type(scope): subject`.

```
fix(core): suppress unused variable warning for _stepN
feat(ci): include byeol binary and pod/sys in release artifacts
doc(reference): use hyperlink instead of markdown link syntax
```

Use one of these types: `feat`, `fix`, `doc`, `style`, `refactor`, `test`, `build`,
`ci`, `chore`. The scope is the module or area you touched, and it may be omitted when
the change is repository-wide.

Explain why in the body, not what. The diff already shows what changed.

Keep mechanical changes separate from meaningful ones. A rename that touches a hundred
files is reviewable on its own; the same rename mixed with a bug fix is not.

---

## Pull requests

Open pull requests against `main`. Describe what changed and why, and mention anything
you deliberately left out.

Smaller is better. If a pull request does two unrelated things, split it. Large
mechanical changes are fine as long as they are mechanical and say so.

When your change spans repositories, land the lower one first. Release it, then raise
the pinned tag in the repository above. Since dependencies are pinned by tag, nothing
downstream moves until you explicitly move it.

CI checks formatting, then builds and tests on Linux, macOS, Windows and WebAssembly.
Please make sure the test suite passes locally before you open the pull request.

---

## Documentation

Documentation lives under `doc/`, split by audience and language. Korean reference
documents are in `doc/ref/ko/`, English in `doc/ref/en/`. Write in the language of the
directory you are editing.

If you add or rename a document, follow
[the document convention](doc/ref/ko/ao-document-convention.md). Filenames carry a
two-letter prefix that determines reading order, and the document ID must match the
filename exactly.

---

## Questions

If something here is unclear or you are unsure whether an idea fits the project, open an
issue and ask before writing the code. It is easier to redirect an idea than a finished
patch.
