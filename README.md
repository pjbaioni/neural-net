Joint project for the university courses Advanced Programming for
Scientific Computing (APSC) and Numerical Analysis for Partial
Differential Equations (NAPDE), Politecnico di Milano, a.y. 2018-2019

Author: Paolo Joseph Baioni

Mailto: paolojoseph.baioni@mail.polimi.it

License: GNU GPL and FDL, see the COPYING file


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
academic year 18-19, equipped with gcc 7.1, see https://gcc.gnu.org/, 
GNU Make 4.2.1, see https://www.gnu.org/software/make/, the Eigen template 
library version 3.3.3, see https://eigen.tuxfamily.org, and the Boost libraries
version 1.63.0, see https://www.boost.org/, and the Git distributed version 
control system version 2.12.0, see https://git-scm.com/.
The gnuplot software is needed too, see http://www.gnuplot.info/; 
during the developement the version 5.0 has been used.


When using APSC modules, to compile and test the program it's enough to:
 - load eigen and boost modules
 - type make
 - type for example make test3

while in the root folder.

To see more available commands, it's possible to type make help.


If modules aren't used, it is needed: 
  - a 2014 (or more) c++ standard compliant compiler
  - a Make implementation
  - the Eigen template library
  - the Boost libraries required by gnuplot-iostream, 
    see ./include/gnuplot-iostream.hpp
  - the gnuplot software

Moreover in this case in the children Makefiles, ./src/Makefile and 
./src/write_set.cpp/Makefile, the preprocessor and the loader options, 
-I$$mkEigenInc, -I$$mkBoostInc and -L$$mkBoostLib, have to be modified so
to point to the correct path, e.g. -I/path/to/correct/location
(Of course, the user can build explicitely the program without using Make,
as written in the documentation file, moreover the non-graphical part of the
program works even without gnuplot and in particular numerical output is 
produced anyway. Git is not mentioned here, but is needed if one wants to
clone the repo or to navigate through the commits history. All the other 
requirements are instead strictly necessary.)


Documentation has been compiled using TeX 3.14159265 (TeX Live 2016/Debian,
see https://www.tug.org/texlive/), and both the .tex file (plus the included
images) and the compiled .pdf file are provided.
In appendix it contains a short FreeFem++ code too, see https://freefem.org/, 
that can be copy-pasted to a .edp file and then run with:

 $ FreeFem++ filename.edp
 
The script has been written and tested with FreeFem++ version 3.47
