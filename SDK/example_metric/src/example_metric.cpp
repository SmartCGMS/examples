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

#include "example_metric.h"

#include <cmath>

CExample_Metric::CExample_Metric(const scgms::TMetric_Parameters &params)
	: mParameters{ params } {

}

CExample_Metric::~CExample_Metric() {

}

HRESULT IfaceCalling CExample_Metric::Accumulate(const double *times, const double *reference, const double *calculated, const size_t count) {

	for (size_t i = 0; i < count; i++) {

		double diff = std::fabs(reference[i] - calculated[i]);
		// the difference should not be NaN, nor infinity
		if (std::isnan(diff) || std::isinf(diff)) {
			continue;
		}

		if (mParameters.use_relative_error) {
			diff /= std::fabs(calculated[i]);
		}
		if (mParameters.use_squared_differences) {
			diff *= diff;
		}

		// push back valid difference to internal vector
		mDifferences.push_back(diff);
	}

	return S_OK;
}

HRESULT IfaceCalling CExample_Metric::Reset() {

	mDifferences.clear();

	return S_OK;
}

HRESULT IfaceCalling CExample_Metric::Calculate(double *metric, size_t *levels_accumulated, size_t levels_required) {

	// did we satisfy the input requirement on level count?
	*levels_accumulated = mDifferences.size();
	if (*levels_accumulated < std::max(static_cast<size_t>(1), levels_required)) {
		return S_FALSE;
	}


	// let us calculate average absolute difference as an example of such metric
	double metricValue = 0;
	
	for (size_t i = 0; i < mDifferences.size(); i++) {
		metricValue += mDifferences[i];
	}
	metricValue /= static_cast<double>(*levels_accumulated);


	// satisfy the prefer_more_levels requirement, if requested
	if (mParameters.prefer_more_levels) {
		metricValue /= static_cast<double>(*levels_accumulated);
	}

	// set the resulting metric
	*metric = metricValue;

	return S_OK;
}

HRESULT IfaceCalling CExample_Metric::Get_Parameters(scgms::TMetric_Parameters *parameters) {

	memcpy(parameters, &mParameters, sizeof(mParameters));

	return S_OK;
}
