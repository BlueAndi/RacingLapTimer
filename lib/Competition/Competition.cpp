/* MIT License
 *
 * Copyright (c) 2020-2022 Andreas Merkle <web@blue-andi.de>
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
#include "Settings.h"

#include <Log.h>

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

bool Competition::begin()
{
    Settings::getInstance().getNumberOfGroups(m_numberOfGroups);

    if (MIN_NUMBER_OF_GROUPS > m_numberOfGroups)
    {
        m_numberOfGroups = MIN_NUMBER_OF_GROUPS;
    }
    else if (MAX_GROUPS < m_numberOfGroups)
    {
        m_numberOfGroups = MAX_GROUPS;
    }

    if (nullptr != m_groups)
    {
        uint8_t idx = 0;

        for(idx = 0; idx < m_numberOfGroups; ++idx)
        {
            String name;
            Settings::getInstance().getGroupName(idx, name);

            m_groups[idx].setName(name);
        }
    }

    return true;
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

bool Competition::setReleasedState(uint8_t activeGroup)
{
    bool isSuccess = false;

    if ((MAX_GROUPS - 1) > activeGroup)
    {
        m_activeGroup = activeGroup;

        LOG_INFO("Active group: %u", m_activeGroup);

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
    groups = m_numberOfGroups;

    return true;
}

bool Competition::setNumberofGroups(uint8_t groups)
{
    uint8_t validGroups = 0;

    if (MIN_NUMBER_OF_GROUPS > groups)
    {
        validGroups = MIN_NUMBER_OF_GROUPS;
    }
    else if (MAX_GROUPS < groups)
    {
        validGroups = MAX_GROUPS;
    }
    else
    {
        validGroups = groups;
    }

    if (validGroups != m_numberOfGroups)
    {
        Settings::getInstance().setNumberOfGroups(validGroups);

        if ((nullptr != m_groups) &&
            (validGroups > m_numberOfGroups))
        {
            uint8_t idx = 0;
            
            for(idx = m_numberOfGroups; idx < validGroups; ++idx)
            {
                m_groups[idx].setName("");
                m_groups[idx].setFastestLapTime(0);
            }
        }

        m_numberOfGroups = validGroups;
    }

    return true;
}

uint32_t Competition::getLaptime(uint8_t group)
{
    uint32_t result = 0;

    if ((nullptr != m_groups) &&
        (m_numberOfGroups > group))
    {
        result = m_groups[group].getfastestLapTime();
    }

    return result;
}

bool Competition::clearLaptime(uint8_t group)
{
    bool isSuccess = false;

    if ((nullptr != m_groups) &&
        (m_numberOfGroups > group))
    {
        m_groups[group].setFastestLapTime(0);
        isSuccess = true;
    }

    return isSuccess;
}

bool Competition::setGroupName(uint8_t group, const String &groupName)
{
    bool isSuccess = false;

    if (nullptr != m_groups)
    {
        m_groups[group].setName(groupName);

        Settings::getInstance().setGroupName(group, groupName);

        isSuccess = true;
    }

    return isSuccess;
}

bool Competition::getGroupName(uint8_t group, String &groupName)
{
    bool isSuccess = false;

    if (nullptr != m_groups)
    {
        groupName = m_groups[group].getName();
        isSuccess = true;
    }

    return isSuccess;
}

bool Competition::clearName(uint8_t group)
{
    return setGroupName(group, "");
}

bool Competition::rejectRun()
{
    bool isSuccess = false;

    if (nullptr != m_groups)
    {
        m_groups[m_lastRunGroup].setFastestLapTime(m_lastRunLapTime);
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

void Competition::updateLapTime(uint32_t lapTime)
{
    m_lastRunGroup      = m_activeGroup;
    m_lastRunLapTime    = m_groups[m_activeGroup].getfastestLapTime();

    m_groups[m_activeGroup].setLapTimeIfFaster(lapTime);

    for (uint8_t group = 0; group < m_numberOfGroups; group++)
    {
        LOG_INFO("Group %u: %u", group, m_groups[group].getfastestLapTime());
    }
}

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/
