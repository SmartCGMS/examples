#include "example_metric.h"

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
