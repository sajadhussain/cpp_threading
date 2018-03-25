# cpp_threading

I'm playing around with C++ threads while also trying to use (ah um, abuse)
newer C++ features.  I will try to implement solutions to the synchronization problems
in the great book "The Little Book of Semaphores" by Allen B. Downey.

I've just started learning this stuff, so most likely there will be lots of bugs
and plain bad C++ usage. There will always be lots of room for improvement,
so please try to contribute if you find this useful or you think it may be useful
to others. This way we can help each other in the learning process.

## Structure
- include/ has the headers
- sem/ has a simple semaphore implementation
- problems/ has solutions to some synchronization problems
- The top level Makefile builds everything with a `make`

## TODO
- Build system and directory structure: I guess the build system and the directory
structure is a giant mess and it will get out of hand pretty fast. Find a way out
of this mess.
- Code could be documented.
