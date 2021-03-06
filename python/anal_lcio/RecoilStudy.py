'''
Sample program to study e1E1 -> e2E2h by python

To run non-interactively, namely run main part of this script, do 

    $ python RecoilStudy.py  


To run interactively, do
    $ python 
    >> from RecoilStudy import *
    >> dir()           # List importted names
    >> help(examplePrintEvent)    # Print help  

    >> reader = getReader()  
    >> examplePrintEvent(reader)  # Execute examplePrintEvent
  

'''

from pyLCIO.io import LcioReader, StdHepReader

import ROOT
ROOT.gROOT.SetBatch()

import pprint

# ====================================================== 
def getReader(datafile = "rv02-00-01.sv02-00-01.mILD_l5_o1_v02.E500-TDR_ws.I106519.Pe2e2h.eL.pR.n001.d_dstm_10263_0.slcio",
              datadir = "/hsm/ilc/grid/storm/prod/ilc/mc-opt-3/ild/dst-merged/500-TDR_ws/higgs_ffh/ILD_l5_o1_v02/v02-00-01/"):
    '''
    Returns LcioReader of input file.   Prepared for interactive use.
  
    :params string datafile : input filename. The default is
    :params string datadir  : directory of input file. The default is
    '''
    inputpath = datadir + datafile
    return LcioReader.LcioReader(datadir + datafile)
  
# ====================================================== 
def examplePrintEvent(reader, maxRead=5):
    ''' 
    Example to access basic information in the DST file 
   
    :param LcioReader reader: LcioReader object for input file.
    :param int maxRead: Number of events to read.
    
    '''
    
    for idx, event in enumerate(reader):
        if maxRead > 0 and idx >= maxRead:
            break
    
        print "\n### Reading event:" + str(idx+1)
        ''' print collections in the event '''
        cols = event.getCollectionNames()
        for col in cols:
            print "  "+col
    
        pfos = event.getCollection("PandoraPFOs")
        print " # of PFO Objects ="+str(len(pfos))
    
        # pfo = pfos.at(0) # Get first element in PandoraPFOs collection
    
        print "PFO: PID Charge  Px pY Pz E "
        for pfo in pfos:
    
            p = pfo.getLorentzVec()
            ene = pfo.getEnergy()
            ch  = pfo.getCharge()
            ptype = pfo.getType()
      
            print "    ",
            print pfo.getType(),
            print pfo.getCharge(),
           
            print str(p.X())+" "+str(p.Y())+" "+str(p.Z()),
            print ene
      
# ====================================================== #
def makeNtuple(reader, rootFile="anal.root", ecm=500.0, maxRead=1000):
    '''
    Analize reader file and create a root Ntuple file   
  
    :params LcioReader reader : reader for an input file
    :params string rootFile: a root file name to write ntuple
    :params float ecm: nominal CM energy 
    :params int maxRead: max number of events to read
  
    '''
    rfile = ROOT.TFile(rootFile, "recreate")
    nt = ROOT.TNtuple("nt","e2E2h analysis", "nminus:nplus:mas:mm")
  
    for idx, event in enumerate(reader):
        if maxRead > 0 and idx >= maxRead:
            break
        
        psum = ROOT.TLorentzVector(0.0, 0.0, 0.0, 0.0)
        pfos = event.getCollection("PandoraPFOs")
        nminus = 0
        nplus = 0
    
        for pfo in pfos:
            p = pfo.getLorentzVec()
            ptype = pfo.getType()
            pabs = p.P()
            if abs(ptype) == 13 and pabs > 10.0 :
                if ptype == 13 and nminus == 0:
                    psum += p
                    nminus = 1
                if ptype == -13 and nplus == 0:
                    psum += p
                    nplus = 1
             
        pini = ROOT.TLorentzVector(0.0, 0.0, 0.0, ecm)
        pmis = pini - psum
        missmass = pmis.M()
        mumumass = psum.M()
        nt.Fill(float(nminus), float(nplus), mumumass, missmass )
    
    print "### Last event was "+str(idx)
    rfile.Write()
    rfile.Close()
      
#====================================================== #
def makePlots(plotdata, pngfile="plots.png"):
    '''
    From rootfile, make plots of mumu mass and missing mass 
  
    :params string plotdata : dict object of root file name and plot options ( {"rootfile":  , "option":   }
    :params string pngfile : plot file name
  
    '''
    
    data = []
    # rfs = []
    # nts = []
    # data = copy.deepcopy(plotdata)
    # Get TNtuple objects from ROOT files
    for pldata in sorted(plotdata):
        rf = pldata["rootfile"]
        opts = pldata["option"]
        rec = {}
        rec["rf"] = ROOT.TFile(rf)
        rec["nt"] = rec["rf"].Get("nt")
        rec["nt"].SetLineColor(opts["color"])
        rec["opts"] = opts
        data.append(rec)

    c1 = ROOT.TCanvas("c1", "Example", 800, 800)
    c1.Divide(1, 2)

    # Draw uppper figure
    c1.cd(1)
    selection = "mas>80.0 && mas < 120.0"
    nent = data[0]["nt"].GetEntries()
    nsel = data[0]["nt"].Draw("mas",selection)
    ROOT.gPad.SetGridx(1)
    ROOT.gPad.SetGridy(1)
    data[0]["eff"] = float(nsel)/float(nent)*100.0
    if len(data) > 1:
        for da in data[1:]:
            nent = da["nt"].GetEntries()
            nsel = da["nt"].Draw("mas",selection,"same")
            da["eff"] = float(nsel)/float(nent)*100.0
    for da in data:
        print "%8.4f" % da["eff"]
 

    xpos = 0.7
    ypos = 0.75
    tobj1 = []
    for da in data:
       text = "%s : Eff=%8.2f %%" % (da["opts"]["legend"], da["eff"])
       ypos -= 0.05
       tobj1.append(ROOT.TLatex(xpos, ypos, text))
       tobj1[-1].SetTextColor(da["opts"]["color"])
       tobj1[-1].SetNDC(True)
       tobj1[-1].Draw()
    
    # Draw second figure
    c1.cd(2)
    selection = "mm>100.0 && mm < 500.0"
    nent = data[0]["nt"].GetEntries()
    nsel = data[0]["nt"].Draw("mm",selection)
    ROOT.gPad.SetGridx(1)
    ROOT.gPad.SetGridy(1)
    data[0]["eff"] = float(nsel)/float(nent)*100.0
    if len(data) > 1:
        for da in data[1:]:
            nent = da["nt"].GetEntries()
            nsel = da["nt"].Draw("mm",selection,"same")
            da["eff"] = float(nsel)/float(nent)*100.0

    xpos = 0.7
    ypos = 0.75
    tobj2 = []
    for da in data:
       text = "%s : Eff=%8.2f %%" % (da["opts"]["legend"], da["eff"])
       ypos -= 0.05
       tobj2.append(ROOT.TLatex(xpos, ypos, text))
       tobj2[-1].SetTextColor(da["opts"]["color"])
       tobj2[-1].SetNDC(True)
       tobj2[-1].Draw()
    # Write results to a file.
    c1.Print(pngfile)  
  
  

#======================================================#
if __name__ == '__main__':

    files = {}
    files["l5"] = ["/hsm/ilc/grid/storm/prod/ilc/mc-opt-3/ild/dst-merged/500-TDR_ws/higgs_ffh/ILD_l5_o1_v02/v02-00-01/rv02-00-01.sv02-00-01.mILD_l5_o1_v02.E500-TDR_ws.I106519.Pe2e2h.eL.pR.n001.d_dstm_10263_0.slcio"]
    files["s5"] = ["/hsm/ilc/grid/storm/prod/ilc/mc-opt-3/ild/dst-merged/500-TDR_ws/higgs_ffh/ILD_s5_o1_v02/v02-00-01/rv02-00-01.sv02-00-01.mILD_s5_o1_v02.E500-TDR_ws.I106519.Pe2e2h.eL.pR.n001.d_dstm_10264_0.slcio"]
    files["dbd"] = ["/hsm/ilc/grid/storm/prod/ilc/mc-dbd/ild/dst-merged/500-TDR_ws/higgs_ffh/ILD_o1_v05/v01-16-p05_500/rv01-16-p05_500.sv01-14-01-p00.mILD_o1_v05.E500-TDR_ws.I106519.Pe2e2h.eL.pR-00001-DST.slcio",
                    "/hsm/ilc/grid/storm/prod/ilc/mc-dbd/ild/dst-merged/500-TDR_ws/higgs_ffh/ILD_o1_v05/v01-16-p05_500/rv01-16-p05_500.sv01-14-01-p00.mILD_o1_v05.E500-TDR_ws.I106519.Pe2e2h.eL.pR.d_dstm_8118_1.slcio"]

    do_anal = True
    maxread = 8996
    # maxread = 100
  
    options = {}
    options = {"l5":{"color":2,"legend":"l5_o1"}, "s5":{"color":3, "legend":"s5_o1"}, "dbd":{"color":4, "legend":"dbd"}}
    plotdata = []

    for key, infiles in files.iteritems():
        print key
        rootfile = "anal-" + key + ".root"
        if do_anal:
            reader = LcioReader.LcioReader()
            for infile in infiles:
                print "Reading " + infile
                reader.addFile(infile)
                   
            makeNtuple(reader, rootFile=rootfile, maxRead=maxread)

        plotdata.append({"rootfile":rootfile, "option":options[key]})
      
    makePlots(plotdata)
  

