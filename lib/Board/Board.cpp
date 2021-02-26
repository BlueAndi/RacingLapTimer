/* MIT License
 *
 * Copyright (c) 2020 Andreas Merkle <web@blue-andi.de>
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
/**************************************************************************************************
*   @file: Board.cpp
*   Implementation of Board.h.
*   @author Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "Board.h"

/* CONSTANTS *************************************************************************************/

/** Serial interface baudrate. */
static const uint32_t SERIAL_BAUDRATE = 115200U;

/** Digital input pin (arduino pin) for the laser obstacle detection sensor. */
static const uint8_t SENSOR_DIN_PIN = 5;

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

/* VARIABLES *************************************************************************************/

/* PUBLIC METHODES *******************************************************************************/

/**************************************************************************************************/

/**
 *  Board Initialization.
 * 
 *  @return success.
 */
bool Board::begin()
{
    bool success = true;

    /* Setup serial interface */
    Serial.begin(SERIAL_BAUDRATE);

    /* Prepare sensor input pin */
    pinMode(SENSOR_DIN_PIN, INPUT);

    return success;
}

/**************************************************************************************************/

/**
 *  Is a roboter detected or not?.
 * 
 *  @return true if Robot is detected. False otherwise.
 */
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

/**************************************************************************************************/

/**
 *  Halts the device for 30 Seconds and restarts the device.
 */
void Board::errorHalt()
{
    Serial.println("Critical Error. Wait for restart (30 Seconds)");
    delay(30000);
    ESP.restart();
}

/* PROTECTED METHODES ****************************************************************************/

/* PRIVATE METHODES ******************************************************************************/

/* EXTERNAL FUNCTIONS ****************************************************************************/

/* INTERNAL FUNCTIONS ****************************************************************************/