/**
 * SmartCGMS - continuous glucose monitoring and controlling framework
 * https://diabetes.zcu.cz/
 *
 * Copyright (c) since 2018 University of West Bohemia.
 *
 * Contact:
 * diabetes@mail.kiv.zcu.cz
 * Medical Informatics, Department of Computer Science and Engineering
 * Faculty of Applied Sciences, University of West Bohemia
 * Univerzitni 8, 301 00 Pilsen
 * Czech Republic
 * 
 * 
 * Purpose of this software:
 * This software is intended to demonstrate work of the diabetes.zcu.cz research
 * group to other scientists, to complement our published papers. It is strictly
 * prohibited to use this software for diagnosis or treatment of any medical condition,
 * without obtaining all required approvals from respective regulatory bodies.
 *
 * Especially, a diabetic patient is warned that unauthorized use of this software
 * may result into severe injure, including death.
 *
 *
 * Licensing terms:
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *
 * a) This file is available under the Apache License, Version 2.0.
 * b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
 *    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
 *    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
 *    Volume 177, pp. 354-362, 2020
 */

#ifndef __HELPER_H__
#define __HELPER_H__

#include <scgms/iface/SimpleIface.h>

#define ec_level 2
#define ec_war  13
#define ec_err  14

extern const GUID signal_IG;
extern const GUID signal_BG;
extern const GUID signal_Carb_Intake;
extern GUID signal_Requested_Insulin_Basal_Rate;

extern TExecute_SCMGS_Configuration Execute_SCGMS_Configuration;
extern TInject_SCGMS_Event Inject_SCGMS_Event;
extern TShutdown_SCGMS Shutdown_SCGMS;

int Load_SCGMS(void); 	/*returns zero on failure, non-zero on succes*/
void Free_SCGMS(void);
void Print_Graph(double device_time, double bg, double basal);
void Schedule_Meals(scgms_execution_t execution, double current_time, uint64_t segment_id);

#endif
