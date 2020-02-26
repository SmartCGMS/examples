#pragma once

#include <rtl/FilterLib.h>
#include <rtl/referencedImpl.h>
#include <rtl/SolverLib.h>
#include <rtl/UILib.h>

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
