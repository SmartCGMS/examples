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

#include "descriptor.h"
#include "example_signal.h"

#include <scgms/iface/DeviceIface.h>
#include <scgms/iface/FilterIface.h>
#include <scgms/rtl/FilterLib.h>
#include <scgms/rtl/manufactory.h>
#include <scgms/utils/descriptor_utils.h>

#include <array>

/*
 * Example model and signal descriptor block
 */

namespace example_model {

	const scgms::NModel_Parameter_Value param_types[param_count] = { scgms::NModel_Parameter_Value::mptDouble, scgms::NModel_Parameter_Value::mptDouble };

	const wchar_t *param_names[param_count] = { L"Example amplitude", L"Example frequency" };
	const wchar_t *param_columns[param_count] = { L"amplitude", L"frequency" };

	const double lower_bound[param_count] = { 0.0 };
	const double upper_bound[param_count] = { 2.0 };

	const size_t signal_count = 1;

	const GUID signal_ids[signal_count] = { signal_id };
	const wchar_t *signal_names[signal_count] = { L"Example signal" };
	const GUID reference_signal_ids[signal_count] = { scgms::signal_IG };

	const scgms::TModel_Descriptor model_descriptor = {
		model_id,
		scgms::NModel_Flags::Signal_Model,
		L"Example model",
		L"example_model",
		param_count,
		param_count,
		param_types,
		param_names,
		param_columns,
		lower_bound,
		default_parameters,
		upper_bound,
		signal_count,
		signal_ids,
		reference_signal_ids
	};

	const scgms::TSignal_Descriptor signal_descriptor = {
		signal_ids[0],
		signal_names[0],
		dsmmol_per_L,							// unit description; used for displaying in various views, etc.
		scgms::NSignal_Unit::mmol_per_L,		// if SmartCGMS supports given unit, it should be chosen from enumeration
		0xFFFF0000,
		0xFFFF0000,
		scgms::NSignal_Visualization::mark,
		scgms::NSignal_Mark::none,
		nullptr
	};
}

/*
 * Array of available model and signal descriptors
 */

const std::array<scgms::TModel_Descriptor, 1> model_descriptions = { { example_model::model_descriptor } };

const std::array<scgms::TSignal_Descriptor, 1> signal_descriptions = { { example_model::signal_descriptor } };

/*
 * Model and signal library interface implementations
 */

extern "C" HRESULT IfaceCalling do_get_model_descriptors(scgms::TModel_Descriptor **begin, scgms::TModel_Descriptor **end) {

	return do_get_descriptors(model_descriptions, begin, end);
}

extern "C" HRESULT IfaceCalling do_get_signal_descriptors(scgms::TSignal_Descriptor * *begin, scgms::TSignal_Descriptor * *end) {

	return do_get_descriptors(signal_descriptions, begin, end);
}

extern "C" HRESULT IfaceCalling do_create_signal(const GUID *calc_id, scgms::ITime_Segment *segment, const GUID* approx_id, scgms::ISignal **signal) {

	if ((calc_id == nullptr) || (segment == nullptr)) return E_INVALIDARG;

	if (*calc_id == example_model::signal_id) {
		scgms::WTime_Segment wsegment{ segment };
		return Manufacture_Object<CExample_Model_Signal>(signal, wsegment);
	}

	return E_NOTIMPL;
}
