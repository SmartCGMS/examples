#pragma once

#include <rtl/FilterLib.h>
#include <rtl/referencedImpl.h>
#include <rtl/UILib.h>

#include "descriptor.h"

#pragma warning( push )
#pragma warning( disable : 4250 ) // C4250 - 'class1' : inherits 'class2::member' via dominance

class CExample_Discrete_Model : public virtual scgms::CBase_Filter, public virtual scgms::IDiscrete_Model {

	private:
		static constexpr double Blood_Step_Per_Minute = 0.1;

	private:
		example_discrete_model::TParameters mParameters;

		// we have to hold the current timestamp
		double mCurrent_Time = 0;

		// hold current internal state of the model (here, just one quantity, to demonstrate the principles)
		double mCur_Blood_Value = 5.5;

		// to demonstrate discrete model properties, we implement "triangle" function - this is helper enum
		enum class NDirection {
			Up,
			Down
		};
		// current direction
		NDirection mDirection = NDirection::Up;

	protected:
		// scgms::CBase_Filter iface implementation
		virtual HRESULT Do_Execute(scgms::UDevice_Event event) override final;
		virtual HRESULT Do_Configure(scgms::SFilter_Configuration configuration, refcnt::Swstr_list& error_description) override final;

		void Advance_Model(double time_advance_delta);
		bool Emit_Blood_Level(double level, double time);

	public:
		CExample_Discrete_Model(scgms::IModel_Parameter_Vector *parameters, scgms::IFilter *output);
		virtual ~CExample_Discrete_Model();

		// scgms::IDiscrete_Model iface
		virtual HRESULT IfaceCalling Set_Current_Time(const double new_current_time) override final;
		virtual HRESULT IfaceCalling Step(const double time_advance_delta) override final;
};

#pragma warning( pop )
