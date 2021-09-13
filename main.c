#define  CRH            0x04
#define  IDR            0x08
#define  ODR            0x0C
#define  BSRR           0x10

#define  RCC_APB2ENR    (*(volatile unsigned int*)0x40021018) 

#define  GPIOA_CRL      (*(volatile unsigned int*)0x40010800)
#define  GPIOB_CRH      (*(volatile unsigned int*)(0x40010C00 + CRH))
#define  GPIOB_IDR      (*(volatile unsigned int*)(0x40010C00 + IDR))
#define  GPIOC_CRL      (*(volatile unsigned int*)0x40011000)
#define  GPIOC_IDR      (*(volatile unsigned int*)(0x40011000 + IDR))
#define  GPIOD_CRL      (*(volatile unsigned int*)0x40011400)
#define  GPIOD_BSRR     (*(volatile unsigned int*)(0x40011400 + BSRR))

unsigned short flag = 0;
int main()
{

  RCC_APB2ENR |= (1<<3) | (1<<5)| (1<<4);
  GPIOD_CRL &= (0x0FF000FF);
  GPIOD_CRL |= (0x30033300);
  
  GPIOC_CRL &= (0xFF0000FF);
  GPIOC_CRL |= (0x00888800);
  
  GPIOB_CRH &= (0xFFFFFFF0);
  GPIOB_CRH |= (0x00000008);
  
  while(1){
    // UP
    if( ~GPIOC_IDR & (1 << 5) ){
      GPIOD_BSRR = (1 << 4) | (1 << 7);
      flag |= (1 << 4) | (1 << 7);
    }
    // Down
    else if( ~GPIOC_IDR & (1 << 2) ){
      GPIOD_BSRR = (1 << 20) | (1 << 23);
      flag &= ~((1 << 4) | (1 << 7)) & 0x9C;
    }
    // Left
    else if( ~GPIOC_IDR & (1 << 3) ){
      GPIOD_BSRR = (1 << 2) | (1 << 3);
      flag |= (1 << 2) | (1 << 3);
    }
    // Right
    else if( ~GPIOC_IDR & (1 << 4) ){
      GPIOD_BSRR = (1 << 18) | (1 << 19);
      flag &= ~((1 << 2) | (1 << 3)) & 0x9C;
    }
    
    else if( ~GPIOB_IDR & (1 << 8) ){
      GPIOD_BSRR = (flag) << 16;
      flag = (~flag) & 0x9C;
      GPIOD_BSRR = flag;
    }
  }
  

  while(1);
}
