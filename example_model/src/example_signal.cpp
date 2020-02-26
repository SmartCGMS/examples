#include "example_signal.h"
#include "descriptor.h"

#include <rtl/SolverLib.h>

#include <cmath>
#include <stdexcept>

CExample_Model_Signal::CExample_Model_Signal(scgms::WTime_Segment segment) : CCommon_Calculated_Signal(segment), mIGSignal{ segment.Get_Signal(scgms::signal_IG) } {
	if (!refcnt::Shared_Valid_All(mIGSignal)) throw std::runtime_error{ "Could not retrieve IG signal from segment (fatal error)" };
}

HRESULT IfaceCalling CExample_Model_Signal::Get_Continuous_Levels(scgms::IModel_Parameter_Vector *params,
	const double* times, double* const levels, const size_t count, const size_t derivation_order) const {

	// 1) convert generic parameter container to model-specific parameter set container
	example_model::TParameters &parameters = scgms::Convert_Parameters<example_model::TParameters>(params, example_model::default_parameters);

	HRESULT rc = S_OK;

	// 2) retrieve IG values - for some reason we may base our model outputs on IG
	std::vector<double> ig_values(count);
	rc = mIGSignal->Get_Continuous_Levels(nullptr, times, ig_values.data(), count, scgms::apxNo_Derivation);
	if (!SUCCEEDED(rc)) {
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
