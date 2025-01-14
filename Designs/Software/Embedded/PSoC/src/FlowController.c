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
#include "FlowController.h"
#include "SerialCom.h"
#include <stdio.h>
#include <stdlib.h>

#define FLOWRATE_CONVERSION_FACTOR 23.0
#define FALSE 0 
#define TRUE 1
static uint16_t flowSensorFreq = 0;

#define SLOPE 1.1763
#define BIAS 0.131
static float flow_ref = 0;



/**
 * @function FlowCounterTimerISRHandler(void)
 * @param None
 * @return void
 * @brief FlowCounter ISR. Takes reading of flow meter every second
 * @author Barron Wong 01/25/19
 */
CY_ISR(FlowCounterTimerISRHandler){
    float flowRate = 0;
    static float dutyCycle = 0.2;
    float difference = 0;
    float kp = .5;
    static float error = 0;
    
    //Take Reading
    flowSensorFreq = FlowSensorCounter_ReadCounter();
    //Start Next Conversion
    FlowControlRegister_Write(1);
    //Clear Interrupt
    FlowCounterTimerISR_ClearPending();  
    
    dutyCycle = dutyCycle+(kp*error)/100.0;
    FlowController_SetFlowDutyCycle(dutyCycle);
    flowRate = SLOPE*FlowController_GetFlowRate() + BIAS;
    error = flow_ref - flowRate;
    printf("Flow Rate: %2.3fLPM Flow Sensor Freq: %d PWM Duty Cycle: %3.3f\r\n", flowRate, flowSensorFreq,dutyCycle*100);
        
}

/**
 * @function FlowController_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief Initializes hardware components necessary for flow control and monitoring
 * @author Barron Wong 01/25/19
*/
void FlowController_Init(void){
    FlowSpeedPWM_Start();
    FlowSpeedPWM_WriteCompare1(0);
    FlowCounterTimerISR_StartEx(FlowCounterTimerISRHandler);
    FlowCountTimer_Start();
    FlowSensorCounter_Start();
    flow_ref = 1.5;
}

/**
 * @function FlowController_GetFlowRate(void)
 * @param None
 * @return Flow rate in liters per minute
 * @brief Converts the current frequency and returns flow rate
 * @author Barron Wong 01/25/19
*/
#define AVG_SIZE 64
float FlowController_GetFlowRate(){
    uint16_t reading = 0;
    static uint16_t ticksTotal = 0;
    static uint8_t index = 0;
    static uint8_t count = 0;
    static uint16_t flowRateTicksHistory[AVG_SIZE];
    
    reading = flowSensorFreq;
    
    return reading/FLOWRATE_CONVERSION_FACTOR;
}
/**
 * @function FlowController_SetFlow(void)
 * @param float value 0 - 1
 * @return SUCCESS or ERROR
 * @brief Sets the voltage of the pump in terms of percentages
 * @author Barron Wong 01/25/19
*/
uint8_t FlowController_SetFlowDutyCycle(float dutyCycle){
    uint16_t pwmCompare = 0;
    if(dutyCycle < 0 || dutyCycle > 1){
        return ERROR;
    }
    
    pwmCompare = (uint16_t) (dutyCycle*PWM_MAX);
    FlowSpeedPWM_WriteCompare1(pwmCompare);
    
    return SUCCESS;
}
/**
 * @function FlowController_SetFlowReference(void)
 * @param float value 0 - 1
 * @return SUCCESS or ERROR
 * @brief Sets the reference flow rate in Liters Per Minute
 * @author Barron Wong 01/25/19
*/
uint8_t FlowController_SetFlowReference(float reference){
    uint16_t pwmCompare = 0;
    if(reference < 0 || reference > 10){
        return ERROR;
    }
    
    flow_ref = reference;
    return SUCCESS;
}


#ifdef FLOWCONTROLLER_TEST    
#define ADC_MAX 37500.0

/*
    Test Harness for checking FlowController Library.
    The test should enable an analog value on pin 0.1.
    This value will be used to set the pwm speed on
    the water pump. The flow rate should also default to 
    4.347.
    
*/

 
    
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    ADC_DelSig_1_Start();
    FlowController_Init();
    SerialCom_Init();
    USBFS_Init();
    
    uint16_t adcReading = 0;;
    uint16_t adcPrev = 0;
    float flowRate = 0;
    float dutyCycle = 0;
    float difference = 0;
    float kp = .005;
    float error = 0;
    
    printf("Hydroponic Automation\r\n");
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    for(;;)
    {
        //Check voltage reading from pin 0.1
//        dutyCycle = dutyCycle+(kp*error)/100.0;
//        FlowController_SetFlowDutyCycle(dutyCycle);
//        flowRate = FlowController_GetFlowRate();
//        error = FLOW_REF - flowRate;
//        
//        printf("Flow Rate: %2.3fLPM PWM Duty Cycle: %3.3f\r\n", flowRate, dutyCycle*100);
    }
    
}
#endif
/* [] END OF FILE */
