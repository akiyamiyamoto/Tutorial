#!/bin/bash



## 1. Initialize the current ilcsoft release

softdir=/cvmfs/ilc.desy.de/sw

. ${softdir}/x86_64_gcc82_centos7/v02-02-01/init_ilcsoft.sh

ildconfig=${softdir}/ILDConfig/v02-02-01/StandardConfig/production


## 2. run the lcgeo/ddsim simulation example

echo "ILCSOFT is ${ILCSOFT}"
which ddsim

detector="ILD_l5_v02"
echo "### Execute ddsim ${detector}"
ddsim --inputFiles ${ildconfig}/Examples/bbudsc_3evt/bbudsc_3evt.stdhep \
      --outputFile bbudsc_3evt.m${detector}.slcio \
      --compactFile ${lcgeo_DIR}/ILD/compact/${detector}/${detector}.xml \
      --steeringFile ${ildconfig}/ddsim_steer.py > ddsim.m${detector}.log 2>&1 

echo "### ddsim completed `date` "
