#include "descriptor.h"
#include "example_metric.h"

#include <rtl/UILib.h>
#include <rtl/SolverLib.h>
#include <rtl/descriptor_utils.h>

#include <array>

/*
 * Example metric descriptor block
 */

namespace example_metric {

	scgms::TMetric_Descriptor descriptor = {
		id,
		L"Example metric"
	};
}

/*
 * Array of available metric descriptors
 */

const std::array<scgms::TMetric_Descriptor, 1> metric_descriptors = { { example_metric::descriptor } };

/*
 * Metric library interface implementations
 */

HRESULT IfaceCalling do_get_metric_descriptors(scgms::TMetric_Descriptor const **begin, scgms::TMetric_Descriptor const **end) {

	return do_get_descriptors(metric_descriptors, begin, end);
}

HRESULT IfaceCalling do_create_metric(const scgms::TMetric_Parameters *parameters, scgms::IMetric **metric) {

	if (parameters->metric_id == example_metric::id)
		return Manufacture_Object<CExample_Metric>(metric, *parameters);

	return E_NOTIMPL;
}
