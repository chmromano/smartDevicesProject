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
#include "giang.h"

#if 0
   
// Assignment 1 week 5 - Measuring time between 2 button presses

#define BUTTON "Zumo03/Button"

//when button is pressed 1st time, button press = true, press time = time
//if 1st button press true, 2nd button pressed, 2nd time - 1st time = measured time

void zmain(void)
{
    TickType_t previous_time = 0;       //set to 0 to avoid print start time of 1st time press button
    printf("Press button!\n");
    bool pressed = false;
    while(true) {
        // Press button
        while(SW1_Read() == 0) {
            pressed = true;
            vTaskDelay(10);
        }
        // Release button
        if (SW1_Read() == 1 && pressed )
        {
            pressed = false;
            TickType_t current_time = xTaskGetTickCount();
            int time = current_time - previous_time;
            if (previous_time > 0)              //only print time when button presse >= 2 times
            {
                print_mqtt(BUTTON, "%d.%d s\n", time/1000, time%1000);
            }
            previous_time = current_time;
        }
    }
    while(true)
    {
        vTaskDelay(100);
    }
}
#endif

#if 0
    
// Assignemnt 2 week 4 Robot sensor following line

#define STOP 2

void zmain(void)
{
    struct sensors_ dig;             //call function sensor
    int count = 0;
    
    reflectance_set_threshold(10000, 10000, 10000, 10000, 10000, 10000);       //set sensor threshold all to 10000
    
    reflectance_start();            //start reflectance sensor
    IR_Start();                     //start IR receiver
    motor_start();                  //start motor
    motor_forward(0,0);             //not move yet
    
    while(SW1_Read());
    BatteryLed_Write(1);            //led on
    vTaskDelay(2000);               //delay 2s
    BatteryLed_Write(0);            //led off
    
    reflectance_digital(&dig);      //Making Reflectance Sensor's period to digital
    motor_forward(70,0);           //Moving forward
    printf("Start\n");
    
    //loop until line counting = numbers of stop
    //follow the line when 2 inside sensor on black area 
    //stop when count equal stop means all sensors on black area
    
    //when R1, R2 is 1, go straight
    //when R2 is 1, turn right until return to 0
    //when L2 is 1, turn left until return to 0
    
    while(count < STOP)             
    {
        reflectance_digital(&dig);
        
        if (dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1)
        {
            count++;
            
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOP)
            {
                reflectance_digital(&dig);
            }
        }
        
        if (dig.R1 == 1 && dig.L1 == 1 && dig.R2 == 0 && dig.R3 == 0 && dig.L2 == 0 && dig.L3 == 0 )
        {
            motor_forward(70,0);
            //printf("forward\n");   (just to check if it works, printf here can make the robot confused)
        }
        
        else if ((dig.R2 == 1 || dig.R3 == 1) && dig.L2 == 0 && dig.L3 == 0)
        {
            motor_turn(30,0,0);
            //printf("turn right\n");
        }
        
        else if ((dig.L2 == 1 || dig.L3 == 1) && dig.R2 == 0 && dig.R3 == 0)
        {
            motor_turn(0,35,0);
            //printf("turn left\n");
        }
    }
    
    motor_forward(0,0);         //robot stops moving
    printf("End\n");
    motor_stop();
    
     while(true)
    {
        vTaskDelay(100);
    }
}

#endif

#if 0
    
// Assignemnt 1 week 4 Robot sensor go straight line

#define STOP 5

void zmain(void)
{
    struct sensors_ dig;             //call function sensor
    int count = 0;
    
    reflectance_set_threshold(10000, 10000, 10000, 10000, 10000, 10000);       //set sensor threshold all to 10000
    
    reflectance_start();            //start reflectance sensor
    IR_Start();                     //start IR receiver
    motor_start();                  //start motor
    motor_forward(0,0);             //not move yet
    
    while(SW1_Read());
    BatteryLed_Write(1);            //led on
    vTaskDelay(2000);               //delay 2s
    BatteryLed_Write(0);            //led off
    
    reflectance_digital(&dig);      //Making Reflectance Sensor's period to digital
    motor_forward(70,0);           //Moving forward
    printf("Start\n");
    
    //loop until line counting = numbers of stop
    //continue moving forward and counting when all sensors on black area (1)
    
    while(count < STOP)             
    {
        reflectance_digital(&dig);
        if (dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1)
        {
            count++;
            
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOP)
            {
                reflectance_digital(&dig);
            }
        }
    }
    
    motor_forward(0,0);         //robot stops moving
    printf("End\n");
    motor_stop();
    
     while(true)
    {
        vTaskDelay(100);
    }
}

#endif
/*    
// Testing another way to edit codes in personal file (Assignemnt 1 week 4)

#define STOP 5

void senor_robot_straight_map(void)
{
    struct sensors_ dig;             //call function sensor
    int count = 0;
    
    reflectance_set_threshold(10000, 10000, 10000, 10000, 10000, 10000);       //set sensor threshold all to 10000
    
    reflectance_start();            //start reflectance sensor
    IR_Start();                     //start IR receiver
    motor_start();                  //start motor
    motor_forward(0,0);             //not move yet
    
    while(SW1_Read());
    BatteryLed_Write(1);            //led on
    vTaskDelay(2000);               //delay 2s
    BatteryLed_Write(0);            //led off
    
    reflectance_digital(&dig);      //Making Reflectance Sensor's period to digital
    motor_forward(70,0);           //Moving forward
    printf("Start\n");
    
    //loop until line counting = numbers of stop
    //continue moving forward and counting when all sensors on black area (1)
    
    while(count < STOP)             
    {
        reflectance_digital(&dig);
        if (dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1)
        {
            count++;
            
            while((dig.R1 == 1 && dig.R2 == 1 && dig.R3 == 1 && dig.L1 == 1 && dig.L2 == 1 && dig.L3 == 1) && count < STOP)
            {
                reflectance_digital(&dig);
            }
        }
    }
    
    motor_forward(0,0);         //robot stops moving
    printf("End\n");
    motor_stop();
    
     while(true)
    {
        vTaskDelay(100);
    }
}
*/

#if 0
// Assignment 1 week 3 Robot run on floor
    
void zmain(void)
{
    printf("\nThis is assignment 1 week 3\n");
    
    motor_start();                  //start motor
    printf("\nSTART\n");
    motor_forward(0,0);             //not move yet
    
    vTaskDelay(3000);
    
    motor_forward(50,6500);
    motor_turn(50,0,1000);          //turn right sharp
    motor_forward(50,5000);
    motor_turn(55,0,1000);          //turn right sharp
    motor_forward(50,6000);
    motor_turn(70,0,1000);          //turn right inside
    motor_forward(50,2800);
    motor_turn(35,0,1000);          //turn right 45 degree
    motor_forward(50,3500);
    
    motor_forward(0,0);             //stop moving
    printf("\nGOAL\n");
    motor_stop();                   //motor stop

    while(true)
    {
        vTaskDelay(100); // sleep (in an infinite loop)
    }
}   
#endif


/* [] END OF FILE */
