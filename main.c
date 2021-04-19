/**************************************************************************************************************
File name                : main.c
Project                  : ROOM CONDITION MONITORING SYSTEM
Author                   : PRANAV PRABHA 
Date                     : 07/12/20
Platform                 : Tiva Development Kit EK-TM4C123GXL
Description              : MAIN function.
***************************************************************************************************************/
#include"tm4c123gh6pm.h"
#include"Header.h"
// Global variable
extern unsigned char Light;	

// Constants for Temperature and Humidity Sensor
#define Clock_GPIO_C	0x04
#define	DIR_PIN4_In		~0x10
#define DIR_PIN4_Out 	0x10
#define	High_In				0x10
#define Low_In 				~0x10
#define	High_Out			0x10
#define Low_Out 			0x00
#define DEN_ENABLE_C4	0x10

int main(void){
// Variables for Humidity and Temperature Sensor
	int i=0;
	unsigned int Data[40];
	int Temp_16, Temp_High, Temp_Low;		// Varaibles for Temperature
	int RH_16, RH_High, RH_Low;					// Variables for Relative Humidity
	int Check_Sum, temp, sum;						// Variables fro Parity Checking

	GPIO_Port_Init();										// Initializes GPIO PORT Registers for LCD
	ADC_Init();													// Initializes Registers for ADC
	SYSCTL_RCGCGPIO_R |= Clock_GPIO_C;	// Clock for Port C
	Delay(100);													// Delay to stabilize clock 
	GPIO_PORTC_DEN_R |= DEN_ENABLE_C4;	// Set Pin C4 as Digital
	Welcome_Screen();
	while(1){														// For taking and displaying values of Sensors again and gain (infinitely)
		while(1)														// Iterates untill temperature and humidity values are true
		{
			GPIO_PORTC_DIR_R |= DIR_PIN4_Out;		// Set Pin C4 as output
			GPIO_PORTC_DATA_R &= Low_In;				// Set data line low for 1ms to awake the sensor
			Delay(1000);
			GPIO_PORTC_DATA_R |= High_In;				// Set data line high for 30us
			Delay(30);
			GPIO_PORTC_DIR_R &= DIR_PIN4_In;		// Set Pin C4 as Input
			
			while((GPIO_PORTC_DATA_R & 0x10) == Low_Out)	// Iterates untill data line is low
			{}
				
			while((GPIO_PORTC_DATA_R & 0x10) == High_Out)	// Iterates untill data line is high
			{}
				
			while(i<40){																		// Iterates to get 40 bits data from the sensor
				
					while((GPIO_PORTC_DATA_R & 0x10) == Low_Out)	// Iterates untill the data line is low
					{}
						
					Delay(45);
						
					if((GPIO_PORTC_DATA_R & 0x10) == Low_Out)			// Check after 45us
						Data[i] = 0;																// if data line is low // 0 bit by sensor 
					else																					// else
						Data[i] = 1;																// 1 bit by sensor
					
					while((GPIO_PORTC_DATA_R & 0x10) == High_Out)	// Iterates till the data line is high
					{}
						
					i++;
			}
			
			i=0;
			
			RH_High = Binary2Decimal(0,8,Data);							// Converts first 8 bits (Humidity bits) of the data into Decimal value
			RH_Low 	= Binary2Decimal(8,16,Data);						// Converts 8-16 bits of (Humidity bits) the data into Decimal value
			Temp_High = Binary2Decimal(17,24,Data);					// Converts 17-24 bits of (Temperature bits) the data into Decimal value
			Temp_Low	=	Binary2Decimal(24,32,Data);					// Converts 24-32 bits of (Temperature bits) the data into Decimal value
			Check_Sum	=	Binary2Decimal(32,40,Data);					// Converts last 8 bits of (Parity bits) the data into Decimal value
			sum = RH_High + RH_Low + Temp_High + Temp_Low;	// Sum the 8 high and 8 low bits of both humidity and temperature
	
			if(sum>255)
				sum -= 256;																		// To keep the sum 8 bit long
			if(Check_Sum == sum)														// Parity checking
				break;																				// if no error, breaks the above while loop
	}
		RH_16 = Binary2Decimal(0,16,Data);						// Converts Humidity bits into Decimal value
		temp = RH_16/10;
	// Display Relative Humidity
		LCD_Send_String("  RH = ");
		LCD_Send_Data(temp/10+48);										// First digit of integeral part of Humidity
		LCD_Send_Data(temp%10+48);										// Second digit of integeral part of Humidity
		LCD_Send_Data('.');														// puts a decimal point
		LCD_Send_Data(RH_16%10+48);										// Decimal part of Humidity
		LCD_Send_Data('%');														// % sign
	
		LCD_Goto_XY(1,2);															// Moves to next line
		Temp_16 = Binary2Decimal(17,32,Data);					// Converts Temperature bits into Decimal value
		temp = Temp_16/10;
	// Display Temperature on Celcuis scale
	
		if(Data[16] == 1)															// if MSB is 1
			LCD_Send_Data('-');														// Temperature is -ve
		
		LCD_Send_String("Temp = ");
		LCD_Send_Data(temp/10+48);										// First digit of integeral part of Temperature
		LCD_Send_Data(temp%10+48);										// Second digit of integeral part of Temperature
		LCD_Send_Data('.');														// puts a decimal point
		LCD_Send_Data(Temp_16%10+48);									// Decimal part of Temperature
		LCD_Send_Data('o');														// degree sign
		LCD_Send_Data('C');														// 'C' for celcius
		
		// Wait for two seconds to display Light Intensity
		Delay(2000000);
		Delay(2000000);
		
		ADC_Start_Sampling();													// Starts sampling of the Analog Light Intensity Signal
		Delay(1000);																	// Wait for 1ms
		// Display Relative Light Intensity
		Display_Light_Intensity();
		Delay(2000000);
		Delay(2000000);
		LCD_Clear();
	}
}
// Function to Display Relative Light Intensity
void Display_Light_Intensity(void)
{
	char Light_Intensity[4]={0, 0, '%',0};
	int li;
	Light_Intensity[1] = Light%10 + 48;	
  li = Light/10;                                  	 // Second digit for Light Intensity
	Light_Intensity[0] = li%10 + 48;							// First digit for Light Intensity
	LCD_Clear();
  LCD_Goto_XY(1, 1);
	LCD_Send_String("Light = ");
	LCD_Send_String((char *) Light_Intensity);
}
void Welcome_Screen(void)
{
	LCD_Send_String("Room");
	LCD_Goto_XY(1,2);
	//Delay(1000000);
	LCD_Send_String("Condition");
	Delay(2000000);
	LCD_Clear();
	LCD_Send_String("Monitoring");
	LCD_Goto_XY(1,2);
	//Delay(1000000);
	LCD_Send_String("System");
	Delay(2000000);
	LCD_Clear();
	LCD_Send_String("By");
	LCD_Goto_XY(1,2);
	//Delay(1000000);
	LCD_Send_String("PRANAV");
	Delay(2000000);
	LCD_Clear();
	
}
