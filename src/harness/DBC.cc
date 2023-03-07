//---------------------------------*-C++-*-----------------------------------//
/*!
 * \file   src/harness/DBC.cc
 * \brief  DBC class definitions.
 * \note   Copyright (c) 2023 Oak Ridge National Laboratory, UT-Battelle, LLC.
 */
//---------------------------------------------------------------------------//

#include "DBC.hh"

#include <sstream>

namespace yayp
{
//===========================================================================//
// EXCEPTION CLASS DEFINITION
//===========================================================================//
/*!
 * \brief Construct taking an error message
 *
 * \param[in] msg  The exception error string
 */
Exception::Exception(const std::string& msg)
    : Base(msg)
{
    /* * */
}

//---------------------------------------------------------------------------//
/*!
 * \brief Specialized constructor for constructing Exceptions.
 *
 * \param[in] msg  The exception error message
 * \param[in] filename The file where the error condition occurred
 * \param[in] line_number The line number where the error occurred.
 */
Exception::Exception(const std::string& msg,
                     const std::string& filename,
                     unsigned int       line_number)
    : Exception(Exception::buildMessage(msg, filename, line_number))
{
    /* * */
}

//---------------------------------------------------------------------------//
// PRIVATE IMPLEMENTATION FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Builds the exception message
 *
 * \param[in] msg         The primary message phrase
 * \param[in] filename    The file where the exception was thrown
 * \param[in] line_number The line number where the exception was thrown
 * \return The constructed exception message
 */
std::string Exception::buildMessage(const std::string& msg,
                                    const std::string& filename,
                                    unsigned int       line_number)
{
    std::ostringstream stream;
    stream << "Caught YAYP exception: " << msg << "\n ^^^ at " << filename
           << ":" << line_number;
    return stream.str();
}

//===========================================================================//
// DBCEXCEPTION CLASS DEFINITON
//===========================================================================//
/*!
 * \brief Constructor for DBCException generated from failed DBC tests
 *
 * \param[in] test        A string representation of the DBC test that failed
 * \param[in] test_type  The type of test (precondition, postcondition, etc...)
 * \param[in] filename    The filename where the exception was thrown
 * \param[in] line_number The line number where the exception was thrown
 */
DBCException::DBCException(const std::string& test,
                           const std::string& test_type,
                           const std::string& filename,
                           unsigned int       line_number)
    : Base(DBCException::buildMessage(test, test_type, filename, line_number))
{
    /* * */
}

//---------------------------------------------------------------------------//
// PRIVATE IMPLEMENTATION FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Builds the exception message
 *
 * \param[in] test        A string representation of the DBC test that failed
 * \param[in] test_type  The type of test (precondition, postcondition, etc...)
 * \param[in] filename    The filename where the exception was thrown
 * \param[in] line_number The line number where the exception was thrown
 * \return The constructed exception message
 */
std::string DBCException::buildMessage(const std::string& test,
                                       const std::string& test_type,
                                       const std::string& filename,
                                       unsigned int       line_number)
{
    std::ostringstream stream;
    stream << "Failed DBC " << test_type << " test: " << test << "\n ^^^ at "
           << filename << ":" << line_number;
    return stream.str();
}

//===========================================================================//
// NOTIMPLEMENTEDEXCEPTION CLASS DEFINITION
//===========================================================================//
/*!
 * \brief Constructor for NotImplementedException when an unimplemented
 *        function is called.
 *
 * \param[in] message  The message to report
 * \param[in] filename The filename where the unimplemented function resides
 * \param[in] line_number The line number where the exception was thrown
 */
NotImplementedException::NotImplementedException(const std::string& message,
                                                 const std::string& filename,
                                                 unsigned int line_number)
    : Base(
        NotImplementedException::buildMessage(message, filename, line_number))
{
    /* * */
}

//---------------------------------------------------------------------------//
// PRIVATE IMPLEMENTATION FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Build the exception message
 *
 * \param[in] message  The message to report
 * \param[in] filename The filename where the unimplemented function resides
 * \param[in] line_number The line number where the exception was thrown
 * \return The constructed exception message
 */
std::string NotImplementedException::buildMessage(const std::string& message,
                                                  const std::string& filename,
                                                  unsigned int line_number)
{
    std::ostringstream stream;
    stream << "Unfortunately, " << message << " is not currently implemented.";
#if YAYP_DBC > 0
    // If DBC is enabled, write the filename and line number where the
    // unimplemented function resides.
    stream << "\n ^^^ at " << filename << ":" << line_number;
#endif
    return stream.str();
}

//===========================================================================//
// NOTREACHABLEEXCEPTION CLASS DEFINITION
//===========================================================================//
/*!
 * \brief Constructor for NotReachableException when a code point that should
 *        be logically unreachable is reached.
 *
 * \param[in] filename    The filename where the unreachable code point is
 *                        located
 * \param[in] line_number The line number where the unreachable code point is
 *                        located.
 */
NotReachableException::NotReachableException(const std::string& filename,
                                             unsigned int       line_number)
    : Base(NotReachableException::buildMessage(filename, line_number))
{
    /* * */
}

//---------------------------------------------------------------------------//
// PRIVATE IMPLEMENTATION FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Build the exception message
 *
 * \param[in] filename  The filename where the unreachable code point is
 *                      located
 * \param[in] line_number The line number where the unreachable code point is
 *                        located.
 * \return The constructed exception message
 */
std::string NotReachableException::buildMessage(const std::string& filename,
                                                unsigned int       line_number)
{
    std::ostringstream stream;
    stream << "Encountered 'unreachable' code point at " << filename << ":"
           << line_number;
    return stream.str();
}

//---------------------------------------------------------------------------//
// HELPER FUNCTIONS
//---------------------------------------------------------------------------//
/*!
 * \brief Throws the DBC exception class.
 *
 * This function provides a fixed point upon which to set a breakpoint in a
 * debugger.
 *
 * \param[in] condition   A string representation of the DBC test that failed
 * \param[in] condition_type  The type of test
 * \param[in] filename        The filename where the exception was thrown
 * \param[in] line_number     The line number where the exception was thrown
 */
void throwDBCException(const std::string& condition,
                       const std::string& condition_type,
                       const std::string& filename,
                       unsigned int       line_number)
{
    throw DBCException(condition, condition_type, filename, line_number);
}

//---------------------------------------------------------------------------//
} // namespace yayp

//---------------------------------------------------------------------------//
// end of src/harness/DBC.cc
//---------------------------------------------------------------------------//
