# Development Instructions for Claude Code

* This document contains crucial information for Claude Code operation. Follow these instructions precisely.

## Prerequisites

### Required Tools
1. **C++ Compiler** - clang is preferred, but gcc and MSVC are also supported
   * clang++ 14+
   * g++ 8+
   * msbuild.exe 17+

2. **Parsing Tools** - Flex and Bison are used for parsing
   * Flex 2.6.0+
   * Bison 3.8.0+

3. **Testing Framework** - Google Test is used for test cases
   * Google Test 1.10.0+

4. **WebAssembly Support** - Emscripten is used for WASM compilation
   * Emscripten 3.1.0+

### Development Environment Setup
- CMake 3.16+ (recommended build system)
- Python 3.8+ (for build scripts)
- Git 2.20+ (for version control)

## Architecture

The system follows a **Layered Architecture** pattern with clear separation of concerns:

### Core Layers

1. **indep** - Platform-dependent code layer
   * Contains all platform-specific implementations
   * All subsequent layers operate platform-independently
   * Conditional compilation (`#IF`, `#ELSE`, etc.) is **only** permitted in the `indep` module
   * As a doxygen comment, this belongs to `@ingroup indep`

2. **clog** - Logging subsystem
   * Handles all logging operations
   * Provides configurable log levels and output formatting
   * Thread-safe logging implementation
   * As a doxygen comment, this belongs to `@ingroup clog`

3. **meta** - Type metadata management
   * Collects and manages type metadata
   * Provides runtime type information
   * Supports reflection capabilities
   * As a doxygen comment, this belongs to `@ingroup meta`

4. **memlite** - Lightweight memory management layer
   * Custom memory pool-based allocation
   * Direct memory management for performance optimization
   * Reduces garbage collection overhead
   * As a doxygen comment, this belongs to `@ingroup memlite`

5. **leaf** - Configuration parsing layer
   * Parses configuration information using the `leaf` language (derived from `byeol`)
   * Handles `manifest.leaf` files in byeol pack distribution
   * Provides structured configuration management
   * As a doxygen comment, this belongs to `@ingroup leaf`

6. **core** - Core language layer
   * Contains `byeol` AST (Abstract Syntax Tree)
   * Handles package loading and management
   * Implements core language features
   * As a doxygen comment, this belongs to `@ingroup core`

7. **Frontend Layers**
   * **sys** - Default bundle pack included with `byeol`
   * **byeol** - Command Line Interface frontend
   * As a doxygen comment, this belongs to `@ingroup frontend`

8. **test** - Test suite layer
   * Contains comprehensive test cases for all above layers
   * Ensures system reliability and correctness

## Project Structure

```
./
├── build/          # Build-related files and scripts
├── module/         # Architecture layers as individual modules
│   ├── indep/      # Platform-dependent code
│   ├── clog/       # Logging subsystem
│   ├── meta/       # Type metadata management
│   ├── memlite/    # Memory management layer
│   ├── leaf/       # Configuration parsing
│   ├── core/       # Core language implementation
│   ├── sys/        # System bundle pack
│   ├── byeol/       # CLI frontend
│   └── test/       # Test cases
├── bin/            # Generated executables (auto-created)
├── external/       # External libraries (auto-generated)
└── docs/           # Documentation files
```

### Important Notes
- `./bin/` is automatically generated during build process
- `./external/` is auto-generated for external libraries - **DO NOT MODIFY MANUALLY**
- Each module should maintain clear interfaces and minimal dependencies

## Basic Comment Usage
- This project uses Doxygen to provide documentation.

- Important algorithms within functions should also be commented when necessary.

- Doxygen comments must be added above function declarations and class declarations using Doxygen syntax.

- Exception: Files in the `test` module do not require Doxygen comments.

- **Important**: Excessive information can be counterproductive. Never add comments to parts of code that are easily understood at first glance. This rule applies to all comments, including Doxygen comments.

- Doxygen comments can be omitted for getter/setter functions when they are self-explanatory.
    - Examples: getName(), setAge(), etc.
    - However, comments must be added if there is complex logic or important considerations.

- For complex APIs or special usage patterns, provide examples using `@code` blocks.
    - Example:
```cpp
/// @brief Creates and initializes a user
/// @details This involves a complex initialization process, please refer to the example below
/// @code
/// UserManager manager;
/// if (manager.createUser("john", "john@email.com")) {
///     // User creation successful
///     manager.initializeUserData("john");
/// }
/// @endcode
```

- Development notes such as TODO, FIXME, HACK should be written outside of Doxygen comments.
    - Temporary notes or work-in-progress content are prohibited in Doxygen comments.
    - Incorrect example:
```cpp
/// TODO: Need to add error handling
/// FIXME: Memory leak exists
/// @brief Reads a file
/// @details TODO: Need to add error handling
```

    - Correct example:
```cpp
// TODO: Need to add error handling
// FIXME: Memory leak exists
/// @brief Reads a file
/// @details Reads a file from the specified path and returns it as a string
```

### Basic Doxygen Comment Usage
- All Doxygen comments must use the `///` style; `/**` or `*/` style is prohibited.

- Empty lines should only be used for paragraph separation within Doxygen comment blocks.

- Use `@` prefix when expressing comment attributes.

- Comments should not exceed 100 columns.

- When providing descriptions, use `-` to separate items if needed.

- Use `@note` or `@warning` for important considerations when necessary.

- When referencing other classes or functions in comments, use `@ref <class or function name>`. Example:
```cpp
/// @brief this function extract date info from @ref buildInformation class using its @ref getDate()
```

- Doxygen comments must be added to three types: files, classes, and functions.

1. File Doxygen comments must include the following:
    - `@file` must always be positioned at the top of header files.
    - Only for header files that contain no classes and only have #define or typedef, add additional file description after `@file` with a line break.
    - Example:
```cpp
@file
/// a bunch of string macros
/// a more elaborated file description you explain
```

2. Class Doxygen comments must include the following information in order:
    - `@ingroup` specifies the module this file belongs to. Group names are described in `Core Layers`.
    - `@brief` briefly describes the role of this class in one line.
    - `@details` provides more detailed information about what was written in `@brief`.
    - Example:
```cpp
/// @ingroup indep
/// @brief it manges data related to build info.
/// @details it contains build number and version and can return whether it's debug binary or not.
/// and you can describe more info for `@details` with newline if you want to.
```

3. Function Doxygen comments must include the following content in order:
    - `@brief` briefly describes the role of this function.
    - `@param` is usually optional, but must be added in the following cases:
        - When the parameter's purpose is not clear from its name alone
        - When there are specific ranges or conditions (e.g., "age (0-150 range)")
        - When null acceptance is important
        - Example:
```cpp
/// @param matrix Input matrix (must not be null, NxM size)
/// @param options Configuration options (nullptr allowed)
```

    - `@return` is usually optional, but should be described when complex or supplementary explanation is important.


## Build Workflow

### Build Commands

1. **Full rebuild** (when new files are added):
   ```bash
   ./build/builder.py dbg
   ```
   * This process takes significant time
   * Use when project structure changes

2. **Incremental build** (if you are only modifying functions/classes):
    * If your OS is not windows, do the following.
    ```bash
    cd ./build/
    make -j$(nproc)
    ```
    * If your OS is windows, do the following.
    ```
    cd ./build/
    msbuild module/frontend/byeol.vcxproj
    ```

   * Faster compilation when making code changes
   * Utilize all available CPU cores

### Build Targets
- `dbg` - Debug build with symbols and assertions
- `rel` - Release build with optimizations
- `test` - Build with test coverage enabled
- `wasm` - WebAssembly build target

## Debugging Workflow

### Step-by-Step Debugging Process

1. **Identify the problem scope**
   * Determine if the issue is in test cases (TC) or main code
   * Check build logs for compilation errors

2. **Locate the failing test case**
   * Identify which TC file and specific test is failing
   * Example: `defFuncTest.lambda1`

3. **Run specific test case**
   ```bash
   cd bin
   ./test --gtest_filter="<yourTcFile.yourTc>" verbose
   ```
   
   Example:
   ```bash
   cd bin
   ./test --gtest_filter="defFuncTest.lambda1" verbose
   ```

4. **Analyze the output**
   * Review detailed logs and error messages
   * Use debugger if necessary:
     ```bash
     gdb ./test
     (gdb) run --gtest_filter="defFuncTest.lambda1" verbose
     ```

5. **Fix and rebuild**
   * Implement the fix based on analysis
   * Rebuild using appropriate build command
   * Re-run tests to verify the fix

6. **Proceed to commit workflow** once the issue is resolved

### Debug Build Features
- Enhanced logging output
- Assertion checking enabled
- Debug symbols included
- Memory leak detection

## Commit Workflow

### Convention Rules

1. **Naming Conventions**
   * **Classes and variables**: Always use camelCase (starting with lowercase)
     ```cpp
     class myClass { int myVariable; };
     ```
   
   * **Macros and #define**: Use UPPER_SNAKE_CASE
     ```cpp
     #define MAX_BUFFER_SIZE 1024
     ```

2. **Naming Philosophy**
   * Prefer abbreviated, semantically rich names
   * Choose intuitive, simple names over complex ones
   * Example: Use `make` instead of `generator`

3. **Macro Usage**
   * Actively use OR macros and TO macros for code clarity
   * Example:
     ```cpp
     const nFlt& grade = fr.sub("grade") TO(template cast<nFlt>()) OR.ret(false);
     ```

### Commit Message Format
```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types:**
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes
- `refactor`: Code refactoring
- `test`: Test additions or modifications
- `build`: Build system changes

**Example:**
```
feat(core): add lambda expression support

Implement lambda expressions in the byeol language parser.
Added support for capture lists and return type deduction.

Closes #123
```

### Pre-commit Checklist
- [ ] Code follows naming conventions
- [ ] All tests pass locally
- [ ] Documentation updated if needed
- [ ] No compiler warnings
- [ ] Code formatted according to style guide

## Additional Development Guidelines

### Code Quality Standards
- Maintain test coverage above 80%
- Follow RAII principles for resource management
- Use smart pointers for memory management
- Implement proper error handling and logging

### Performance Considerations
- Profile code regularly using built-in profiling tools
- Optimize hot paths identified through profiling
- Consider memory usage patterns in memlite layer

### Documentation Requirements
- Update API documentation for public interfaces
- Maintain inline comments for complex algorithms
- Keep this development guide updated with architecture changes

## Troubleshooting

### Common Issues
1. **Build failures**: Check tool versions and dependencies
2. **Test failures**: Verify test data and environment setup
3. **Memory issues**: Use valgrind or AddressSanitizer
4. **Performance problems**: Profile with gprof or perf

### Getting Help
- Check existing documentation in `./docs/`
- Review test cases for usage examples
- Consult architecture diagrams for system understanding
