/* Examples and Documentation for
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
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "config.h"
#include "helper.h"

#include <iface/SimpleIface.h>
#include <rtl/guid.h>

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <float.h>

scgms_execution_t executor;

double recent_basal_rate = -DBL_MAX;

static HRESULT Set_Basal_Rate(TSCGMS_Event_Data *event) {
		//SCGMS_Execution_Callback guarantees that event is IG level

	event->signal_id = signal_Requested_Insulin_Basal_Rate;

	const double max_basal = 10.0;
	const double insulin_sensitivity = 2.0;
	const double max_IG = 40.0;
	const double IG_threshold = 5.5;	//mmol/L

	if (event->level > IG_threshold) {

		if (event->level >= max_IG) {
			event->level = max_IG;
		}

		event->level -= IG_threshold;
		event->level *= insulin_sensitivity;

		if (event->level >= max_basal) {
			event->level = max_basal;
		}
	}
	else {
		event->level = 0.0;	//near hypo, suspend the basal
	}
	
	if (event->level != recent_basal_rate) {	//change the basal only if it changes
		recent_basal_rate = event->level;
		return (Inject_SCGMS_Event(executor, event) == TRUE) ? S_OK : E_FAIL;
	}
	else {
		return S_OK;	//no change
	}
}

HRESULT SimpleCalling SCGMS_Execution_Callback(TSCGMS_Event_Data *event) {
	
	HRESULT rc = S_OK;
	int cho_event = 0;

	switch (event->event_code) {
		case ec_level: {
			if (IsEqualGUID(&(event->signal_id), &signal_IG)) {
				rc = Set_Basal_Rate(event);
			}

			if (IsEqualGUID(&(event->signal_id), &signal_BG)) {
				Print_Graph(event->device_time, event->level, recent_basal_rate);
			}

			cho_event = IsEqualGUID(&(event->signal_id), &signal_Carb_Intake);
			break;
		}
		case ec_war: {
			printf("Warning: " WSTRING_FORMATTER "\n", event->str);
			break;
		}
		case ec_err: {
			printf("Error: " WSTRING_FORMATTER "\n", event->str);
			break;
		}
	}

	if (cho_event == 0) {	//do not inject meal event, when processing a meal event
							//to avoid infinite recursion
		Schedule_Meals(executor, event->device_time, event->segment_id);
	}

	return rc;
}


int MainCalling main(int argc, char **argv) {
	if (!Load_SCGMS()) {
		printf("Cannot load the SCGMS engine!\n"
			"Make sure that this executable is in the same directory the scgms dynamic library.\n");
		return -2;
	}

	setlocale(LC_ALL, "");
	if (!_get_current_locale()) {
		setlocale(LC_ALL, "C.UTF-8");
	}

	executor = Execute_SCGMS_Configuration(configuration, SCGMS_Execution_Callback);
	if (executor) {
		Shutdown_SCGMS(executor, TRUE);
	}
	else {
		printf("Error: unable to execute the configuration!\n");
		Free_SCGMS();
		return -1;
	}

	Free_SCGMS();
	return 0;
}
