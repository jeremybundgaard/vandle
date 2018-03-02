#!/bin/bash

if [[ $# -ne 1 ]]; then

	echo "USAGE: $macros/makeFitType5trees.sh <origTree.root>"
	exit
fi
	origTree=$1
	newTree=$origTree.fitType5
	echo root -l -q -b $macros/makeFitType5trees.C\+\(\"$origTree\",\"$newTree\"\)
	time root -l -q -b $macros/makeFitType5trees.C\+\(\"$origTree\",\"$newTree\"\)
