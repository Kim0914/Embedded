#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "lcd.h"
#include "touch.h"

uint16_t led_on;
uint16_t led_1;
uint16_t led_2;
uint16_t count;
uint16_t flag;
uint16_t led_1_flag;
uint16_t led_2_flag;

void RCC_Configure(void){
    /* Alternate Function IO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    /* LED port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    
    /* TIM2 clock enable*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

void GPIO_Configure(void){
    //LED1 PD2
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    //LED2 PD3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void TIM2_Configure(void){
    //1초마다 count되도록 설정
    TIM_TimeBaseInitTypeDef TIM_InitStructure;
    //TIM2 ENABLE
    TIM_Cmd(TIM2, ENABLE);
    //AutoreloadRegister ENABLE
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    
    TIM_InitStructure.TIM_Prescaler = 7200-1;
    TIM_InitStructure.TIM_Period = 10000-1;
    TIM_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_InitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM2, &TIM_InitStructure);
}

void NVIC_Configure(void){
    //TIM2 interrrupt
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}

void TIM2_IRQHandler(){
    count++;
    if(flag == 1){
        led_1++;
        led_1 = led_1 % 2;
        led_2++;
        led_2 = (led_2 % 5);
    }
    if(flag == 0){
        LCD_ShowString(20,40,"OFF",RED,WHITE);
    }
    else if(flag == 1){
        LCD_ShowString(20,40,"ON ",RED,WHITE);
        if(led_2 == 4){
            led_2_flag++;
            led_2_flag = led_2_flag % 2;
        }
    }
    
    if(led_1 == 1){
        GPIO_SetBits(GPIOD,GPIO_Pin_2);
        LCD_ShowString(80,280,"ON ",RED,WHITE);
    }
    else if(led_1 == 0){
        GPIO_ResetBits(GPIOD,GPIO_Pin_2);
        LCD_ShowString(80,280,"OFF",RED,WHITE);
    }
    
    if(led_2_flag == 1){
        GPIO_SetBits(GPIOD,GPIO_Pin_3);
        LCD_ShowString(200,280,"ON ",RED,WHITE);
    }
    else if(led_2_flag == 0){
        GPIO_ResetBits(GPIOD,GPIO_Pin_3);
        LCD_ShowString(200,280,"OFF",RED,WHITE);
    }
    
    
    LCD_ShowNum(150,220,count,4,RED,WHITE);
    
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

//메인문 예시
int main() {
    uint16_t recSize = 30;
    uint16_t x = 0, y = 0;
    uint16_t pix_x = 0,pix_y = 0;
    led_on = 0;
    led_1 = 0;
    led_2 = 0;
    count = 0;
    flag = 0;
    led_1_flag = 0;
    led_2_flag = 0;
    // LCD 관련 설정은 LCD_Init에 구현되어 있으므로 여기서 할 필요 없음
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    NVIC_Configure();
    TIM2_Configure();
    
    // ------------------------------------

    LCD_Init();
    Touch_Configuration();
    Touch_Adjust();
    LCD_Clear(WHITE);
    
    
    while(1){
        /*TEAM NAME*/
        LCD_ShowString(20,20,"MON_TEAM03",BLUE,WHITE);
        
        /*Button*/
        LCD_ShowString(40,150,"BUT",RED,WHITE);
        LCD_DrawRectangle(40-recSize,150-recSize,40+recSize,150+recSize);
        Touch_GetXY(&x,&y,1);
        Convert_Pos(x,y,&pix_x,&pix_y);
        if((10<=pix_x && pix_x <= 70) && (120<=pix_y && pix_y<=180))
        {
            
            flag++;
            flag = flag%2;
            pix_x = 0;
            pix_y = 0;
            x=0;
            y=0;
            
        }
        /*COUNT*/
        LCD_ShowString(20,220,"COUNT",RED,WHITE);
        
        /*LED*/
        LCD_ShowString(10,280,"LED 1 :",RED,WHITE);
        LCD_ShowString(120,280,"LED 2 :",RED,WHITE);
    }
}
