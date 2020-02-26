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
