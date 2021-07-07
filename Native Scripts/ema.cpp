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


struct TCustom_Data {	
	double recent = 0.0;
};


#include <iface/NativeIface.h>

/*
 * Exponential Moving Average
 * Required Signal 1 - signal to be weighted
 * Required Signal 2 - the weighted signal
 * 
 
 */



void execute([[maybe_unused]] GUID& sig_id,double& device_time, double& level,
	[[maybe_unused]] HRESULT& rc, TNative_Environment& environment, const void* context) {

	if (environment.current_signal_index == 0) {
		auto& recent = environment.custom_data->recent;

		if (recent != 0.0) {
		  const auto& alpha = environment.parameters[0];

		  recent = alpha*level + (1.0-alpha)*recent;
		} else
		   recent = level;

		environment.send(&environment.signal_id[1], device_time, recent, nullptr, context);

	}
		
}