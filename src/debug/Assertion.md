# Assertion Class Documentation

The `Assertion` class provides a simple yet flexible debugging and logging system for the AuraGL engine. It offers various levels of message logging (info, warning, error) and supports variadic message formatting.

## Debug Macros

The class provides four main macros that are active when `COMETA_DEBUG` is defined:

```cpp
COMETA_ASSERT(x)    // For information messages
COMETA_WARNING(x)   // For warning messages
COMETA_ERROR(x)     // For error messages
COMETA_MSG(...)     // For formatted messages with multiple arguments
```

When `COMETA_DEBUG` is not defined, these macros are replaced with `((void)0)`, effectively removing them from the release build.

## Core Methods

### Assert
```cpp
inline static void Assert(const char* msg)
```
Prints an information message with the `[INFO]` prefix.

### Warning
```cpp
inline static void Warning(const char* msg)
```
Prints a warning message with the `[WARNING]` prefix.

### Error
```cpp
inline static void Error(const char* msg)
```
Prints an error message with the `[ERROR]` prefix.

### Message
```cpp
template<typename... Args>
inline static void Message(const Args&... args)
```
Prints a formatted message with the `[MESSAGE]` prefix, combining multiple arguments of different types.

## Usage Examples

### Basic Usage
```cpp
// Information message
COMETA_ASSERT("Initialization complete");

// Warning message
COMETA_WARNING("Resource loading delayed");

// Error message
COMETA_ERROR("Failed to load texture");
```

### Formatted Messages
```cpp
// Combining different types
COMETA_MSG("Loading level ", 5, " with ", 1000, " objects");

// Using with strings
std::string playerName = "Player1";
COMETA_MSG("Welcome ", playerName, " to level ", 1);
```

## Implementation Details

The class uses template metaprogramming to handle different types of arguments in the `Message` method. The private `append` functions handle type conversion:

- Numeric types are converted using `std::to_string`
- C-style strings are appended directly
- `std::string` objects are appended directly

## Debug vs Release

In debug builds (when `COMETA_DEBUG` is defined), all logging functions are active. In release builds, the macros are replaced with no-ops, ensuring zero overhead in production code.

## Thread Safety

The class uses `std::cout` for output, which provides synchronization for individual calls. However, multiple messages from different threads may be interleaved.

## Dependencies

- `<iostream>` for output operations
- Standard C++ string handling