This package contain analysis codes for the high rate $\mu RWell$ detector prototypes

* The package allows you to analyze pedestal runs, and plots pedestals
* With existing codes you also can analyze the production runs, and you also can write your own analysis codes
* It produces nice plots as well.

## Decoding
Before going into details of the installation, please make sure you have the correct version of the coatjava and the sqlite file.

Please use the following version of the coatjava
`git@github.com:rafopar/coatjava.git`

For the sqlite file you need to use the file which is located in work disk
`/work/clas12/rafopar/uRWELL/Readout/TT_Tests/LDRD.sqlite`

Then before the decoding you need to set the `CCDB_CONNECTION` to point to that sqlite file, for example on bash you can do
`export CCDB_CONNECTION=sqlite:////work/clas12/rafopar/uRWELL/Readout/TT_Tests/LDRD.sqlite`

## Installation
The 1st step is to clone the repository to your place by running the command

`git clone git@github.com:rafopar/uRwell_LDRD.git`

The hipo directory uses LZ4 libraries, and for your own installation you might need to 
edit the file `cmake_modules/FindLZ4.cmake` and in particular to replace location of LZ4 include file and the library.

`find_path(LZ4_INCLUDE_DIR NAMES lz4.h HINTS group/clas12/packages/lz4/lib)`  
`find_library(LZ4_LIBRARY NAMES lz4 HINTS /group/clas12/packages/lz4/lib)`

