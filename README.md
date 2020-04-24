Project for the university courses Advanced Programming for
Scientific Computing (APSC) and Numerical Analysis for Partial
Differential Equations (NAPDE), Politecnico di Milano, a.y. 2018-2019

Author: Paolo Joseph Baioni

Mailto: paolojoseph.baioni@mail.polimi.it

The repo contains 4 directories:
 - data, containing input data, output data, and gnuplot scripts
   to generate graph from them
 - doc, containing documentation of the code and of the
   adopted mathematical methodology
 - include, containing header files which have to been included,
   among which Optimizers.hpp, that holds the full implementation
   of the optimizers, since they are class templates
 - src, containing the main program, the sources and the subdir
   write_set, where the subprogram used to generate datasets is kept

When using APSC modules, to compile the program it's enough to:
 - load eigen and boost modules
 - type make
while in the root folder.
To see more available commands, it's possible to type make help.
 
Code has been developed mainly in Debian 9.9 using the modules of the APSC 
course, academic year 18-19. Tests performed on a Xubuntu 19.10 virtual machine
with the same modules installed have shown identical results, while tests done
on a Lubuntu 18.04 LTS virtual machine running the a.y. 19-20 modules have
shown some numeric differences, which however didn't affect the execution of 
the program.
In every case gnuplot is required (tested versions: 5.0, 5.2), but numerical 
output is produced anyway, on the terminal and on the output data file.

If modules aren't used, it is needed: 
  - a 2014 (or more) c++ standard compliant compiler (only gcc tested, 
    versions: 7.1, 9.2)
  - the Eigen template library (tested versions: 3.3.3, 3.3.7), see 
    https://eigen.tuxfamily.org
  - the Boost libraries required by gnuplot-iostream (tested version: 1.63.0, 
    1.72.0), see ./include/gnuplot-iostream.hpp and https://www.boost.org/
Moreover in this case in the children Makefiles,src/Makefile and 
src/write_set.cpp/Makefile, the preprocessor and the loader options, 
-I$$mkEigenInc, -I$$mkBoostInc and -L$$mkBoostLib, have to be modified so to
point to the correct path, e.g. -I/path/to/correct/location

Documentation has been compiled using TeX 3.14159265 (TeX Live 2016/Debian),
in appendix it contains a short FreeFem++ code too, see https://freefem.org/, 
that can be copy-pasted to a .edp file and then run with:
 $ FreeFem++ filename.edp
This has been tested only on Debian 9.9 with FreeFem++ v  3.47 (date 2016-06-16)

