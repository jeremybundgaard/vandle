#!/bin/bash

hkTrees=/p/lscratche/jerbundg/data/thesisData/2013/hkConfigs
fpTrees=/p/lscratche/jerbundg/data/thesisData/2013/fpConfigs

msub -q pshort -l walltime=5:00:00 $macros/compDiffRuns.sh cfNoMask1	\
hk $hkTrees/nEvents-1/cfNoMask1.trees.root
msub -q pshort -l walltime=5:00:00 $macros/compDiffRuns.sh cfNoMask2	\
hk $hkTrees/nEvents-1/cfNoMask2.trees.root
msub -q pshort -l walltime=2:00:00 $macros/compDiffRuns.sh cfCFT1		\
hk $hkTrees/nEvents-1/cfCFT1.trees.root
msub -q pshort -l walltime=2:00:00 $macros/compDiffRuns.sh cfCFT2 		\
hk $hkTrees/nEvents-1/cfCFT2.trees.root
msub -q pshort -l walltime=5:00:00 $macros/compDiffRuns.sh cfNoMask1	\
fp $fpTrees/nEvents-1/cfNoMask1.trees.root
msub -q pshort -l walltime=5:00:00 $macros/compDiffRuns.sh cfNoMask2	\
fp $fpTrees/nEvents-1/cfNoMask2.trees.root
msub -q pshort -l walltime=2:00:00 $macros/compDiffRuns.sh cfCFT1		\
fp $fpTrees/nEvents-1/cfCFT1.trees.root
msub -q pshort -l walltime=2:00:00 $macros/compDiffRuns.sh cfCFT2 		\
fp $fpTrees/nEvents-1/cfCFT2.trees.root
