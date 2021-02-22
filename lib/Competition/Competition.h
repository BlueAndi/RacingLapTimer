/**************************************************************************************************
* File: Competition.h
* @brief: Competition Handler
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
#ifndef COMPETITION_H_
#define COMPETITION_H_

/* INCLUDES **************************************************************************************/
#include <Arduino.h>

/* CONSTANTS *************************************************************************************/

/* FORWARD DECLARATIONS **************************************************************************/

/* INLINE FUNCTIONS ******************************************************************************/

/* PROTOTYPES ************************************************************************************/

class Competition
{
public:
    /** Competition states */
    typedef enum
    {
        COMPETITION_STATE_UNRELEASED = 0, /**< Competition is not released yet */
        COMPETITION_STATE_RELEASED,       /**< Competition is released, but not started yet. */
        COMPETITION_STATE_STARTED,        /**< Competition is started */
        COMPETITION_STATE_FINISHED        /**< Competition is finished */

    } CompetitionState;

    Competition();            /**< Default Constructor */
    ~Competition();           /**< Default Destructor */
    bool begin();             /**< Initialization of Module */
    bool handleCompetition(String &outputMessage); /**< Handle the competition state machine */
    bool setReleasedState();  /**< Sets gCompetitionState to COMPETITION_STATE_RELEASED */
private:
    /** Current competition state */
    CompetitionState gCompetitionState = COMPETITION_STATE_UNRELEASED;

    /**
    * After the first detection of the robot with the ext. sensor, this consider
    * the duration in ms after that the sensor will be considered again.
    */
    const uint32_t SENSOR_BLIND_PERIOD = 400;

    /** Competition start timestamp in ms */
    uint32_t gStartTimestamp = 0;
};

#endif /*COMPETITION_H_*/