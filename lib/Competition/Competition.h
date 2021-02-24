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