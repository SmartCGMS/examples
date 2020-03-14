#include "helper.h"

#include <math.h>
#include <stdio.h>

void Issue_Segment_Start(scgms_execution_t execution, uint8_t event_code, double device_time, const uint64_t segment_id) {
	TSCGMS_Event_Data event;
	event.event_code = event_code;
	event.device_time = device_time;
	event.segment_id = segment_id;

	const GUID null_guid = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
	event.signal_id = null_guid;
}

double first_day = -1.0;

void Print_Graph(double device_time, double bg, double basal) {
	const double one_hour = 1.0 / 24.0;
	const double one_minute = one_hour / 60.0;

	if (first_day < 0.0) {
		first_day = trunc(device_time);
		printf("day number. hour:minute  | denotes basal insulin rate, * denotes blood glucose level, range 0 to 15 both\n");
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
	for (size_t i = 0; i < (size_t)graph_width; i++) {
		if (i == bg_offset) ln[i] = '*';
		else if (i == basal_offset) ln[i] = '|';
		else ln[i] = ' ';
	}

	printf("%s", ln);
}