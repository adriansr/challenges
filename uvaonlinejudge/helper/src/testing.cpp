// ------------- helper.cpp begin ---------------------
// accepted flags:
// #define HELPER_WITH_FILE // provice InputFile class
//
#include <cstdio>
#include <cinttypes>
#include <cerrno>
#include <unistd.h>

#if defined(HELPER_WITH_FILE)
#	include <fcntl.h>
#endif

namespace helper {
	
	class Exception {
		public:
			Exception(const std::string& msg)
				: m_msg(msg)
			{
			}

			const char *what() const {
				return m_msg.c_str();
			}

		private:
			std::string m_msg;
	};

#if defined(HELPER_WITH_FILE)
	// mostly for testing
	class InputFile {
		public:
			InputFile(const char *path)
			{
				m_fd = open(path,O_RDONLY);
				if (m_fd == -1)
				{
					throw Exception("Unable to open file");
				}
			}

			~InputFile()
			{
				if (m_fd!=-1) {
					// not checking for EINTR because I want the fastest best-effort
					close(m_fd);
					m_fd = -1;
				}
			}

			int fd() const
			{
				return m_fd;
			}

		private:
			int m_fd;
	};
#endif

	template <typename DefaultIntType = uint32_t>
	class IOHelper {
		public:
			IOHelper(int fd_in = 0, // stdin
					 int fd_out = 1, // stdout
					 size_t bufferSize = 4096
					 )
				: m_fd_in(fd_in)
				, m_fd_out(fd_out)
				, m_buffer_used(0)
			{
				m_buffer.reserve( bufferSize );
			}
			
			std::string read_line()
			{
				size_t pos;
				while ( (pos=m_buffer.find('\n',m_buffer_used)) == m_buffer.npos && read_more() ) 
					 ;
				auto base = m_buffer_used;
				//std::cerr << "base: " << base << " pos " << pos << std::endl;
				if (pos != m_buffer.npos) {
					m_buffer_used = pos + 1;
					return m_buffer.substr(base,pos-base);
				}
				m_buffer_used = pos;
				return m_buffer.substr(base);
			}

		private:
			bool read_more()
			{
				m_buffer.erase(0,m_buffer_used);
				m_buffer_used = 0;
				
				size_t capacity = m_buffer.capacity(),
						   used = m_buffer.size(),
						   want = capacity - used;
				if (want < (capacity>>1) )
				{
					capacity <<= 1;
					want += capacity;
				}
				m_buffer.resize(capacity);
				ssize_t nread;
				while ( (nread=read( m_fd_in, &m_buffer[used], want)) == -1 && errno==EINTR)
					;
				//std::cerr << "read_more " << (nread>0) << std::endl;
				if (nread>0)
				{
					m_buffer.resize(used + nread);
					return true;
				}
				m_buffer.resize(used);
				return false;
			}

			int m_fd_in,
				m_fd_out;
			size_t m_buffer_used;
			std::string m_buffer;
	};

}

// ------------- helper.cpp end ------------------------

