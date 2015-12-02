bin_PROGRAMS=helper_test
helper_test_SOURCES=helper_test.cpp
helper_test_CXXFLAGS=-Wall -std=c++11 -Os
helper_test_CPPFLAGS=$(CPPFLAGS) $(BOOST_CPPFLAGS) -I../src
helper_test_LDFLAGS=$(BOOST_UNIT_TEST_FRAMEWORK_LDFLAGS)
helper_test_LDADD=$(BOOST_UNIT_TEST_FRAMEWORK_LIBS)
