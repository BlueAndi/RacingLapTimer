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

#ifndef COMPETITION_H_
#define COMPETITION_H_

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <Arduino.h>

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
     *  Default Constructor.
     */
    Competition();

    /**
     *  Default Destructor.
     */
    ~Competition();

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
    bool setReleasedState(const uint8_t &activeGroup);

    /**
     *  Checks if the number of Groups is valid, and returns it.
     * 
     *  @param[out] groups Variable to write the number of groups to. 
     *  @return If the number of Groups is bigger than 0, returns true. Otherwise, false.
     */
    bool getNumberofGroups(uint8_t &groups);

    /**
     *  Saves the configured number of Groups
     * 
     *  @param[in] groups Client's number of Groups 
     *  @return If the number of Groups successfully saved, returns true. Otherwise, false.
     */
    bool setNumberofGroups(const uint8_t &groups);

    /**
    *   Retrieves the laptime from a group.
    * 
    *   @param output Buffer to write the Group's laptime to.
    *   @param group Number of Group to retrieve value for.
    */
    void getLaptime(uint32_t &output, const uint8_t group);

private:
    /**
     *  Updates fastest Lap Time of the currently Active Group.
     *
     *  @param[in] runtime Duration of Competition Lap
     */
    void updateLapTime(uint32_t &runtime);

    /**
     *  After the first detection of the robot with the ext. sensor, this consider
     *  the duration in ms after that the sensor will be considered again.
     */
    static const uint32_t SENSOR_BLIND_PERIOD = 400;

    /**
     *  Maximum Number of Participating Groups 
     */
    static const uint8_t MAX_NUMBER_OF_GROUPS = 10;

    /** Competition start timestamp in ms. */
    uint32_t m_startTimestamp;

    /** Current competition state. */
    CompetitionState m_competitionState;

    /** Number of Groups */
    uint8_t m_numberOfGroups;

    /** Array to contain the fastest time of each group */
    uint32_t m_resultTable[MAX_NUMBER_OF_GROUPS];

    /** Group that has been RELEASED for the run */
    uint8_t m_activeGroup;
};

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Variables
 *****************************************************************************/

/******************************************************************************
 * External functions
 *****************************************************************************/

/******************************************************************************
 * Local functions
 *****************************************************************************/

#endif /* COMPETITION_H_ */