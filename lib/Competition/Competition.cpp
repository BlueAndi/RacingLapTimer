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
#include "FlashMem.h"

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

Competition::Competition() : m_startTimestamp(0),
                             m_competitionState(COMPETITION_STATE_UNRELEASED),
                             m_numberOfGroups(0),
                             m_activeGroup(0)
{
    for (uint8_t group = 0; group < MAX_NUMBER_OF_GROUPS; group++)
    {
        m_resultTable[group] = 0;
    }
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
                outputMessage = "EVT;FINISHED;";
                outputMessage += duration;
                outputMessage += ';';
                outputMessage += m_activeGroup;
                isSuccess = true;
                m_competitionState = COMPETITION_STATE_FINISHED;
                updateLapTime(duration);
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

bool Competition::setReleasedState(const uint8_t &activeGroup)
{
    bool isSuccess = false;

    if (MAX_NUMBER_OF_GROUPS - 1 > activeGroup)
    {
        m_activeGroup = activeGroup;
        Serial.printf("Active Group: %d \n", m_activeGroup);

        if ((COMPETITION_STATE_UNRELEASED == m_competitionState) ||
            (COMPETITION_STATE_FINISHED == m_competitionState))
        {
            isSuccess = true;
            m_competitionState = COMPETITION_STATE_RELEASED;
        }
    }

    return isSuccess;
}

bool Competition::getNumberofGroups(uint8_t &groups)
{
    bool isSuccess = false;

    if (Flash::getUInt8(Flash::NVM_GROUPS_ADDRESS, m_numberOfGroups))
    {
        if (MIN_NUMBER_OF_GROUPS > m_numberOfGroups)
        {
            m_numberOfGroups = MIN_NUMBER_OF_GROUPS;
        }
        else if (MAX_NUMBER_OF_GROUPS < m_numberOfGroups)
        {
            m_numberOfGroups = MAX_NUMBER_OF_GROUPS;
        }

        groups = m_numberOfGroups;
        isSuccess = true;
    }

    return isSuccess;
}

bool Competition::setNumberofGroups(const uint8_t &groups)
{
    bool isSuccess = false;

    uint8_t validGroups = 0;

    if (MIN_NUMBER_OF_GROUPS > groups)
    {
        validGroups = MIN_NUMBER_OF_GROUPS;
    }
    else if (MAX_NUMBER_OF_GROUPS < groups)
    {
        validGroups = MAX_NUMBER_OF_GROUPS;
    }
    else
    {
        validGroups = groups;
    }

    if (Flash::setUInt8(Flash::NVM_GROUPS_ADDRESS, validGroups))
    {
        m_numberOfGroups = validGroups;
        isSuccess = true;
    }

    return isSuccess;
}

void Competition::getLaptime(uint32_t &output, const uint8_t group)
{
    output = m_resultTable[group];
}

/******************************************************************************
 * Protected Methods
 *****************************************************************************/

/******************************************************************************
 * Private Methods
 *****************************************************************************/

void Competition::updateLapTime(uint32_t &runtime)
{
    if ((runtime < m_resultTable[m_activeGroup]) ||
        (0 == m_resultTable[m_activeGroup]))
    {
        m_resultTable[m_activeGroup] = runtime;
    }

    for (uint8_t group = 0; group < m_numberOfGroups; group++)
    {
        Serial.printf("Group %d: %d \n", group, m_resultTable[group]);
    }
}

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/
