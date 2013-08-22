TODO
----
The basic functionality of this library that was met with the original
implementation: read in a mangle mask and efficiently search if a set of 
points exist within that mask. 

However, a number of useful extensions could be easily added. The list 
is broken into two parts, library functions and utility codes.

### Library:
 * write mangle polygons
 * easy ways to tag polygons
 * methods to link additional information to each polygon
 * function to check uniqueness of polyids
 * code optimization (profile and then optimize functions)
 * tweaks for library and/or bindings use (e.g. for ruby or python)

### Utilities (examples):
 * `mply_trim` like tool but to utilize multiple masks (or veto masks)
 * tool to check if input data list fills all polygons
