##############################################################################
# Use gSOAP to generate onvif required source and header files

# Set ONVIF_GENERATION_DIR to the folder where the generated files will be put in
# Set ONVIF_GENERATED_OUTPUT to a list of needed sources for compilation
# Set ONVIF_WSDLS to point to the required onvif WSDL files
# Tell cmake that all the files ONVIF_GENERATION_DIR are generated files so it doesn't complain during the cmake build.

# use WSDL2H with the type map from configuration/typemap.dat and set WSDL files to generate a onvif.h
# use SOAPCPP2 to generate c++ proxy classes and stubs from the previously generated onvif.h

# Create a custom target "ONVIF_GENERATION_TARGET" to allow the generation to be a dependency step
#
##############################################################################
# Courtesy of Bob Thomas <bob.thomas295@gmail.com>
##############################################################################

SET (ONVIF_GENERATION_DIR ${CMAKE_BINARY_DIR}/gen)
SET (ONVIF_GENERATED_OUTPUT 
    ${ONVIF_GENERATION_DIR}/soapDeviceBindingProxy.cpp
	${ONVIF_GENERATION_DIR}/soapPTZBindingProxy.cpp
	${ONVIF_GENERATION_DIR}/soapPullPointSubscriptionBindingProxy.cpp
	${ONVIF_GENERATION_DIR}/soapRemoteDiscoveryBindingProxy.cpp
    ${ONVIF_GENERATION_DIR}/soapMediaBindingProxy.cpp
    ${ONVIF_GENERATION_DIR}/soapC.cpp
)

SET (ONVIF_WSDLS 
http://www.onvif.org/onvif/ver10/device/wsdl/devicemgmt.wsdl
http://www.onvif.org/onvif/ver10/events/wsdl/event.wsdl
http://www.onvif.org/onvif/ver10/display.wsdl
http://www.onvif.org/onvif/ver10/deviceio.wsdl
http://www.onvif.org/onvif/ver20/imaging/wsdl/imaging.wsdl
http://www.onvif.org/onvif/ver10/media/wsdl/media.wsdl
http://www.onvif.org/onvif/ver20/ptz/wsdl/ptz.wsdl
http://www.onvif.org/onvif/ver10/receiver.wsdl
http://www.onvif.org/onvif/ver10/recording.wsdl
http://www.onvif.org/onvif/ver10/search.wsdl
http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl
http://www.onvif.org/onvif/ver10/replay.wsdl 

)

set_source_files_properties(${ONVIF_GENERATION_DIR}/soapC.cpp PROPERTIES GENERATED TRUE)

add_custom_command(
	OUTPUT ${ONVIF_GENERATION_DIR}/onvif.h
	COMMAND ${GSOAP_WSDL2H} -t ${CMAKE_SOURCE_DIR}/configuration/typemap.dat -O4 -P -x -o onvif.h ${ONVIF_WSDLS}
	WORKING_DIRECTORY ${ONVIF_GENERATION_DIR}
	DEPENDS gSOAP
	COMMENT "Generating onvif.h from WSDL"
)
add_custom_command(
	OUTPUT ${ONVIF_GENERATED_OUTPUT}
	COMMAND ${GSOAP_SOAPCPP2} -2 -C -a -d ${ONVIF_GENERATION_DIR} -I ${GSOAP_ROOT}/gsoap/include ${GSOAP_ROOT}/gsoap/custom ${GSOAP_ROOT}/gsoap/import -j -x ${ONVIF_GENERATION_DIR}/onvif.h
	DEPENDS ${ONVIF_GENERATION_DIR}/onvif.h
	COMMENT "Generating proxy classes from onvif.h"
)

add_custom_target(
	ONVIF_GENERATION_TARGET
	DEPENDS ${ONVIF_GENERATED_OUTPUT}
)
