//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/Macros.hh
 * \brief  Common compiler-specific macros.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_HARNESS_MACROS_HH
#define YAYP_HARNESS_MACROS_HH

//---------------------------------------------------------------------------//
/*!
 * \def YAYP_UNLIKELY(condition)
 * \brief Mark the result of this condition to be "unlikely".
 *
 * This asks the compiler to move the section of code to a "cold" part of the
 * instructions, improving instruction locality. It should be used primarily
 * for error checking conditions.
 *
 * Best practice is for this macro to be used with empirical profiling data.
 * When in doubt, let the compiler/cpu do its own branch prediction.
 *
 * Example:
 * \code
 *   if (YAYP_UNLIKELY(a > 10))
 *   {
 *      // Very unlikely to go in here
 *   }
 * \endcode
 */
#if defined(__clang__) || defined(__GNUC__)
// GCC and Clang support the same builtin
#define YAYP_UNLIKELY(COND) __builtin_expect(!!(COND), 0)
#else
// No other compilers seem to have a similar builtin
#define YAYP_UNLIKELY(COND) COND
#endif

//---------------------------------------------------------------------------//
/*!
 * \def YAYP_LIKELY(condition)
 * \brief Mark the result of this condition to be "likely".
 *
 * This helps inform the compiler of expected branch prediction.  It tells the
 * compiler to move the section of code to a "hot" part of the instructions.
 *
 * Best practice is for this macro to be used with empirical profiling data.
 * When in doubt, let the compiler/cpu do its own branch prediction.
 *
 * Example:
 * \code
 *   if (YAYP_LIKELY(a < 10))
 *   {
 *      // Very likely to go in here.
 *   }
 * \endcode
 */
#if defined(__clang__) || defined(__GNUC__)
// GCC and Clang support the same builtin
#define YAYP_LIKELY(COND) __builtin_expect(!!(COND), 1)
#else
// No other compilers seem to have a similar builtin
#define YAYP_LIKELY(COND) COND
#endif

//---------------------------------------------------------------------------//
/*!
 * \def YAYP_FORCE_INLINE
 * \brief Attempt to force a function to be inlined.
 *
 * This directs the compiler to inline it, even when optimizations are
 * disabled. For very low-level functions, this can significantly improve
 * performance of debug code (and thus speed up the write/build/test cycle).
 *
 * Example:
 * \code
 *   YAYP_FORCE_INLINE int get_an_int();
 * \endcode
 */
// MSC first, since it may also define __GNUC__
#if defined(_MSC_VER)
#define YAYP_FORCE_INLINE inline __forceinline
// GCC and Clang use same syntax
#elif defined(__clang__) || defined(__GNUC__)
#define YAYP_FORCE_INLINE inline __attribute__((always_inline))
// Intel depends on compiler version
#elif defined(__INTEL_COMPILER)
#if __INTEL_COMPILER >= 1200
#define YAYP_FORCE_INLINE inline __attribute__((always_inline))
#else
#define YAYP_FORCE_INLINE inline __forceinline
#endif
// Otherwise just set to "inline" and hope the compiler does the correct thing
#else
#define YAYP_FORCE_INLINE inline
#endif

//---------------------------------------------------------------------------//
/*!
 * \def YAYP_UNREACHABLE
 * \brief Mark a point in code as being impossible to reach in normal
 *        execution.
 *
 * See https://clang.llvm.org/docs/LanguageExtensions.html#builtin-unreachable
 * or https://msdn.microsoft.com/en-us/library/1b3fsfxw.aspx
 *
 * Note that this assumes Clang is newer than some ancient version.
 * A bug in GCC (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57438) causes
 * functions with *only* an unreachable statement to throw a linker assertion
 * on the mac.
 *
 * \note This macro is usually unused; instead, the macro \c Not_Reachable
 * defined in DBC.hh should be used instead (to provide a more detailed error
 * message in case the point *is* reached).
 */
#if defined(__clang__)
#define YAYP_UNREACHABLE __builtin_unreachable()
#elif defined(__GNUC__)
#if __GNUC__ >= 7
#define YAYP_UNREACHABLE __builtin_unreachable()
#else
// Workaround for GCC bug, see above.
#define YAYP_UNREACHABLE __builtin_trap()
#endif
#elif defined(_MSC_VER)
#define YAYP_UNREACHABLE __assume(false)
#else
#define YAYP_UNREACHABLE
#endif

//---------------------------------------------------------------------------//
/*!
 * \def YAYP_DEPRECATED_FUTURE
 * \brief Indicates that the entity should be deprecated if compiling with
 *        greater than C++17
 */
#if __cplusplus >= 202002L
#define YAYP_DEPRECATED_FUTURE [[deprecated]]
#else
#define YAYP_DEPRECATED_CPP20
#endif

//---------------------------------------------------------------------------//
#endif // YAYP_HARNESS_MACROS_HH
//---------------------------------------------------------------------------//
// end of src/harness/Macros.hh
//---------------------------------------------------------------------------//
