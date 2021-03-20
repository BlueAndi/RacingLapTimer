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
 * @brief  Main entry point
 * @author Andreas Merkle <web@blue-andi.de>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "Board.h"
#include "FlashMem.h"
#include "WIFI.h"
#include "LapTriggerWebServer.h"
#include "Competition.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** WLAN Instance */
static WIFI                 gWlan;

/** Competition Instance */
static Competition          gLapTrigger;

/** WebServer Instance */
static LapTriggerWebServer  gWebServer(gLapTrigger);

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Variables
 *****************************************************************************/

/******************************************************************************
 * External functions
 *****************************************************************************/

/**
 * Setup the system.
 */
void setup() /* cppcheck-suppress unusedFunction */
{
    bool isError = false;

    /* Initialize HAL. */
    if (false == Board::begin())
    {
        Serial.printf("%lu: failed to start Board. \n", millis());
        isError = true;
    }
    /* Mount persistent memory. */
    else if (false == Flash::begin())
    {
        Serial.printf("%lu: Failed to mount persistent memory.\n", millis());
        isError = true;
    }
    /* Start Wireless Connection. */
    else if (false == gWlan.begin())
    {
        Serial.printf("%lu: Failed to start WLAN.\n", millis());
        isError = true;
    }
    /* Mount filesystem. */
    else if (false == LittleFS.begin())
    {
        Serial.printf("%lu: Failed to mount filesystem.\n", millis());
        isError = false;
    }
    /* Start Web Server. */
    else if (false == gWebServer.begin())
    {
        Serial.printf("%lu: Failed to start Web Server.\n", millis());
        isError = true;
    }
    else
    {
        /* Successful */
        ;
    }

    if (true == isError)
    {
        Board::errorHalt();
    }
    else
    {
        Serial.printf("%lu: Ready.\n", millis());
    }
}

/**
 * Main loop, which is called periodically.
 */
void loop() /* cppcheck-suppress unusedFunction */
{
    bool isSuccess = true;

    if (false == gWebServer.runCycle())
    {
        isSuccess = false;
    }
    else if (false == gWlan.runCycle())
    {
        isSuccess = false;
    }
    else
    {
        /* Successful */
        ;
    }

    if (false == isSuccess)
    {
        Board::errorHalt();
    }
}

/******************************************************************************
 * Local functions
 *****************************************************************************/
