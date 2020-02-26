#include "example_discrete_model.h"
#include "descriptor.h"

#include <rtl/SolverLib.h>

CExample_Discrete_Model::CExample_Discrete_Model(scgms::IModel_Parameter_Vector *parameters, scgms::IFilter *output)
	: CBase_Filter(output),
	  mParameters(scgms::Convert_Parameters<example_discrete_model::TParameters>(parameters, example_discrete_model::default_parameters.vector)) {

}

CExample_Discrete_Model::~CExample_Discrete_Model() {

}

HRESULT CExample_Discrete_Model::Do_Execute(scgms::UDevice_Event event) {

	// just forward the event through the filter chain
	return Send(event);
}

HRESULT CExample_Discrete_Model::Do_Configure(scgms::SFilter_Configuration configuration, refcnt::Swstr_list& error_description) {

	return S_OK;
}

HRESULT IfaceCalling CExample_Discrete_Model::Set_Current_Time(const double new_current_time) {

	mCurrent_Time = new_current_time;

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
	evt.level() = level;
	return SUCCEEDED(Send(evt));
}
