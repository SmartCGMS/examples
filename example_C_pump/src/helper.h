#ifndef __HELPER_H__
#define __HELPER_H__

#include "../../../common/iface/SimpleIface.h"

#define ec_level 2
#define ec_war  13
#define ec_err  14

extern const GUID signal_IG;
extern const GUID signal_BG;
extern const GUID signal_Carb_Intake;
extern GUID signal_Requested_Insulin_Basal_Rate;

extern scgms_execution_t SimpleCalling Execute_SCGMS_Configuration(const char *config, TSCGMS_Execution_Callback callback);
extern BOOL SimpleCalling Inject_SCGMS_Event(const scgms_execution_t execution, const TSCGMS_Event_Data *event);
extern void SimpleCalling Shutdown_SCGMS(const scgms_execution_t execution, BOOL wait_for_shutdown);


void Print_Graph(double device_time, double bg, double basal);
void Schedule_Meals(scgms_execution_t execution, double current_time, uint64_t segment_id);

#endif 