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

#ifndef COMPETITION_H_
#define COMPETITION_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>
#include "Group.h"

/******************************************************************************
 * Macros
 *****************************************************************************/

/******************************************************************************
 * Types and Classes
 *****************************************************************************/

/**
 *  Competition Handler Class.
 */
class Competition
{
public:
    /**
     *  Competition states.
     */
    typedef enum
    {
        COMPETITION_STATE_UNRELEASED = 0, /**< Competition is not released yet. */
        COMPETITION_STATE_RELEASED,       /**< Competition is released, but not started yet. */
        COMPETITION_STATE_STARTED,        /**< Competition is started. */
        COMPETITION_STATE_FINISHED        /**< Competition is finished. */

    } CompetitionState;

    /**
     * Constructs the competition.
     * 
     * @param[in] groups    List of with max. supported groups.
     * @param[in] maxGroups Max. number of groups in the list.
     */
    Competition(Group* groups, size_t maxGroups) :
        m_groups(groups),
        MAX_GROUPS(maxGroups),
        m_lastRunGroup(0),
        m_lastRunLapTime(0),
        m_startTimestamp(0),
        m_competitionState(COMPETITION_STATE_UNRELEASED),
        m_numberOfGroups(0),
        m_activeGroup(0)
    {
    }

    /**
     * Destroys the competition.
     */
    ~Competition()
    {
    }

    /**
     * Initialize the competition by loading settings.
     * 
     * @return If successful, it will return true otherwise false.
     */
    bool begin();

    /**
     *  Handle the competition state machine, depending on the user input 
     *  from web frontend and sensor input.
     * 
     *  @param[out] outputMessage Message to be sent to Client through Web Socket.
     *  @return If robot is detected during the correct competition state, returns true. Otherwise, false
     */
    bool handleCompetition(String &outputMessage);

    /**
     *  Checks the current Competition state to be either Unreleased or Finished. 
     *  Releases the competition if found in any of these states.
     * 
     *  @param[in] activeGroup Number of currently Active Group in Client.
     *  @return If competition is released, returns true. Otherwise, false.
     */
    bool setReleasedState(uint8_t activeGroup);

    /**
     *  Retrieves if the number of Groups is valid, and returns it.
     * 
     *  @param[out] groups Variable to write the number of groups to. 
     *  @return If the number of Groups has been succesfully retrieved, returns true. Otherwise, false.
     */
    bool getNumberofGroups(uint8_t &groups);

    /**
     *  Sets the configured number of Groups
     * 
     *  @param[in] groups Client's number of Groups 
     *  @return If the number of Groups successfully set, returns true. Otherwise, false.
     */
    bool setNumberofGroups(uint8_t groups);

    /**
     *   Retrieves the laptime from a group.
     *   @param[in] group Number of Group to retrieve value for.
     *   @return If number of group is valid, returns the saved laptime. Else, returns 0.
     */
    uint32_t getLaptime(uint8_t group);

    /**
     *  Sets the Laptime of the selected group to 0 as a default value.
     * 
     *  @param[in] group Number of Group to clear the laptime for.
     *  @return If succesfully cleared returns true. Otherwise, false.
     */
    bool clearLaptime(uint8_t group);

    /**
     *  Sets the Name of the selected group
     *
     *  @param[in] group Number of Group to set the name for.
     *  @param[in] groupName Chosen Name of the group
     *  @return If the name of the groups is successfully set, returns true. Otherwise, false.
     */
    bool setGroupName(uint8_t group, const String &groupName);

    /**
     *  Retrieves the name of the selected group
     *  
     *  @param[in] group Number of Group to get Name for.
     *  @param[out] groupName Name of selected group.
     *  @return  If the name of the groups is successfully retrieved, returns true. Otherwise, false.
     */
    bool getGroupName(uint8_t group, String &groupName);

    /**
     *  Sets the Name of the selected group to "" as a default value.
     * 
     *  @param[in] group Number of Group to clear the Name for.
     *  @return If succesfully cleared returns true. Otherwise, false.
     */
    bool clearName(uint8_t group);

    /**
     *  Rejects the last fastest time of a group, reverting it to its previous fastest time.
     *  
     *  @return If succesfully rolled back returns true, Otherwise, false.
     */
    bool rejectRun();

private:
    /**
     *  Updates fastest Lap Time of the currently Active Group.
     *
     *  @param[in] lapTime Duration of Competition Lap
     */
    void updateLapTime(uint32_t lapTime);

    /**
     *  After the first detection of the robot with the ext. sensor, this consider
     *  the duration in ms after that the sensor will be considered again.
     */
    static const uint32_t SENSOR_BLIND_PERIOD   = 400;

    /**
     *  Minimum Number of Participating Groups 
     */
    static const uint8_t MIN_NUMBER_OF_GROUPS   = 1;

    /** List of max. supported groups. Not all may participate in the competition. */
    Group*              m_groups;

    /** Max. number of groups, who can participate. */
    const size_t        MAX_GROUPS;

    /** The id of the group which did the last run. */
    size_t              m_lastRunGroup;

    /** The lap time in ms of the last run. */
    uint32_t            m_lastRunLapTime;

    /** Competition start timestamp in ms. */
    uint32_t            m_startTimestamp;

    /** Current competition state. */
    CompetitionState    m_competitionState;

    /** Number of Groups */
    uint8_t             m_numberOfGroups;

    /** Group that has been RELEASED for the run */
    uint8_t             m_activeGroup;

    /* Default constructor not allowed. */
    Competition();
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* COMPETITION_H_ */