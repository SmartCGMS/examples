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
