/**************************************************************************************************************
File name                : adc.c
Project                  : ROOM CONDITION MONITORING SYSTEM
Author                   : PRANAV PRABHA 
Date                     : 07/12/20
Platform                 : Tiva Development Kit EK-TM4C123GXL
Description              : Program to initialize adc for LDR.
***************************************************************************************************************/
#include"tm4c123gh6pm.h"
#include"Header.h"
unsigned char Light = 0;	
// ADC 0 interrupt service routine for sequencer 3
void ADC0Seq3_Handler(void){
unsigned int adc_data = 0; 
  // Read the temperature raw value from ADC FIFO	3
	adc_data = (ADC0_SSFIFO3_R  & 0xFFF);
	
	// Convert the raw value to Celsius scale
	Light = (unsigned char) (((adc_data)*3.3/4096) * 100);
  // Clear the interrupt from ADC0 sequencer 3	
	ADC0_ISC_R |= 0x08;	
	//DisableInterrupts();
}

// Intialization routine for setting up the required ports
void ADC_Init(void)
{		
  // GPIO configuration for ADC 0 analog input 1
	// as a temperature sensor is connected to GPIO pin PE2
	SYSCTL_RCGCGPIO_R  |=  0x00000010; //Port E clock enable
	
  GPIO_PORTE_DEN_R &= ~(0x04);
	GPIO_PORTE_AMSEL_R |= 0x04;

	// Enable the clock for ADC0
  SYSCTL_RCGCADC_R |= 0x00000001; //ADC clock enable
	Delay(3);
	
	ADC0_PC_R |= 0x03;				// 250 Ksps 
	
	// Select AN1 (PE2) as the analog input 
	ADC0_SSMUX3_R = 0x01;
	
	// 1st sample is end of sequence and source of interrupt
	ADC0_SSCTL3_R  |= 0x06;		
	
	// 16x oversampling and then averaged
	ADC0_SAC_R |= 0x04;
	
	// Unmask ADC0 sequence 3 interrupt
	ADC0_IM_R  |= 0x08;
	// Enable ADC0 sequencer 3 interrupt in NVIC
	NVIC_EN0_R = NVIC_EN0_INT17;	
	
	// Configure ADC0 module for sequencer 3
	ADC0_ACTSS_R  = 0x00000008;	
	
}

void ADC_Start_Sampling(void)
{
	// Processor sample sequencer initiate for sequencer 3
	ADC0_PSSI_R |= 0x08;	  
}
