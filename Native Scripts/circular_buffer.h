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
 * a) For non-profit, academic research, this software is available under the
 *      GPLv3 license.
 * b) For any other use, especially commercial use, you must contact us and
 *       obtain specific terms and conditions for the use of the software.
 * c) When publishing work with results obtained using this software, you agree to cite the following paper:
 *       Tomas Koutny and Martin Ubl, "Parallel software architecture for the next generation of glucose
 *       monitoring", Procedia Computer Science, Volume 141C, pp. 279-286, 2018
 */

#pragma once

#include <iface/DeviceIface.h>

template <size_t N>
struct TCircular_Buffer {	
protected:
	struct TLevel {
		double device_time, level;
	};

	size_t head = 0;	//head must be zero initially due to the native script memory initialization
	size_t element_count = 0;
	std::array<TLevel, N> levels;
public:
	//caller is responsible for pushing values with monotonically increasing device time
	void push(const double device_time, const double level) noexcept {

		//when pushing, we will be continuously overwriting the past values
		levels[head].device_time = device_time;
		levels[head].level = level;
		head = (head + 1) % N;
		element_count = std::max(element_count, head);
	}

	//returns nan if there's no such level
	double level(const double desired_time, const size_t derivative_order) const noexcept  {
		//we need to find such a level whose device time greater than the desired device time

		auto next_pos = [](const size_t idx) {return (idx + 1) % N; };
		auto prev_pos = [](const size_t idx) { return idx > 0 ? idx - 1 : N - 1; };

		auto get_k = [&prev_pos, this](const size_t idx)->double {
			auto& hi = levels[idx];
			auto& lo = levels[prev_pos(idx)];
			const double dx = hi.device_time - lo.device_time;
			const double dy = hi.level - lo.level;

			return dy != 0.0 ? dx / dy : 0.0;
		};

		size_t idx = element_count >= N ? head : 0; //let's search from the oldest value		
		bool found = false;		

		do  {
			found = levels[idx].device_time >= desired_time;
			if (found) 
				break;			

			idx = next_pos(idx);
		} while (idx != head);

		if (!found)
			return std::numeric_limits<double>::quiet_NaN();

		
		switch (derivative_order) {
			case scgms::apxNo_Derivation:
				if (desired_time != levels[idx].device_time) {
					//we need to adjust the level by going to the past, if possible
					idx = prev_pos(idx);	//we go one level back to the past - i.e.; the nearest time that pre-dates the desired level
					if (idx >= element_count-1)
						return std::numeric_limits<double>::quiet_NaN();	//don't have the data

					const double k = get_k(idx);
					return levels[idx].level - k * (levels[idx].device_time - desired_time);
				} else
					//exact match
					return levels[idx].level;
				

			case scgms::apxFirst_Order_Derivation:
				return next_pos(head) != idx ? get_k(idx) : std::numeric_limits<double>::quiet_NaN();

			default:
				return 0.0;
		}
	}
};