#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"

uint32_t flag = 0;
uint32_t light;
volatile uint32_t ADC_Value[1];
int color[12] = {WHITE,CYAN,BLUE,RED,MAGENTA,LGRAY,GREEN,YELLOW,BROWN,BRRED,GRAY};
uint32_t THRESHOLD = 700;

void RCC_Configure(void)
{
    /* Alternate Function IO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

}

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void ADC_Configure(void){

  ADC_InitTypeDef ADC_10;
  ADC_DeInit(ADC1);
  ADC_10.ADC_Mode = ADC_Mode_Independent;
  ADC_10.ADC_ScanConvMode = DISABLE;
  ADC_10.ADC_ContinuousConvMode = ENABLE;
  ADC_10.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_10.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_10.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_10);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12,1, ADC_SampleTime_239Cycles5);
  ADC_DMACmd(ADC1, ENABLE);
  ADC_Cmd(ADC1, ENABLE);

  ADC_ResetCalibration(ADC1);
  while (ADC_GetResetCalibrationStatus(ADC1));
  ADC_StartCalibration(ADC1);
  while (ADC_GetCalibrationStatus(ADC1));
  ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void DMA_Configure(void){
    DMA_InitTypeDef DMA_Instructure;

    DMA_Instructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_Instructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_Value[0];
    DMA_Instructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_Instructure.DMA_BufferSize = 4;
    DMA_Instructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_Instructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_Instructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_Instructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
    DMA_Instructure.DMA_Mode = DMA_Mode_Circular;
    DMA_Instructure.DMA_Priority = DMA_Priority_High;
    DMA_Instructure.DMA_M2M = DMA_M2M_Enable;
    DMA_Init(DMA1_Channel1, &DMA_Instructure);
    DMA_Cmd(DMA1_Channel1, ENABLE);
}   

int main()
{
  SystemInit();
  RCC_Configure();
  GPIO_Configure();
  ADC_Configure();
  DMA_Configure();
  
  LCD_Init();
  //LCD_Clear(WHITE);

  while(1){
      ADC_DMACmd(ADC1, ENABLE);
      flag = (ADC_Value[0] < THRESHOLD) ? 1 : 0;
      if (flag) {
        LCD_Clear(GRAY);
        LCD_ShowNum(40, 140, ADC_Value[0], 10, WHITE, GRAY); // 조도 값 출력
      }
      else {
        LCD_Clear(WHITE);
        LCD_ShowNum(40, 140, ADC_Value[0], 10, GRAY, WHITE); // 조도 값 출력
          
      } 
  }
}
