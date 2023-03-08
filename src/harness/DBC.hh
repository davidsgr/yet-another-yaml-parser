//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file  src/harness/DBC.hh
 * \brief DBC class declaration.
 *
 * Design-by-contract requires that all preconditions and postconditions are
 * satisfied in every function.  This is implemented using macros which can be
 * removed in release builds.
 *
 * The DBC macros are enabled at configure time using the YAYP_DBC macro,
 * where:
 *  - YAYP_DBC == 0  All DBC statements turned off
 *  - YAYP_DBC >  0  Function preconditions are tested with REQUIRE
 *  - YAYP_DBC >  1  Function postconditions are tested with ENSURE and
 *                   REMEMBER
 *  - YAYP_DBC >  2  Intermediate function results are tested with CHECK
 *
 * \note Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//
#ifndef YAYP_HARNESS_DBC_HH
#define YAYP_HARNESS_DBC_HH

#include <exception>
#include <string>

#include "Macros.hh"

#ifndef YAYP_DBC
#define YAYP_DBC 3 // By default, we turn all DBC ON
#endif

//===========================================================================//
/* \def YAYP_ASSERT_
 * \brief Basic DBC macro
 *
 * The following definitions of assertions help ensure that they're always
 * followed by a semicolon, and that if assertions are disabled we don't get
 * "unused variable" warnings. Instead of using (void)sizeof(COND), which
 * doesn't work for lambdas, we simply have a null-op "if" statement, which
 * even with -O0 GCC and Clang will compile out.
 *
 * Additionally, we use the "YAYP_UNLIKELY" macro (see Macros.hh) to ask the
 * compiler to mark the message-building and assertion call as "cold", placing
 * it outside of the main program flow (improving instruction locality).
 *
 * See:
 * https://stackoverflow.com/questions/5252375/custom-c-assert-macro
 */
#define YAYP_ASSERT_(COND, COND_TYPE)                                        \
    do                                                                       \
    {                                                                        \
        if (YAYP_UNLIKELY(!(COND)))                                          \
        {                                                                    \
            ::yayp::throwDBCException(#COND, COND_TYPE, __FILE__, __LINE__); \
        }                                                                    \
    } while (false)
#define YAYP_NOASSERT_(COND, COND_TYPE) \
    do                                  \
    {                                   \
        if (false && (COND)) {}         \
    } while (false)

//===========================================================================//
/*!
 * \def YAYP_REQUIRE
 * \brief Defines a macro for testing function preconditions.
 *
 * This macro compiles out if YAYP_DBC == 0.
 */
#if YAYP_DBC > 0
#define YAYP_REQUIRE_ON
#define YAYP_REQUIRE(COND) YAYP_ASSERT_(COND, "precondition")
#else
#define YAYP_REQUIRE(COND) YAYP_NOASSERT_(COND, "precondition")
#endif

//===========================================================================//
/*!
 * \def YAYP_ENSURE
 * \brief Defines a macro for testing function postconditions.
 *
 * This macro compiles out if YAYP_DBC <= 1.
 */
#if YAYP_DBC > 1
#define YAYP_ENSURE_ON
#define YAYP_ENSURE(COND) YAYP_ASSERT_(COND, "postcondition")
#else
#define YAYP_ENSURE(COND) YAYP_NOASSERT_(COND, "postcondition");
#endif

//===========================================================================//
/*!
 * \def YAYP_REMEMBER
 * \brief Defines a macro for defining temporary variables to use with
 *        YAYP_ENSURE and YAYP_CHECK.
 *
 * This macro compiles out if YAYP_DBC <= 1.
 */
#if YAYP_DBC > 1
#define YAYP_REMEMBER_ON
#define YAYP_REMEMBER(COND) COND
#else
#define YAYP_REMEMBER(COND)
#endif

//===========================================================================//
/*!
 * \def YAYP_CHECK
 * \brief Defines a macro for performing intermediate checks.
 *
 * This macro compiles out if YAYP_DBC <= 2.
 */
#if YAYP_DBC > 2
#define YAYP_CHECK_ON
#define YAYP_CHECK(COND) YAYP_ASSERT_(COND, "intermediate")
#else
#define YAYP_CHECK(COND) YAYP_NOASSERT_(COND, "intermediate")
#endif

//===========================================================================//
/*!
 * \def YAYP_NOT_IMPLEMENTED
 * \brief Defines a placeholder macro for code blocks that have not yet been
 *        implemented.
 */
#define YAYP_NOT_IMPLEMENTED(MSG) \
    throw ::yayp::NotImplementedException(MSG, __FILE__, __LINE__)

//===========================================================================//
/*!
 * \def YAYP_NOT_REACHABLE
 * \brief Defines a macro that indicates that a block of code should not be
 *        logically reachable.
 *
 * If DBC is enabled, then Not_Reachable_Assertion is thrown.  Otherwise, the
 * YAYP_UNREACHABLE macro is used.
 */
#if YAYP_DBC > 0
#define YAYP_NOT_REACHABLE() \
    throw ::yayp::NotReachableException(__FILE__, __LINE__)
#else
#define YAYP_NOT_REACHABLE() YAYP_UNREACHABLE
#endif

namespace yayp
{
//===========================================================================//
/*!
 * \class Exception
 * \brief Base exception class.
 *
 * This class inherits from std::runtime_error, and provides the ability to
 * catch YAYP exceptions separately from standard exceptions (or others).
 * This class represents a "generic" YAYP exception
 *
 * \example src/harness/test/tstDBC.cc
 */
//===========================================================================//

class Exception : public std::runtime_error
{
    using Base = std::runtime_error;

  public:
    // Constructor inherited from std::runtime_error
    explicit Exception(const std::string& msg);

    // Specialized constructor for constructing Exceptions. Takes a
    // message, filename, and line number
    Exception(const std::string& msg,
              const std::string& filename,
              unsigned int       line_number);

  private:
    // >>> IMPLEMENTATION
    // Build the exception message
    static std::string buildMessage(const std::string& msg,
                                    const std::string& filename,
                                    unsigned int       line_number);
};

//===========================================================================//
/*!
 * \class DBCException
 * \brief Exception class for failed design-by-contract checks.
 *
 * This class inherits from Exception and is thrown when a YAYP_REQUIRE,
 * YAYP_ENSURE, or YAYP_CHECK DBC statement fails.
 *
 * \example src/harness/test/tstDBC.cc
 */
//===========================================================================//

class DBCException final : public Exception
{
    using Base = Exception;

  public:
    // Constructor taking the DBC test, the type of DBC check that failed, the
    // file name, and the line number
    DBCException(const std::string& test,
                 const std::string& test_type,
                 const std::string& filename,
                 unsigned int       line_number);

  private:
    // >>> IMPLEMENTATION
    // Build the exception message
    static std::string buildMessage(const std::string& test,
                                    const std::string& test_type,
                                    const std::string& filename,
                                    unsigned int       line_number);
};

//===========================================================================//
/*!
 * \class NotImplementedException
 * \brief Exception class thrown when not implemented functions are called.
 *
 * This class inherits from Exception and is thrown whenever a function is
 * called but has not yet been implemented.
 *
 * \example Utilities/harness/test/tstDBC.cc
 */
class NotImplementedException final : public Exception
{
    using Base = Exception;

  public:
    // Construct taking a message and the filename and line number
    NotImplementedException(const std::string& message,
                            const std::string& filename,
                            unsigned int       line_number);

  private:
    // >>> IMPLEMENTATION
    // Build the exception message
    static std::string buildMessage(const std::string& message,
                                    const std::string& filename,
                                    unsigned int       line_number);
};

//===========================================================================//
/*!
 * \class NotReachableException
 * \brief Exception class when a code section is reached that should be
 * unreachable.
 *
 * This class inherits from Exception and is thrown when a code block is
 * reached that should not be logically reachable.
 *
 * \example Utilities/harness/test/tstDBC.cc
 */
class NotReachableException final : public Exception
{
    using Base = Exception;

  public:
    // Constructor taking a filename and line number
    NotReachableException(const std::string& file, unsigned int line);

  private:
    // >>> IMPLEMENTATION
    // Build the exception message
    static std::string buildMessage(const std::string& file, unsigned int line);
};

//---------------------------------------------------------------------------//
// HELPER FUNCTIONS
//---------------------------------------------------------------------------//
// Throws a DBC exception
[[noreturn]] void throwDBCException(const std::string& condition,
                                    const std::string& condition_type,
                                    const std::string& filename,
                                    unsigned int       line_number);

//---------------------------------------------------------------------------//
} // namespace yayp

//---------------------------------------------------------------------------//
#endif // YAYP_HARNESS_DBC_HH
//---------------------------------------------------------------------------//
// end of src/harness/DBC.hh
//---------------------------------------------------------------------------//
