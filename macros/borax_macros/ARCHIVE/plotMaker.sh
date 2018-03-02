#!/bin/bash

root -q -b $myana/anahists/plotMaker.C\+\(\"$1\"\)

$myana/cleanup.sh

if [[ -d plots ]]; then
	mv *png plots
else
	mkdir plots
	mv *png plots
fi
