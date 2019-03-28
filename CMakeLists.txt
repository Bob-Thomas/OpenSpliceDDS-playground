cmake_minimum_required (VERSION 3.11)
project (dds-hello-world)
include(FetchContent)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")



include (MacroOpenSplice)
include (osmacros)
include (gitModules)

find_package (OpenSplice REQUIRED)

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${OpenSplice_INCLUDE_DIRS}
)

set (sourceFolder ${CMAKE_SOURCE_DIR}/src)
set (idlFolder ${sourceFolder}/models)
set (idls JoystickData.idl)

# Find all IDL files in src/models and generate the cpp structures for it
foreach(idl ${idls})
	OpenSplice_IDLGEN (${idlFolder}/${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/\\1.cpp" VARS_1 ${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/\\1.h" VARS_2 ${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/\\1Dcps.cpp" VARS_3 ${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/\\1Dcps.h" VARS_4 ${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/\\1Dcps_impl.cpp" VARS_5 ${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/\\1Dcps_impl.h" VARS_6 ${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/\\1SplDcps.cpp" VARS_7 ${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/\\1SplDcps.h" VARS_8 ${idl})
	string (REGEX REPLACE "\(.*\).idl" "./gen/ccpp_\\1.h" VARS_9 ${idl})
	set(OpenSplice_DATAMODEL ${OpenSplice_DATAMODEL} ${VARS_1} ${VARS_2} ${VARS_3} ${VARS_4} ${VARS_5} ${VARS_6} ${VARS_7} ${VARS_8} ${VARS_9})
endforeach(idl)

set (PUB_SOURCES
 ${joystick_SOURCE_DIR}/joystick.cc
 ${sourceFolder}/publisher.cpp
)
set (SUB_SOURCES
 ${sourceFolder}/subscriber.cpp
)

set (PUB_EXE publisher)
set (SUB_EXE subscriber)
set (DATAMODEL datamodel)

add_executable (${PUB_EXE} ${PUB_SOURCES})
add_executable (${SUB_EXE} ${SUB_SOURCES})

set_property(TARGET ${PUB_EXE} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${SUB_EXE} PROPERTY CXX_STANDARD 17)

add_library (${DATAMODEL} SHARED ${OpenSplice_DATAMODEL})

add_definitions (
 ${OpenSplice_DEFINITIONS}
 ${DEFINITIONS}
)

target_link_libraries (
  ${DATAMODEL} 
  ${OpenSplice_LIBRARIES}
  stdc++fs
)

target_link_libraries (${PUB_EXE}
 ${OpenSplice_LIBRARIES}
 ${DATAMODEL} 
 ${CPR_LIBRARIES}

)

target_link_libraries (${SUB_EXE}
 ${OpenSplice_LIBRARIES}
 ${DATAMODEL}
 ${CPR_LIBRARIES}
)