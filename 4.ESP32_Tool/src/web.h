#ifndef _WEB_
#define _WEB_

#include "sys.h"

extern int success_flag; 
extern String style;
extern String loginIndex;
extern String serverIndex;

void Web_Setup(void);
void Web_Run(void);
#endif