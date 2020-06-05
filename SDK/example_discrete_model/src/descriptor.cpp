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

#include <lang/dstrings.h>
#include <iface/DeviceIface.h>
#include <rtl/SolverLib.h>
#include <utils/descriptor_utils.h>
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
		scgms::NModel_Flags::Discrete_Model,
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
	if (!model_id) return E_INVALIDARG;

	if (*model_id == example_discrete_model::model_id) {
		return Manufacture_Object<CExample_Discrete_Model>(model, parameters, output);
	}

	return E_NOTIMPL;
}
