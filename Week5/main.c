

#define  RCC_APB2ENR    (*(volatile unsigned int*)0x40021018) 
#define  GPIOA_CRL     (*(volatile unsigned int*)0x40010800)
#define  GPIOB_CRL     (*(volatile unsigned int*)0x40010C00)
#define  GPIOB_ODR    (*(volatile unsigned int*)(0x40010C00 + 0x0C))

#define  GPIOC_CRL     (*(volatile unsigned int*)0x40011000)
#define  GPIOC_IDR    (*(volatile unsigned int*)(0x40011000 + 0x08))

#define  GPIOD_CRL     (*(volatile unsigned int*)0x40011400)
#define  GPIOD_CRH     (*(volatile unsigned int*)(0x40011400 + 0x04))
#define  GPIOD_BSRR    (*(volatile unsigned int*)(0x40011400 + 0x10))
#define  GPIOD_IDR      (*(volatile unsigned int*)(0x40011400 + 0x08))

int main()
{

  RCC_APB2ENR |= (1<<4) | (1<<5);    
  GPIOD_CRH &= (0xFFF00FFF);
  GPIOD_CRH |= (0x00038000);    // PD11, PD12
  
  GPIOD_CRL &= (0xFFFFFFF0);
  GPIOD_CRL |= (0x00000003);    // PD0
  
  GPIOC_CRL &= (0xFF0FF0FF);
  GPIOC_CRL |= (0x00800800);    // PC2, PD5
  
  while(1){
    
    if( ~GPIOC_IDR & (1 << 5)){           // UP
      GPIOD_BSRR = (1 << 12);
    }
    
    else if(~GPIOD_IDR & (1 << 11)){     // Button
      GPIOD_BSRR = (1 << 28);
    }
    
  }
  
  GPIOD_BSRR = 0xffffffff;

  while(1);
}
