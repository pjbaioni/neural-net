Project for the university courses Advanced Programming for
Scientific Computing (APSC) and Numerical Analysis for Partial
Differential Equations (NAPDE), Politecnico di Milano, a.y. 2018-2019

Author: Paolo Joseph Baioni

Mailto: paolojoseph.baioni@mail.polimi.it

Development is still ongoing, at the moment of writing:

The repo contains 4 directories:
 - data, containing input data, output data, plots and
   gnuplot script to generate them
 - doc, containing documentation of the code and of the
   adopted mathematical methodology
 - include, containing header files which have to been included,
   among which Optimizers.hpp, that holds the full implementation
   of the optimizers, since they are class templates
 - src, containing the main program, the sources and the subdir
   write_set, where the subprogram used to generate datasets is kept

When using APSC modules, to compile main program:
 - load eigen and boost modules
 - go to src/ and run make

and to compile write_set.cpp:
 - load eigen
 - go to src/write_set
 - type g++ -std=c++14 -I$mkEigenInc -I./../../include/ write_set.cpp
 
Code has been developed mainly under Debian 9.9 using the modules of the
APSC course, academic year 18-19; some test under a Lubuntu 18.04 LTS
virtual machine running the a.y. 19-20 modules have shown some differences
that are being investigated.
In both cases gnuplot is required (tested versions: 5.0, 5.2), but numerical 
output is produced anyway, on the terminal and on the output data file.

If you don't use modules, you need a c++ compiler (only gcc tested, versions:
7.1, 9.2), the Eigen template library (tested versions: 3.3.3, 3.3.7), 
see https://eigen.tuxfamily.org, and the Boost libraries required by 
gnuplot-iostream (tested version: 1.63.0, 1.72.0), see 
./include/gnuplot-iostream.hpp and https://www.boost.org/
Moreover in this case the "include" preprocessor options (-I./path/to/dir)
have to be modified to point to the correct path, both in Makefile and
during the compilation of write_set.cpp.

Documentation has been compiled using TeX 3.14159265 (TeX Live 2016/Debian),
it contains a short FreeFem++ code too, see https://freefem.org/, that can
be copy-pasted to a .edp file, which can be run with:
 $ FreeFem++ filename.edp
This has been tested only on Debian 9.9 with FreeFem++ v  3.47 (date 2016-06-16)

