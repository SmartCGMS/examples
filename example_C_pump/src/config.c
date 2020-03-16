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

#include "config.h"

/* Content of the config.ini file.
 * Alternatively, you can use bin2hex
 * or load it from an external file 
 * at the runtime.
 *
 * Use the visualization.ini config
 * to view the result of the C-pump
 * glucose-level regulation.
 */

const char* configuration =
	"; Signal generator\n"
	"[Filter_001_{9EEB3451-2A9D-49C1-BA37-2EC0B00E5E6D}]\n"
	"Model = {8114B2A6-B4B2-4C8D-A029-625CBDB682EF}\n"
	"Feedback_Name =\n"
	"Synchronize_To_Signal = false\n"
	"Synchronization_Signal = {FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF}\n"
	"Stepping = 0.003472\n"
	"Maximum_Time = 3.000000\n"
	"Shutdown_After_Last = true\n"
	"Parameters = 0.005000 0.005000 0.000000 0.100000 0.001000 0.001000 8.000000 10.000000 0.050000 0.010000 0.010000 0.010000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 -0.500000 -10.000000 0.000000 -1.000000 0.000000 0.050000 0.028735 0.028344 0.000050 0.300000 0.100000 0.100000 12.000000 70.000000 0.220000 0.050000 0.050000 0.040000 95.000000 9.200000 220.000000 100.000000 0.000000 0.000000 0.000000 0.000000 0.000000 95.000000 0.000000 0.929000 -0.037000 0.000000 0.023310 0.000000 0.018600 0.350000 0.100000 0.100000 0.050000 1.000000 0.300000 0.300000 18.000000 100.000000 1.000000 1.000000 1.000000 1.000000 200.000000 20.000000 300.000000 300.000000 100.000000 200.000000 150.000000 150.000000 50.000000 300.000000 5.000000 2.000000 0.000000 5.000000 0.041667 0.000000 0.041667 1.500000\n"

	"; Signal mapping\n"
	"[Filter_002_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {582CFE11-68B9-4E5E-B50B-1978C077A84E}\n"
	"Signal_Dst_Id = {3034568D-F498-455B-AC6A-BCF301F69C9E}\n"

	"; Signal mapping\n"
	"[Filter_003_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {E1CD07EF-B079-4911-B79B-D203486101C8}\n"
	"Signal_Dst_Id = {F666F6C2-D7C0-43E8-8EE1-C8CAA8F860E5}\n"

	"; Signal decoupling\n"
	"[Filter_004_{0BB71190-8709-4990-97FA-6A4DB679EF1D}]\n"
	"Signal_Src_Id = {F666F6C2-D7C0-43E8-8EE1-C8CAA8F860E5}\n"
	"Signal_Dst_Id = {706E7FDB-8F22-486F-BFA5-6A56D3514209}\n"
	"Remove_from_source = false\n"
	"Condition = is_level_event && ((level >= 3.9) && (level <= 10.0))\n"
	"Collect_statistics = true\n"
	"Output_CSV_file = time_in_range.csv\n"

	//for the clarity of the output log, 
	//let us throw away the rest of the in-silico patient signals
	"; Signal mapping\n"
	"[Filter_005_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {3BD439F0-8B5A-4004-9B70-24E5E2523B7A}\n"
	"Signal_Dst_Id = {706E7FDB-8F22-486F-BFA5-6A56D3514209}\n"

	"; Signal mapping\n"
	"[Filter_006_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {9CDDC004-12CA-4380-96FF-8319A8CE79A8}\n"
	"Signal_Dst_Id = {706E7FDB-8F22-486F-BFA5-6A56D3514209}\n"

	"; Signal mapping\n"
	"[Filter_007_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {755CFD08-2B12-43B6-A455-58061568446E}\n"
	"Signal_Dst_Id = {706E7FDB-8F22-486F-BFA5-6A56D3514209}\n"

	"; Signal mapping\n"
	"[Filter_008_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {FC556839-D6C0-4646-A346-0821D25F7E29}\n"
	"Signal_Dst_Id = {706E7FDB-8F22-486F-BFA5-6A56D3514209}\n"

	"; Log\n"
	"[Filter_009_{C0E942B9-3928-4B81-9B43-A347668200BA}]\n"
	"Log_File = C_pump.log";