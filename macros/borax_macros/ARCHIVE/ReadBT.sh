#!/bin/bash

if [[ ! -d plots ]]; then
	mkdir plots
fi

outname=plot.`basename $1 .root`
root -l -q -b $macros/ReadBT.C\(\"$1\",\"$outname\"\)


rm ReadBT_*
