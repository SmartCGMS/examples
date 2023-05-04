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

#include "circular_buffer.h"

/*
 * Computes blood glucose level from the interstitial glucose level using the diffusion model, version 1
 * see https://doi.org/10.1016/j.compbiomed.2013.08.008
 * Set the first signal to the source signal, e.g.; the interstiital-fluid glucose level
 * Set the second signal to the blood glucose level; e.g.; diffusion bg
 *
 * 1st parameter is p
 * 2nd parameter is cg
 * 3rd parameter is c
 * 4th parameter is dt
 */



constexpr size_t recent_levels_count = 12;	//12 is one hour

struct TCustom_Data {
	TCircular_Buffer<recent_levels_count * 2> recent_levels;	//*2 to mask excessive levels
};

#include <iface/NativeIface.h>

#include <cmath>
#include <limits.h>

void execute([[maybe_unused]] GUID& sig_id, double& device_time, double& level,
	HRESULT& rc, TNative_Environment& environment, const void* context) {

	constexpr size_t input_sig_idx = 0;
	constexpr size_t output_sig_idx = 1;

	constexpr size_t idxP = 0;
	constexpr size_t idxCg = 1;
	constexpr size_t idxc = 2;
	constexpr size_t idxDt = 3;



	if (environment.current_signal_index == input_sig_idx) {
		//this is the source signal
		auto& levels = environment.custom_data->recent_levels;
		levels.push(device_time, level);


		const auto& parameters = environment.parameters;
		const double& ig_at_dt = level;
		const double present_time_t = device_time - parameters[idxDt];
		const double ig_at_t = levels.level(present_time_t, scgms::apxNo_Derivation);

		if (!std::isnan(ig_t)) {

			//original diffusion model
			//floattype alpha = params.cg;
			//floattype beta = params.p - alpha * presentist;
			//floattype gamma = params.c - futureist;
			//
			//floattype D = beta * beta - 4.0*alpha*gamma;
			//alpha*BG^2 + beta*BG + gamma == 0
			//BG = 0.5*(-beta+D^0.5)/alpha


			const double p = parameters[idx_P];			
			const double cg = parameters[idx_Cg];

			bg = std::numeric_limits<double>::quiet_Nan();
			if (cg != 0.0) {
				
				const double beta = p - cg * ig_at_t;
				const double gamma = parameters[idx_C] - ig_at_dt;
				const double D = std::max(0.0, beta * beta - 4.0 * cg * gamma);

				bg = 0.5 * (-beta + std::sqrt(D)) / cg;	
			}
			else {
				//if cg is zero, then it degraded to simple linear regression
				if (p != 0.0)
					bg = (level - parameters[idx_C]) / p;
				else
					bg = -parameters[idx_C];	//degraded to a constant
			}

			rc = environment.send(&environment.signal_id[output_sig_idx],
				present_time_t,
				bg,
				nullptr, context);
			
		}
	}

}