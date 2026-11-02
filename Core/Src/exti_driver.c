#include "exti_driver.h"

void EXTI_Init(EXTI_Config_t *cfg) {

	RCC->AHB1ENR |= (1<<cfg->port);
	
	GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(AHB1PERIPH_BASE + (cfg->port*0x0400));
	
	GPIOx->MODER &= ~(3U<<(cfg->pin*2));
	GPIOx->PUPDR &= ~(3U<<(cfg->pin*2));
	GPIOx->PUPDR |= (2U<<(cfg->pin*2)); 
	
	RCC->APB2ENR |= (1<<14);
	uint8_t exticr_reg = cfg->pin / 4;
	uint8_t exticr_pos = (cfg->pin % 4) * 4;
	SYSCFG->EXTICR[exticr_reg] &= ~(0xFU << exticr_pos);
	SYSCFG->EXTICR[exticr_reg] |= (cfg->port << exticr_pos);
	EXTI->IMR |= (1 << cfg->pin);
	
	EXTI->RTSR &= ~(1 << cfg->pin);
	EXTI->FTSR &= ~(1 << cfg->pin);
	
	if(cfg->edge == EXTI_EDGE_RISING || cfg->edge == EXTI_EDGE_BOTH){
		EXTI->RTSR |= (1 << cfg->pin);
		}
	if(cfg->edge == EXTI_EDGE_FALLING || cfg->edge == EXTI_EDGE_BOTH){
		EXTI->FTSR |= (1 << cfg->pin);
		}
	if(cfg->pin <=4){
		uint32_t irq_num = 6 + cfg->pin;
		NVIC->ISER[0] |= (1 << irq_num);
	}
}
