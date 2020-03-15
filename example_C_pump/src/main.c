#include "config.h"
#include "helper.h"

#include "../../../common/iface/SimpleIface.h"
#include "../../../common/rtl/guid.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <float.h>


scgms_execution_t executor;

double recent_basal_rate = -DBL_MAX;


HRESULT Set_Basal_Rate(TSCGMS_Event_Data *event) {
		//SCGMS_Execution_Callback guarantees that event is IG level
		
	event->signal_id = signal_Requested_Insulin_Basal_Rate;

	
	const double max_basal = 10.0;
	const double insulin_sensitivity = 3.0;		
	const double max_IG = 40.0;
	const double IG_threshold = 5.5;	//mmol/L

	if (event->level > IG_threshold) {
		if (event->level >= max_IG) event->level = max_IG;
		event->level -= IG_threshold;
		event->level *= insulin_sensitivity;

		if (event->level >= max_basal) event->level = max_basal;
	}
	else
		event->level = 0.0;	//near hypo, suspend the basal
	
	if (event->level != recent_basal_rate) {	//change the basal only if it changes
		recent_basal_rate = event->level;
		return Inject_SCGMS_Event(executor, event) == TRUE ? S_OK : E_FAIL;
	}
	else
		return S_OK;	//no change
}




HRESULT SimpleCalling SCGMS_Execution_Callback(TSCGMS_Event_Data *event) {
	
	HRESULT rc = S_OK;
	int cho_event = 0;

	switch (event->event_code) {
		case ec_level: {
			if (IsEqualGUID(&(event->signal_id), &signal_IG))
				rc = Set_Basal_Rate(event);

			if (IsEqualGUID(&(event->signal_id), &signal_BG))
				Print_Graph(event->device_time, event->level, recent_basal_rate);
			}

			cho_event = IsEqualGUID(&(event->signal_id), &signal_Carb_Intake);

			break;
		
		case ec_war: printf("Warning: %ws\n", event->str);
			break;
		case ec_err: printf("Error: %ws\n", event->str);
			break;
	}

	if (cho_event == 0)	//do not inject meal event, when processing a meal event
						//to avoid infinite recursion
		Schedule_Meals(executor, event->device_time, event->segment_id);

	return rc;
}



int MainCalling main(int argc, char **argv) {
	setlocale(LC_ALL, "");
	if (!_get_current_locale()) 
		setlocale(LC_ALL, "C.UTF-8");	

	executor = Execute_SCGMS_Configuration(configuration, SCGMS_Execution_Callback);
	if (executor) {
		Shutdown_SCGMS(executor, TRUE);
	}
	else {
		printf("Error: unable to execute the configuration!\n");
		return -1;
	}

	return 0;
}