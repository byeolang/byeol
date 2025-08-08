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

2. **clog** - Logging subsystem
   * Handles all logging operations
   * Provides configurable log levels and output formatting
   * Thread-safe logging implementation

3. **meta** - Type metadata management
   * Collects and manages type metadata
   * Provides runtime type information
   * Supports reflection capabilities

4. **memlite** - Lightweight memory management layer
   * Custom memory pool-based allocation
   * Direct memory management for performance optimization
   * Reduces garbage collection overhead

5. **leaf** - Configuration parsing layer
   * Parses configuration information using the `leaf` language (derived from `byeol`)
   * Handles `manifest.leaf` files in byeol pack distribution
   * Provides structured configuration management

6. **core** - Core language layer
   * Contains `byeol` AST (Abstract Syntax Tree)
   * Handles package loading and management
   * Implements core language features

7. **Frontend Layers**
   * **sys** - Default bundle pack included with `byeol`
   * **byeol** - Command Line Interface frontend

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

## Build Workflow

### Build Commands

1. **Full rebuild** (when new files are added):
   ```bash
   ./build/builder.py dbg
   ```
   * This process takes significant time
   * Use when project structure changes

2. **Incremental build** (when only functions/classes are modified):
   ```bash
   cd ./build/
   make -j$(nproc)
   ```
   * Faster compilation for code changes
   * Utilizes all available CPU cores

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
