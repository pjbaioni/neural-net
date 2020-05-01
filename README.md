Joint project for the university courses Advanced Programming for
Scientific Computing (APSC) and Numerical Analysis for Partial
Differential Equations (NAPDE), Politecnico di Milano, a.y. 2018-2019

Author: Paolo Joseph Baioni

Mailto: paolojoseph.baioni@mail.polimi.it

The repo contains 4 directories:
 - data, containing input data, output data, and gnuplot scripts to
   generate graph from them
 - doc, containing documentation of the code and of the adopted
   mathematical methodology
 - include, containing header files which have to been included,
   among which Optimizers.hpp, that holds the full implementation
   of the optimizers, since they are class templates
 - src, containing the main program source files and the subdirectory
   write_set, where the subprogram used to generate datasets is kept

Code has been developed in Debian 9.9 using the modules of the APSC course, 
academic year 18-19, equipped with gcc 7.1, the Eigen template library
version 3.3.3, see https://eigen.tuxfamily.org, and the Boost libraries
version 1.63.0, see https://www.boost.org/. For the graphical output the 
gnuplot software is needed too; during the developement the version 5.0 
has been used.

When using APSC modules, to compile and test the program it's enough to:
 - load eigen and boost modules
 - type make
 - type for example make test3
while in the root folder.
To see more available commands, it's possible to type make help.

If modules aren't used, it is needed: 
  - a 2014 (or more) c++ standard compliant compiler
  - the Eigen template library
  - the Boost libraries required by gnuplot-iostream, 
    see ./include/gnuplot-iostream.hpp
Moreover in this case in the children Makefiles, ./src/Makefile and 
./src/write_set.cpp/Makefile, the preprocessor and the loader options, 
-I$$mkEigenInc, -I$$mkBoostInc and -L$$mkBoostLib, have to be modified so
to point to the correct path, e.g. -I/path/to/correct/location

Documentation has been compiled using TeX 3.14159265 (TeX Live 2016/Debian),
both the .tex file (plus the included images) and the .pdf file are provided.
In appendix it contains a short FreeFem++ code too, see https://freefem.org/, 
that can be copy-pasted to a .edp file and then run with:
 $ FreeFem++ filename.edp
The script has been written and tested with FreeFem++ version 3.47
