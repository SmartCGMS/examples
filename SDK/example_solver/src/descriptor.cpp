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

#include "descriptor.h"
#include "example_solver.h"

#include <iface/DeviceIface.h>
#include <iface/UIIface.h>
#include <lang/dstrings.h>
#include <utils/descriptor_utils.h>
#include <rtl/UILib.h>
#include <rtl/SolverLib.h>

#include <array>

/*
 * Example solver descriptor block
 */

namespace example_solver {

	const scgms::TSolver_Descriptor descriptor = {
		id,
		L"Example solver",
		false,
		0,
		nullptr
	};
}

/*
 * Array of available solver descriptors
 */

const std::array<scgms::TSolver_Descriptor, 1> solver_descriptions = { example_solver::descriptor };

/*
 * Solver library interface implementations
 */

HRESULT IfaceCalling do_get_solver_descriptors(scgms::TSolver_Descriptor **begin, scgms::TSolver_Descriptor **end) {

	return do_get_descriptors(solver_descriptions, begin, end);
}

HRESULT IfaceCalling do_solve_generic(const GUID *solver_id, solver::TSolver_Setup *setup, solver::TSolver_Progress *progress) {

	if (*solver_id == example_solver::descriptor.id) {
		CExample_Solver solver(*setup);

		return solver.Solve(*progress);
	}

	return E_NOTIMPL;
}
