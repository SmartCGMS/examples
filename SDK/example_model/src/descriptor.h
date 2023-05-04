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

#pragma once

#include <rtl/guid.h>

namespace example_model {

	const size_t param_count = 2;
	const double default_parameters[param_count] = { 1.0, 1.0 };

	struct TParameters {
		union {
			struct {
				double example_amplitude;
				double example_frequency;
			};
			double vector[param_count];
		};
	};

	constexpr GUID model_id = { 0xac716662, 0x3fb, 0x497f, { 0x88, 0x54, 0xde, 0x7a, 0xf1, 0x46, 0x72, 0x5e } }; // {AC716662-03FB-497F-8854-DE7AF146725E}
	constexpr GUID signal_id = { 0x315feca4, 0x720b, 0x4881, { 0x92, 0x11, 0xae, 0x75, 0x13, 0x60, 0x7b, 0x46 } }; // {315FECA4-720B-4881-9211-AE7513607B46}
}
