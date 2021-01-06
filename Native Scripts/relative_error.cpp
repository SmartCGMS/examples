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
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#include <iface/NativeIface.h>

#include <cmath>

/*
 * Computes absolute-error signal 
 * Set the synchronization signal to scgms::signal_All to get invoked always
 * Set the required signal 1 to the computed signal, which contains an error
 * Set the required signal 2 to the reference signal, against which to compare
 *	Actually, it does not matter with absolute error, but it does with relative error.
 * Set the required signal 3 to the desired output-signal id
 * Set the first parameter either to nan, zero or the maximum time distance of two levels
 *	For example, CGMS has sampling period of 5 minutes. Hence, the maximum distance is 2.5 minute.
 *  nan and zero means to use a default value, which we set to 2.5 minute
 *
 * 
 * Unlike SmartCGMS's error filter, this computation is less precise but requires no additional memory.
 * In the practice, with e.g., CGMS, the precision loss should be clinically insignificant with properly configured signals.
 */

void execute(GUID& sig_id, double& device_time, double& level,
	HRESULT& rc, const TNative_Environment& environment, const void* context) {

	const auto sig_idx = environment.current_signal_index;
	if ((sig_idx == 1) || (sig_idx == 2)) {
		//we are interested in this signal				
		const auto slope_class = std::fpclassify(environment.slope[sig_idx]);
		if ((slope_class == FP_NORMAL) || (slope_class == FP_ZERO)) { //we need the slope to calculate the error
																	  //zero is also a valid slope

			//let's find the desired distance
			const double max_time_distance = std::isnormal(environment.parameters[0]) ? environment.parameters[0] : 2.5 * scgms::One_Minute;

			//let's find the time of the-other-signal's recent level		
			const auto other_idx = sig_idx ^ 3; //sig_idx == 1 ? 2 : 1;

			const double other_time = environment.device_time[other_idx];
			if (std::isnormal(other_time)) {

				const double time_distance = device_time - other_time;
				//in the comparison, do not forget that some signals might be in the future => test for positivity
				if ((time_distance < max_time_distance) && (time_distance >= 0.0)) {

					//we are in the desired time range, let's step back the current level back in time					
					const double adjusted_level = level - time_distance * environment.slope[sig_idx];

					//we are almost there - we just need to get the correctly adjusted reference level
					const double reference_level = sig_idx == 1 ? environment.level[other_idx] : adjusted_level;

					//and that's it - we have erything we need to produce the error signal's next level
					if (reference_level != 0.0) {
						rc = environment.send(&environment.signal_id[3],
							other_time,
							std::fabs((adjusted_level - environment.level[other_idx])/reference_level),
							nullptr, context);
					}
				}
			}
		}
	}
}