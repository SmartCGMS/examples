#include "descriptor.h"

#include <lang/dstrings.h>
#include <iface/DeviceIface.h>
#include <rtl/SolverLib.h>
#include <rtl/descriptor_utils.h>
#include <rtl/manufactory.h>

#include "example_discrete_model.h"

#include <vector>

/*
 * Example discrete model descriptor block
 */

namespace example_discrete_model {

	const wchar_t *model_param_ui_names[model_param_count] = {
		L"low",
		L"high"
	};

	const scgms::NModel_Parameter_Value model_param_types[model_param_count] = {
		scgms::NModel_Parameter_Value::mptDouble,
		scgms::NModel_Parameter_Value::mptDouble
	};

	constexpr size_t number_of_calculated_signals = 1;

	const GUID calculated_signal_ids[number_of_calculated_signals] = {
		blood_signal_id
	};

	const wchar_t* calculated_signal_names[number_of_calculated_signals] = {
		L"Blood glucose"
	};

	const GUID reference_signal_ids[number_of_calculated_signals] = {
		scgms::signal_BG
	};

	scgms::TModel_Descriptor descriptor = {
		model_id,
		L"Example discrete model",
		nullptr,
		model_param_count,
		model_param_types,
		model_param_ui_names,
		nullptr,
		lower_bounds.vector,
		default_parameters.vector,
		upper_bounds.vector,

		number_of_calculated_signals,
		calculated_signal_ids,
		reference_signal_ids,
	};

	const scgms::TSignal_Descriptor bg_desc{ blood_signal_id, L"Example discrete model - blood glucose", dsmmol_per_L, scgms::NSignal_Unit::mmol_per_L, 0xFFFF0000, 0xFFFF0000, scgms::NSignal_Visualization::smooth, scgms::NSignal_Mark::none, nullptr };
}

/*
 * Array of available model and signal descriptors
 */

const std::array<scgms::TModel_Descriptor, 1> model_descriptors = { { example_discrete_model::descriptor } };

const std::array<scgms::TSignal_Descriptor, 1> signal_descriptors = { { example_discrete_model::bg_desc } };

/*
 * Discrete model library interface implementations
 */

extern "C" HRESULT IfaceCalling do_get_model_descriptors(scgms::TModel_Descriptor **begin, scgms::TModel_Descriptor **end) {

	return do_get_descriptors(model_descriptors, begin, end);
}

extern "C" HRESULT IfaceCalling do_get_signal_descriptors(scgms::TSignal_Descriptor **begin, scgms::TSignal_Descriptor **end) {

	return do_get_descriptors(signal_descriptors, begin, end);
}

extern "C" HRESULT IfaceCalling do_create_discrete_model(const GUID *model_id, scgms::IModel_Parameter_Vector *parameters, scgms::IFilter *output, scgms::IDiscrete_Model **model) {

	if (*model_id == example_discrete_model::model_id) {
		return Manufacture_Object<CExample_Discrete_Model>(model, parameters, output);
	}

	return E_NOTIMPL;
}
