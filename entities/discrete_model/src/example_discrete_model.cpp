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

#include "example_discrete_model.h"
#include "descriptor.h"

#include <scgms/rtl/SolverLib.h>

CExample_Discrete_Model::CExample_Discrete_Model(scgms::IModel_Parameter_Vector *parameters, scgms::IFilter *output)
	: CBase_Filter(output),
	  mParameters(scgms::Convert_Parameters<example_discrete_model::TParameters>(parameters, example_discrete_model::default_parameters.vector)) {

}

CExample_Discrete_Model::~CExample_Discrete_Model() {

}

HRESULT CExample_Discrete_Model::Do_Execute(scgms::UDevice_Event event) {

	// just forward the event through the filter chain
	return mOutput.Send(event);
}

HRESULT CExample_Discrete_Model::Do_Configure(scgms::SFilter_Configuration configuration, refcnt::Swstr_list& error_description) {

	return S_OK;
}

HRESULT IfaceCalling CExample_Discrete_Model::Initialize(const double new_current_time, const uint64_t segment_id) {

	mCurrent_Time = new_current_time;
	mSegment_id = segment_id;

	return S_OK;
}

HRESULT IfaceCalling CExample_Discrete_Model::Step(const double time_advance_delta) {

	if (time_advance_delta > 0.0) {
		// really advance the model here
		Advance_Model(time_advance_delta);

		// advance the timestamp
		mCurrent_Time += time_advance_delta;
	}

	if (!Emit_Blood_Level(mCur_Blood_Value, mCurrent_Time)) {
		return E_FAIL;
	}

	return S_OK;
}

void CExample_Discrete_Model::Advance_Model(double time_advance_delta) {

	// note that time_advance_delta may be anything between zero and infinity, so the model must support stepping with various step sizes
	// if, for some reason, the model does not support such stepping, it is also allowed to emit levels at supported times - the model doesn't have
	// to emit levels on every Step method call, neither is required to emit just one value set

	// in our case, our example model is able to step with variable step size, so it's not a problem to advance the model just to the requested time

	double real_step = Blood_Step_Per_Minute * (time_advance_delta / scgms::One_Minute);

	if (mDirection == NDirection::Up) {

		mCur_Blood_Value += real_step;
		if (mCur_Blood_Value > mParameters.high) {
			mDirection = NDirection::Down;
		}
	} else if (mDirection == NDirection::Down) {
		mCur_Blood_Value -= real_step;
		if (mCur_Blood_Value < mParameters.low) {
			mDirection = NDirection::Up;
		}
	}
}

bool CExample_Discrete_Model::Emit_Blood_Level(double level, double time)
{
	scgms::UDevice_Event evt{ scgms::NDevice_Event_Code::Level };
	evt.signal_id() = example_discrete_model::blood_signal_id;
	evt.device_id() = example_discrete_model::model_id;
	evt.device_time() = time;
	evt.segment_id() = mSegment_id;
	evt.level() = level;
	return Succeeded(mOutput.Send(evt));
}
