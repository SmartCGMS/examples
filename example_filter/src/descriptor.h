#pragma once

#include <rtl/guid.h>

namespace example_filter {

	constexpr GUID id = { 0x893fdedc, 0xc5ec, 0x45cc, { 0xba, 0x9, 0xc3, 0x1e, 0x14, 0xf6, 0xdb, 0x42 } }; // {893FDEDC-C5EC-45CC-BA09-C31E14F6DB42}

	extern const wchar_t* rsExample_Config_String;
	extern const wchar_t* rsExample_Config_Double;
}
