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
#include "project.h"

#define OUTPUT_TOLERANCE        5
#define VOLTAGE_0_8             326 // 327.68
#define POWER_LIMIT             500
#define MAIN_ROUTINE_DELAY_MS   10

uint16 power_target = 0;

void AdjustOutput();

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    PWM_Acc_Start();
    ADC_SAR_Seq_Start();
    
    for(uint32 i=0;i<1000;i++)
    {
        AdjustOutput();
        
        CyDelay(MAIN_ROUTINE_DELAY_MS);
    }

    power_target = 50;    
    for(uint32 i=0;i<1000;i++)
    {
        AdjustOutput();
        
        CyDelay(MAIN_ROUTINE_DELAY_MS);
    }
    
    power_target = 500;
    for(;;)
    {
        AdjustOutput();
        
        CyDelay(MAIN_ROUTINE_DELAY_MS);
    }
}

void AdjustOutput()
{
    uint16 output_ch0;
    uint16 output_ch1;    
    uint16 target_ch0;
    uint16 target_ch1;
    
    target_ch0 = power_target + VOLTAGE_0_8;
    target_ch1 = power_target + VOLTAGE_0_8 * 2;    
    
    ADC_SAR_Seq_StartConvert();
    ADC_SAR_Seq_IsEndConversion(ADC_SAR_Seq_WAIT_FOR_RESULT);
    output_ch0 = ADC_SAR_Seq_GetResult16(0);
    output_ch1 = ADC_SAR_Seq_GetResult16(1);
    ADC_SAR_Seq_StopConvert();
    
    // Adjust output channel 0 (V)
    if(output_ch0 < target_ch0 - OUTPUT_TOLERANCE)
    {
        if(PWM_Acc_ReadCompare1() < PWM_Acc_ReadPeriod()) {PWM_Acc_WriteCompare1(PWM_Acc_ReadCompare1() + 1);}
    }
    if(output_ch0 > target_ch0 + OUTPUT_TOLERANCE)
    {
        if(PWM_Acc_ReadCompare1() > 0) {PWM_Acc_WriteCompare1(PWM_Acc_ReadCompare1() - 1);}
    }
    
    // Adjust output channel 1 (V+0.8)
    if(output_ch1 < target_ch1 - OUTPUT_TOLERANCE)
    {
        if(PWM_Acc_ReadCompare2() < PWM_Acc_ReadPeriod()) {PWM_Acc_WriteCompare2(PWM_Acc_ReadCompare2() + 1);}
    }
    if(output_ch1 > target_ch1 + OUTPUT_TOLERANCE)
    {
        if(PWM_Acc_ReadCompare2() > 0) {PWM_Acc_WriteCompare2(PWM_Acc_ReadCompare2() - 1);}
    }
}
/* [] END OF FILE */
