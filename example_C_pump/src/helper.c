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

#include "helper.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include <utils\winapi_mapping.h>

TExecute_SCMGS_Configuration Execute_SCGMS_Configuration;
TInject_SCGMS_Event Inject_SCGMS_Event;
TShutdown_SCGMS Shutdown_SCGMS;

const GUID signal_IG = { 0x3034568d, 0xf498, 0x455b,{ 0xac, 0x6a, 0xbc, 0xf3, 0x1, 0xf6, 0x9c, 0x9e } };
const GUID signal_BG = { 0xf666f6c2, 0xd7c0, 0x43e8,{ 0x8e, 0xe1, 0xc8, 0xca, 0xa8, 0xf8, 0x60, 0xe5 } };
const GUID signal_Carb_Intake = { 0x37aa6ac1, 0x6984, 0x4a06,{ 0x92, 0xcc, 0xa6, 0x60, 0x11, 0xd, 0xd, 0xc7 } };
GUID signal_Requested_Insulin_Basal_Rate = { 0xb5897bbd, 0x1e32, 0x408a, { 0xa0, 0xd5, 0xc5, 0xbf, 0xec, 0xf4, 0x47, 0xd9 } };



typedef struct {
	int slot;
	double cho;
} TMeal;

//five minutes slots per single day, each slot contains a CHO content or zero as none meal
//all meals follow the UVA scenario, just preceding by 30 minutes
const int meal_count = 5;
TMeal meal_schedule[5] = { {78, 45.0}, {135, 70.0 }, {186, 7.0}, {210, 80.0}, {270, 5.0} }; 

int current_meal_index = -1;
int next_meal_slot = 0;
double next_meal_cho = -1.0;

void Inject_CHO(scgms_execution_t execution, double device_time, double level, const uint64_t segment_id) {
	TSCGMS_Event_Data event;
	memset(&event, 0, sizeof(event));

	event.event_code = ec_level;	//Level
	event.level = level;
	event.device_time = device_time;
	event.segment_id = segment_id;

	event.signal_id = signal_Carb_Intake;

	Inject_SCGMS_Event(execution, &event);
}

void Schedule_Meal(const TMeal *meal) {
	next_meal_slot = meal->slot + (rand() % 12);
	next_meal_cho = meal->cho*0.8 + ((double)rand() / (double)RAND_MAX)*meal->cho*0.4;
}

void Schedule_Meals(scgms_execution_t execution, double current_time, uint64_t segment_id) {
	const int current_slot = (int)((current_time - (int)current_time)*24.0*12.0);

	if (current_meal_index < 0) {
		srand((unsigned int) time(NULL));
		
		for (int i = 0; i < meal_count; i++) {
			if (meal_schedule[i].slot == current_slot) {
				current_meal_index = i;
				Schedule_Meal(&meal_schedule[i]);
				break;
			}
		}
	}

	if (current_slot == next_meal_slot) {
		printf("Ingesting meal, CHO content: %dg\n", (int)next_meal_cho);
		Inject_CHO(execution, current_time, next_meal_cho, segment_id);
		current_meal_index = (current_meal_index+1) % meal_count;
		Schedule_Meal(&meal_schedule[current_meal_index]);
	}

}


double first_day = -1.0;

void Print_Graph(double device_time, double bg, double basal) {
	const double one_hour = 1.0 / 24.0;
	const double one_minute = one_hour / 60.0;

	if (first_day < 0.0) {
		first_day = trunc(device_time);
		printf("SmartCGMS - continuous glucose monitoring and controlling framework\n"			
			"Copyright(c) since 2018 University of West Bohemia.\n\n"
			"Contact:\n"
			"https://diabetes.zcu.cz/\n"
			"diabetes@mail.kiv.zcu.cz\n"
			"Medical Informatics, Department of Computer Science and Engineering\n"
			"Faculty of Applied Sciences, University of West Bohemia\n" 
			"Univerzitni 8, 301 00 Pilsen\n"
			"Czech Republic\n\n"

			"Licensing terms:\n"
			"See the enclosed license in the source code files.\n\n"

			"Purpose of this software:\n"
			"This software demonstrates the use of SmartCGMS simple interface, which allow an easy interoperability\n"
			"with different languages. Particularly, it demonstrates how use SmartCGMS as diabetic in-silico patient,\n"
			"while implementing a simple insulin-pump controller. Meals are scheduled dynamically, with a random noise.\n\n"
			"left columne - day number. hour: minute\n"
			"| symbol denotes basal insulin rate [U/Hr]\n"
			"* symbol denotes blood glucose level [mmol/L]\n"
			". symbols denote target blood glucose level range; 3.9 and 10.0 mmol/L respectively\n\n"
			"To analyze complete results, use:\n"
			"* gpredict3 visualization.ini to visualize the C_pump.log of this demonstration\n"
			"* time_in_range.csv to review the time-in-range statistics\n\n");
	}

	double day_number, hours, minutes;
	hours = 24.0*modf(device_time, &day_number);
	day_number = 1.0 + day_number - first_day;
	minutes = 60.0 * (hours - (long)hours);
	if (minutes == 60.0) {
		minutes = 0.0;
		hours += 1.0;
		if (hours >= 24.0) {
			hours = 0.0;
			day_number += 1.0;
		}
	}

	printf("%1d. %02d:%02d ", (int)day_number, (int)hours, (int)minutes);
	char ln[72];
	ln[70] = '\n';
	ln[71] = 0;
	const double graph_width = 70.0;	//60 columns
	const double max_level = 15.0;
	if (bg > max_level) bg = max_level;

	size_t bg_offset = (size_t)(graph_width * bg / max_level);
	size_t basal_offset = (size_t)(graph_width * basal / max_level);
	const size_t low_offset = (size_t)(graph_width * 3.9 / max_level);
	const size_t high_offset = (size_t)(graph_width * 10.0 / max_level);

	for (size_t i = 0; i < (size_t)graph_width; i++) {
		if (i == bg_offset) ln[i] = '*';
		else if (i == basal_offset) ln[i] = '|';
		else if ((i == low_offset) || (i == high_offset)) ln[i] = '.';
		else ln[i] = ' ';
	}

	printf("%s", ln);
}

/*
//The scms dynamic library exports the following three functions, whose prototype uses scgms_execution_t
extern scgms_execution_t SimpleCalling Execute_SCGMS_Configuration(const char *config, TSCGMS_Execution_Callback callback);
extern BOOL SimpleCalling Inject_SCGMS_Event(const scgms_execution_t execution, const TSCGMS_Event_Data *event);
extern void SimpleCalling Shutdown_SCGMS(const scgms_execution_t execution, BOOL wait_for_shutdown);
*/

HMODULE scgms = NULL;

int Load_SCGMS() {
	scgms = LoadLibraryW(L"scgms.dll");
	if (scgms) {
		Execute_SCGMS_Configuration = (TExecute_SCMGS_Configuration) GetProcAddress(scgms, "Execute_SCGMS_Configuration");
		Inject_SCGMS_Event = (TInject_SCGMS_Event) GetProcAddress(scgms, "Inject_SCGMS_Event");
		Shutdown_SCGMS = (TShutdown_SCGMS) GetProcAddress(scgms, "Shutdown_SCGMS");

		if (!(Execute_SCGMS_Configuration && Inject_SCGMS_Event  && Shutdown_SCGMS)) {
			Free_SCGMS();
		}
	}

	return scgms ? 1 : 0;
}

void Free_SCGMS() {
	if (scgms) {
		FreeLibrary(scgms);
		scgms = NULL;
	}
}
