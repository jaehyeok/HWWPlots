Tool to make the plots for the CMS HWW legacy paper
---------------------------------------------------

Usage example:

```
.x finalPlot.C+(0, 5, "E_{T}^{miss}", "GeV", "histo.root", "histo_zhpresel_met", 1, 125, 24.4, false, 1)
```

with the following arguments:

- nsel : what type of plots
- ReBin : rebinning
- XTitle[300] : x axis title
- units[300] : x axis units
- plotName[300] : name of the plot
- outputName[300] : name of the output file
- isLogY : log y option
- MassH : higgs mass
- lumi : integrated lumi
- doMCRatio : add the data/mc ratio plot 
- singnalZoom : value by which the signal has to be multiplied


The input root file is expected to contain histograms with the following names:

- WW      : WW bkg
- DYjets  : DY + jets bkg
- top     : top and ttbar bkg
- VV      : VV excluding WW
- VVV     : VV (excluding WW) and VVV (read for VH H > lvjj only)
- Wjets   : W + jets
- Wg      : Vgamma
- Wgs     : Vgamma*
- CMSdata : data
- ggH     : gluon fusion singal
- qqH     : vbf signal (if missing is not used)
- VH      : VH signal (if missing is not used)

The available analysis categories for the plotting are the following:

- nsel == 0 : HWW main analysis
- nsel == 1 :
- nsel == 2 : VH 3 leptons analysis
- nsel == 3 :
- nsel == 4 :
- nsel == 5 : VBF @ WW level
- nsel == 6 : VBF @ Higgs level
- nsel == 7 : VH V > jj
- nsel == 8 : ZH H > lvjj
- nsel == 9 :
