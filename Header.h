/**************************************************************************************************************
File name                : Header.h
Project                  : INDOOR WEATHER MONITORING
Author                   : PRANAV PRABHA 
Date                     : 07/12/20
Platform                 : Tiva Development Kit EK-TM4C123GXL
Description              : Program to define function prototypes
***************************************************************************************************************/
#define CLOCK_GPIOE        0x00000010   //Port E clock control
#define NVIC_EN0_INT17     0x00020000 //Interrupt 17 for ADC0 Seq3

// Functions prototypes for ADC
void ADC_Init(void);
void ADC_Start_Sampling(void);
void Display_Light_Intensity(void);

// Functions prototypes for LCD
void LCD_Init(void);
void LCD_Clear(void);
void LCD_Send_Command(unsigned char command);
void LCD_Send_Data(unsigned char data);
void LCD_Goto_XY(unsigned char x, unsigned char y);
void LCD_Send_String( char *ptr);
void GPIO_Port_Init(void);

// Functions prototypes for Humidity and Temperature Sensors
int Binary2Decimal(int Bit_L, int Bit_H, unsigned int Binary[]);

void Delay(unsigned int T_uS);

extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

void Welcome_Screen(void);
