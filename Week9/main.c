#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

#include "misc.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void EXTI_Configure(void);
void USART1_Init(void);
void NVIC_Configure(void);

void EXTI15_10_IRQHandler(void);

void Delay(void);

void sendDataUART1(uint16_t data);

// void LED_Forward();

// void LED_Backward();


// uint32_t volatile flag = 0;
//---------------------------------------------------------------------------------------------------

// uint16_t led_pins[] = {GPIO_Pin_7, GPIO_Pin_4, GPIO_Pin_3, GPIO_Pin_2};

void RCC_Configure(void)
{
	/* UART TX/RX port clock enable */ // PA9, PA10
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	/* USART1 clock enable */ //
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	/* USART2 clock enable */ //
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART2, ENABLE);

	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* UART1 pin setting */
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	//RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* UART2 pin setting */
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	//RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void EXTI_Configure(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;

    /* USART1: RX*/
   	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);
    EXTI_InitStructure.EXTI_Line = EXTI_Line10;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* USART2: RX*/
   	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource11);
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void USART1_Init(void)
{
	USART_InitTypeDef USART1_InitStructure;
	USART_InitTypeDef USART2_InitStructure;

	// Enable the USART1, 2 peripheral
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	
	USART1_InitStructure.USART_BaudRate = 28800;
	USART1_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART1_InitStructure.USART_StopBits = USART_StopBits_1;
	USART1_InitStructure.USART_Parity = USART_Parity_No;
	USART1_InitStructure.USART_Mode = (USART_Mode_Rx | USART_Mode_Tx);
	USART1_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART1_InitStructure);

	USART2_InitStructure.USART_BaudRate = 28800;
	USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART2_InitStructure.USART_StopBits = USART_StopBits_1;
	USART2_InitStructure.USART_Parity = USART_Parity_No;
	USART2_InitStructure.USART_Mode = (USART_Mode_Rx | USART_Mode_Tx);
	USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART2_InitStructure);
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // UART1
	// 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IPR0_PRI_0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // UART2
	// 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IPR0_PRI_0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler() {
	uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);

        // TODO implement
        if (word == 'a') {
        	// 1 2 3 4
		flag = 0;
        	LED_Forward();
        } 
        else if (word == 'b') {
        	// 4 3 2 1
		flag = 1;
        	LED_Backward();
        }

        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
}

// TODO!!!: USART2 handler

void sendDataUART1(uint16_t data) {
	/* Wait till TC is set */
	while ((USART1->SR & USART_SR_TC) == 0);
	USART_SendData(USART1, data);
}

int main(void)
{

    SystemInit();

    RCC_Configure();

    GPIO_Configure();

    EXTI_Configure();

    USART1_Init();

    NVIC_Configure();

    while (1);
}
