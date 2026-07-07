#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cassert>
#include <sys/mman.h>

using namespace std;

struct MemoryMappedFile {
  MemoryMappedFile& operator=(const MemoryMappedFile& other)=delete;
  MemoryMappedFile(const MemoryMappedFile& other)=delete;

  int _fd=-1; // file descriptor
  void* _mem=0; // memory area
  size_t _size=0;
  size_t _offset=0;
  
  MemoryMappedFile(const std::string& filename, size_t offset=0):
    _offset(offset)
  {
    _fd=open(filename.c_str(), O_RDWR|O_CREAT, 0660);
    assert(_fd>0 && "file create fail");
    cerr << "create ok" << endl;

    struct stat statbuf;
    if (fstat(_fd, &statbuf)<0) {
      assert(0 && "file size fail");
    }
    
    if (offset>statbuf.st_size)
      assert(0 && "wrong offset");
    cerr << "size:" << statbuf.st_size << endl;
    
    _size=statbuf.st_size-_offset;
    _mem=0;
    if (_size) {
      _mem = mmap(nullptr, _size, PROT_READ|PROT_WRITE, MAP_SHARED, _fd, _offset);
    }
    cerr << "mmapped_size:" << _size << endl;
    //assert(_mem && "mmap on create fail");
  }
  
  void resize(size_t new_size) {
    if (new_size==_size)
      return;
    
    if (_mem) {
      munmap(_mem, _size);
      _mem=0;
      _size=0;
    }
    
    if (_fd) {
      int retval=ftruncate(_fd, new_size);
      cerr << "retval: "<< retval << endl;
      assert(retval>=0 && "error on truncate");
    }
    _size=new_size;
    _mem = mmap(nullptr, _size, PROT_READ|PROT_WRITE, MAP_SHARED, _fd, _offset);
  }
};


int main(int argc, char** argv) {
  MemoryMappedFile mfile(argv[1]);

  cerr << "argc: " << argc << endl;
  if (argc<3){
    int size=8192;
    mfile.resize(size);
    unsigned char *c=(unsigned char*) mfile._mem;
    cerr << "scrivo" << endl;
    for(int i=0; i<size; ++i) {
      c[i]=i;
    }
  } else {
    cerr << "lego" << endl;
    unsigned  char* c = (unsigned char*) mfile._mem;
    for(int i=0; i<mfile._size; ++i) {
      printf("%d ", (int) c[i]);
    }
  }
}

