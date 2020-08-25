//  Root based classesOAA
#include "TGraphAsymmErrors.h"
#include <vector>
#include "TKey.h"
#include "TSystem.h"

// std lib
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <time.h>

// load provate headers
#include "Headers/CMS_lumi.h"
#include "Headers/Macro.h"

// declare functions used in macro
void CheckCompatability(TList* list1, TList* list2, TString filter);
std::vector<TString> GetHistList(TList* list, TList* list2, TString obj_list, TString filter_string, bool check_subdir , TString _dirname);
std::vector<TString> GetHistListTMP(TList* list, TString obj_list, TString filter_string, bool check_subdir , TString _dirname);
bool IsPathExist(const std::string &s);
const std::string currentDateTime() ;


//  Setup input and output dir
// ********* USER MOD *********
TString path_subfix="/Users/john/Plots/Type1/signal_comp/";
TString out_dir="SignalComp";
// ********* USER MOD *********                                                                                   

//**************************************************
//
// Bacis script to plot comparison between two files
//
//**************************************************


// To run:
// root -l -q -b Type1_SignalComp.C


//   edit the following function to filter hist names plotted:
//   GetHistList(file_sig1->GetListOfKeys() , "","*", plot_sub_dirs);

//


// call to main function

void Type1_SignalComp(bool plot_sub_dirs=false){

  cout << " " << endl;
  cout << "Type1_SignalComp::INFO  Comparison plotter for Type 1 analysis" << endl;
  if(plot_sub_dirs)cout << "Type1_SignalComp::INFO  plot_sub_dirs if set to true" << endl;
  else {
    cout << "Type1_SignalComp::INFO  plot_sub_dirs if set to false " << endl;
    cout << "Type1_SignalComp::INFO  (to create plots in sub-directories run: root -l -q -v  'Type1_SignalComp.C(true)'"	<< endl;
  }
  cout << " " << endl;
  
  // ============== set style format
  
  setTDRStyle();

  // ============== set TH1 sum weights for error calculation with weighted bins
  TH1::SetDefaultSumw2(true);


  // ============== Input file configuration
  // ********* USER MOD *********                                                                              
  TFile * file_sig1 = new TFile((path_subfix+"Files/Signal__/HNtypeI_JA_HN_S_mm_200.root"));
  TFile * file_sig2 = new TFile((path_subfix+"Files/Signal__/HNtypeI_JA_HNToMuMu_Sch_M200.root"));
  // ********* USER MOD *********                                                                                   
  cout << " " << endl;
  cout << "Type1_SignalComp::INPUT_FILES " << endl;  
  cout << "File 1: " << file_sig1-> GetName() << endl;
  cout << "File 2: " << file_sig2-> GetName() << endl;
  cout << " " << endl;

  // ============================================================================                                
  // ********* USER MOD *********
  TString _filter= "*";  // * means all hists in files are plotted
  TString obj_type="TH1_TH2";
  // ********* USER MOD *********                                                                                    //_filter= "50gev";  // only make plots with this string in
  
  // ============================================================================
  // **********************  Function to check histogram compatability
  // ============================================================================
  CheckCompatability(file_sig1->GetListOfKeys() , file_sig2->GetListOfKeys() , _filter);
  // ============================================================================
  // **********************  Function to get list of histograms from file
  // ============================================================================        
  vector<TString> histlist=   GetHistList(file_sig1->GetListOfKeys() , file_sig2->GetListOfKeys(), obj_type, _filter, plot_sub_dirs,"");

  //================= loop over all histograms
  for(unsigned int ihist=0; ihist < histlist.size(); ihist++){

    //===========  check event_type 
    TString event_type="";

    //============ assign histlist_s as string for histogram cut
    TString histlist_s;
    for(unsigned int ih=0; ih < histlist.size(); ih++){
      if(histlist[ihist].Contains( "_"+histlist[ih])) histlist_s=histlist[ih];
    }

    //==========************************************************************
    //==========************************************************************
    // Make all directories for output files
    if(!IsPathExist(string(path_subfix+"Plots/"+out_dir)))  gSystem->mkdir(path_subfix+"Plots/"+out_dir);



    //============  some histogram settings to make plots look better
    TString x_axis_title="";
    TString y_axis_title="";
    float ratio_y_aspect=1.;

    // ********* USER MOD *********	     

    if(histlist[ihist].Contains("mu_Neutrino_eta")){
      x_axis_title="#mu_{N} #eta";
      y_axis_title="/ events";
    }
    else     if(histlist[ihist].Contains("mu_Neutrino_pt")){
      x_axis_title="#mu_{N} p_{T} (GeV)";
      y_axis_title="/ GeV";
    }

    else if(histlist[ihist].Contains("mu_W_eta")){
      x_axis_title="#mu_{W} #eta ";
      y_axis_title="/ events";
      ratio_y_aspect=1.5;
    }
    else if(histlist[ihist].Contains("mu_W_pt")){
      x_axis_title="#mu_{W}  p_{T} (GeV) ";
      y_axis_title="/ GeV";
      ratio_y_aspect=1.5;
    }
     else if(histlist[ihist].Contains("Neutrino_eta")){
      x_axis_title="N #eta ";
      y_axis_title="/ events";
      ratio_y_aspect=1.5;
    }
     else if(histlist[ihist].Contains("Neutrino_pt")){
      x_axis_title="N  p_{T} (GeV) ";
      y_axis_title="/ GeV";
      ratio_y_aspect=1.5;
    }
       else if(histlist[ihist].Contains("Neutrino_mass")){
      x_axis_title="m_{N} (GeV) ";
      y_axis_title="/ GeV";
      ratio_y_aspect=1.5;
    }
    else if(histlist[ihist].Contains("jet_w_eta")){
      x_axis_title="Jet_{W} #eta";
      y_axis_title="/ events";
    }
     else if(histlist[ihist].Contains("jet_w_pt")){
      x_axis_title="Jet_{W}  p_{T} (GeV)";
      y_axis_title="/ GeV";
    }
    // ********* USER MOD *********		\

    
    ///=====================================================
    //// make Legend for top pad
    ///=====================================================

    TLegend* legendH;
    legendH = new TLegend(0.25, 0.8, 0.5, 0.95);
    legendH->SetFillColor(kWhite);
    legendH->SetTextSize(0.03);
    legendH->SetBorderSize(0);
    legendH->SetTextFont(42);

    
    ///=====================================================                                                                                                                                                  
    ///=====================================================                                                                                                                                                  

    TString hist=   histlist[ihist];
    TH1D* h_data1 = (TH1D*)file_sig1->Get(hist);
    TH1D* h_data2 = (TH1D*)file_sig2->Get(hist);
    if(!h_data1) continue;
    if(!h_data2) continue;

    
    //--- set y axis title 
    h_data1->GetYaxis()->SetTitle("Entries "+y_axis_title);
    

    //---- rebin hists
    bool rebin_conf=true;
    int rebin =1;
    int nbins=h_data1->GetNbinsX();

    int rebin_cat=0;
    if(h_data1->GetEntries() > 10000) rebin_cat=1;
    else     if(h_data1->GetEntries() > 1000) rebin_cat=2;
    else     if(h_data1->GetEntries() > 100) rebin_cat=3;
    else rebin_cat=4;

    int n(2);
    vector<int>  rebin_list;
    while(rebin_conf){                                                                                                                                                                                    
      if(!(nbins%n)) rebin_list.push_back(n);  
      if(rebin_list.size() >3) rebin_conf=false; 
      if(n >= nbins) rebin_conf=false;
      n++;
    }

    if(rebin_cat==1)rebin =1;
    if(rebin_cat==2&& rebin_list.size()>0)rebin = rebin_list[0];
    if(rebin_cat==3&& rebin_list.size()>1)rebin = rebin_list[1];
    if(rebin_cat==4&& rebin_list.size()>2)rebin = rebin_list[2];
    

    //--- rebin hists
    h_data1->Rebin(rebin);
    h_data2->Rebin(rebin);  


    //--- naming of hists in legend

    // ********* USER MOD *********                                                                             
    legendH->AddEntry(h_data1, "m_{N} = 200 GeV Official","l");
    legendH->AddEntry(h_data2, "m_{N} = 200 GeV Private","l");
    
    // ********* USER MOD *********	      


    //=== Draw canvas			
    
    unsigned int outputWidth = 1200;
    unsigned int outputHeight = 1200;
    
    TCanvas* c1 = new TCanvas(("Plot"+histlist[ihist]), "Plot"+histlist[ihist], outputWidth,outputHeight);
    
    c1->cd();
    gPad->SetLogz(1);

    
    //=== Draw hists on top pad
    h_data1->SetLineColor(kRed);
    h_data2->SetLineColor(kBlack);
    h_data1->SetMarkerStyle(22);
    h_data1->SetMarkerSize(1.);
    h_data1->SetLineWidth(2.);
    h_data2->SetLineWidth(2.);
    
    h_data1->GetXaxis()->SetLabelSize(0.); ///
    h_data1->GetXaxis()->SetTitle("");
    h_data1->SetFillColor(kRed);



    /// ============================= Sort out put on histogram
    std::ostringstream out_os;
    std::ostringstream out_ss;
    std::ostringstream out_r;
    std::ostringstream out_rE;
    out_os.precision(0);
    out_ss.precision(0);
    out_os << std::fixed << h_data1->GetEntries();
    out_ss << std::fixed << h_data2->GetEntries();



    // clone histograms so rebinning does not effect main plots
    TH1F* h1 = (TH1F*)h_data1->Clone(histlist[ihist]+"_1");
    TH1F* h2 = (TH1F*)h_data2->Clone(histlist[ihist]+"_2");

    
    /// ============================= SCALE ss hist
    h_data1->Scale(h_data2->Integral()/h_data1->Integral());

    
    /// ==================== get Y axis range by checknihg histogram max Y values
    float maxratio=1.3;
    if(rebin_cat==2) maxratio=1.3;;
    if(rebin_cat==3) maxratio=1.5;;
    if(rebin_cat==4) maxratio=2.;;
    float ymax=h_data1->GetMaximum()*maxratio*ratio_y_aspect;
    h_data1->GetYaxis()->SetRangeUser(0., ymax);
    if(h_data1->GetMaximum() < h_data2->GetMaximum()){
      ymax=h_data2->GetMaximum()*maxratio*ratio_y_aspect;
      h_data1->GetYaxis()->SetRangeUser(0., ymax);
    }
    

    ///================ DRAWING

    h_data1->Draw("hist");
    h_data2->Scale(1.);
    h_data2->SetMarkerStyle(21);
    h_data2->SetMarkerSize(1.);
    h_data1->SetMarkerColor(kRed);

    h_data2->Draw("p same");

    /// ================== MAKE RATIO
    gPad->Update();

    // How large fraction that will be taken up by the data/MC ratio part
    double FIGURE2_RATIO = 0.3;
    double SUBFIGURE_MARGIN = 0.;
    c1->SetBottomMargin(FIGURE2_RATIO);
    TPad *p = new TPad( "p_test", "", 0, 0, 1, 1.0 - SUBFIGURE_MARGIN, 0, 0, 0);  // create new pad, fullsize to have equal font-sizes in both plots
    p->SetTopMargin(1-FIGURE2_RATIO);   // top-boundary (should be 1 - thePad->GetBottomMargin() )
    p->SetFillStyle(0);     // needs to be transparent
    p->Draw();
    p->cd();
    p->SetTicks(0,1);
    
    
    //=== clone hist of num
    TH1F* hdev = (TH1F*)h_data1->Clone("hdev");
    
    hdev->GetXaxis()->SetLabelSize(0.); ///
    hdev->GetXaxis()->SetTitle(x_axis_title);
    
    hdev->Divide(h_data2);
    
    //=== setup ratio hist
    hdev->GetYaxis()->SetLabelSize(0.03);
    hdev->GetYaxis()->SetTitleSize(0.03);
    hdev->GetXaxis()->SetLabelSize(0.03);
    hdev->GetXaxis()->SetTitleSize(0.03);
    

    // ********* USER MOD *********
    hdev->GetYaxis()->SetTitle( "N_{official}/ N_{private} ");
    hdev->GetYaxis()->SetRangeUser(0.,+4.);    
    hdev->GetYaxis()->SetNdivisions(5);
    // ********* USER MOD *********

    
    hdev->SetMarkerStyle(20);
    hdev->SetLineColor(kRed);
    hdev->SetFillColor(kRed);
    
    hdev->Draw("pE");
    
    
    
    //=== Draw line on bottom pad
    
    TLine *devz = new TLine(hdev->GetBinLowEdge(hdev->GetXaxis()->GetFirst()),1.0,hdev->GetBinLowEdge(hdev->GetXaxis()->GetLast()+1),1.0  );
    devz->SetLineWidth(1);
    devz->SetLineStyle(1);
    devz->Draw("SAME");


    
    CMS_lumiDef( c1, 4, 2 );
    
    legendH->SetFillColor(kWhite);
    legendH->SetTextSize(0.03);
    legendH->Draw();
    
    
    TLatex label;
    label.SetTextSize(0.025);
    label.SetTextColor(2);
    label.SetTextFont(42);
    label.SetNDC();
    label.SetTextColor(1);


    /// h1 is  ss l; h2 is os
    ///================ rebin to make 1 bin

    h1->Rebin(h1->GetNbinsX());
    h2->Rebin(h2->GetNbinsX());

    // call sumw2 to get correct error
    h1->Sumw2();
    h2->Sumw2();

    h2->Divide(h1);

    
    c1->Update();

    TString label_out=histlist[ihist];
    c1->SaveAs(path_subfix+"Plots/"+out_dir+"/"+label_out +".pdf");

    gPad->RedrawAxis();
  }

  return;
  
}


void CheckCompatability(TList* list1, TList* list2, TString filter){

  map<TString,TString> klist1;
  map<TString,TString> klist2;

  ofstream outfile;
  std::string t=currentDateTime();

  outfile.open("Type1_SignalComp_warnings"+t+".log");
  bool _warnings(false);
   

  if (!list1) { printf("<E> No keys found in file\n") ; exit(1) ; }
  TIter next(list1) ;
  TKey* key ;
  TObject* obj ;
  
  while ( (key = (TKey*)next()) ) {
    obj = key->ReadObj() ;
    TString hname = obj->GetName();		
    TString objname= obj->ClassName();
    if(hname.Contains(filter))    klist1[hname]=objname;
    else if (filter=="*")  klist1[hname]=objname;
  }
  
  if (!list2) { printf("<E> No keys found in file\n") ; exit(1) ; }
  TIter next2(list2) ;
  TKey* key2 ;
  TObject* obj2 ;

  while ( (key2 = (TKey*)next2()) ) {
    obj2 = key2->ReadObj() ;
    TString hname = obj2->GetName();		
    TString objname= obj2->ClassName();
    if(hname.Contains(filter))    klist2[hname]=objname;
    else if (filter=="*")  klist2[hname]=objname;

  }

  
  
  for( map<TString,TString>::iterator it = klist1.begin(); it!= klist1.end(); it++){

    map<TString,TString>::iterator it2 = klist2.find(it->first);
    if(it2== klist2.end()) {
      outfile << "Type1_SignalComp::WARNING  Hist " << it->first << " class " << it->second << " is in file 1 and not in file 2" << endl;
      _warnings=true;
    }
  }
  
  for( map<TString,TString>::iterator it = klist2.begin(); it!= klist2.end(); it++){
    
    map<TString,TString>::iterator it2 = klist1.find(it->first);
    if(it2== klist1.end()) {
      outfile	<< "Type1_SignalComp::WARNING  Hist " << it->first << " class " << it->second << " is in file 2 and not in file 1" << endl;
      _warnings=true;

    }
  }
  outfile.close();
  if(_warnings) {
    cout << " " << endl;
    cout << "Type1_SignalComp::WARNING while runing script. Check Type1_SignalComp_warnings"+t+".log" << endl;
    cout << " " << endl;
  }
  else system(("rm Type1_SignalComp_warnings"+t+".log").c_str());
}


vector<TString> GetHistList(TList* list, TList* list2, TString obj_list, TString filter_string, bool check_subdir , TString _dirname){

  if (!list) { printf("<E> No keys found in file\n") ; exit(1) ; }
  TIter next(list) ;
  TKey* key ;
  TObject* obj ;


  
  vector<TString> histlist;

  vector<TString> histlist2= GetHistListTMP(list2, obj_list, filter_string, check_subdir, _dirname);
  for(unsigned int k=0; k < histlist2.size(); k++){
    cout << histlist2[k] << endl;
  }
  
  while ( (key = (TKey*)next()) ) {
    obj = key->ReadObj() ;
    if ( (strcmp(obj->IsA()->GetName(),"TProfile")!=0)
         && (obj_list!=""&& !obj->InheritsFrom(obj_list))
	 && (!obj->InheritsFrom("TH2"))
         && (!obj->InheritsFrom("TH1")) )
      {
          printf("<W> Object %s is not 1D or 2D histogram : "
                 "will not be converted\n",obj->GetName()) ;

      }
    else if(obj->InheritsFrom("TDirectoryFile")){
      if(check_subdir) {
	
	cout << "Type1_SignalComp::SUBDIRECTORY " << obj->GetName() << " " << obj->ClassName() << endl;
	
	TDirectory* dir = (TDirectoryFile*)obj;

	vector<TString> sublist = GetHistListTMP(dir->GetListOfKeys(), obj_list, filter_string, check_subdir , _dirname+ TString ( obj->GetName())+"/");
	
	for(unsigned int i=0; i < sublist.size(); i++){
	  int len= TString(sublist[i]).Length () ;
	  int len_dir= _dirname.Length () ;
	  TString suff_name="";
	  for(unsigned int j=0; j < 30- len; j++) suff_name+=" ";
	  TString suff_dir="";
	  for(unsigned int j=0; j < 15- len_dir; j++) suff_dir+=" ";
	  
	  bool hist_ok=false;
	  for(unsigned int k=0; k < histlist2.size(); k++){
	    if(histlist2[k] == sublist[i]) hist_ok=true;
	  }
	  if (hist_ok) {
	    histlist.push_back(sublist[i]);

	    if(!IsPathExist(string(path_subfix+"Plots/"+out_dir+"/")+ obj->GetName()))  gSystem->mkdir(path_subfix+"Plots/"+out_dir+"/"+obj->GetName());


	    TString fix_sublist=sublist[i].ReplaceAll(TString(obj->GetName())+"/","");
	    TString class_name="";
	    if(1){
	      TIter nextD(dir->GetListOfKeys()) ;
	      TKey* keyD ;
	      TObject* objD ;
	      while ( (keyD = (TKey*)nextD()) ) {
		objD = keyD->ReadObj() ;
		if(TString(objD->GetName()).Contains(fix_sublist)) class_name=objD->ClassName();
	      }
	    }
	    
	    if(_dirname=="")cout << "Type1_SignalComp::INFO Dir = " << obj->GetName()  << "          Hist =  " <<  fix_sublist + suff_name<< "          class = " << class_name << endl;
	    else cout << "Type1_SignalComp::INFO Dir = " << _dirname << suff_dir +"  Hist =  " <<  sublist[i] + suff_name<< "  class = " << obj->ClassName() << endl;
	    
	  }
	}
      }
    }// end TDirectoryFile
    else{

      int len= TString(obj->GetName()).Length () ;
      int len_dir= _dirname.Length () ;
      TString suff_name="";
      for(unsigned int j=0; j < 30- len; j++) suff_name+=" ";       
      TString suff_dir="";
      for(unsigned int j=0; j < 15- len_dir; j++) suff_dir+=" ";

      TString hname = obj->GetName();
      bool hist_ok=false;
      for(unsigned int k=0; k < histlist2.size(); k++){
	if(histlist2[k] == hname) hist_ok=true;
      }
      if(hist_ok){
	if(_dirname=="")cout << "Type1_SignalComp::INFO Dir = --               Hist =  " <<  obj->GetName() + suff_name<< "  class = " << obj->ClassName() << endl;
	else cout << "Type1_SignalComp::INFO Dir = " << _dirname << suff_dir +"  Hist =  " <<  obj->GetName() + suff_name<< "  class = " << obj->ClassName() << endl;
	
	if(filter_string=="*") histlist.push_back(hname);
	else if(hname.Contains(filter_string))	histlist.push_back(hname);
      }
    }
    
  }

  return histlist;
}

vector<TString> GetHistListTMP(TList* list,TString obj_list, TString filter_string, bool check_subdir , TString _dirname){

  if (!list) { printf("<E> No keys found in file\n") ; exit(1) ; }
  TIter next(list) ;
  TKey* key ;
  TObject* obj ;



  vector<TString> histlist;

  while ( (key = (TKey*)next()) ) {
    obj = key->ReadObj() ;

    if ( (strcmp(obj->IsA()->GetName(),"TProfile")!=0)
         && (obj_list!=""&& !obj->InheritsFrom(obj_list))
         && (!obj->InheritsFrom("TH2"))
         && (!obj->InheritsFrom("TH1")) )
      {
          printf("<W> Object %s is not 1D or 2D histogram : "
                 "will not be converted\n",obj->GetName()) ;

      }
    else if(obj->InheritsFrom("TDirectoryFile")){
      if(check_subdir) {


        TDirectory* dir = (TDirectoryFile*)obj;
	vector<TString> sublist = GetHistListTMP(dir->GetListOfKeys(),obj_list, filter_string, check_subdir , _dirname+ TString ( obj->GetName())+"/");
	for(unsigned int i=0; i < sublist.size(); i++){
	  histlist.push_back(sublist[i]);
	}
      }
    }// end TDirectoryFile                                                                                                                                                             
    else{

      TString hname = obj->GetName();      
      if(filter_string=="*") histlist.push_back(hname);
      else if(hname.Contains(filter_string))    histlist.push_back(hname);
    }
    
  }

  return histlist;
}


const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}


bool IsPathExist(const std::string &s)
{

  struct stat buffer;
  return (stat (s.c_str(), &buffer) == 0);
}
void CMS_lumiDef( TPad* pad, int iPeriod, int iPosX )
{
  bool outOfFrame    = false;
  if( iPosX/10==0 )
    {
      outOfFrame = true;
    }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  int align_ = 10*alignX_ + alignY_;

  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  float e = 0.025;

  pad->cd();

  TString lumiText;
  if( iPeriod==1 )
    {
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
    }
  else if ( iPeriod==2 )
    {
      lumiText += " (8 TeV)";
    }
  else if( iPeriod==3 )
    {
      lumiText = lumi_8TeV;
      lumiText += " (8 TeV)";
      lumiText += " + ";
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
    }
  else if ( iPeriod==4 )
    {
      lumiText += lumi_13TeV;
      lumiText += " (13 TeV)";
    }
  else if ( iPeriod==7 )
    {
      if( outOfFrame ) lumiText += "#scale[0.85]{";
      lumiText += lumi_13TeV;
      lumiText += " (13 TeV)";
      lumiText += " + ";
      lumiText += lumi_8TeV;
      lumiText += " (8 TeV)";
      lumiText += " + ";
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
      if( outOfFrame) lumiText += "}";
    }
  else if ( iPeriod==12 )
    {
      lumiText += "8 TeV";
    }

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);

  float extraTextSize = extraOverCmsTextSize*cmsTextSize;

  latex.SetTextFont(42);
  latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize*t);
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  if( outOfFrame )
    {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11);
      latex.SetTextSize(cmsTextSize*t);
      latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
    }

  pad->cd();

  float posX_;
  if( iPosX%10<=1 )
    {
      posX_ =   l + relPosX*(1-l-r);
    }
  else if( iPosX%10==2 )
    {
      posX_ =  l + 0.5*(1-l-r);
    }
  else if( iPosX%10==3 )
    {
      posX_ =  1-r - relPosX*(1-l-r);
    }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame )
    {
      if( drawLogo )
        {
          posX_ =   l + 0.045*(1-l-r)*W/H;
          posY_ = 1-t - 0.045*(1-t-b);
          float xl_0 = posX_;
          float yl_0 = posY_ - 0.15;
          float xl_1 = posX_ + 0.15*H/W;
          float yl_1 = posY_;
          TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
          TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
          pad_logo->Draw();
          pad_logo->cd();
          CMS_logo->Draw("X");
          pad_logo->Modified();
          pad->cd();
        }
      else
        {
          latex.SetTextFont(cmsTextFont);
          latex.SetTextSize(cmsTextSize*t);
          latex.SetTextAlign(align_);
          latex.DrawLatex(posX_, posY_, cmsText);
          if( writeExtraText )
            {
              latex.SetTextFont(extraTextFont);
              latex.SetTextAlign(align_);
              latex.SetTextSize(extraTextSize*t);
              latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
            }
        }
    }
  else if( writeExtraText )
    {
      if( iPosX==0)
        {
          posX_ =   l +  relPosX*(1-l-r);
          posY_ =   1-t+lumiTextOffset*t;
        }
      latex.SetTextFont(extraTextFont);
      latex.SetTextSize(extraTextSize*t);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, extraText);
    }
  return;
}

