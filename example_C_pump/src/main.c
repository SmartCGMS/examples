#include "config.h"

#include "../../../common/iface/SimpleIface.h"
#include "../../../common/rtl/guid.h"

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <float.h>

#define ec_level 2
#define ec_info 12
#define ec_war  13
#define ec_err  14

const GUID signal_IG = { 0x3034568d, 0xf498, 0x455b,{ 0xac, 0x6a, 0xbc, 0xf3, 0x1, 0xf6, 0x9c, 0x9e } };
GUID signal_Requested_Insulin_Basal_Rate = { 0xb5897bbd, 0x1e32, 0x408a, { 0xa0, 0xd5, 0xc5, 0xbf, 0xec, 0xf4, 0x47, 0xd9 } };

extern scgms_execution_t SimpleCalling Execute_SCGMS_Configuration(const char *config, TSCGMS_Execution_Callback callback);
extern BOOL SimpleCalling Inject_SCGMS_Event(const scgms_execution_t execution, const TSCGMS_Event_Data *event);
extern void SimpleCalling Shutdown_SCGMS(const scgms_execution_t execution, BOOL wait_for_shutdown);

scgms_execution_t executor;
double last_basal_rate = -DBL_MAX;

HRESULT Set_Basal_Rate(TSCGMS_Event_Data *event) {
		//SCGMS_Execution_Callback guarantees that event is IG level
		
	event->signal_id = &signal_Requested_Insulin_Basal_Rate;

	
	const double max_basal = 3.0;
	const double insulin_sensitivity = 1.0;		
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
	
	if (event->level != last_basal_rate) {	//change the basal only if it changes
		last_basal_rate = event->level;
		return Inject_SCGMS_Event(executor, event) == TRUE ? S_OK : E_FAIL;
	}
	else
		return S_OK;	//no change
}



void print_msg(int code, wchar_t *msg) {
	switch (code) {
		case ec_info: printf("Information: ");	break;
		case ec_war:  printf("Warning: ");		break;
		case ec_err:  printf("Error: ");		break;
	}

	printf("%ws\n", msg);
}


HRESULT SimpleCalling SCGMS_Execution_Callback(TSCGMS_Event_Data *event) {
	
	HRESULT rc = S_OK;

	switch (event->event_code) {
		case ec_level:
			if (IsEqualGUID(event->signal_id, &signal_IG))
				rc = Set_Basal_Rate(event);
			break;

		case ec_info:
		case ec_war:
		case ec_err: print_msg(event->event_code, event->str);
			break;
	}

	return rc;
}



int MainCalling main(int argc, char **argv) {
	setlocale(LC_ALL, "");
	if (!_get_current_locale()) setlocale(LC_ALL, "C.UTF-8");

	executor = Execute_SCGMS_Configuration(configuration, SCGMS_Execution_Callback);
	if (executor)
		Shutdown_SCGMS(executor, TRUE);
	else {
		printf("Error: unable to execute the configuration!\n");
		return -1;
	}

	return 0;
}