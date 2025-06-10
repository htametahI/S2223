all: SortData Clock Alphacal

SortData: SortCode.cxx SortCode.h
	g++ SortCode.cxx -std=c++0x \
	-I$(GRSISYS)/include \
	-L$(GRSISYS)/lib \
	`grsi-config --cflags --all-libs --GRSIData-libs` \
	-o SortData

Clock: clock_check.cxx
	g++ clock_check.cxx -std=c++0x \
	-I$(GRSISYS)/include \
	-L$(GRSISYS)/lib \
	`grsi-config --cflags --all-libs --GRSIData-libs` \
	-o Clock

Alphacal: AlphaCalibration.c
	g++ AlphaCalibration.c -std=c++0x \
	-I$(GRSISYS)/include \
	-L$(GRSISYS)/lib \
	`grsi-config --cflags --all-libs --GRSIData-libs` \
	-o Alphacal

clean:
	rm -rf *~ SortData Clock Alphacal
