
#include <fstream>

#include <sys/types.h>
#include <unistd.h>

// from: http://media.unpythonic.net/emergent-files/01108826729/popen2.c
// Note: only works on POSIX
struct popen2 {
  pid_t child_pid;
  int from_child, to_child;
};

int popen2(const char* cmdline, struct popen2* childinfo);

// ## #WARNING, ALL THE CODE SHOULD NOW BE LICENSED AS GPL3

/**
 *  @brief Provides a layer of compatibility for C/POSIX.
 *  @ingroup io
 *
 *  This GNU extension provides extensions for working with standard C
 *  FILE*'s and POSIX file descriptors.  It must be instantiated by the
 *  user with the type of character used in the file stream, e.g.,
 *  file_buffer<char>.
*/
template <typename _CharT, typename _Traits = std::char_traits<_CharT> >
class file_buffer : public std::basic_filebuf<_CharT, _Traits> {
 public:
  // Types:
  typedef _CharT char_type;
  typedef _Traits traits_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;
  typedef std::size_t size_t;

 public:
  /**
   * deferred initialization
  */
  file_buffer() : std::basic_filebuf<_CharT, _Traits>() {}

  /**
   *  @param  __fd  An open file descriptor.
   *  @param  __mode  Same meaning as in a standard filebuf.
   *  @param  __size Optimal or preferred size of internal buffer,
   *                 in chars.
   *
   *  This constructor associates a file stream buffer with an open
   *  POSIX file descriptor. The file descriptor will be automatically
   *  closed when the file_buffer is closed/destroyed.
  */
  file_buffer(int __fd, std::ios_base::openmode __mode,
              size_t __size = static_cast<size_t>(BUFSIZ));

  /**
   *  @param  __f  An open @c FILE*.
   *  @param  __mode  Same meaning as in a standard filebuf.
   *  @param  __size Optimal or preferred size of internal buffer,
   *                 in chars.  Defaults to system's @c BUFSIZ.
   *
   *  This constructor associates a file stream buffer with an open
   *  C @c FILE*.  The @c FILE* will not be automatically closed when the
   *  file_buffer is closed/destroyed.
  */
  file_buffer(std::__c_file* __f, std::ios_base::openmode __mode,
              size_t __size = static_cast<size_t>(BUFSIZ));

  /**
   *  Closes the external data stream if the file descriptor constructor
   *  was used.
  */
  virtual ~file_buffer();

  /**
   *  @return  The underlying file descriptor.
   *
   *  Once associated with an external data stream, this function can be
   *  used to access the underlying POSIX file descriptor.  Note that
   *  there is no way for the library to track what you do with the
   *  descriptor, so be careful.
  */
  int fd() { return this->_M_file.fd(); }

  /**
   *  @return  The underlying FILE*.
   *
   *  This function can be used to access the underlying "C" file pointer.
   *  Note that there is no way for the library to track what you do
   *  with the file, so be careful.
   */
  std::__c_file* file() { return this->_M_file.file(); }
};

template <typename _CharT, typename _Traits>
file_buffer<_CharT, _Traits>::~file_buffer() {}

template <typename _CharT, typename _Traits>
file_buffer<_CharT, _Traits>::file_buffer(int __fd,
                                          std::ios_base::openmode __mode,
                                          size_t __size) {
  this->_M_file.sys_open(__fd, __mode);
  if (this->is_open()) {
    this->_M_mode = __mode;
    this->_M_buf_size = __size;
    this->_M_allocate_internal_buffer();
    this->_M_reading = false;
    this->_M_writing = false;
    this->_M_set_buffer(-1);
  }
}

template <typename _CharT, typename _Traits>
file_buffer<_CharT, _Traits>::file_buffer(std::__c_file* __f,
                                          std::ios_base::openmode __mode,
                                          size_t __size) {
  this->_M_file.sys_open(__f, __mode);
  if (this->is_open()) {
    this->_M_mode = __mode;
    this->_M_buf_size = __size;
    this->_M_allocate_internal_buffer();
    this->_M_reading = false;
    this->_M_writing = false;
    this->_M_set_buffer(-1);
  }
}