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

#include "level.h"

/*
 * Computes relative-error signal 
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
	HRESULT& rc, TNative_Environment& environment, const void* context) {

	auto [computed, reference, adjusted_time, valid] = Adjust_Levels<1, 2>(environment);
	if (valid) {
		if (reference != 0.0)
			rc = environment.send(&environment.signal_id[3],
				adjusted_time,
				std::fabs((computed - reference) / reference),
				nullptr, context);
	}
}