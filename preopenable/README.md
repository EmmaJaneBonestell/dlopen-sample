Unless you have a version of libltdl compiled without support for libdl (or your OS's equivalent), 
you will still be dependent on libdl. 

This can be compiled with a single line, and you may remove "-ldl" if you replace "-lltdl" 
with an appropriately compiled libltdl version.

gcc -static -Wl,-static -o main main.c bromine.c chlorine.c -lltdl -ldl

To confirm, just run strace ./main HOCl HOBr . Note how no calls to the filesystem (to find chlorine.so
and bromine.so) are made.
