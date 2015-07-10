/*
 * Alarms.h
 *
 *  Created on: 10 Jul 2015
 *      Author: Jurie
 */

#ifndef SRC_ALARMS_H_
#define SRC_ALARMS_H_


namespace alarms
{
	extern volatile bool alarmTrigger;

	extern int alarmActive;

	extern int continualAlarmInterval;
	void  ALRMhandlerCont(int in);
	void  ALRMhandlerOnce(int in);

}



#endif /* SRC_ALARMS_H_ */
