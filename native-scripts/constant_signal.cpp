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

#include <iface/NativeIface.h>


/*
 * Constant signal
 * First Required Signal - signal on which to emit the constant signal, could be scgms::signal_All too
 * First parameter - the constant signal to be emitted
 * 
 
 */



void execute([[maybe_unused]] GUID& sig_id,double& device_time, [[maybe_unused]] double& level,
	[[maybe_unused]] HRESULT& rc, TNative_Environment& environment, const void* context) {

	constexpr GUID constant_signal = { 0xfc616dd, 0x7bf, 0x49ea, { 0xbf, 0x2c, 0xfc, 0xa1, 0x39, 0x42, 0x9f, 0x5 } }; //{0FC616DD-07BF-49EA-BF2C-FCA139429F05}

	environment.send(&constant_signal, device_time, environment.parameters[0], nullptr, context);
}
