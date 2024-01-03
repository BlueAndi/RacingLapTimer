/* MIT License
 *
 * Copyright (c) 2020 - 2024 Andreas Merkle <web@blue-andi.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*******************************************************************************
    DESCRIPTION
*******************************************************************************/
/**
 * @brief  Logger for debug purposes
 * @author Andreas Merkle <web@blue-andi.de>
 */

#ifndef LOG_H_
#define LOG_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>

/******************************************************************************
 * Macros
 *****************************************************************************/

/** Log information, which is useful for the normal user. */
#define LOG_INFO(...)       Log::print(__FILE__, __LINE__, Log::LOG_INFO, __VA_ARGS__)

/** Log warning in case there is something the user should know, but the system can continoue without limitation. */
#define LOG_WARNING(...)    Log::print(__FILE__, __LINE__, Log::LOG_WARNING, __VA_ARGS__)

/** Log error in case a failure happened, but the system can continoue with limitation. */
#define LOG_ERROR(...)      Log::print(__FILE__, __LINE__, Log::LOG_ERROR, __VA_ARGS__)

/** Log fatal error in case a failure happened and the system can not continoue. */
#define LOG_FATAL(...)      Log::print(__FILE__, __LINE__, Log::LOG_FATAL, __VA_ARGS__)

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 *  Logging functionality
 */
namespace Log
{

/** Supported log levels. */
typedef enum
{
    LOG_INFO = 0,   /**< Level: Information */
    LOG_WARNING,    /**< Level: Warning */
    LOG_ERROR,      /**< Level: Error */
    LOG_FATAL       /**< Level: Fatal error */

} Level;

/******************************************************************************
 * Functions
 *****************************************************************************/

/**
 * Print log message.
 * 
 * @param[in] filename      The name of the file, where the log message is located.
 * @param[in] lineNumber    The line number in the file, where the log message is located.
 * @param[in] level         The severity level.
 * @param[in] format        The format with the variable argument list.
 */
void print(const char* filename, int lineNumber, Level level, const char* format, ...);

/**
 * Print log message.
 * 
 * @param[in] filename      The name of the file, where the log message is located.
 * @param[in] lineNumber    The line number in the file, where the log message is located.
 * @param[in] level         The severity level.
 * @param[in] msg           The log message content.
 */
void print(const char* filename, int lineNumber, Level level, const String& msg);

};

#endif /* LOG_H_ */