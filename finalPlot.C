// .x finalPlot.C+(0,5,"E_{T}^{miss}","GeV","histo.root","histo_zhpresel_met",1,125,24.4, doDataMCRatio, signalZoom)

#include "TROOT.h"
#include "TSystem.h"
#include "TInterpreter.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TPad.h"
#include "Math/QuantFuncMathCore.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "StandardPlot.C"

void 
finalPlot (int nsel             = 0, 
           int ReBin            = 1, 
           char XTitle[300]     = "N_{jets}", 
           char units[300]      = "", 
           char plotName[300]   = "histo_nice.root", 
           char outputName[300] = "njets",
           bool isLogY          = false, 
           int MassH            = 160, 
           double lumi          = 4.6, 
           bool doDataMCRatio   = true, 
           int signalZoom       = 1
  ) 
{
  
  gInterpreter->ExecuteMacro("GoodStyle.C");
  gROOT->LoadMacro("StandardPlot.C");

  std::cout << "reading " << plotName << std::endl ;
  TFile* file = new TFile(plotName, "read");

  //PG prepare the object that is making the plots
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ----

  std::cout << "setting up the plot object " << std::endl ;

  StandardPlot myPlot;
  myPlot.setLumi(lumi);
  myPlot.setLabel(XTitle);
  myPlot.setSignalZoom (signalZoom) ;
  if     (lumi ==    4.9) myPlot.addLabel("#sqrt{s} = 7 TeV");
  else if(lumi ==   19.5) myPlot.addLabel("#sqrt{s} = 8 TeV");
  else if(lumi ==   24.4) myPlot.addLabel("#sqrt{s} = 7+8 TeV");
  else if(lumi ==   4.91) myPlot.addLabel("SSSF #sqrt{s} = 7 TeV");
  else if(lumi ==  19.51) myPlot.addLabel("SSSF #sqrt{s} = 8 TeV");
  else if(lumi ==   4.92) myPlot.addLabel("OSSF #sqrt{s} = 7 TeV");
  else if(lumi ==  19.52) myPlot.addLabel("OSSF #sqrt{s} = 8 TeV");
  else                    myPlot.addLabel(""); 
  myPlot.setMass (MassH) ;
  myPlot.setUnits(units);
  myPlot.SetColorsAndLabels () ;

  //PG get the bkg histograms from the file
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ----

  std::cout << "getting histograms " << std::endl ;

  TH1F* hWW     = (TH1F*) file->Get ("WW");
  TH1F* hZZ     = (TH1F*) file->Get ("ZZ");
  TH1F* hFakes     = (TH1F*) file->Get ("fakes");
  TH1F* hZJets  = (TH1F*) file->Get ("DY+jets");
  TH1F* hTop    = (TH1F*) file->Get ("top");
  TH1F* hVV     = (TH1F*) file->Get ("VV"); 
  TH1F* hVVV    = (TH1F*) file->Get ("VVV"); 
  TH1F* hWZ    = (TH1F*) file->Get ("WZ"); 
  TH1F* hWJets  = (TH1F*) file->Get ("W+jets");
  TH1F* hWg     = (TH1F*) file->Get ("Wg");
  TH1F* hWgs    = (TH1F*) file->Get ("Wgs");
  TH1F* hVg     = (TH1F*) file->Get ("Vg");
  
  double scale = 1;
  if (hWW)    hWW   ->Scale(scale);
  if (hZZ)    hZZ   ->Scale(scale);
  if (hFakes) hFakes->Scale(scale);
  if (hZJets) hZJets->Scale(scale);
  if (hTop)   hTop  ->Scale(scale);
  if (hVV)    hVV   ->Scale(scale);
  if (hVVV)   hVVV  ->Scale(scale);
  if (hWJets) hWJets->Scale(scale);
  if (hWg)    hWg   ->Scale(scale);
  if (hWgs)   hWgs  ->Scale(scale);
  if (hVg)    hVg   ->Scale(scale);

  //PG get the signal histograms from the file
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  TH1F* hggH   = (TH1F*) file->Get("ggH");
  TH1F* hqqH   = (TH1F*) file->Get("qqH");
  TH1F* hVH    = (TH1F*) file->Get("VH");

  if (hggH) hggH->Scale (scale * signalZoom);
  if (hqqH) hqqH->Scale (scale * signalZoom);
  if (hVH)  hVH->Scale  (scale * signalZoom);

  //PG assing the plots to the object making the plots,
  //PG according to the channel
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ----

  std::cout << "assigning histos to the plotting object" << std::endl ;

  // nsel == 1 means HWW analysis
  if(nsel == 0 || nsel == 1) {
    std::cout << "nsel = " << nsel << ", main analysis plots" << std::endl ;
    if(hWW->GetSumOfWeights(   ) > 0) myPlot.setMCHist(iWW,      (TH1F*)hWW   ->Clone("hWW"));
    if(hZJets->GetSumOfWeights() > 0) myPlot.setMCHist(iZJets,   (TH1F*)hZJets->Clone("hZJets"));
    if(hTop->GetSumOfWeights()   > 0) myPlot.setMCHist(iTop,     (TH1F*)hTop  ->Clone("hTop"));
    if(hVV->GetSumOfWeights()    > 0) myPlot.setMCHist(iVV,      (TH1F*)hVV   ->Clone("hVV")); 
    if(hWJets->GetSumOfWeights() > 0) myPlot.setMCHist(iWJets,   (TH1F*)hWJets->Clone("hWJets"));
    //if(hWg->GetSumOfWeights()    > 0) myPlot.setMCHist(iWgamma,  (TH1F*)hWg->Clone("hWg"));
    //if(hWgs->GetSumOfWeights()   > 0) myPlot.setMCHist(iWgammaS, (TH1F*)hWgs->Clone("hWgs"));
    TH1F* hHWW     = (TH1F*) hggH->Clone ("hggH");
    if (hqqH != 0) hHWW->Add (hqqH) ;
    if (hVH != 0)  hHWW->Add (hVH) ;
    myPlot.setMCHist (iHWW, (TH1F*) hHWW->Clone ("hHWW")) ;
    //myPlot.setIsHWWOverlaid(true);

  }
  // nsel == 2 means VH > 3 leptons
  else if (nsel == 2 || nsel == 3) {

    TH1F* hZGamma = (TH1F*)file->Get("histo6");
    if(hZGamma->GetSumOfWeights() > 0) myPlot.setMCHist(iZGamma, (TH1F*)hZGamma->Clone("hZGamma"));

    //myPlot.setMCHist(iWW,    (TH1F*)hWW   ->Clone("hWW"));
    //myPlot.setMCHist(iZJets, (TH1F*)hZJets->Clone("hZJets"));
    myPlot.setMCHist(iZZ,    (TH1F*)hTop  ->Clone("hTop"));
    myPlot.setMCHist(iWZ,    (TH1F*)hVV   ->Clone("hVV")); 
    myPlot.setMCHist(iFakes, (TH1F*)hWJets->Clone("hWJets"));
    TH1F* hHWW     = (TH1F*) hggH->Clone ("hWW");
    if (hqqH != 0) hHWW->Add (hqqH) ;
    if (hVH != 0)  hHWW->Add (hVH) ;
    myPlot.setMCHist (iVH, (TH1F*) hHWW->Clone("hVH")) ;
  }
  else if (nsel == 4) {
    myPlot.setMCHist(iZJets, (TH1F*)hWW   ->Clone("hWW"));
    myPlot.setMCHist(iVVV,   (TH1F*)hZJets->Clone("hZJets"));
    myPlot.setMCHist(iWZ,    (TH1F*)hTop  ->Clone("hTop"));
    myPlot.setMCHist(iZZ,    (TH1F*)hVV   ->Clone("hVV")); 
    myPlot.setMCHist(iEM,    (TH1F*)hWJets->Clone("hWJets"));
  }
  else if (nsel == 5) {
    std::cout << "nsel = " << nsel << ", VBF/VH analysis plots (at WW level) (Fig. 2)" << std::endl ;
    myPlot.setMCHist (iWW,    (TH1F*) hWW   ->Clone ("hWW"));    // x
    myPlot.setMCHist (iVV,    (TH1F*) hVV   ->Clone ("hVV"));    // x
    myPlot.setMCHist (iWJets, (TH1F*) hWJets->Clone ("hWJets")); // x
    myPlot.setMCHist (iZJets, (TH1F*) hZJets->Clone("hZJets"));  // x
    myPlot.setMCHist (iWgamma,(TH1F*) hVg   ->Clone ("hVg"));    // x
    myPlot.setMCHist (iTop,   (TH1F*) hTop  ->Clone("hTop"));    // x
    myPlot.setMCHist (iggH,   (TH1F*) hggH  ->Clone ("hggH"));
    myPlot.setMCHist (iVBF,   (TH1F*) hqqH  ->Clone ("hVBF"));
//     myPlot.setMCHist (iVH,    (TH1F*) hVH   ->Clone ("hVH"));
    myPlot._sampleLabel[iWgamma] = "V+#gamma/V+#gamma*";
    myPlot._sampleLabel[iVV] = "WZ/ZZ/VVV";
//     myPlot.setIsHWWOverlaid(true);

    myPlot._position[0]  = iVV;
    myPlot._position[1]  = iWJets;
    myPlot._position[2]  = iWZ;
    myPlot._position[3]  = iZZ;
    myPlot._position[4]  = iFakes;
    myPlot._position[5]  = iZGamma;
    myPlot._position[6]  = iVVV;
    myPlot._position[7]  = iEM;
    myPlot._position[8]  = iWgamma;
    myPlot._position[9]  = iWgammaS;
    myPlot._position[10] = iTop;
    myPlot._position[11] = iZJets;
    myPlot._position[12] = iWW;
    myPlot._position[13] = iHWW;
    myPlot._position[14] = iggH;
    myPlot._position[15] = iVBF;
    myPlot._position[16] = iVH;
    myPlot._position[17] = nSamples;
    
  }
  else if (nsel == 6) {
    std::cout << "nsel = " << nsel << ", VBF analysis plots (at Higgs level) (Fig. 10)" << std::endl ;
    myPlot.setMCHist (iWW,    (TH1F*) hWW   ->Clone ("hWW"));
    myPlot.setMCHist (iVV,    (TH1F*) hVV   ->Clone ("hVV"));
    myPlot.setMCHist (iWJets, (TH1F*) hWJets->Clone ("hWJets"));
    myPlot.setMCHist (iZJets, (TH1F*) hZJets->Clone("hZJets"));
    myPlot.setMCHist (iWgamma,(TH1F*) hVg   ->Clone ("hVg"));
    myPlot.setMCHist (iTop,   (TH1F*) hTop  ->Clone("hTop"));
    myPlot.setMCHist (iggH,   (TH1F*) hggH  ->Clone ("hggH")); //---- ggH + VBF fused into "ggH" --> "H"
    myPlot._sampleLabel[iWgamma] = "V+#gamma/V+#gamma*";
    myPlot._sampleLabel[iVV] = "WZ/ZZ/VVV";
    myPlot._sampleLabel[iggH] = "H";
    myPlot.setBreakdown(true);
    myPlot.setIsHWWOverlaid(true);
  
    myPlot._position[0]  = iVV;
    myPlot._position[1]  = iWJets;
    myPlot._position[2]  = iWZ;
    myPlot._position[3]  = iZZ;
    myPlot._position[4]  = iFakes;
    myPlot._position[5]  = iZGamma;
    myPlot._position[6]  = iVVV;
    myPlot._position[7]  = iEM;
    myPlot._position[8]  = iWgamma;
    myPlot._position[9]  = iWgammaS;
    myPlot._position[10] = iTop;
    myPlot._position[11] = iZJets;
    myPlot._position[12] = iWW;
    myPlot._position[13] = iHWW;
    myPlot._position[14] = iggH;
    myPlot._position[15] = iVBF;
    myPlot._position[16] = iVH;
    myPlot._position[17] = nSamples;
    
    myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("errorBand")));
  }
  else if (nsel == 7) {
    std::cout << "nsel = " << nsel << ", VH analysis plots (at Higgs level) (Fig. 12)" << std::endl ;
    myPlot.setMCHist (iWW,    (TH1F*) hWW   ->Clone ("hWW"));
    myPlot.setMCHist (iVV,    (TH1F*) hVV   ->Clone ("hVV"));
    myPlot.setMCHist (iWgamma,(TH1F*) hVg   ->Clone ("hVg"));
    myPlot.setMCHist (iWJets, (TH1F*) hWJets->Clone ("hWJets"));
    myPlot.setMCHist (iZJets, (TH1F*) hZJets->Clone("hZJets"));
    myPlot.setMCHist (iTop,   (TH1F*) hTop  ->Clone("hTop"));
    myPlot.setMCHist (iggH,   (TH1F*) hggH  ->Clone ("hggH")); //---- ggH + VBF fused into "ggH" --> "H"
//     myPlot.setMCHist (iVBF,   (TH1F*) hqqH  ->Clone ("hVBF"));
//     myPlot.setMCHist (iVH,    (TH1F*) hVH   ->Clone ("hVH"));
    myPlot._sampleLabel[iWgamma] = "V+#gamma/V+#gamma*";
    myPlot._sampleLabel[iVV] = "WZ/ZZ/VVV";
    myPlot._sampleLabel[iggH] = "H";
    myPlot.setBreakdown(true);
    myPlot.setIsHWWOverlaid(true);
    
    myPlot._position[0]  = iVV;
    myPlot._position[1]  = iWJets;
    myPlot._position[2]  = iWZ;
    myPlot._position[3]  = iZZ;
    myPlot._position[4]  = iFakes;
    myPlot._position[5]  = iZGamma;
    myPlot._position[6]  = iVVV;
    myPlot._position[7]  = iEM;
    myPlot._position[8]  = iWgamma;
    myPlot._position[9]  = iWgammaS;
    myPlot._position[10] = iTop;
    myPlot._position[11] = iZJets;
    myPlot._position[12] = iWW;
    myPlot._position[13] = iHWW;
    myPlot._position[14] = iggH;
    myPlot._position[15] = iVBF;
    myPlot._position[16] = iVH;
    myPlot._position[17] = nSamples;
    
    myPlot.set_ErrorBand(*((TGraphAsymmErrors*) file->Get("errorBand")));
  }
  else if (nsel == 8) {
    std::cout << "nsel = " << nsel << ", ZH analysis plots" << std::endl ;
    myPlot.setMCHist (iZZ,    (TH1F*) hZZ->Clone ("hZZ"));
    myPlot.setMCHist (iWZ,   (TH1F*) hWZ->Clone("hWZ"));
    myPlot.setMCHist (iFakes, (TH1F*) hFakes  ->Clone("hFakes"));
    myPlot.setMCHist (iVH,    (TH1F*) hVH  ->Clone ("hVH"));
    myPlot._sampleLabel[iWZ    ] = " WZ/VVV";
  }
  //PG get the data histogram
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  std::cout << "getting data" << std::endl ;

  TH1F *hData = (TH1F*)file->Get("Data"); 

  std::cout << "passing data to the plotting object" << std::endl ;
  myPlot.setDataHist((TH1F*)hData->Clone("data"));

  std::cout << "printout" << std::endl ;

  TCanvas* c1 = new TCanvas("c1", "c1");

  if(isLogY == true) c1->SetLogy();

  std::cout << "call the draw method of the plotting tool" << std::endl ;
  myPlot.Draw(ReBin);  // Can pass a rebin 
  std::cout << "done" << std::endl ;
  c1->GetFrame()->DrawClone();

  //hggH->Rebin(ReBin);
  //hqqH->Rebin(ReBin);
  //hVH ->Rebin(ReBin);
  //hggH->Draw("same,hist");
  //hqqH->Draw("same,hist");
  //hVH ->Draw("same,hist");
  //std::cout << hggH->GetSumOfWeights() << " " << hqqH->GetSumOfWeights() << " " << hVH->GetSumOfWeights() << std::endl;

  char CommandToExec[300];
  sprintf(CommandToExec,"mkdir plots");
  gSystem->Exec(CommandToExec);  

  char myOutputFile[300];
  sprintf(myOutputFile,"plots/%s.eps",outputName);
  c1->SaveAs(myOutputFile);
  sprintf(myOutputFile,"plots/%s.png",outputName);
  c1->SaveAs(myOutputFile);
  sprintf(myOutputFile,"plots/%s.pdf",outputName);
  c1->SaveAs(myOutputFile);

  if(doDataMCRatio == true){
    // Data - MC
    //--------------------------------------------------------------------------
    TCanvas* c2 = new TCanvas("c2", "c2", 575, 10, 550, 600);

    TH1F* dt = (TH1F*)hData->Clone();
    TH1F* mc = (TH1F*)hWW->Clone();

    dt->SetNameTitle("dt", "dt");
    mc->SetNameTitle("mc", "mc");

    //PG sum up all the bkg histos saved in myPlot object
    for (int i = 0 ; i < nSamples ; ++i)
      {
        if (myPlot._bkgHist[i] != 0)
          mc->Add(myPlot._bkgHist[i]) ;    
      }
  
//    mc->Add(hZJets);
//    mc->Add(hTop  );
//    mc->Add(hVV   );
//    mc->Add(hWJets);
//    mc->Add(hHWW  );

    dt->Rebin(ReBin);
    mc->Rebin(ReBin);
    
    TH1F* diff = (TH1F*)dt->Clone();

    diff->SetNameTitle("", "");

    diff->Reset();
    double sum[2] = {0., 0.};

    for (Int_t i=1; i<=diff->GetNbinsX(); i++) {

      Float_t a = dt->GetBinContent(i);
      Float_t b = mc->GetBinContent(i);
      sum[0] = sum[0] + dt->GetBinContent(i);
      sum[1] = sum[1] + mc->GetBinContent(i);

      Float_t sa = dt->GetBinError(i);
      Float_t sb = mc->GetBinError(i);

      Float_t d  = 1.0;
      Float_t sd = 1.0;      
      if(a > 0 && b >0){
          d  = a / b;
          sd = sqrt(sa/a*sa/a+sb/b*sb/b)*d;
          //d  = a - b;
          //sd = sqrt(sa*sa+sb*sb);

        printf("data(%d): %f mc: %f -> data/mc = %f\n",i,a,b,a/b);
          diff->SetBinContent(i, d);
          diff->SetBinError  (i, sd);
      } else {
          diff->SetBinContent(i, 1.0);
          diff->SetBinError  (i, 0.0);
          //diff->SetBinContent(i, 0.0);
          //diff->SetBinError  (i, 0.0);
      }
    }
    printf("data: %f mc: %f -> data/mc = %f\n",sum[0],sum[1],sum[0]/sum[1]);

    diff->Draw();
    //hHWW->Rebin(ReBin);
    //hHWW->Draw("same,hist");
    
    diff->SetMinimum(0.4);
    diff->SetMaximum(1.6);

    gStyle->SetOptStat(0);

    //TBox* tenPercentBox = new TBox(diff->GetXaxis()->GetXmin(), 0.90,
    //               diff->GetXaxis()->GetXmax(), 1.10);

    //tenPercentBox->SetFillColor(kAzure-9);

    //tenPercentBox->Draw("same");

    AxisFonts((TAxis*)diff->GetXaxis(), "x", XTitle);
    AxisFonts((TAxis*)diff->GetYaxis(), "y", "data / MC");

    TLine* oneLine = new TLine(diff->GetXaxis()->GetXmin(), 0,
                   diff->GetXaxis()->GetXmax(), 0);

    oneLine->SetLineStyle(3);
    oneLine->SetLineWidth(3);

    oneLine->Draw("same");

    diff->SetMarkerStyle(kFullCircle);
    diff->SetLineWidth(0);
    diff->SetFillColor(kGray+1);
    diff->SetFillStyle(1001);
    diff->Draw("AE2,same"); 
    diff->Draw("sameaxis");
    diff->Draw("same");

    c2->GetFrame()->DrawClone();
  }
}
