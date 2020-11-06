/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "chris.h"

#if 0
//Function for week 3 assignment 2
#endif

#if 1
//function to test turns
//motor_turn(100, 30, 2993); gives a 360 degree turn
void zmain(void){
    motor_start();
    motor_turn(10, 3, 2993);
    motor_stop();
    
    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
}
#endif
/* [] END OF FILE */
