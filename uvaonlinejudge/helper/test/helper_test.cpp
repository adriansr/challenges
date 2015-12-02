#define BOOST_TEST_MODULE helper_test
#include <boost/test/included/unit_test.hpp>

#define HELPER_WITH_FILE
#include "helper.cpp"

BOOST_AUTO_TEST_SUITE(helper)

BOOST_AUTO_TEST_CASE(read_line)
{
	for (int bitsize = 0; bitsize < 14; bitsize ++) {
		
		helper::InputFile in("readline.txt");
		const int fdout = 1;
		size_t size = 1 << bitsize;
		std::cerr << "Starting test with " << size << std::endl;
		helper::IOHelper<> io(in.fd(),fdout,size);
		std::string s;

		int nlines = 0;
		for (;!(s=io.read_line()).empty();nlines++) {
			std::cerr << "line '" << s << "' (" << s.size() << ")" << std::endl;
		}
		std::cerr << "Read " << nlines << " lines" << std::endl;
	}

	// my boost crashes so..
}

BOOST_AUTO_TEST_SUITE_END()

