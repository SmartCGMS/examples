#pragma once

#include <rtl/guid.h>

namespace example_model {

	const size_t param_count = 2;
	const double default_parameters[param_count] = { 1.0, 1.0 };

	struct TParameters {
		union {
			struct {
				double example_amplitude;
				double example_frequency;
			};
			double vector[param_count];
		};
	};

	constexpr GUID model_id = { 0xac716662, 0x3fb, 0x497f, { 0x88, 0x54, 0xde, 0x7a, 0xf1, 0x46, 0x72, 0x5e } }; // {AC716662-03FB-497F-8854-DE7AF146725E}
	constexpr GUID signal_id = { 0x315feca4, 0x720b, 0x4881, { 0x92, 0x11, 0xae, 0x75, 0x13, 0x60, 0x7b, 0x46 } }; // {315FECA4-720B-4881-9211-AE7513607B46}
}
