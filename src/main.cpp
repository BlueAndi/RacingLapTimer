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
#include "WLAN.h"
#include "WebServer.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/** Competition states */
typedef enum
{
    COMPETITION_STATE_UNRELEASED = 0, /**< Competition is not released yet */
    COMPETITION_STATE_RELEASED,       /**< Competition is released, but not started yet. */
    COMPETITION_STATE_STARTED,        /**< Competition is started */
    COMPETITION_STATE_FINISHED        /**< Competition is finished */

} CompetitionState;

/******************************************************************************
 * Prototypes
 *****************************************************************************/
static void handleCompetition();

/******************************************************************************
 * Variables
 *****************************************************************************/

/**
 * After the first detection of the robot with the ext. sensor, this consider
 * the duration in ms after that the sensor will be considered again.
 */
static const uint32_t SENSOR_BLIND_PERIOD = 400;

/** Competition start timestamp in ms */
static uint32_t gStartTimestamp = 0;

/** Current competition state */
static CompetitionState gCompetitionState = COMPETITION_STATE_UNRELEASED;

/** WLAN Instance */
static WLAN wlan;

/** WebServer Instance */
static Server webserver;

/******************************************************************************
 * External functions
 *****************************************************************************/

/**
 * Setup the system.
 */
void setup()
{
    bool isError = false;

    if (false == Board::begin())
    {
        Serial.printf("%lu: failed to start Board. \n", millis());
        isError = true;
    }
    else if (false == wlan.begin())
    {
        Serial.printf("%lu: Failed to start AP.\n", millis());
        isError = true;
    }
    else if (false == webserver.begin())
    {
        Serial.printf("%lu: Failed to start AP.\n", millis());
        isError = true;
    }

    if (true == isError)
    {
        Board::errorHalt();
    }
    else
    {
        Serial.printf("%lu: Ready.\n", millis());
    }

    return;
}

/**
 * Main loop, which is called periodically.
 */
void loop()
{
    webserver.cycle();
    handleCompetition();
}

/******************************************************************************
 * Local functions
 *****************************************************************************/



/**
 * Handle the competition state machine, depending on the user input from
 * web frontend and sensor input.
 */
static void handleCompetition()
{
    uint32_t duration = 0;

    switch (gCompetitionState)
    {
    case COMPETITION_STATE_UNRELEASED:
        /* Don't care about external sensor.
         * User must release the first competition.
         */
        break;

    case COMPETITION_STATE_RELEASED:
        /* React on external sensor */
        if (true == Board::isRobotDetected())
        {
            gStartTimestamp = millis();
            webserver.WS_textAll("EVT;STARTED");
            gCompetitionState = COMPETITION_STATE_STARTED;
        }
        break;

    case COMPETITION_STATE_STARTED:
        duration = millis() - gStartTimestamp;

        /* React on external sensor */
        if (SENSOR_BLIND_PERIOD <= duration)
        {
            if (true == Board::isRobotDetected())
            {
                webserver.WS_textAll(String("EVT;FINISHED;") + duration);
                gCompetitionState = COMPETITION_STATE_FINISHED;
            }
        }
        break;

    case COMPETITION_STATE_FINISHED:
        /* Don't care about external sensor.
         * User must release next competition.
         */
        break;

    default:
        break;
    }
}
