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
 * @brief  A group who participate in the competition.
 * @author Andreas Merkle <web@blue-andi.de>
 */

#ifndef GROUP_H_
#define GROUP_H_

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
 * A group or in other words a team which takes part in the challenge.
 * It shall contain all group relevant informations.
 */
class Group
{
public:

    /**
     * Constructs a group with a empty name.
     */
    Group() :
        m_name(),
        m_fastestLapTime(0)
    {
    }

    /**
     * Destroys the group.
     */
    ~Group()
    {
    }

    /**
     * Get the group name.
     * 
     * @return Group name 
     */
    String getName() const
    {
        return m_name;
    }

    /**
     * Set the group name.
     * 
     * @param[in] name  The name of the group.
     */
    void setName(const String& name)
    {
        m_name = name;
    }

    /**
     * Get the fastest lap time in ms.
     * 
     * @return Lap time in ms 
     */
    uint32_t getfastestLapTime() const
    {
        return m_fastestLapTime;
    }

    /**
     * Set the fastest lap time in ms.
     * 
     * @param[in] lapTime   The fastest lap time in ms.
     */
    void setFastestLapTime(uint32_t lapTime)
    {
        m_fastestLapTime = lapTime;
    }

    /**
     * Set lap time only, if it is faster than the current one.
     * If the current lap time is 0, it will be set in any case.
     * 
     * @param[in] lapTime   The lap time in ms.
     */
    void setLapTimeIfFaster(uint32_t lapTime)
    {
        if ((0 == m_fastestLapTime) ||
            (lapTime < m_fastestLapTime))
        {
            m_fastestLapTime = lapTime;
        }
    }

private:

    String      m_name;             /**< The name of the group, must not be unique. */
    uint32_t    m_fastestLapTime;   /**< The fastest lap time in ms. */
};

/******************************************************************************
 * Functions
 *****************************************************************************/

#endif /* GROUP_H_ */