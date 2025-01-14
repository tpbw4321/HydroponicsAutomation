/*
 * File:   WaterLevelController.h
 * Author: bwong20
 *
 * Software to control the flow 
 *
 *
 *
 * PWM_TEST (in the .c file) conditionally compiles the test harness for the code. 
 * 
 *
 * Created on Janruary 25, 2019
 */

#include "WaterLevelController.h"
#include "sensor_data.h"
#include <stdio.h>

#include "project.h"

#define SUCCESS 1
#define ERROR -1
#define MAX_HEIGHT 16.2
#define MARGIN 0.05
#define LOWER_THRESH sd.h2o_target - MARGIN*sd.h2o_target
extern uint8_t EC_Enable;
extern uint8_t pH_Enable;
/**
 * @function WaterLevelControllerISRHandler(void)
 * @param None
 * @return None
 * @brief Control ISR set on 100ms timer. Turns on pump if
 *        water level is below the target
 * @author Barron Wong 05/06/19
 */
CY_ISR(WaterLevelControllerISRHandler){
    float waterLevel = WaterLevelController_GetWaterLevel();
    
    WaterLevelControllerISR_ClearPending();
    if (waterLevel < LOWER_THRESH){
        
        WaterLevelControlReg_Write(1);
        EC_Enable = OFF;
        pH_Enable = OFF;
    }
    else if (waterLevel >= sd.h2o_target){
        WaterLevelControlReg_Write(0);
        EC_Enable = ON;
    }
}

/**
 * @function WaterLevelController_Init(void)
 * @param None
 * @return None
 * @brief Initializes hardware components necessary for water level control
 * @author Barron Wong 05/06/19
*/
void WaterLevelController_Init(){
    WaterLevelControlTimer_Start();
    WaterLevelControllerISR_StartEx(WaterLevelControllerISRHandler);
}

/**
 * @function WaterLevelController_GetWaterLevel(void)
 * @param None
 * @return Height of water level in centimeters
 * @brief Get the water level and returns the height in centimeters
 * @author Barron Wong 05/013/19
*/
float WaterLevelController_GetWaterLevel(){
    return sd.h2o_level;
}
/**
 * @function WaterLevelController_TurnOff(void)
 * @param None
 * @return Height of water level in centimeters
 * @brief None
 * @author Barron Wong 06/05/19
*/
void WaterLevelController_TurnOff(){
    WaterLevelControllerISR_Disable();
}
/**
 * @function WaterLevelController_TurnOn(void)
 * @param None
 * @return None
 * @brief Turn on water level controller
 * @author Barron Wong 06/05/19
*/
void WaterLevelController_TurnOn(){
    WaterLevelControllerISR_Enable();
}
#ifdef WATERLEVELCONTROLLER_TEST
int main(){
    WaterLevelController_Init();
    WaterLevelControlReg_Write(1);
}
#endif

/* [] END OF FILE */
