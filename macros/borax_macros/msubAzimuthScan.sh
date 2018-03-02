#!/bin/bash

actinide=$1
recoType=$2
phiMin=$3
phiMax=$4
phiStepSize=$5
plotdir=$6

echo root -l -q -b azimuthScan.C\+\($actinide,\"$recoType\",$phiMin,$phiMax,$phiStepSize\)
time root -l -q -b azimuthScan.C\+\($actinide,\"$recoType\",$phiMin,$phiMax,$phiStepSize\) >> $plotdir/log.output.phiMin${phiMin}
# time root -l -q -b azimuthScan.C\+\($actinide,\"$recoType\",$phiMin,$phiMax,$phiStepSize\)