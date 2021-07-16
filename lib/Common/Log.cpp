/* MIT License
 *
 * Copyright (c) 2020-2021 Andreas Merkle <web@blue-andi.de>
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

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Log.h"

/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and classes
 *****************************************************************************/

/******************************************************************************
 * Prototypes
 *****************************************************************************/

static void printHead(const char* filename, int lineNumber, Log::Level level);

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/******************************************************************************
 * Public Methods
 *****************************************************************************/

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

void Log::print(const char* filename, int lineNumber, Log::Level level, const char* format, ...)
{
    const size_t    MESSAGE_BUFFER_SIZE = 512;
    char            buffer[MESSAGE_BUFFER_SIZE];
    va_list         args;

    printHead(filename, lineNumber, level);

    va_start(args, format);
    vsnprintf(buffer, MESSAGE_BUFFER_SIZE, format, args);
    va_end(args);

    Serial.printf("%s\n", buffer);
}

void Log::print(const char* filename, int lineNumber, Log::Level level, const String& msg)
{
    printHead(filename, lineNumber, level);

    Serial.printf("%s\n", msg.c_str());
}

/******************************************************************************
 * Local Functions
 *****************************************************************************/

static void printHead(const char* filename, int lineNumber, Log::Level level)
{
    const char* levelStr = "Unknown";

    switch(level)
    {
    case Log::LOG_INFO:
        levelStr = "Info";
        break;

    case Log::LOG_WARNING:
        levelStr = "Warning";
        break;

    case Log::LOG_ERROR:
        levelStr = "Error";
        break;

    case Log::LOG_FATAL:
        levelStr = "Fatal";
        break;

    default:
        break;
    }

    Serial.printf("%6lu %16s (%5u) %7s ", millis(), filename, lineNumber, levelStr);
}