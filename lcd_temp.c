static void LCD_WR_REG(uint16_t LCD_Reg)
{
	// TODO implement using GPIO_ResetBits/GPIO_SetBits
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);  // LCD_RS(0);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);   // LCD_CS(0);
	GPIO_ResetBits(GPIOD, GPIO_Pin_14);  // LCD_WR(0);

	GPIO_Write(GPIOE, LCD_Reg);

	// TODO implement using GPIO_ResetBits/GPIO_SetBits
	GPIO_SetBits(GPIOD, GPIO_Pin_14);    // LCD_WR(1);
	GPIO_SetBits(GPIOC, GPIO_Pin_6);     // LCD_CS(1);	
}

static void LCD_WR_DATA(uint16_t LCD_Data)
{
	// TODO implement using GPIO_ResetBits/GPIO_SetBits

	GPIO_SetBits(GPIOD, GPIO_Pin_13);    // LCD_RS(1);
	GPIO_ResetBits(GPIOC, GPIO_Pin_6);   // LCD_CS(0);
	GPIO_ResetBits(GPIOD, GPIO_Pin_14);  // LCD_WR(0);

	GPIO_Write(GPIOE, LCD_Data);

	// TODO implement using GPIO_ResetBits/GPIO_SetBits
	GPIO_SetBits(GPIOD, GPIO_Pin_14);    // LCD_WR(1);
	GPIO_SetBits(GPIOC, GPIO_Pin_6);     // LCD_CS(1);
}
