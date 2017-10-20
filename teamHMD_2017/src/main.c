/*====================================================================
* Project:  Board Support Package (BSP) examples
* Function: empty main function
*
* Copyright HighTec EDV-Systeme GmbH 1982-2013
*====================================================================*/
#include <stdlib.h>
#include "systemController.h"
#include "vehicleController.h"
#include "cameraController.h"
#include "timerController.h"
#include "interrupts.h"
#include "imageProcess.h"
#include "queueADT.h"
#include "typedefinition.h"
#include "global.h"
#include "Schedular.h"
#include "encoder_set.h"
#include "DMS.h"

unsigned short lcdImage[IMAGE_HEIGHT*IMAGE_WIDTH] = {0, };

queue* driveQ = NULL;

int main(void)
{
	queue newQ;
	queueInit(&newQ,QUEUE_SIZE);
	driveQ = &newQ;

	systemInit();
	peripheralInit();
	peripheralOn();


	while(1)
	{

		if(Scheduler_10ms()) {
			Hill();
			schoolZoneStatus();
			Car_rpm_find();
			AEB();
		}
	}
	return EXIT_SUCCESS;
}
