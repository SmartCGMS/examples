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


#include "circular_buffer.h"



constexpr size_t recent_levels_count = 10;	//must not be greater than  native::max_parameter_count


struct TCustom_Data {	
	TCircular_Buffer<recent_levels_count*2> recent_levels;	//*2 to mask excessive levels
};


#include <iface/NativeIface.h>

#include <array>
#include <cmath>


const std::array<double, recent_levels_count> time_offset = {	  0.0,						 -5.0 * scgms::One_Minute,
																-10.0 * scgms::One_Minute,	-15.0 * scgms::One_Minute,
																-20.0 * scgms::One_Minute,	-25.0 * scgms::One_Minute,
																-30.0 * scgms::One_Minute,	-35.0 * scgms::One_Minute,
																-40.0 * scgms::One_Minute,	-45.0 * scgms::One_Minute};										


/*
 * Weighted Moving Average
 * Required Signal 1 - signal to be weighted
 * Required Signal 2 - the weighted signal
 * 
 
 */



void execute([[maybe_unused]] GUID& sig_id,double& device_time, double& level,
	[[maybe_unused]] HRESULT& rc, TNative_Environment& environment, const void* context) {

	if (environment.current_signal_index == 0) {
		auto& levels = environment.custom_data->recent_levels;
		const auto& params = environment.parameters;

		levels.push(device_time, level);
		
		double result = 0.0;
		bool no_nan = true;
		for (size_t i = 0; i < recent_levels_count; i++) {
                        const auto &current_parameter = params[i];

                        if (std::isnormal(current_parameter)) {
                            const double offseted_level = levels.level(device_time + time_offset[i], scgms::apxNo_Derivation);
                            if (std::isnan(offseted_level)) {
                                    no_nan = false;
                                    break;
                            }

                            result += offseted_level * current_parameter;
                        }
		}

		if (no_nan) {
			environment.send(&environment.signal_id[1], device_time, result, nullptr, context);
		}
	}
		
}
