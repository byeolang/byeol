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

5. **stela** - Configuration parsing layer
   * Parses configuration information using the `stela` language (derived from `byeol`)
   * Handles `manifest.stela` files in byeol pack distribution
   * Provides structured configuration management
   * As a doxygen comment, this belongs to `@ingroup stela`

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

## When to Read Additional Documentation

### ⚠️ CRITICAL: Architecture and Implementation Guidelines

**BEFORE modifying C++ code, you MUST read the detailed architecture document:**

📖 **READ**: `doc/ref/ko/architecture-and-class.md`

**WHEN to read this document:**
- ✅ Implementing new classes or functions in `module/` directory
- ✅ Modifying existing C++ code structure or logic
- ✅ Designing class hierarchies or inheritance patterns
- ✅ Working with core language features (AST, type system, memory management)
- ✅ Adding new features that interact with existing architecture
- ✅ Refactoring code across multiple modules

**When you DON'T need to read it:**
- ❌ Simple documentation updates (*.md files)
- ❌ Build script changes only (`build/` directory)
- ❌ Test case analysis without code modification
- ❌ Minor comment or formatting fixes
- ❌ Configuration file updates (CMakeLists.txt, *.cmake)

**WHY this is important:**
The architecture document contains:
- Detailed design patterns and principles specific to this codebase
- Class relationship diagrams and inheritance hierarchies
- Memory management rules and best practices
- Type system implementation details
- Critical guidelines that prevent common mistakes

**Failure to read this document before code modification may result in:**
- Violating established design patterns
- Breaking existing abstractions
- Introducing memory management issues
- Creating incompatible interfaces

## Project Structure

```
./
├── build/          # Build-related files and scripts
├── module/         # Architecture layers as individual modules
│   ├── indep/      # Platform-dependent code
│   ├── clog/       # Logging subsystem
│   ├── meta/       # Type metadata management
│   ├── memlite/    # Memory management layer
│   ├── stlea/      # Configuration parsing
│   ├── core/       # Core language implementation
│   ├── sys/        # System bundle pack
│   ├── byeol/      # CLI frontend
│   └── test/       # Test cases
├── bin/            # Generated executables (auto-created)
├── external/       # External libraries (auto-generated)
└── docs/           # Documentation files
```

### Important Notes
- `./bin/` is automatically generated during build process
- `./external/` is auto-generated for external libraries - **DO NOT MODIFY MANUALLY**
- Each module should maintain clear interfaces and minimal dependencies

## Coding Conventions

**All coding style and naming conventions are defined in:**

📖 **READ**: `doc/ref/ko/CONVENTION.md`

This includes:
- Naming conventions (camelCase for classes/variables, UPPER_SNAKE_CASE for macros)
- Code style (indentation, braces, line length, etc.)
- Documentation standards (Doxygen comments)
- File structure and organization

## Documentation Conventions

**When creating or modifying markdown documentation, you MUST read:**

📖 **READ**: `doc/ref/ko/document-convention.md`

This includes:
- Document tone and writing style
- Doxygen compatibility requirements
- Document structure and navigation patterns
- Style annotation system for code blocks
- Examples and their importance
- File naming conventions and document hierarchy


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
    * on linux:
    ```bash
    cd ./build/
    make -j$(nproc)
    ```
    * on mac:
    ```bash
    cd ./build/
    make -j$(sysctl -n hw.ncpu)
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
- [ ] Code follows conventions in `doc/ref/ko/convention-rules.md`
- [ ] All tests pass locally
- [ ] Documentation updated if needed
- [ ] No compiler warnings
- [ ] Code formatted with `./build/builder.py format`

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
