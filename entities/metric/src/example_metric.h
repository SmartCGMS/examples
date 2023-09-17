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

#include <scgms/rtl/FilterLib.h>
#include <scgms/rtl/referencedImpl.h>
#include <scgms/rtl/SolverLib.h>
#include <scgms/rtl/UILib.h>

#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

class CExample_Metric : public virtual scgms::IMetric, public virtual refcnt::CReferenced {

	protected:
		const scgms::TMetric_Parameters mParameters;
		std::vector<double> mDifferences;
	public:
		CExample_Metric(const scgms::TMetric_Parameters &params);
		virtual ~CExample_Metric();

		virtual HRESULT IfaceCalling Accumulate(const double *times, const double *reference, const double *calculated, const size_t count) final;
		virtual HRESULT IfaceCalling Reset() final;
		virtual HRESULT IfaceCalling Calculate(double *metric, size_t *levels_accumulated, size_t levels_required) final;
		virtual HRESULT IfaceCalling Get_Parameters(scgms::TMetric_Parameters *parameters) final;
};

#pragma warning( pop )
