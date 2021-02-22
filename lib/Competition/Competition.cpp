/**************************************************************************************************
* File: Competition.cpp
* @brief: Implementation of Competition.h
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "Competition.h"
#include "Board.h"

/* CONSTANTS *************************************************************************************/

/**
* After the first detection of the robot with the ext. sensor, this consider
* the duration in ms after that the sensor will be considered again.
*/
static const uint32_t SENSOR_BLIND_PERIOD = 400;

/** Competition start timestamp in ms */
uint32_t gStartTimestamp = 0;

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

/* VARIABLES *************************************************************************************/

/* PUBLIC METHODES *******************************************************************************/

/**************************************************************************************************/

/**
*   Default Constructor
*/
Competition::Competition()
{
}

/**************************************************************************************************/

/**
*   Default Destructor
*/
Competition::~Competition()
{
}

/**************************************************************************************************/

/**
 * Handle the competition state machine, depending on the user input from
 * web frontend and sensor input.
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
        /* React on external sensor */
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

        /* React on external sensor */
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
*   Sets gCompetitionState to COMPETITION_STATE_RELEASED
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