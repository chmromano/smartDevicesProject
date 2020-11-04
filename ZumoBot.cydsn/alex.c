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
#include "alex.h"

#if 0
int zmain (void)
{
    motor_start();
    Ultra_Start();
    vTaskDelay(1000);
    
    
    while(true) {
        int d = Ultra_GetDistance();
        if (d>10) {
        motor_forward50();
         
    }
    else {
        motor_littleback();
        tank_randomturn();
        motor_forward50();
    }
        printf("distance = %d\r\n", d);
        vTaskDelay(200);
    }  
}
#endif






void tank_turn(int8 speed, uint32 delay)
{
    SetMotors(0,1, speed, speed, delay);
}
void tank_leftturn90(void)
{
    SetMotors(0,1, 25, 25, 1050);
}
void tank_rightturn90(void)
{
    SetMotors(1,0, 25, 25, 1050);
}
void tank_leftturn270(void)
{
    SetMotors(0,1, 25, 25, 1050);
}
void tank_rightturn270(void)
{
    SetMotors(1,0, 25, 25, 1050);
}
void tank_turn180(void)
{
    SetMotors(0,1, 25, 25, 1050);
}
void motor_littleback(void)
{
    SetMotors(1,1, 15, 15, 1000);
}

void tank_randomturn(void)
{
    srand((unsigned)time(NULL));
    int n = rand() % 2;
    if (n==0) {
        tank_leftturn90();
    }
    else {
        tank_rightturn90();
        }
}
void motor_forward50(void)
{
    SetMotors(0,0, 50, 50, 0);
}

/* [] END OF FILE */

