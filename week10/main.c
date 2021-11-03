#include "stm32f10x.h"
#include "core_cm3.h"
#include "misc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "lcd.h"
#include "touch.h"

volatile uint8_t light;

int color[12] =
{WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};

// void SystemInit(void)
// {
// 	//TODO
// }

void RCC_Configure(void)
{
	// PB3 (sensor pin)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	// ADC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Alternate Function IO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // PB3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ADC_Configure(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_Cmd(ADC1, ENABLE);
}

void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

    // PB3
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_IPR0_PRI_0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXIT3_IRQnHandler(){
  if(EXTI_GetITStatus(EXTI_Line3)){
    light = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
    EXTI_ClearITPendingBit(EXTI_Line3);
  }
}

int main() {
	//LCD ���� ������ LCD_Init �� �����Ǿ� �����Ƿ� ���⼭ �� �ʿ� ����
	SystemInit();
	RCC_Configure();
	GPIO_Configure();
	ADC_Configure();
	NVIC_Configure();

	LCD_Init();
	Touch_Configuration();
	Touch_Adjust();
	LCD_Clear(WHITE);

	while(1){
          
                uint16_t pos_x, x = 0;
                uint16_t pos_y, y = 0;
		
                // LCD �� ��� �� ��ġ ��ǥ �б�
                LCD_ShowString(40, 40, "MON_TEAM03", color[11], color[0]); // TEAM_03 ���
                
                
                Touch_GetXY(&pos_x, &pos_y, 1);
                Convert_Pos(pos_x, pos_y, &x, &y);
                LCD_ShowNum(60, 150, x, 10, color[11], color[0]); // x��ǥ ���
                LCD_ShowNum(60, 180, y, 10, color[11], color[0]); // y��ǥ ���
                LCD_DrawCircle(x, y, 5); // ��ġ�� �ڸ��� ���׶�� �׸���
                LCD_ShowNum(60, 300, light, 10, color[11], color[0]); // ���� �� ���               
	}
}