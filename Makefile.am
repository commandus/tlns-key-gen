SUBDIRS = .
ACLOCAL_AMFLAGS = -I m4

VERSION = "0.1"
VERSION_INFO = 0:1:0

common_flags = -I. -Ithird_party

TEST_FILES = 

AM_CPPFLAGS=$(common_flags)
AM_CXXFLAGS=$(common_flags) -std=c++11
COMMON_CPP_FLAGS = -D_GLIBCXX_USE_CXX11_ABI=0

CLEANFILES = $(GEN) $(TEST_FILES)

#
# Binaries
#
bin_PROGRAMS = tlns-key-gen

SRC_AES = third_party/system/crypto/aes.c third_party/system/crypto/cmac.c
SRC_ARGTABLE = third_party/argtable3/argtable3.c
SRC_TLNS_KEY_GEN = tlns-key-gen.cpp key128gen.cpp lorawan-types.cpp lorawan-conv.cpp lorawan-string.cpp

nobase_dist_include_HEADERS = \
	third_party/system/crypto/aes.h third_party/system/crypto/cmac.h \
	third_party/argtable3/argtable3.h \
	key128gen.h err-list.h \
	lorawan-types.h lorawan-const.h lorawan-string.h lorawan-error.h

tlns_key_gen_SOURCES = \
	$(SRC_AES) $(SRC_ARGTABLE) $(SRC_TLNS_KEY_GEN)
tlns_key_gen_LDADD = $(commonlibs)
tlns_key_gen_CPPFLAGS = -DTLNS_VERSION=$(VERSION)
# tlns_key_gen_LDFLAGS = -version-info $(VERSION_INFO)

#
# Configs, readme, CMake etc.
#
configdir = $(datadir)
dist_config_DATA = \
	autogen.sh CMakeLists.txt CODE_OF_CONDUCT.md CONTRIBUTING.md COPYING HISTORY LICENSE README.md TODO \
	third_party/argtable3/README
	tests/test-key.cpp
  
#
# Tests
#
test_key_SOURCES = \
	tests/test-key.cpp

check_PROGRAMS =
if ENABLE_TESTS
	check_PROGRAMS += test-key
endif

TESTS = $(check_PROGRAMS)
