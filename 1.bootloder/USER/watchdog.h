#ifndef __WATCHDOG__
#define __WATCHDOG__

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_wwdg.h"
#include "misc.h"
void Watchdog_Config();
#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/

