#!/bin/bash 

# Create a directory to run sample whizard job

mkdir -p jobs

rundir=E250-TDR_ws.Pnnh.Gwhizard-1_95.eL.pR.I990001

cp -a build/whizard-1.95/results jobs/${rundir}

/bin/cp tools/sample/* jobs/${rundir}

echo "A sample directory to generate 250 GeV nnh events was created"
echo "in jobs/${rundir}.  To run jobs, cd to this directory and do "
echo "  . subjob.sh "
echo "A batch job, nnh-whizard, is submit. Do bjobs to see job status."
echo "The job log will be found in run_whizard.log."

