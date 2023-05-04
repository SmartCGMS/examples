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

#include <iface/NativeIface.h>

#include <cmath>
#include <tuple>

/*
  This will get the current level and attempt to return adjusted levels based on the environment.current_signal_index
  Returns <adjusted computed idx signal level, dtto reference signal, adjusted device time and bool, whether the result is valid.
 */
template <size_t computed_idx, size_t reference_idx>
inline std::tuple<double, double, double, bool> Adjust_Levels(const TNative_Environment& environment) {
	std::tuple<double, double, double, bool> result{ std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN(), false };

	const auto sig_idx = environment.current_signal_index;
	if ((sig_idx == computed_idx) || (sig_idx == reference_idx)) {
		//we are interested in this signal				
		const auto slope_class = std::fpclassify(environment.slope[sig_idx]);
		if ((slope_class == FP_NORMAL) || (slope_class == FP_ZERO)) { //we need the slope to calculate the error
																	  //zero is also a valid slope

			//let's find the desired distance
			const double max_time_distance = std::isnormal(environment.parameters[0]) ? environment.parameters[0] : 2.5 * scgms::One_Minute;

			//let's find the time of the-other-signal's recent level		
			const auto other_idx = sig_idx == computed_idx ? reference_idx : computed_idx;

			const double other_time = environment.device_time[other_idx];
			if (std::isnormal(other_time)) {

				const double time_distance = environment.device_time[sig_idx] - other_time;
				//in the comparison, do not forget that some signals might be in the future => test for positivity
				if ((time_distance < max_time_distance) && (time_distance >= 0.0)) {

					//we are in the desired time range, let's step back the current level back in time					
					const double adjusted_level = environment.level[sig_idx] - time_distance * environment.slope[sig_idx];

					std::get<0>(result) = sig_idx == computed_idx ? adjusted_level : environment.level[other_idx];
					std::get<1>(result) = sig_idx == computed_idx ? environment.level[other_idx] : adjusted_level;
					std::get<2>(result) = other_time;
					std::get<3>(result) = true;
				}
			}
		}
	}

	return result;
}