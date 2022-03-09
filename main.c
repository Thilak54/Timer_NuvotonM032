/**************************************************************************************
 * NAME       : Thilak V
 * FILENAME   : Timer_10ms.c
 * DESCRIPTION: Configure a Timer for 10ms
 * DATE       : 26-02-2022
***************************************************************************************/

#include <stdio.h>
#include "NuMicro.h"

void UART_Open(UART_T *uart, uint32_t u32baudrate);
void Timer0_Enable(void);
void SYS_Init(void)
{

    SYS_UnlockReg();

    
    CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);

    CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);

    
    CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HIRC, CLK_CLKDIV0_HCLK(1));

    CLK->APBCLK0=(CLK->APBCLK0 & ~(0x1<<2))|(1<<2);
     CLK->CLKSEL1=(CLK->CLKSEL1 &~(0x7<<8))|(7<<8);


   
    CLK_EnableModuleClock(UART0_MODULE);

    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    
    SystemCoreClockUpdate();
    PB->MODE=0X1<<10;
    SYS->GPB_MFPL=0XE<<24;

    /* Set PB multi-function pins for UART0 RXD=PB.12 and TXD=PB.13 */
   // SYS->GPB_MFPH = (SYS->GPB_MFPH & ~(SYS_GPB_MFPH_PB12MFP_Msk | SYS_GPB_MFPH_PB13MFP_Msk))    |       \
                    (SYS_GPB_MFPH_PB12MFP_UART0_RXD | SYS_GPB_MFPH_PB13MFP_UART0_TXD);

    
    SYS_LockReg();
}

void Timer0_IRQHandler(void)
{

    PB5=~PB5;
    TIMER0->INTSTS &=0X1<<0;
    printf("hai");
}
void TIF(void)
{
	TIMER0->CTL|=0X1<<29;  //Timer time-out interrupt Enabled.
	NVIC->ISER[0]|=0X1<<8;  //Set interrupt enable
}

int main()
{
    SYS_Init();
         
    UART_Open(UART0, 115200);

    printf("hello");
    Timer0_Enable();
           TIF();


    while(1);
}
void Timer0_Enable(void)
{
	TIMER0->CTL|=0X1<<27;       //Set the operating mode
    TIMER0->CTL|=0X4;           //set the prescale value
    TIMER0->CMP|=96000; //set the counter value for 10ms
    TIMER0->CTL|=0X1<<30;      //Start the counting

}
