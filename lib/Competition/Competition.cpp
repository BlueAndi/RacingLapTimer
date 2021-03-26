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
 * @brief  Competition Handler.
 * @author Gabryel Reyes <gabryelrdiaz@gmail.com>
 */

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "Competition.h"
#include "Board.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Compiler Switches
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/******************************************************************************
 * Local Variables
 *****************************************************************************/

/******************************************************************************
 * Public Methods
 *****************************************************************************/

Competition::Competition() :
    m_startTimestamp(0),
    m_competitionState(COMPETITION_STATE_UNRELEASED),
    m_numberOfGroups(10)
{
}

Competition::~Competition()
{
}

bool Competition::handleCompetition(String &outputMessage)
{
    bool isSuccess = false;
    uint32_t duration = 0;

    switch (m_competitionState)
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
            m_startTimestamp = millis();
            outputMessage = "EVT;STARTED";
            isSuccess = true;
            m_competitionState = COMPETITION_STATE_STARTED;
        }
        break;

    case COMPETITION_STATE_STARTED:
        duration = millis() - m_startTimestamp;

        /* React on external sensor. */
        if (SENSOR_BLIND_PERIOD <= duration)
        {
            if (true == Board::isRobotDetected())
            {
                outputMessage = String("EVT;FINISHED;") + duration;
                isSuccess = true;
                m_competitionState = COMPETITION_STATE_FINISHED;
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

    return isSuccess;
}

bool Competition::setReleasedState()
{
    bool isSuccess = false;

    if ((COMPETITION_STATE_UNRELEASED == m_competitionState) ||
        (COMPETITION_STATE_FINISHED == m_competitionState))
    {
        isSuccess = true;
        m_competitionState = COMPETITION_STATE_RELEASED;
    }

    return isSuccess;
}

bool Competition::getNumberofGroups(uint8_t &groups)
{
    bool isSuccess = false;
    if(0 != m_numberOfGroups)
    {
        groups = m_numberOfGroups;
        isSuccess = true;
    }
    return isSuccess;
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/
