#
# Generate LoRaWAN keys
#

cmake_minimum_required(VERSION 3.9)
set(PROJECT_NAME "tlns-key-gen")
project(${PROJECT_NAME} VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)

set(SRC_AES
	third_party/system/crypto/aes.c third_party/system/crypto/cmac.c
)

set(SRC_ARGTABLE
	third_party/argtable3/argtable3.c
)

set(SRC_TLNS_KEY_GEN
	tlns-key-gen.cpp key128gen.cpp lorawan-types.cpp lorawan-conv.cpp lorawan-string.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#	set (OS_SPECIFIC_LIBS wsock32 ws2_32 Userenv iphlpapi)
	set (OS_SPECIFIC_LIBS ws2_32)
endif()

include_directories("third_party" ".")

add_executable(tlns-key-gen ${SRC_TLNS_KEY_GEN} ${SRC_AES} ${SRC_ARGTABLE})
target_link_libraries(tlns-key-gen PRIVATE ${OS_SPECIFIC_LIBS})
# target_include_directories(tlns-key-gen PRIVATE )
# target_compile_definitions(tlns-key-gen PRIVATE )

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/tlns-get-keys DESTINATION cmake)
INSTALL(FILES README.md LICENSE HISTORY COPYING TODO CODE_OF_CONDUCT.md CONTRIBUTING.md DESTINATION doc)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
include(CPack)
