/*
 * MyTask.h
 *
 *  Created on: Nov 22, 2019
 *      Author: izzat
 */

#ifndef INC_MYTASK_H_
#define INC_MYTASK_H_

#include "string.h"
#include "stdio.h"

void MyTask_Init(void);
void MyTask_Run(void);
void MyTask_Error_Handler(_Bool, char *);
void MyTask_Display(_Bool);


#endif /* INC_MYTASK_H_ */
