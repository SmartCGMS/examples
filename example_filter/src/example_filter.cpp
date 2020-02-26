#include "example_filter.h"
#include "descriptor.h"

CExample_Filter::CExample_Filter(scgms::IFilter *output) : CBase_Filter(output) {
	//
}

CExample_Filter::~CExample_Filter() {
	//
}

HRESULT IfaceCalling CExample_Filter::QueryInterface(const GUID*  riid, void ** ppvObj) {
	if (Internal_Query_Interface<scgms::IFilter>(example_filter::id, *riid, ppvObj)) return S_OK;

	return E_NOINTERFACE;
}

HRESULT IfaceCalling CExample_Filter::Do_Configure(scgms::SFilter_Configuration configuration, refcnt::Swstr_list& error_description) {

	// let us read configured parameters
	auto example_str = configuration.Read_String(example_filter::rsExample_Config_String);
	auto example_num = configuration.Read_Double(example_filter::rsExample_Config_Double, 123.0); // default value parameter is optional

	// here we can put an extra constraint on some of the configuration parameters
	// if the constraint fails, we should return appropriate error code according to COM specification
	if (example_num < 0) {
		error_description.push(L"Example number parameter must be non-negative!");
		return E_INVALIDARG;
	}

	// return S_OK on success
	return S_OK;
}

HRESULT IfaceCalling CExample_Filter::Do_Execute(scgms::UDevice_Event event) {

	// here we can work with the event, that was executed on us

	if (event.is_level_event()) {
		// if this is a level event, the "level" field is valid
		// we may, for some reason, want to modify it; note the level() call returns a l-value reference to an internal field
		//event.level() += 1.0;
	}
	else if (event.is_info_event()) {
		// if this is an info event, the "info" field is valid
		// we may, for some reason, want to read, parse it or overwrite it; note the "info" field is a container built on top of internal field
		//event.info.set(L"A new value of info event");
	}
	else if (event.is_parameters_event()) {
		// is this is a parameters event, the "parameters" field is valid
		// we may, for some reason, want to read and apply parameters; note the "parameters" field is a container built on top of internal field
		// the "signal_id" field always refers to a valid model signal GUID
		
		// example: read parameters one-by-one
		//double *begin, *end;
		//if (event.parameters->get(&begin, &end) == S_OK) {
		//	for (double* paramPtr = begin; paramPtr != end; paramPtr++) {
		//		double parameter = *paramPtr;
		//		// ...
		//	}
		//}
	}

	// let us emit a new event every time a "blood glucose" (BG) signal of "level" greater than 10.0 comes, just to demonstrate how to send a new event through the filter chain
	if (event.is_level_event() && event.signal_id() == scgms::signal_BG) {

		if (event.level() > 10.0) {

			scgms::UDevice_Event evt{ scgms::NDevice_Event_Code::Information };
			evt.device_id() = example_filter::id;
			evt.signal_id() = Invalid_GUID;
			evt.device_time() = event.device_time(); // reuse original event's device time
			evt.info.set(L"Example info: BG is greater than 10 mmol/L !");

			// additional event send failure should be detected and the error propagated either immediatelly, or after the original event sending
			// note this will prepend the new event before the original one
			auto rc = Send(std::move(evt));
			if (!SUCCEEDED(rc)) {
				return rc;
			}
		}
	}

	// after all our work is done, we may chose to:
	//   1) send the event further to the chain - this is expected behaviour by default
	//   2) consume the event, so no further event sees it; therefore, no Send() method call would be there

	// we chose to send the event further to the chain
	return Send(event);
};
