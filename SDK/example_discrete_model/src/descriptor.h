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

#pragma once

#include <rtl/guid.h>

namespace example_discrete_model {

	constexpr GUID model_id = { 0xeebef929, 0x1a1c, 0x4242, { 0xb0, 0x34, 0xa9, 0x68, 0x69, 0x39, 0xa1, 0xea } };			// {EEBEF929-1A1C-4242-B034-A9686939A1EA}
	constexpr GUID blood_signal_id = { 0x1fcd6062, 0xa0b3, 0x41a6, { 0xa5, 0xbc, 0x2a, 0xf8, 0x6e, 0x22, 0x9a, 0xb3 } };	// {1FCD6062-A0B3-41A6-A5BC-2AF86E229AB3}

	constexpr size_t model_param_count = 2;

	struct TParameters {
		union {
			struct {
				double low, high;
			};
			double vector[model_param_count];
		};
	};

	constexpr example_discrete_model::TParameters lower_bounds = { { {
		1.0,		// a
		3.0,		// b
	}} };
	constexpr example_discrete_model::TParameters default_parameters = { { {
		3.0,		// a
		10.0,		// b
	}} };
	constexpr example_discrete_model::TParameters upper_bounds = { { {
		10.0,		// a
		50.0,		// b
	}} };
}
