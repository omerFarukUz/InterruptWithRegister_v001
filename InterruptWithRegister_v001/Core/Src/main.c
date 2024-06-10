#include "string.h"
#include "main.h"

int x=0;

void RCC_Config()
{
	RCC->CR |= 0x30000; 			  // HSEON and HSEONRDY enable
	while(!(RCC->CR & 0x20000));      // HSEON Ready Flag wait
	RCC->CR |= 0x80000;				  // CSS enable
	RCC->PLLCFGR |= 0x400000; 		  // PLL e HSE sectik
	RCC->PLLCFGR |= 0x000004; 		  // PLL M = 4
	RCC->PLLCFGR |= 0x005A00; 		  // PLL N=168
	RCC->PLLCFGR |= 0x000000; 		  // PLL P=2
	RCC->CFGR |= 0x000000; 		 	  // AHB Prescaler = 1
	RCC->CFGR |= 0x080000; 		  	  // APB2 Prescaler = 2
	RCC->CFGR |= 0x001400; 		  	  // APB1 Presclaer = 4
	RCC->CIR |= 0x00080000;			  // HSERDY Flag Clear
	RCC->CIR |= 0x00800000;			  // CSS Flag Clear

}

void GPIO_Config()
{
	RCC->AHB1ENR |= 0x0F;            // GPIOA,GPIOB,GPIOC and GPIOD active
	GPIOB->MODER |= 0x10004001;		 // GPIOB port pin 14-7-0 choose output mode
	GPIOB->OTYPER |= 0x00;			 // GPIOB choose push-pull
	GPIOB->OSPEEDR |= 0x20008002;    // GPIOB port pin 14-7-0 choose high speed mode
	GPIOB->PUPDR |= 0x20008002;      // GPIOB port pin 14-7-0 choose pull down mode
	GPIOC->MODER |= 0x00;			 // GPIOC port choose input

}

void EXTI_Config()
{

	RCC->APB2ENR |= 0x4000;			 //SYSCFG active (EXTI)

	SYSCFG->EXTICR[3] |= 0x20;		 //SYSCFG external interrupt configuration register 4 (C Port is activated) EXTI Line
	SYSCFG->EXTICR[0] |= 0x00;       // EXTI[0] Line A port is activated(EXTI0 and EXTI1)

	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(EXTI0_IRQn);

	NVIC_SetPriority(EXTI15_10_IRQn , 0);
	NVIC_SetPriority(EXTI0_IRQn , 1);

	EXTI->IMR = 0x2003;
	EXTI->RTSR = 0x2003;

}

void EXTI15_10_IRQHandler()
{
	if(EXTI->PR & 0x2000)
	{
		GPIOB->ODR = 0x0001;


	}

	EXTI->PR |= (0x1 << 13U);

}

void EXTI0_IRQHandler()
{
	if(EXTI->PR & 0x0001)
	{
		GPIOB->ODR = 0x0080;


	}

	EXTI->PR |= (0x1 << 0U);

}



int main(void)
{

  RCC_Config();
  GPIO_Config();
  EXTI_Config();


  while (1)
  {
	  x++;
	  if(x>1000000) {

		  GPIOB->ODR = 0x00;
		  x=0;
	  }
/*	  x++;

	  if(x>1000000)
	  {
		  GPIOB->ODR = 0x4080;

	  }
	  else GPIOB->ODR = 0x00;
	  if(x>2000000)x=0;*/

  }

}

