#include "descriptor.h"
#include "example_filter.h"

#include <iface/DeviceIface.h>
#include <iface/FilterIface.h>

#include <rtl/FilterLib.h>
#include <rtl/manufactory.h>
#include <rtl/descriptor_utils.h>

#include <array>

/*
 * Example filter descriptor block
 */

namespace example_filter {

	constexpr size_t param_count = 2;

	const scgms::NParameter_Type param_type[param_count] = {
		scgms::NParameter_Type::ptWChar_Array,
		scgms::NParameter_Type::ptDouble
	};

	const wchar_t* ui_param_name[param_count] = {
		L"Example string parameter",
		L"Example numeric parameter"
	};

	const wchar_t* rsExample_Config_String = L"example-string";
	const wchar_t* rsExample_Config_Double = L"example-number";

	const wchar_t* config_param_name[param_count] = {
		rsExample_Config_String,
		rsExample_Config_Double
	};

	const wchar_t* ui_param_tooltips[param_count] = {
		L"This is just an example of possible string filter parameter",
		L"This is just an example of possible numeric filter parameter"
	};

	const wchar_t* filter_name = L"Example filter";

	const scgms::TFilter_Descriptor descriptor = {
		id,
		scgms::NFilter_Flags::None,
		filter_name,
		param_count,
		param_type,
		ui_param_name,
		config_param_name,
		ui_param_tooltips
	};
}

/*
 * Array of available filter descriptors
 */

const std::array<scgms::TFilter_Descriptor, 1> filter_descriptions = { { example_filter::descriptor } };

/*
 * Filter library interface implementations
 */

extern "C" HRESULT IfaceCalling do_get_filter_descriptors(scgms::TFilter_Descriptor **begin, scgms::TFilter_Descriptor **end) {

	return do_get_descriptors(filter_descriptions, begin, end);
}

extern "C" HRESULT IfaceCalling do_create_filter(const GUID *id, scgms::IFilter *output, scgms::IFilter **filter) {

	if (*id == example_filter::descriptor.id) {
		return Manufacture_Object<CExample_Filter>(filter, output);
	}

	return E_NOTIMPL;
}
