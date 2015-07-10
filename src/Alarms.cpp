/*
 * Alarms.cpp
 *
 *  Created on: 10 Jul 2015
 *      Author: Jurie
 */

#include <unistd.h>
#include <signal.h>

#include "Alarms.h"


volatile bool alarms::alarmTrigger = false;
int alarms::alarmActive = false;
int alarms::continualAlarmInterval = 0;

void  alarms::ALRMhandlerOnce(int in)
{
	signal(SIGALRM,SIG_IGN);
	alarmTrigger = true;
}

void  alarms::ALRMhandlerCont(int in)
{
	signal(SIGALRM,SIG_IGN);
	alarmTrigger = true;
	signal(SIGALRM,ALRMhandlerCont);
	alarm(continualAlarmInterval);
}
