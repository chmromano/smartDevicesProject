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
//week 4 assigment 2
#if 0
void zmain(void)
{
    struct sensors_ dig;
    int count = 0;
    int line = 0;
    int stop = 2;

    reflectance_set_threshold(9000, 9000, 11000, 11000, 9000, 9000);

    reflectance_start();
    IR_Start();
    motor_start();
    motor_forward(0, 0);

    while (SW1_Read())
        ;
    BatteryLed_Write(1);
    vTaskDelay(1000);
    BatteryLed_Write(0);
    motor_forward(200, 0);

    while (count < stop)
    {
        reflectance_digital(&dig);

        if (dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1)
        {
            count++;
            if (count == 1)
            {
                line = 1;
            }
            if (count == 2)
            {
                line = 2;
            }
            
            while ((dig.L3 == 1 && dig.L2 == 1 && dig.L1 == 1 && dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1) && count < stop)
            {
                reflectance_digital(&dig);
            }
        }
        //stop at first line and wait for IR signal
        if (line == 1)
        {
            motor_forward(0, 0);
            IR_wait();
            motor_forward(100, 0);
            line = 0;
        }
        // moving foward at speed of 255 if L1 and L2 detect black
        if (dig.L1 == 1 && dig.R1 == 1)
        {

            motor_forward(255, 0);
        }
        // turning right at speed of 255 if L1 detect white and L2 detect black
        if (dig.L1 == 0 && dig.R1 == 1)
        {

            SetMotors(0, 0, 255, 0, 0);
        }
        // turning left at speed of 255 if L1 detect black and L2 detect white
        if (dig.L1 == 1 && dig.R1 == 0)
        {

            SetMotors(0, 0, 0, 255, 0);
        }
        
        
    }

    motor_forward(0, 0);
    motor_stop();

    while (true)
    {
        vTaskDelay(100);
    }
}
#endif




//week 4 assignment 3

#if 0
void zmain (void)
{
    struct sensors_ dig;
    int count = 0;
    int line = 0;
    int stop = 5;
   
    
    reflectance_set_threshold(9000,9000,11000,11000,9000,9000);
    
    reflectance_start();
    IR_Start();
    motor_start();
    motor_forward(0,0);
    
    while(SW1_Read());
    BatteryLed_Write(1);
    vTaskDelay(1000);
    BatteryLed_Write(0);
    motor_forward(200,0);
    
    while(count < stop) {
        reflectance_digital(&dig);
        
        if(dig.L3 ==1 && dig.L2 ==1 && dig.L1 ==1 && dig.R1 ==1 && dig.R2 ==1 && dig.R3 ==1) {
            count++;
            if (count == 1){
                line = 1;
            
            }
            if (count == 2){
                line = 2;
            
            }
            if (count == 3){
                line = 3;
            
            }if (count == 4){
                line = 4;
            
            }
            while((dig.L3 ==1 && dig.L2 ==1 && dig.L1 ==1 && dig.R1 ==1 && dig.R2 ==1 && dig.R3 ==1)&& count < stop) {
                reflectance_digital(&dig);
            }
        }
        if( dig.L1 ==1 && dig.R1 ==1){
                
                motor_forward(50,0);
            }
        if( dig.L1 ==0 && dig.R1 ==1){
                
                SetMotors(0,0,10,0,0);
            }
        if( dig.L1 ==1 && dig.R1 ==0){
                
                SetMotors(0,0,0,10,0);
            }
        //wait IR signal at first line
        if(line == 1){
            motor_forward(0,0);
            IR_wait();
            motor_forward(100,0);
            line = 0;
        }
        //second line turn left
        if(line == 2){
            //give motor a inital short abrupt left turn with delay of 250
            SetMotors(0,0,0,100,250);
            //let motor continue turn left with no delay, and turning motion will stop as long as sensors L1 and R1 detect black line.
            SetMotors(0,0,0,20,0);
            line = 0;
        }
        //third line turn right
        if(line == 3){
            SetMotors(0,0,100,0,250);
            SetMotors(0,0,20,0,0);
            line = 0;
        }
        //fourth line turn right
        if(line == 4){
            SetMotors(0,0,100,0,250);
            SetMotors(0,0,20,0,0);
            line = 0;
        }
    }
    
    motor_forward(0,0);
    motor_stop();
    
    
    while(true)
    {
        vTaskDelay(100); 
    }
    
}
#endif
//week 3 assignment 3
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






// tank turn
void tank_turn(int8 speed, uint32 delay)
{
    SetMotors(0,1, speed, speed, delay);
}
//left 90 degree tank turn 
void tank_leftturn90(void)
{
    SetMotors(0,1, 25, 25, 1050);
}
//right 90 degree tank turn
void tank_rightturn90(void)
{
    SetMotors(1,0, 25, 25, 1050);
}
//left 270 degree tank turn 
void tank_leftturn270(void)
{
    SetMotors(0,1, 25, 25, 1050);
}
//right 270 degree tank turn 
void tank_rightturn270(void)
{
    SetMotors(1,0, 25, 25, 1050);
}
//180 degree tank turn
void tank_turn180(void)
{
    SetMotors(0,1, 25, 25, 1050);
}
// tank moving little back
void motor_littleback(void)
{
    SetMotors(1,1, 15, 15, 1000);
}
// tank randomtrunturn
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
// tank moving foward at speed of 50
void motor_forward50(void)
{
    SetMotors(0,0, 50, 50, 0);
}

/* [] END OF FILE */


