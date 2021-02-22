/**************************************************************************************************
* File: Board.cpp
* @brief: Implementation of Board.h
* @author: Gabryel Reyes <gabryelrdiaz@gmail.com>
**************************************************************************************************/
/* INCLUDES **************************************************************************************/
#include "Board.h"

/* CONSTANTS *************************************************************************************/

/** Serial interface baudrate. */
static const uint32_t SERIAL_BAUDRATE = 115200U;

/** Digital input pin (arduino pin) for the laser obstacle detection sensor. */
static const uint8_t SENSOR_DIN_PIN = 5;

/* MACROS ****************************************************************************************/

/* TYPES *****************************************************************************************/

/* PROTOTYPES ************************************************************************************/

/* VARIABLES *************************************************************************************/

/* PUBLIC METHODES *******************************************************************************/

/**************************************************************************************************/

/**
*   Board Initialization
*   @return success
*/
bool Board::begin()
{
    bool success = true;

    /* Setup serial interface */
    Serial.begin(SERIAL_BAUDRATE);

    /* Prepare sensor input pin */
    pinMode(SENSOR_DIN_PIN, INPUT);

    return success;
}

/**************************************************************************************************/

/**
 * Is a roboter detected or not?
 *
 * @return If a roboter is detected, it will return true otherwise false.
 */
bool Board::isRobotDetected()
{
    bool isDetected = false;
    int state = digitalRead(SENSOR_DIN_PIN);

    if (HIGH == state)
    {
        isDetected = true;
    }

    return isDetected;
}

/**************************************************************************************************/

/**
*   Halts the device for 30 Seconds and restarts
*/
void Board::errorHalt()
{
    Serial.println("Critical Error. Wait for restart (30 Seconds)");
    delay(30000);
    ESP.restart();
}

/* PROTECTED METHODES ****************************************************************************/

/* PRIVATE METHODES ******************************************************************************/

/* EXTERNAL FUNCTIONS ****************************************************************************/

/* INTERNAL FUNCTIONS ****************************************************************************/