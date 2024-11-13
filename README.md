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


## Analyzing Pedestal runs

From time to time there is a need to take pedestal runs and analyze data, which includes:
* Calculating pedestals for all channels
* Writing pedestals to an appropriate files
* Making plots, e.g.:
	* mean and $\sigma$ of the RMS as a function of channel for each Hybrid
	* 2D RMS vs channel

To do this, assuming you have already decoded the run, you should run the command
`./CalculatePedestals.exe $RUN` 
where $Run is the run number.

It will analyze the data, calculated pedestals, write Pedestals file `PedFiles/Pedestals_$RUN.dat`, it also will draw plots and save them in the subdirectory `Figs`.  File names are usually self-explanatory.

## Zero suppression skim

Decoded files are huge, and there is no Zero suppression on $\mu RWell$ hits. It is not practical to run do regular analysis on those file. Instead We first do a skim, which keeps only hits that are at least $3\sigma$ above the pedestal. This is rather loose skim, however enormously reduces the file, size and also makes the analysis on those file very fast.

 In addition the $\mu RWell$  hits are written in the new more compact bank named "**uRwell::Hit**". This bank has following variables:
 - **sec** is the sector, in our case, it is a detector identifier: with the following mapping:
	 - 0: GEM0
	 - 1: GEM1
	 - 2: Item1
	 - 3: Item2
	 - 4: Item3
	 - 5: Item4
- **layer** is the layer: 1=Xstrips, 2=Ystrips.
- **strip** is the strip number.
- **stripLocal** is just for convenience, it ranges from 1 to 128, and represents the connector pin number of the hit in the given Hybrid.
- **adc** is the extracted ADC value.
- **adcRel** is ADC in units of the RMS of the given channel. As an example if the given channel has RMS 5, and the ADC is 55, then the adcRel = adc/RMS = 5.5.
- **ts** is the time sample of the highest ADC of the given hit

  An example events is shown below. It has 1 hit in Item1 ,layer 1, and 2 hits in Item 2 layer 1

`uRwell::Hitext,p=previous, q=quit, h=help), Type Bank Name or id : uRwell::Hit`  
`position for [uRwell::Hit] == 310`  
* `NODE * group =     90, item =   1, type = 11, size =       48`  
          `sec :          2         3         3`  
        `layer :          1         1         1`  
        `strip :         67        37       109`  
          `adc :    15.8889   18.0000   17.4444`  
       `adcRel :     3.3061    3.6906    3.2958`  
           `ts :          7         5         2`

To run executable for running the Zero Suppresion, is `./SkimZeroSuppression.exe`, it takes two arguments, the run number and the file index.

`./SkimZeroSuppression.exe $RUN $FILE_IND`

It will produce a file named `Skim_ZeroSuppr_$RUN$_$FILE_IND$.hipo`

Note: `SkimZeroSuppression.exe` needs to know pedestals for each channel, and it will look for a file `PedFiles/Pedestals_$RUN$.dat`. Usually, before running the `SkimZeroSuppression.exe`, one can link the pedestal file to the most recent Pedestal file created by the `CalculatePedestals.exe` executable.

## Analyzing the data

There is an analysis code, that runs over skimmed file and produces histograms.
To run use

`./AnaData.exe -r $RUN -f $FILE_IND -t $THRESHOLD -m $MINHITS`

* **$RUN** is the run number
* **$FILE_IND** is the index of a file in the given run
* **$THRESHOLD** is the threshold on hits you would like to use in the analysis. It is in units of $\sigma$
* **$MINHITS** is the minimum number of hits you would like to use in the analysis

Newer analysis histograms should be added in this code AnaData.cc, or new Analysis codes should be developed.

## Drawing Plots
The executable `DrawBasicPlots.exe` reads the output root file of the `AnaData.exe`, makes plots then saves them in the sub-directory `Figs`.

To run use
`./DrawBasicPlots.exe -r $RUN -t $THRESHOLD -m $MINHITS`

Meaning of arguments is the same in the paragraph "**Analyzing the data**"
