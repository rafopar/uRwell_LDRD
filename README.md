This package contain analysis codes for the high rate $\mu RWell$ detector prototypes

* The package allows you to analyze pedestal runs, and plots pedestals
* With existing codes you also can analyze the production runs, and you also can write your own analysis codes
* It produces nice plots as well.

## Decoding
Before going into details of the installation, please make sure you have the correct version of the coatjava and the sqlite file.

Please use the a specific branch of coatjava

`git clone git@github.com:rafopar/coatjava.git`
`git checkout 2-implement-vmm3-decoding-the-version-of-jastrzembski`

To compile run
`./build-coatjava.sh`


For the sqlite file you need to use the file which is located in work disk
`/work/clas12/rafopar/uRWELL/Readout/TT_Tests/LDRD.sqlite`

Then before the decoding you need to set the `CCDB_CONNECTION` to point to that sqlite file, for example on bash you can do
`export CCDB_CONNECTION=sqlite:////work/clas12/rafopar/uRWELL/Readout/TT_Tests/LDRD.sqlite`

To decode evio files, you can use the command:
`decoder -i /path/to/EVIO_FILE/urwell_FILE.evio -o decoded_File.hipo -c 1`

## Installation
The 1st step is to clone the repository to your place by running the command

`git clone git@github.com:rafopar/uRwell_LDRD.git`

The hipo package uses LZ4 libraries, and for your own installation you might need to 
edit the file `cmake_modules/FindLZ4.cmake` and in particular to replace location of LZ4 include file and the library.

`find_path(LZ4_INCLUDE_DIR NAMES lz4.h HINTS /group/clas12/packages/lz4/lib)`  
`find_library(LZ4_LIBRARY NAMES lz4 HINTS /group/clas12/packages/lz4/lib)`


To install the software 

`cmake -S . -Bbuild - DCMAKE_INSTALL_PREFIX=/Path/where/You/Want/Files/ToBe/Installed/`
`cmake --build build`
`cmake --install build`

When your installation is finished, the directory following directory structures will be created in the installation directory

`├── bin`  
`│   ├── AnaData.exe`  
`│   ├── CalculatePedestals.exe`  
`│   ├── DrawBasicPlots.exe`  
`│   ├── Figs`  
`│   ├── PedFiles`  
`│   └── SkimZeroSuppression.exe`  
`├── lib`  
`│   ├── libHipo4.a`  
`│   ├── liburwellLDRDlib.so -> liburwellLDRDlib.so.1`  
`│   ├── liburwellLDRDlib.so.1 -> liburwellLDRDlib.so.1.0.1`  
`│   └── liburwellLDRDlib.so.1.0.1`  
`└── share`  
   `└── pkgconfig`  
       `└── urwellLDRDLib.pc`

The `lib` directory contains libraries, in the `bin` you will find executables and two subdirectories, `Figs` and `PedFiles` where you should expect figures and Pedestal files to be added.