////////////////////////////////////////////////////////////////////////////////
/// @file     SYSTEM_MM32.C
/// @author   AE TEAM
/// @brief    THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
////////////////////////////////////////////////////////////////////////////////
/// @attention
///
/// THE EXISTING FIRMWARE IS ONLY FOR REFERENCE, WHICH IS DESIGNED TO PROVIDE
/// CUSTOMERS WITH CODING INFORMATION ABOUT THEIR PRODUCTS SO THEY CAN SAVE
/// TIME. THEREFORE, MINDMOTION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
/// CONSEQUENTIAL DAMAGES ABOUT ANY CLAIMS ARISING OUT OF THE CONTENT OF SUCH
/// HARDWARE AND/OR THE USE OF THE CODING INFORMATION CONTAINED HEREIN IN
/// CONNECTION WITH PRODUCTS MADE BY CUSTOMERS.
///
/// <H2><CENTER>&COPY; COPYRIGHT MINDMOTION </CENTER></H2>
////////////////////////////////////////////////////////////////////////////////

// Define to prevent recursive inclusion
#define _SYSTEM_MM32_C_
#include "mm32_device.h"

/// Uncomment the line corresponding to the desired System clock (SYSCLK)
/// frequency (after reset the HSI is used as SYSCLK source)
///
/// IMPORTANT NOTE:
/// ==============
/// 1. After each device reset the HSI is used as System clock source.
///
/// 2. Please make sure that the selected System clock doesn't exceed your device's
/// maximum frequency.
///
/// 3. If none of the define below is enabled, the HSI is used as System clock
/// source.
///
/// 4. The System clock configuration functions provided within this file assume that:
/// - For Low, Medium and High density Value line devices an external 8MHz
/// crystal is used to drive the System clock.
/// - For Low, Medium and High density devices an external 8MHz crystal is
/// used to drive the System clock.
/// - For Connectivity line devices an external 25MHz crystal is used to drive
/// the System clock.
/// If you are using different crystal you have to adapt those functions accordingly.

/// Uncomment the following line if you need to relocate your vector Table in
/// Internal SRAM.
///#define VECT_TAB_SRAM
#define VECT_TAB_OFFSET  0x0
/// Vector Table base offset field.
/// This value must be a multiple of 0x200.





///////////////////////////////////////////////////////////////
///Clock Definitions
///////////////////////////////////////////////////////////////
//单片机主频设置，如果更改，还需要对应修改分频值
//不超过168M,推荐150M及以下 
#define SYSCLK_FREQ_XXMHz 120000000  
u32 SystemCoreClock         = SYSCLK_FREQ_XXMHz;

__I u8 AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

static void SetSysClock(void);
static void SetSysClockToXX(void);

/// @brief  Setup the microcontroller system
///         Initialize the Embedded Flash Interface, the PLL and update the
///         SystemCoreClock variable.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None
void SystemInit (void)
{
  //Reset the RCC clock configuration to the default reset state(for debug purpose)
  //Set HSION bit
  RCC->CR |= (u32)0x00000001;
  
  //Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits
  RCC->CFGR &= (u32)0xF8FFC00C;
  
  //Reset HSEON, CSSON and PLLON bits
  RCC->CR &= (u32)0xFEF6FFFF;
  
  //Reset HSEBYP bit
  RCC->CR &= (u32)0xFFFBFFFF;
  
  //Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
  RCC->CFGR &= (u32)0xFF3CFFFF;
  RCC->CR &= (u32)0x008FFFFF;
  
  //Disable all interrupts and clear pending bits
  RCC->CIR = 0x009F0000;
  //Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers
  //Configure the Flash Latency cycles and enable prefetch buffer
  SetSysClock();
}


/// @brief  use to return the pllm&plln.
/// @param  pllclkSourceFrq : PLL source clock frquency;
///         pllclkFrq : Target PLL clock frquency;
///         plln : PLL factor PLLN
///         pllm : PLL factor PLLM
/// @retval amount of error
u32 AutoCalPllFactor(u32 pllclkSourceFrq, u32 pllclkFrq, u8* plln, u8* pllm)
{
  u32 n, m;
  u32 tempFrq;
  u32 minDiff = pllclkFrq;
  u8  flag = 0;
  for(m = 0; m < 4 ; m++) {
    for(n = 0; n < 64 ; n++) {
      tempFrq =  pllclkSourceFrq * (n + 1) / (m + 1);
      tempFrq = (tempFrq >  pllclkFrq) ? (tempFrq - pllclkFrq) : (pllclkFrq - tempFrq) ;
      
      if(minDiff > tempFrq) {
        minDiff =  tempFrq;
        *plln = n;
        *pllm = m;
      }
      if(minDiff == 0) {
        flag = 1;
        break;
      }
    }
    if(flag != 0) {
      break;
    }
  }
  return  minDiff;
}
static void DELAY_xUs(u32 count)
{
  u32 temp;
  SysTick->CTRL = 0x0;                                                        //disable systick function
  SysTick->LOAD = count * 8;                                                  //time count for 1us with HSI as SYSCLK
  SysTick->VAL = 0x00;                                                        //clear counter
  SysTick->CTRL = 0x5;                                                        //start discrease with Polling
  do {
    temp = SysTick->CTRL;
  } while((temp & 0x01) && !(temp & (1 << 16)));                             //wait time count done
  SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;                                  //Close Counter
  SysTick->VAL = 0X00;                                                        //clear counter
}
/// @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
/// @param  None
/// @retval None

static void SetSysClock(void)
{
  CACHE->CCR &= ~(0x3 << 3);
  CACHE->CCR |= 1;
  while((CACHE->SR & 0x3) != 2);
  SetSysClockToXX();
}


/// @brief  Sets System clock frequency to XXMHz and configure HCLK, PCLK2
///         and PCLK1 prescalers.
/// @note   This function should be used only after reset.
/// @param  None
/// @retval None

static void SetSysClockToXX(void)
{
  __IO u32 j, temp, tn, tm;
  __IO u32 StartUpCounter = 0, HSEStatus = 0;
  
  u8 plln, pllm;
  
  RCC->CR |= RCC_CR_HSION;
  while(!(RCC->CR & RCC_CR_HSIRDY));
  //PLL SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------
  //Enable HSE
  RCC->CR |= ((u32)RCC_CR_HSEON);
  
  DELAY_xUs(5);
  
  if(SystemCoreClock > 96000000) {
    RCC->APB1ENR |= RCC_APB1ENR_PWR;
    PWR->CR &= ~(3 << 14);
    PWR->CR |= 3 << 14;
  }
  //Wait till HSE is ready and if Time out is reached exit
  while(1) {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    if(HSEStatus != 0)
      break;
    StartUpCounter++;
    if(StartUpCounter >= (10 * HSE_STARTUP_TIMEOUT))
      return;
  }
  
  if ((RCC->CR & RCC_CR_HSERDY) == RESET) {
    //If HSE fails to start-up, the application will have wrong clock
    //configuration. User can add here some code to deal with this error
    HSEStatus = (u32)0x00;
    return;
  }
  
  HSEStatus = (u32)0x01;
  DELAY_xUs(5);
  
  SystemCoreClock         = SYSCLK_FREQ_XXMHz;
  //Enable Prefetch Buffer
  FLASH->ACR |= FLASH_ACR_PRFTBE;
  //Flash 0 wait state ,bit0~2
  FLASH->ACR &= ~FLASH_ACR_LATENCY;
  temp = (SystemCoreClock - 1) / 24000000;//FLASH时钟为24M
  FLASH->ACR |= (temp & FLASH_ACR_LATENCY);
  RCC->CFGR &= (~RCC_CFGR_HPRE) & ( ~RCC_CFGR_PPRE1) & (~RCC_CFGR_PPRE2);
  
  //Long.Qiu HCLK = AHB = FCLK = SYSCLK 
  RCC->CFGR |= (u32)RCC_CFGR_HPRE_DIV1;
  
  //L.O.N.G.Q.I.U. system clock config
  if(SystemCoreClock > 120000000)
  {
    //PCLK2 = APB2 = HCLK
    RCC->CFGR |= (u32)RCC_CFGR_PPRE2_DIV2;//
    //PCLK1 = APB1 = HCLK
    RCC->CFGR |= (u32)RCC_CFGR_PPRE1_DIV2;
  }
  else
  {
    //PCLK2 = APB2 = HCLK
    RCC->CFGR |= (u32)RCC_CFGR_PPRE2_DIV1;
    //PCLK1 = APB1 = HCLK
    RCC->CFGR |= (u32)RCC_CFGR_PPRE1_DIV1;
  }  
  
  AutoCalPllFactor(HSE_VALUE, SystemCoreClock, &plln, &pllm);
  
  RCC->PLLCFGR &= ~((u32 ) RCC_PLLCFGR_PLLSRC | RCC_PLLCFGR_PLLXTPRE) ;
  RCC->PLLCFGR |= (u32 ) RCC_PLLCFGR_PLLSRC  ;
  
  tm = (((u32)pllm) & 0x07);
  tn = (((u32)plln) & 0x7F);
  
  RCC->APB1ENR |= RCC_APB1ENR_PWR;
  RCC->PLLCFGR &= (u32)((~RCC_PLLCFGR_PLL_DN) & (~RCC_PLLCFGR_PLL_DP));
  RCC->PLLCFGR |= ((tn << RCC_PLLCFGR_PLL_DN_Pos) | (tm << RCC_PLLCFGR_PLL_DP_Pos));
  
  //Enable PLL
  RCC->CR |= RCC_CR_PLLON;
  //Wait till PLL is ready
  while((RCC->CR & RCC_CR_PLLRDY) == 0) {
    __ASM ("nop") ;//__NOP();
  }
  //Select PLL as system clock source
  RCC->CFGR &= (u32)((u32)~(RCC_CFGR_SW));
  RCC->CFGR |= (u32)RCC_CFGR_SW_PLL;
  
  //Wait till PLL is used as system clock source
  while ((RCC->CFGR & (u32)RCC_CFGR_SWS) != (u32)RCC_CFGR_SWS_PLL) {
    __ASM ("nop") ;//__NOP();
  }
  
  DELAY_xUs(1);
  // set HCLK = AHB = FCLK = SYSCLK divided by 2
  RCC->CFGR &= (~(RCC_CFGR_PPRE_0));
  DELAY_xUs(1);
  
  // set HCLK = AHB = FCLK = SYSCLK divided by 1
  RCC->CFGR &= (~(RCC_CFGR_PPRE_3));
  
  DELAY_xUs(1);
  
}

