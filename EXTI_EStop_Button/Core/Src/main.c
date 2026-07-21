#include "stm32f4xx.h"
#include "exti_driver.h"

volatile uint8_t e_stop_flag = 0;

void my_callback(void)
{
	e_stop_flag = 1; // Time-critical action: Set the flag and exit the ISR immediately.
}
void EXTI0_IRQHandler(void)
{
	if(EXTI->PR & (1 << 0)){
		EXTI->PR = (1 << 0);
		my_callback();
	}
}
int main(void)
{
	DBGMCU->CR |= (1<<0);
	EXTI_Config_t cfg ={
			.port = EXTI_PORT_A, // GPIO port where the interrupt pin is connected (GPIOA)
			.pin = 0, // Pin number where the button is connected (PA0)
			.edge = EXTI_EDGE_FALLING, // Falling edge trigger, representing the opening of the NC E-Stop circuit
			.callback = my_callback // Address of the callback function to execute on interrupt
	};

	RCC->AHB1ENR |= (1 << 3);

	GPIOD->MODER &= ~(3U << (12 * 2));//Green Led Output
	GPIOD->MODER |= (1U << (12 * 2));

	GPIOD->MODER &= ~(3U << (14 * 2)); //Red Led Output
	GPIOD->MODER |= (1U << (14 * 2));

	GPIOD->ODR |= (1 << 12);
	GPIOD->ODR &= ~(1 << 14);

	EXTI_Init(&cfg);

	while(1){
			if(e_stop_flag){
			GPIOD->ODR &= ~(1 << 12);// Turn OFF Green Led
			GPIOD->ODR |= (1 << 14); //Turn On Red Led
			e_stop_flag = 0; // Clear Flag
			}

		__asm volatile("wfi");
	}
}
