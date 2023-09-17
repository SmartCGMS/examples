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
	"Model = {B387A874-8D1E-460B-A5EC-BA36AB7516DE}\n"
	"Time_Segment_Id = 1\n"
	"Feedback_Name = \n"
	"Synchronize_To_Signal = false\n"
	"Synchronization_Signal = {FFFFFFFF-FFFF-FFFF-FFFF-FFFFFFFFFFFF}\n"
	"Stepping = 0.003472\n"
	"Maximum_Time = 4.000000\n"
	"Shutdown_After_Last = true\n"
	"Parameters = 0.000000 0.000000 0.000000 20.000000 20.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 20.000000 10.000000 30.000000 10.200000 0.000000 1.000000 0.000000 0.200000 0.050000 0.400000 0.005000 0.005000 100.000000 0.010000 0.005000 0.001000 0.010000 0.050000 0.001000 0.050000 0.000100 0.000100 0.002000 0.050000 0.000010 20.000000 0.100000 0.200000 0.002000 0.008000 0.000100 0.000500 0.050000 1.000000 0.010000 0.010000 0.010000 1.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 0.000000 1.000000 1.000000 0.000000 0.000000 0.000000 265.370000 162.457000 5.504330 0.000000 100.250000 100.250000 3.207630 72.434200 141.154000 265.370000 102.320000 138.560000 100.250000 0.089060 0.046122 0.003793 0.703910 0.210570 1.915200 0.054906 0.031319 253.520000 0.087114 0.058138 0.027802 0.154460 0.225027 0.090011 0.231690 0.004637 0.004690 0.012080 0.900000 0.000500 339.000000 1.000000 3.266730 0.015200 0.076600 0.001900 0.007800 1.238620 4.731410 0.050000 0.050000 0.050000 10.000000 0.950000 0.120000 0.400000 0.300000 0.080000 0.020000 0.050000 30.000000 0.000000 15.000000 15.000000 500.000000 500.000000 500.000000 500.000000 500.000000 50.000000 300.000000 200.000000 300.000000 200.000000 500.000000 500.000000 500.000000 250.000000 300.000000 200.000000 0.800000 1.000000 0.500000 2.000000 2.000000 10.000000 2.000000 0.500000 500.000000 0.600000 0.200000 0.200000 0.900000 1.000000 1.000000 1.000000 0.050000 0.020000 0.500000 3.000000 0.010000 1000.000000 5.000000 20.000000 0.800000 0.900000 0.050000 0.100000 10.000000 20.000000 1.000000 1.000000 1.000000 100.000000 3.000000 1.000000 2.000000 2.000000 1.000000 0.800000 1.000000 200.000000 200.000000 100.000000 100.000000\n"
	"Time_Segment_Id = 1\n"
	"Echo_Default_Parameters_As_Event = false\n"

	"; Signal mapping\n"
	"[Filter_002_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {55B07D3D-0D99-47D0-8A3B-3E543C25E5B1}\n"
	"Signal_Dst_Id = {3034568D-F498-455B-AC6A-BCF301F69C9E}\n"

	"; Signal mapping\n"
	"[Filter_003_{8FAB525C-5E86-AB81-12CB-D95B1588530A}]\n"
	"Signal_Src_Id = {1EEE155A-9150-4958-8AFD-3161B73CF9FC}\n"
	"Signal_Dst_Id = {F666F6C2-D7C0-43E8-8EE1-C8CAA8F860E5}\n"

	"; Signal decoupling\n"
	"[Filter_004_{0BB71190-8709-4990-97FA-6A4DB679EF1D}]\n"
	"Signal_Src_Id = {F666F6C2-D7C0-43E8-8EE1-C8CAA8F860E5}\n"
	"Signal_Dst_Id = {706E7FDB-8F22-486F-BFA5-6A56D3514209}\n"
	"Remove_from_source = false\n"
	"Condition = is_level_event && ((level >= 3.9) && (level <= 10.0))\n"
	"Collect_statistics = false\n"
	"Output_CSV_file = time_in_range.csv\n"

	"; Log\n"
	"[Filter_005_{C0E942B9-3928-4B81-9B43-A347668200BA}]\n"
	"Log_File = C_pump.log\n";
