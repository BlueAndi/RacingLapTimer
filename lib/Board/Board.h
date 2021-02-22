/**************************************************************************************************
* File: Board.h
* @brief: Implementation of ESP8266 Board
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
#ifndef BOARD_H_
#define BOARD_H_

/* INCLUDES **************************************************************************************/
#include <Arduino.h>

/* CONSTANTS *************************************************************************************/

/* FORWARD DECLARATIONS **************************************************************************/

/* INLINE FUNCTIONS ******************************************************************************/

/* PROTOTYPES ************************************************************************************/

namespace Board
{
    bool begin();           /**< Board Initialization  */
    bool isRobotDetected(); /**< Is a roboter detected or not? */
    void errorHalt();       /**< Halts the device for 30 Seconds and restarts */
};

#endif /*BOARD_H_*/