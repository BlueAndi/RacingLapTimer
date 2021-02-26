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
*   @file Competition.cpp
*   Implementation of Competition.h.
*   @author Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "Competition.h"
#include "Board.h"

/* CONSTANTS *************************************************************************************/

/**
 *  After the first detection of the robot with the ext. sensor, this consider
 *  the duration in ms after that the sensor will be considered again.
 */
static const uint32_t SENSOR_BLIND_PERIOD = 400;

/** Competition start timestamp in ms. */
uint32_t gStartTimestamp = 0;

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

/* VARIABLES *************************************************************************************/

/* PUBLIC METHODES *******************************************************************************/

/**************************************************************************************************/

/**
 *  Default Constructor.
 */
Competition::Competition()
{
}

/**************************************************************************************************/

/**
 *  Default Destructor.
 */
Competition::~Competition()
{
}

/**************************************************************************************************/

/**
 *  Handle the competition state machine, depending on the user input 
 *  from web frontend and sensor input.
 * 
 *  @param outputMessage Message to be sent to Client through Web Socket.
 *  @return success.
 */
bool Competition::handleCompetition(String &outputMessage)
{
    bool success = false;
    uint32_t duration = 0;

    switch (gCompetitionState)
    {
    case COMPETITION_STATE_UNRELEASED:
        /* Don't care about external sensor.
         * User must release the first competition.
         */
        break;

    case COMPETITION_STATE_RELEASED:
        /* React on external sensor. */
        if (true == Board::isRobotDetected())
        {
            gStartTimestamp = millis();
            outputMessage = "EVT;STARTED";
            success = true;
            gCompetitionState = COMPETITION_STATE_STARTED;
        }
        break;

    case COMPETITION_STATE_STARTED:
        duration = millis() - gStartTimestamp;

        /* React on external sensor. */
        if (SENSOR_BLIND_PERIOD <= duration)
        {
            if (true == Board::isRobotDetected())
            {
                outputMessage = String("EVT;FINISHED;") + duration;
                success = true;
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

    return success;
}

/**************************************************************************************************/

/**
 *  Sets gCompetitionState to COMPETITION_STATE_RELEASED.
 * 
 *  @return Success.
 */
bool Competition::setReleasedState()
{
    bool success = false;

    if ((COMPETITION_STATE_UNRELEASED == gCompetitionState) ||
        (COMPETITION_STATE_FINISHED == gCompetitionState))
    {
        success = true;
        gCompetitionState = COMPETITION_STATE_RELEASED;
    }
    return success;
}

/* PROTECTED METHODES ****************************************************************************/

/* PRIVATE METHODES ******************************************************************************/

/* EXTERNAL FUNCTIONS ****************************************************************************/

/* INTERNAL FUNCTIONS ****************************************************************************/