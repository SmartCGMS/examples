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

#include "example_signal.h"
#include "descriptor.h"

#include <scgms/rtl/SolverLib.h>

#include <cmath>
#include <stdexcept>

CExample_Model_Signal::CExample_Model_Signal(scgms::WTime_Segment segment) : CCommon_Calculated_Signal(segment), mIGSignal{ segment.Get_Signal(scgms::signal_IG) } {
	if (!refcnt::Shared_Valid_All(mIGSignal)) throw std::runtime_error{ "Could not retrieve IG signal from segment (fatal error)" };
}

HRESULT IfaceCalling CExample_Model_Signal::Get_Continuous_Levels(scgms::IModel_Parameter_Vector *params,
	const double* times, double* const levels, const size_t count, const size_t derivation_order) const {

	// 1) convert generic parameter container to model-specific parameter set container
	example_model::TParameters parameters = scgms::Convert_Parameters<example_model::TParameters>(params, example_model::default_parameters);

	HRESULT rc = S_OK;

	// 2) retrieve IG values - for some reason we may base our model outputs on IG
	std::vector<double> ig_values(count);
	rc = mIGSignal->Get_Continuous_Levels(nullptr, times, ig_values.data(), count, scgms::apxNo_Derivation);
	if (!Succeeded(rc)) {
		return rc;
	}

	// 3) model output based on input values

	// example model output - sine wave added to IG levels for no apparent reason
	for (size_t i = 0; i < count; i++) {

		// if not IG is available (should not happen at all - IG is a reference signal, hence is always available at a given time), output NaN
		// quiet_NaN on model output signals the calculate filter to "ask for a value again later"
		if (std::isnan(ig_values[i])) {
			levels[i] = std::numeric_limits<double>::quiet_NaN();
		} else {
			levels[i] = ig_values[i] + parameters.example_amplitude * std::sin(times[i] * parameters.example_frequency);
		}
	}

	return rc;
}

HRESULT IfaceCalling CExample_Model_Signal::Get_Default_Parameters(scgms::IModel_Parameter_Vector *parameters) const {

	double *params = const_cast<double*>(example_model::default_parameters);
	return parameters->set(params, params + example_model::param_count);
}
