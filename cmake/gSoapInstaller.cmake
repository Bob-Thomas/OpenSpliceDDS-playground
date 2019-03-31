##############################################################################
# Donwload and build gsoap 2.8.82

# Once finished building gsoap rename.
#   smdevp.c to smdevp.cpp
#   mecevp.c to mecevp.cpp
#   wsaapi.c to wsaapi.cpp 

# To allow c++ compiliation in source.

# Set GSOAP_ROOT as the root folder where gsoap got downloaded
# Set GSOAP_WSDL2H to point to the wsd2lh executeable
# Set GSOAP_SOAPCPP2 to point to the soapcpp2 executable
# Set GSOAP_INCLUDE_DIRS to the needed include folders for our gsoap compiliation
# Set GSOAP_PLUGIN_SOURCES to the c and cpp files needed for succesfull compilation

# Tell cmake that GSOAP_PLUGIN_SOURCES en stdsoap2.cpp are generated files so it doesn't complain during the cmake build.
#
##############################################################################
# Courtesy of Bob Thomas <bob.thomas295@gmail.com>
##############################################################################

include(ExternalProject)

SET (GSOAP_URL https://downloads.sourceforge.net/project/gsoap2/gsoap-2.8/gsoap_2.8.82.zip)
SET (GSOAP_URL_HASH MD5=3c050f02d7628811b909377b2ecc1fbe)
SET (GSOAP_ROOT ${CMAKE_BINARY_DIR}/_deps/gsoap/src/gSOAP)
SET (GSOAP_WSDL2H ${GSOAP_ROOT}/gsoap/bin/wsdl2h)
SET (GSOAP_SOAPCPP2 ${GSOAP_ROOT}/gsoap/bin/soapcpp2)

SET (GSOAP_INCLUDE_DIRS 
    ${GSOAP_ROOT}/gsoap/plugin
    ${GSOAP_ROOT}/gsoap/include
    ${GSOAP_ROOT}/gsoap/
)

SET (GSOAP_PLUGIN_SOURCES
    ${GSOAP_ROOT}/gsoap/plugin/smdevp.cpp
    ${GSOAP_ROOT}/gsoap/plugin/mecevp.cpp
    ${GSOAP_ROOT}/gsoap/plugin/wsaapi.cpp
    ${GSOAP_ROOT}/gsoap/plugin/wsseapi.cpp
    ${GSOAP_ROOT}/gsoap/plugin/threads.c
    ${GSOAP_ROOT}/gsoap/custom/duration.c
    ${GSOAP_ROOT}/gsoap/dom.cpp
    ${GSOAP_ROOT}/gsoap/stdsoap2.cpp
)

set_source_files_properties(${GSOAP_PLUGIN_SOURCES} PROPERTIES GENERATED TRUE)
set_source_files_properties(${GSOAP_ROOT}/gsoap/stdsoap2.cpp PROPERTIES GENERATED TRUE)

ExternalProject_Add(
  gSOAP 
  PREFIX ${CMAKE_BINARY_DIR}/_deps/gsoap
  URL ${GSOAP_URL}
  URL_HASH ${GSOAP_URL_HASH}
  CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/_deps/gsoap/src/gSOAP/configure --prefix=${GSOAP_ROOT}/gsoap
  INSTALL_COMMAND make install
  # Change .c files to cpp to make sure they compile as c++
  COMMAND cp ${GSOAP_ROOT}/gsoap/plugin/smdevp.c ${GSOAP_ROOT}/gsoap/plugin/smdevp.cpp
  COMMAND cp ${GSOAP_ROOT}/gsoap/plugin/mecevp.c ${GSOAP_ROOT}/gsoap/plugin/mecevp.cpp
  COMMAND cp ${GSOAP_ROOT}/gsoap/plugin/wsaapi.c ${GSOAP_ROOT}/gsoap/plugin/wsaapi.cpp
)
