# SmartCGMS - continuous glucose monitoring and controlling framework
# https://diabetes.zcu.cz/
#
# Copyright (c) since 2018 University of West Bohemia.
#
# Contact:
# diabetes@mail.kiv.zcu.cz
# Medical Informatics, Department of Computer Science and Engineering
# Faculty of Applied Sciences, University of West Bohemia
# Univerzitni 8, 301 00 Pilsen
# Czech Republic
# 
# 
# Purpose of this software:
# This software is intended to demonstrate work of the diabetes.zcu.cz research
# group to other scientists, to complement our published papers. It is strictly
# prohibited to use this software for diagnosis or treatment of any medical condition,
# without obtaining all required approvals from respective regulatory bodies.
#
# Especially, a diabetic patient is warned that unauthorized use of this software
# may result into severe injure, including death.
#
#
# Licensing terms:
# Unless required by applicable law or agreed to in writing, software
# distributed under these license terms is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#
# a) This file is available under the Apache License, Version 2.0.
# b) When publishing any derivative work or results obtained using this software, you agree to cite the following paper:
#    Tomas Koutny and Martin Ubl, "SmartCGMS as a Testbed for a Blood-Glucose Level Prediction and/or 
#    Control Challenge with (an FDA-Accepted) Diabetic Patient Simulation", Procedia Computer Science,  
#    Volume 177, pp. 354-362, 2020

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT("SmartCGMS-example_filter")

SET(PROJ "example_filter")
SET(CMAKE_CXX_STANDARD 17)
SET(SMARTCGMS_COMMON_DIR "" CACHE PATH "SmartCGMS 'common' directory location")

FILE(GLOB SRC_FILES "src/*.cpp" "src/*.hpp" "src/*.c" "src/*.h")
IF(WIN32)
	FILE(GLOB SRC_WIN_FILES "src/win/*.cpp" "src/win/*.hpp" "src/win/*.h" "src/win/*.def")
	SET(SRC_FILES "${SRC_FILES};${SRC_WIN_FILES}")
ENDIF()

INCLUDE_DIRECTORIES("${SMARTCGMS_COMMON_DIR}")

SET(COMMON_FILES )
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/iface/DeviceIface.cpp")

LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/rtl/referencedImpl.cpp")
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/rtl/scgmsLib.cpp")
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/rtl/Dynamic_Library.cpp")
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/rtl/DeviceLib.cpp")
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/rtl/FilterLib.cpp")
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/rtl/FilesystemLib.cpp")
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/rtl/UILib.cpp")
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/rtl/hresult.c")

LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/lang/dstrings.cpp")

LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/utils/string_utils.cpp")
LIST(APPEND COMMON_FILES "${SMARTCGMS_COMMON_DIR}/scgms/utils/winapi_mapping.c")

SOURCE_GROUP("common" FILES ${COMMON_FILES})
SOURCE_GROUP("src" FILES ${SRC_FILES})

ADD_LIBRARY(${PROJ} SHARED ${SRC_FILES};${COMMON_FILES})
