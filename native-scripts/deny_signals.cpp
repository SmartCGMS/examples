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
 * deny_signals 
 * You can configure up to 9 signals (native::max_signal_count-1), which will be dropped.
 * -1 because we need to set the first, synchronization signal to scgms::signal_All, to get invoked always.
 * 
 * If the incoming signal is one of the configured ones, then 
 * environment.current_signal_index < native::max_signal_count
 * Hence, we can use it for a quick check whether it is a configured signal or not.
 *
 */

void execute([[maybe_unused]] GUID& sig_id, [[maybe_unused]] double& device_time, [[maybe_unused]] double& level,
	HRESULT& rc, TNative_Environment& environment, [[maybe_unused]] const void* context) {

	if (environment.current_signal_index < native::max_signal_count)
		//There are two ways to drop this event
		//	scgms::signal_Null - like moving it to /dev/nul
		//	S_FALSE - faster, returns success code indicating that the signal was not forward on purpose
		//sig_id = scgms::signal_Null;	//succes, but throw this event away
		rc = S_FALSE;
}