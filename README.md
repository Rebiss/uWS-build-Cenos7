# glibc-2.17 Problem Centos7.

Install clang 5+ (LLVM)
`
    $ sudo yum install centos-release-scl llvm-toolset-7
`
Recursively clone the repo
`
    $ git clone --recursive https://github.com/uNetworking/uWebSockets.js.git --depth=1
`
Replace build.c and Makefile with the patched ones
Build it
`
    $ make
`

### Centos7 (Alternative update glibc-2.17->2.18 )
$ curl -O http://ftp.gnu.org/gnu/glibc/glibc-2.18.tar.gz
$ tar zxf glibc-2.18.tar.gz
$ cd glibc-2.18/
$ mkdir build
$ cd build/
$ ../configure --prefix=/usr
$ make -j2
$ make install
