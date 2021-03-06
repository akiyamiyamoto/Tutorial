#!/bin/bash

. /cvmfs/ilc.desy.de/sw/x86_64_gcc82_centos7/v02-02-01/init_ilcsoft.sh

echo ${lcgeo_DIR}

ddsim  --compactFile sample.xml \
       --gun.energy "10.0*GeV" \
       --enableGun \
       --gun.particle mu- \
       --gun.thetaMin 0.5 \
       --gun.thetaMax 1.0 \
       --gun.distribution uniform \
       --macroFile init_vis.mac \
       --runType vis

#       --numberOfEvents 5
#       --gun.distribution ffbar \


