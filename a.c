void EXTI9_5_IRQHandler(void){ // Joystick UP (port 5)
	if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5) == Bit_RESET) {
			// TODO implement
			
		}
        EXTI_ClearITPendingBit(EXTI_Line5);
	}
	
}

void EXTI2_IRQHandler(void){ // Joystick down (port 2)
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) == Bit_RESET) {
			// TODO implement
			
		}
        EXTI_ClearITPendingBit(EXTI_Line2);
	}
}
