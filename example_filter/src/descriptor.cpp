/* Examples and Documentation for
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
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "descriptor.h"
#include "example_filter.h"

#include <iface/DeviceIface.h>
#include <iface/FilterIface.h>

#include <rtl/FilterLib.h>
#include <rtl/manufactory.h>
#include <utils/descriptor_utils.h>

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
