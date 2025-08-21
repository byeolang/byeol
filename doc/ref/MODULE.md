# Architecture Modules

## indep - Platform Dependent Layer
@ingroup indep

Platform-dependent code layer that contains all platform-specific implementations.

This layer handles all platform-specific code and enables the rest of the system to operate 
platform-independently. Conditional compilation (`#IF`, `#ELSE`, etc.) is only permitted in this module.

## clog - Logging Subsystem
@ingroup clog

Logging subsystem that handles all logging operations.

Provides configurable log levels and output formatting with thread-safe logging implementation.
This layer ensures consistent logging across the entire system.

## meta - Type Metadata Management
@ingroup meta

Type metadata management layer that collects and manages type metadata.

Provides runtime type information and supports reflection capabilities for the byeol language.
Essential for dynamic type operations and introspection.

## memlite - Lightweight Memory Management
@ingroup memlite

Lightweight memory management layer with custom memory pool-based allocation.

Provides direct memory management for performance optimization and reduces garbage collection 
overhead through efficient memory pool strategies.

## leaf - Configuration Parsing Layer
@ingroup leaf

Configuration parsing layer that handles leaf language parsing.

Parses configuration information using the leaf language (derived from byeol) and handles 
`manifest.leaf` files in byeol pack distribution for structured configuration management.

## core - Core Language Layer
@ingroup core

Core language layer containing byeol AST and core language features.

Contains the byeol Abstract Syntax Tree (AST), handles package loading and management, 
and implements the fundamental features of the byeol programming language.

## sys - System Bundle Pack
@ingroup sys

Default bundle pack included with byeol.

Provides the standard library and built-in functionality that comes with the byeol 
language distribution.

## byeol - Command Line Interface Frontend
@ingroup byeol

Command Line Interface frontend for the byeol language.

Provides the main entry point and user interface for interacting with the byeol 
language system through command line operations.

## test - Test Suite Layer
@ingroup test

Comprehensive test suite for all system layers.

Contains test cases for all architectural layers to ensure system reliability, 
correctness, and maintainability of the byeol language implementation.
