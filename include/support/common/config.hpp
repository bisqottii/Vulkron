#pragma once

// ============================================================================
// VERSION INFORMATION
// ============================================================================

/**
 * @brief Major version number (incremented for breaking API changes)
 *
 * Follows semantic versioning: MAJOR.MINOR.PATCH
 * - MAJOR: Incompatible API changes
 * - MINOR: Backward-compatible functionality additions
 * - PATCH: Backward-compatible bug fixes
 */
#define VULKRON_VERSION_MAJOR 0

/**
 * @brief Minor version number (incremented for new backward-compatible features)
 *
 * @see VULKRON_VERSION_MAJOR
 */
#define VULKRON_VERSION_MINOR 1

/**
 * @brief Patch version number (incremented for bug fixes)
 *
 * @see VULKRON_VERSION_MAJOR
 */
#define VULKRON_VERSION_PATCH 0

// ============================================================================
// PLATFORM DETECTION
// ============================================================================

#if defined(_WIN32) || defined(_WIN64)
/**
 * @brief Defined when compiling for Windows platforms
 *
 * Set to 1 when targeting Microsoft Windows (32-bit or 64-bit).
 * Automatically enables Windows-specific optimizations and workarounds.
 */
#define VULKRON_PLATFORM_WINDOWS 1

#elif defined(__linux__)
/**
 * @brief Defined when compiling for Linux platforms
 *
 * Set to 1 when targeting Linux-based systems.
 * Enables POSIX APIs and Linux-specific optimizations.
 */
#define VULKRON_PLATFORM_LINUX 1

#else
/**
 * @brief Platform detection failed
 *
 * @error Vulkron only supports Windows and Linux platforms.
 * Other platforms (macOS, iOS, Android, etc.) are not currently supported.
 */
#error "Vulkron only supports Windows and Linux"
#endif

// ============================================================================
// COMPILER OPTIMIZATION HINTS
// ============================================================================

/**
 * @brief Forces compiler to always inline this function
 *
 * Eliminates function call overhead by inserting function body directly
 * at call site. Use for small, frequently-called functions (< 10 lines).
 *
 * @example @code
 * VULKRON_ALWAYS_INLINE bool is_depth_format(VkFormat fmt) {
 *     return fmt == VK_FORMAT_D16_UNORM || fmt == VK_FORMAT_D32_SFLOAT;
 * }
 * @endcode
 */
#define VULKRON_ALWAYS_INLINE __attribute__((always_inline)) inline

/**
 * @brief Prevents compiler from inlining this function
 *
 * Useful for large functions, debugging (keeps function in stack traces),
 * or preventing code bloat from excessive inlining.
 *
 * @example @code
 * VULKRON_NOINLINE void initialize_vulkan_instance() {
 *     // Complex multi-step initialization
 * }
 * @endcode
 */
#define VULKRON_NOINLINE __attribute__((noinline))

/**
 * @brief Marks function that never returns to calling code
 *
 * Function will terminate program, throw (if exceptions enabled),
 * or enter infinite loop. Helps compiler optimize away unreachable code.
 *
 * @example @code
 * [[noreturn]] VULKRON_NORETURN void fatal_error(const char* msg) {
 *     fprintf(stderr, "Fatal: %s\n", msg);
 *     std::abort();
 * }
 * @endcode
 */
#define VULKRON_NORETURN __attribute__((noreturn))

/**
 * @brief Aggressively inline all function calls within this function
 *
 * Forces inlining of every function call inside the marked function,
 * regardless of size limits. Use cautiously for critical hot paths.
 *
 * @warning Can cause significant code bloat if overused
 *
 * @example @code
 * VULKRON_FLATTEN void render_frame() {
 *     update();    // Will be inlined
 *     draw();      // Will be inlined
 *     present();   // Will be inlined
 * }
 * @endcode
 */
#define VULKRON_FLATTEN __attribute__((flatten))

/**
 * @brief Marks function as pure (no side effects, can read globals)
 *
 * Function's output depends only on parameters and possibly global/static
 * variables. No side effects. Multiple calls with same inputs can be CSE'd.
 *
 * @see VULKRON_CONST for stricter version
 *
 * @example @code
 * VULKRON_PURE int calculate_index(int x, int y, int width) {
 *     return y * width + x;  // Only depends on parameters
 * }
 * @endcode
 */
#define VULKRON_PURE __attribute__((pure))

/**
 * @brief Marks function as const (no side effects, doesn't read globals)
 *
 * Function's output depends ONLY on parameters (not even global variables).
 * Most restrictive purity guarantee. Enables most aggressive optimizations.
 *
 * @see VULKRON_PURE for less restrictive version
 *
 * @example @code
 * VULKRON_CONST int align_up(int value, int alignment) {
 *     return (value + alignment - 1) & ~(alignment - 1);
 * }
 * @endcode
 */
#define VULKRON_CONST __attribute__((const))

/**
 * @brief Marks function as rarely executed (cold code path)
 *
 * Helps compiler optimize instruction cache layout by placing this function
 * in separate section. Use for error handling, initialization, cleanup.
 *
 * @see VULKRON_HOT for opposite
 *
 * @example @code
 * VULKRON_COLD void handle_vulkan_error(VkResult result) {
 *     // Rarely called error handler
 * }
 * @endcode
 */
#define VULKRON_COLD __attribute__((cold))

/**
 * @brief Marks function as frequently executed (hot code path)
 *
 * Helps compiler optimize instruction cache layout by keeping this function
 * with other hot code. Use for render loops, tight simulation updates.
 *
 * @see VULKRON_COLD for opposite
 *
 * @example @code
 * VULKRON_HOT void submit_command_buffer(VkQueue queue, VkCommandBuffer cmd) {
 *     // Called every frame
 * }
 * @endcode
 */
#define VULKRON_HOT __attribute__((hot))

/**
 * @brief Branch prediction hint: condition is LIKELY true
 *
 * Tells CPU branch predictor this condition is probable (~90%+ true).
 * Improves speculative execution and pipeline efficiency.
 *
 * @param x Boolean expression to evaluate
 * @return Same boolean value, but with optimization hint
 *
 * @see VULKRON_UNLIKELY for opposite
 *
 * @example @code
 * VkResult result = vkCreateDevice(...);
 * if (VULKRON_LIKELY(result == VK_SUCCESS)) {
 *     // Fast path (expected case)
 * }
 * @endcode
 */
#define VULKRON_LIKELY(x) __builtin_expect(!!(x), 1)

/**
 * @brief Branch prediction hint: condition is UNLIKELY true
 *
 * Tells CPU branch predictor this condition is improbable (~10% or less true).
 * Improves speculative execution for exceptional cases.
 *
 * @param x Boolean expression to evaluate
 * @return Same boolean value, but with optimization hint
 *
 * @see VULKRON_LIKELY for opposite
 *
 * @example @code
 * VkResult result = vkQueueSubmit(...);
 * if (VULKRON_UNLIKELY(result != VK_SUCCESS)) {
 *     // Error handling (rare case)
 * }
 * @endcode
 */
#define VULKRON_UNLIKELY(x) __builtin_expect(!!(x), 0)

/**
 * @brief Compiler assumption: expression is always true
 *
 * Tells compiler to assume expression evaluates to true, enabling
 * aggressive optimizations. UNDEFINED BEHAVIOR if assumption is false.
 *
 * @warning Use only when you can mathematically guarantee truth
 * @param x Expression that compiler should assume is true
 *
 * @example @code
 * void process_aligned_data(void* data) {
 *     VULKRON_ASSUME(data != nullptr);
 *     VULKRON_ASSUME(reinterpret_cast<uintptr_t>(data) % 16 == 0);
 *     // Compiler can use aligned load/store instructions
 * }
 * @endcode
 */
#define VULKRON_ASSUME(x) __builtin_assume(x)

/**
 * @brief Assumes pointer is 8-byte aligned (Vulkan standard)
 *
 * Vulkan handles and allocations are typically 8-byte aligned.
 * This assumption enables better code generation for memory operations.
 *
 * @param ptr Pointer to check/assume alignment of
 *
 * @example @code
 * void upload_to_buffer(VkBuffer buffer, const void* data) {
 *     VULKRON_ASSUME_VK_ALIGNED(buffer);
 *     VULKRON_ASSUME_VK_ALIGNED(data);
 *     // Compiler can generate optimal 8-byte aligned copies
 * }
 * @endcode
 */
#define VULKRON_ASSUME_VK_ALIGNED(ptr) VULKRON_ASSUME((uintptr_t)(ptr) % 8 == 0)

// ============================================================================
// FEATURE FLAGS (DISABLED BY DEFAULT)
// ============================================================================

/**
 * @def VULKRON_ENABLE_PROFILER
 * @brief Enable profiling instrumentation throughout the library
 *
 * When defined, adds profiling scopes and markers to track performance.
 * Significantly increases overhead - only enable during development.
 *
 * @note Currently disabled - implement in vulkron_profiler.hpp
 */
// #define VULKRON_ENABLE_PROFILER

/**
 * @def VULKRON_ENABLE_DEBUGGER
 * @brief Enable debugger integration and enhanced error reporting
 *
 * When defined, adds debug breaks, enhanced logging, and debugging utilities.
 * Use during development to catch errors early.
 *
 * @note Currently disabled - implement in vulkron_debugger.hpp
 */
// #define VULKRON_ENABLE_DEBUGGER

// ============================================================================
// DLL/SHARED LIBRARY SUPPORT
// ============================================================================

#if defined(VULKRON_SHARED) && VULKRON_PLATFORM_WINDOWS
#ifdef VULKRON_EXPORTS
/**
 * @brief Marks symbols for export from DLL (Windows)
 *
 * Used when building Vulkron as a shared library on Windows.
 * Applied to public API functions and classes.
 */
#define VULKRON_API __declspec(dllexport)
#else
/**
 * @brief Marks symbols for import from DLL (Windows)
 *
 * Used when consuming Vulkron as a shared library on Windows.
 * Applied to public API functions and classes.
 */
#define VULKRON_API __declspec(dllimport)
#endif
#elif defined(VULKRON_SHARED) && VULKRON_PLATFORM_LINUX
#ifdef VULKRON_EXPORTS
/**
 * @brief Marks symbols with default visibility (Linux/macOS)
 *
 * Used when building Vulkron as a shared library on Unix-like systems.
 * Applied to public API functions and classes.
 */
#define VULKRON_API __attribute__((visibility("default")))
#else
/**
 * @brief No special visibility needed (Linux/macOS consumer)
 *
 * Used when consuming Vulkron as a shared library on Unix-like systems.
 */
#define VULKRON_API
#endif
#else
/**
 * @brief No DLL/visibility attributes needed (static library)
 *
 * Used when building or consuming Vulkron as a static library.
 * All symbols have internal linkage appropriate for static linking.
 */
#define VULKRON_API
#endif

/**
 * @brief Marks symbols for internal library use only
 *
 * Functions and variables marked with VULKRON_INTERNAL have static linkage,
 * preventing them from being visible outside the translation unit.
 * Use for implementation details that should not be part of the public API.
 *
 * @example @code
 * VULKRON_INTERNAL void internal_helper() {
 *     // Only visible in this .cpp file
 * }
 * @endcode
 */
#define VULKRON_INTERNAL static

// ============================================================================
// FIXED-SIZE TYPE ALIASES
// ============================================================================

#include <cstddef> ///< For size_t, ptrdiff_t
#include <cstdint> ///< For int8_t, uint8_t, etc.

/**
 * @namespace vulkron
 * @brief Main namespace for all Vulkron types and functions
 *
 * Contains type aliases, classes, and utilities for the Vulkron
 * Vulkan wrapper library.
 */
namespace vulkron {

    /**
     * @brief 8-bit signed integer (exact width)
     * @see int8_t
     */
    using s8 = int8_t;

    /**
     * @brief 16-bit signed integer (exact width)
     * @see int16_t
     */
    using s16 = int16_t;

    /**
     * @brief 32-bit signed integer (exact width)
     * @see int32_t
     */
    using s32 = int32_t;

    /**
     * @brief 64-bit signed integer (exact width)
     * @see int64_t
     */
    using s64 = int64_t;

    /**
     * @brief 8-bit unsigned integer (exact width)
     * @see uint8_t
     */
    using u8 = uint8_t;

    /**
     * @brief 16-bit unsigned integer (exact width)
     * @see uint16_t
     */
    using u16 = uint16_t;

    /**
     * @brief 32-bit unsigned integer (exact width)
     * @see uint32_t
     */
    using u32 = uint32_t;

    /**
     * @brief 64-bit unsigned integer (exact width)
     * @see uint64_t
     */
    using u64 = uint64_t;

    /**
     * @brief 32-bit floating point (IEEE 754 single precision)
     * @see float
     */
    using f32 = float;

    /**
     * @brief 64-bit floating point (IEEE 754 double precision)
     * @see double
     */
    using f64 = double;

    /**
     * @brief Architecture-dependent unsigned size type
     *
     * Use for array indices, loop counters, and memory sizes.
     * Matches the platform's pointer size (32-bit or 64-bit).
     * @see size_t
     */
    using usize = size_t;

    /**
     * @brief Raw byte type for memory manipulation
     *
     * Use for low-level memory operations instead of char or unsigned char.
     * @see std::byte
     */
    using byte = std::byte;

    /**
     * @brief Boolean type (always use instead of int for boolean values)
     *
     * Ensures type safety and prevents accidental integer conversions.
     * @see bool
     */
    using b8 = bool;

} // namespace vulkron