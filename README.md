Project for the university course Advanced Programming for
Scientific Computing, Politecnico di Milano

Author: Paolo Joseph Baioni

Mailto: paolojoseph.baioni@mail.polimi.it

Development is still ongoing, at the moment of writing:

The repo contains 3 directories:
 - data, containing input data, output data, plots and
   gnuplot script to generate them
 - include, containing header files which have to been included,
   among which Optimizers.hpp, that holds the full implementation
   of the optimizers, since they are class templates
 - src, containing the main program, the sources and the subdir
   write_set, where the subprogram used to generate datasets is kept
   
To compile main program:
 - load eigen and boost modules
 - go to src/ and run make

To compile write_set.cpp:
 - load eigen
 - go to src/write_set
 - type g++ -std=c++14 -I$mkEigenInc -I./../../include/ write_set.cpp
 
Code has been developed mainly under Debian 9.9 using the modules of the
PACS course, academic year 18-19; some test under a Lubuntu 18.04 LTS
virtual machine running the a.y. 19-20 modules have shown some differences
that are being investigated.
In both cases gnuplot is required (version 5.0), but numerical output
is produced anyway, on the terminal and on the output file.