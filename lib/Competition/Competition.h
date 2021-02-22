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

/**************************************************************************************************/

/**
*   Competition Handler Class
*/
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

    Competition();                                 /**< Default Constructor */
    ~Competition();                                /**< Default Destructor */
    bool handleCompetition(String &outputMessage); /**< Handle the competition state machine */
    bool setReleasedState();                       /**< Sets gCompetitionState to COMPETITION_STATE_RELEASED */
private:
    /** Current competition state */
    CompetitionState gCompetitionState = COMPETITION_STATE_UNRELEASED;
};

#endif /*COMPETITION_H_*/