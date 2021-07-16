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
 * @brief  Implementation of ESP8266 Board.
 * @author Gabryel Reyes <gabryelrdiaz@gmail.com>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Board.h"

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

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/** Serial interface baudrate. */
static const uint32_t   SERIAL_BAUDRATE         = 115200U;

/** Digital input pin (arduino pin) for the laser obstacle detection sensor. */
static const uint8_t    SENSOR_DIN_PIN          = 5U;

/** Duration in ms before the MCU will be reset, caused by fatal error halt. */
static const uint32_t   FATAL_ERROR_WAIT_TIME   = 30000U;

/******************************************************************************
 * Public Methods
 *****************************************************************************/

bool Board::begin()
{
    bool isSuccess = true;

    /* Setup serial interface */
    Serial.begin(SERIAL_BAUDRATE);
    Serial.printf("\n");

    /* Prepare sensor input pin */
    pinMode(SENSOR_DIN_PIN, INPUT);

    return isSuccess;
}

bool Board::isRobotDetected()
{
    bool isDetected = false;
    int state = digitalRead(SENSOR_DIN_PIN);

    if (HIGH == state)
    {
        isDetected = true;
    }

    return isDetected;
}

void Board::errorHalt()
{
    delay(FATAL_ERROR_WAIT_TIME);
    ESP.restart();
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External Functions
 *****************************************************************************/

/******************************************************************************
 * Local Functions
 *****************************************************************************/