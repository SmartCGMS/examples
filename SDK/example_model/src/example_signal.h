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

#pragma once

#include <rtl/Common_Calculated_Signal.h>

#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

class CExample_Model_Signal : public virtual CCommon_Calculated_Signal {

	private:
		// stored reference to a different signal within a segment; this is retrieved during object construction
		scgms::SSignal mIGSignal;

	public:
		CExample_Model_Signal(scgms::WTime_Segment segment);
		virtual ~CExample_Model_Signal() = default;

		// scgms::ISignal iface
		virtual HRESULT IfaceCalling Get_Continuous_Levels(scgms::IModel_Parameter_Vector *params, const double* times, double* const levels, const size_t count, const size_t derivation_order) const final;
		virtual HRESULT IfaceCalling Get_Default_Parameters(scgms::IModel_Parameter_Vector *parameters) const final;
};

#pragma warning( pop )
