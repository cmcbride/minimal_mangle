MINIMAL MANGLE
==============
Simplified C-only code utilities to minimally use mangle polygons.

The intention is to work like the original MANGLE code:
http://space.mit.edu/~molly/mangle/

The main focus of these utilites:
 1. clean c-only code without globals.
 2. few to no dependencies.
 3. simple to use and utilize within larger codes.

There is no attempt to provide the complete functionality that MANGLE
does, nor have flexible input.  Please use the full MANGLE utilties if
you need to convert to an accepted polygon format that this code can
handle.

Prefix for functions is "mply_", short for mangle polygon.

NOTE: there is no attempt to separate "internal" and "external" functions
by name.


USAGE
-----
The easiest way to use these function in your code is simply to include
the source directly.

For example:

    #include <minimal_mangle.c>


Then make sure to include the directory where the source code lives in
the compilation (-I for gcc).

You can simply copy the respective files to your source code directory, and
then pass the current directory to the compiler.  For example:

    % gcc -I. ...

For some examples codes, and some functioning tools, please see the
examples/ subdirectory.


DEPENDENCIES
------------
These make use of the "check_utils", which are available
at:
    https://github.com/cmcbride/check_utils

And use the simple_reader functionality from:
    https://github.com/cmcbride/simple_lib

Copies of the necessary source files is included for simplicity.


SOME NOTES
----------
Some terminology, which is used internally and in function names:

 "ID" : polyid / pixel_id (the values listed in the polygon file)

 "INDEX" : the internal storage index (unique identifiers in code)


TODO
----
Documentation!
