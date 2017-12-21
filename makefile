all:biu

example:db/bitcask.cc db/bitcask.h example.cc
				g++ -std=c++11 db/bitcask.cc example.cc -o example

biu:db/bitcask.cc db/bitcask.h
				g++ -std=c++11 -fPIC -shared -o biu.so db/bitcask.cc

clean:
				rm -rf *.o biu
