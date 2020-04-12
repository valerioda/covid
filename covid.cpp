#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <stdlib.h>

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TCut.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TMath.h"
#include "TF1.h"
#include "TStyle.h"
#include "TChain.h"
#include "TSpectrum.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TSpectrum.h"
#include "TPaveLabel.h"
#include "TLine.h"
#include "TLegend.h"
#include "TLatex.h"

using namespace std;

int main(int argc, char *argv[]){
  TApplication *myapp = new TApplication("myapp",0,0);
  TString name = "covid.txt";
  ifstream file;
  file.open(name);
  double x1,x2,x3,x4,x5,x6;
  double contagiati[1000];
  double contagiati_err[1000];
  double date[1000];
  double time[1000];
  double guariti[1000];
  double positivi[1000];
  double morti[1000];
  int sec = 60.*60.*24.;
  int j = 0;
  while (1){
    file >> x1 >> x2 >> x3 >> x4;
    if (!file.good()) break;
    date[j] = x1 * sec;
    contagiati[j] = x2;
    contagiati_err[j] = sqrt(x2);
    guariti[j] = x4;
    morti[j] = x3;
    positivi[j] = x2 - x3 - x4;
    j++;
  }
  file.close();
  cout << "days " << j << endl;
  //double date[k] = {1*sec, 8*sec, 14*sec, 20*sec, 21*sec};
  TDatime dh("2020-02-22 12:00:00");//starting day
  int X0 = dh.Convert();
  gStyle->SetTimeOffset(X0);
  
  TGraph *g_1 = new TGraph(j, date, contagiati);
  g_1->GetXaxis()->SetTimeFormat("%d/%m");
  g_1->GetXaxis()->SetTimeDisplay(1);
  g_1->SetTitle("COVID-19 in Italia");
  g_1->GetXaxis()->SetTitle("data");
  //g_1->GetXaxis()->SetNdivisions(30);
  g_1->GetYaxis()->SetTitle("numero casi totali");
  g_1->SetMarkerColor(4);
  g_1->SetLineColor(4);
  g_1->SetMarkerStyle(21);

  TCanvas *c = new TCanvas("c","COVID-19",1400,600);
  c->SetGridy();
  //c->Divide(1,2);
  c->cd();
  g_1->Draw("APL");
  TF1 *expo = new TF1("expo","exp([0]+x*[1])",date[0],date[j-1]);
  g_1->Fit("expo","","",date[0],date[27]);
  
  double raddoppio = log(2)*1/(expo->GetParameter(1))/60/60/24;
  double raddoppio_err = expo->GetParError(1)/expo->GetParameter(1)*raddoppio;

  TLegend *leg = new TLegend(0.2,0.7,0.55,0.85);
  leg->SetBorderSize(0);
  leg->AddEntry(g_1, "contagiati","P");
  leg->AddEntry(expo, Form("Tempo raddoppio: %4.1f #pm %4.1f giorni",raddoppio,raddoppio_err),"L");
  //leg->AddEntry(g_3, "guariti","P");
  //leg->AddEntry(g_2, "morti","P");
  //leg->Draw();
  c->Update();
  TLine *zone = new TLine(6.3*sec,c->GetUymin(),6.3*sec,c->GetUymax());
  zone->SetLineStyle(2);
  zone->SetLineWidth(1);
  //zone->SetLineColor(kGray);
  TLine *resto = new TLine(15.3*sec,c->GetUymin(),15.3*sec,c->GetUymax());
  resto->SetLineStyle(2);
  TLine *chiudi = new TLine(28.3*sec,c->GetUymin(),28.3*sec,c->GetUymax());
  chiudi->SetLineStyle(2);
  zone->Draw();
  resto->Draw();
  chiudi->Draw();
  leg->Draw();
  c->Update();
  TLatex* lat = new TLatex();
  lat->SetTextFont(43);
  lat->SetTextSize(20);
  lat->SetTextAlign(22);
  lat->SetTextColor(kBlack);
  lat->SetTextAlign(12);//centered
  lat->SetTextAngle(90);
  lat->DrawLatex(6*sec,10000,"decreto ""zone rosse""");
  lat->DrawLatex(15*sec,10000,"decreto #iorestoacasa");
  lat->DrawLatex(28*sec,60000,"decreto ""chiudi Italia""");
  //c->RedrawAxis();
  c->Print("covid-casi-totali.pdf");

  /////////////////////////////////////////////////////////////////
  
  TCanvas *c2 = new TCanvas("c2","COVID-19",1400,600);
  c2->SetGridy();
  c2->cd();
  gStyle->SetTimeOffset(X0);
  //gPad->SetLogy();
  
  TGraph *g_2 = new TGraph(j, date, morti);
  g_2->SetTitle("COVID-19 in Italia");
  g_2->SetMarkerColor(1);
  g_2->SetLineColor(1);
  g_2->SetMarkerStyle(21);
  
  TGraph *g_3 = new TGraph(j, date, guariti);
  g_3->SetMarkerColor(3);
  g_3->SetLineColor(3);
  g_3->SetMarkerStyle(21);

  TGraph *g_4 = new TGraph(j, date, positivi);
  g_4->SetTitle("COVID-19 in Italia");
  g_4->GetXaxis()->SetTimeFormat("%d/%m");
  g_4->GetXaxis()->SetTimeDisplay(1);
  //g_4->GetXaxis()->SetNdivisions(30);
  g_4->GetXaxis()->SetTitle("data");
  g_4->GetYaxis()->SetTitle("numero casi");
  g_4->SetMarkerColor(2);
  g_4->SetLineColor(2);
  g_4->SetMarkerStyle(21);
  
  g_4->Draw("APL");
  g_2->Draw("PL");
  g_3->Draw("PL");
  c2->Update();
  TLegend *leg2 = new TLegend(0.2,0.7,0.45,0.85);
  leg2->SetBorderSize(0);
  leg2->AddEntry(g_4, "positivi","P");
  leg2->AddEntry(g_3, "guariti","P");
  leg2->AddEntry(g_2, "deceduti","P");
  leg2->Draw();
  c2->Update();
  c2->Print("covid-evoluzione-complessiva.pdf");

  ////////////////////////////////////////////////////////////////

  TCanvas *c1 = new TCanvas("c1","COVID-19",1400,600);
  c1->cd();
  c1->SetGridy();
  gStyle->SetOptStat(0);
  gStyle->SetTimeOffset(X0);
  
  TH1D *htot = new TH1D("htot","", j-1, 0, (j-1)*sec );
  htot->SetFillColor(4);
  htot->SetBarWidth(0.25);
  htot->SetBarOffset(0.);
  htot->SetTitle("COVID-19 in Italia");
  htot->GetXaxis()->SetTitle("data");
  htot->GetYaxis()->SetTitle("nuovi casi giornalieri");
  htot->GetXaxis()->SetTimeFormat("%d %b");
  htot->GetXaxis()->SetTimeDisplay(1);
  htot->SetLineWidth(0);
  TH1D *hpos = new TH1D("hpos","", j-1, 0, (j-1)*sec );
  hpos->SetFillColor(2);
  hpos->SetBarWidth(0.25);
  hpos->SetBarOffset(0.25);
  hpos->SetLineWidth(0);
  TH1D *hmor = new TH1D("hmor","", j-1, 0, (j-1)*sec );
  hmor->SetFillColor(1);
  hmor->SetBarWidth(0.25);
  hmor->SetBarOffset(0.75);
  hmor->SetLineWidth(0);;
  TH1D *hgua = new TH1D("hgua","", j-1, 0, (j-1)*sec );
  hgua->SetFillColor(3);
  hgua->SetBarWidth(0.25);
  hgua->SetBarOffset(0.5);
  hgua->SetLineWidth(0);
  int incr[1000] = {0};
  for (int i = 1; i < j; i++){
    htot->SetBinContent(i,contagiati[i]-contagiati[i-1]);
    if( i > 1 ) incr[i] = (int)(htot->GetBinContent(i)-htot->GetBinContent(i-1))/htot->GetBinContent(i-1)*100.;
    //cout << incr[i] << endl;
    hpos->SetBinContent(i,positivi[i]-positivi[i-1]);
    hgua->SetBinContent(i,guariti[i]-guariti[i-1]);
    hmor->SetBinContent(i,morti[i]-morti[i-1]);
  }

  htot->Draw("B");
  hpos->Draw("Bsame");
  hgua->Draw("Bsame");
  hmor->Draw("Bsame");
  
  ////////////////////////////////////////////////////
  
  int width = 7;
  double pos_avg[1000];
  double mor_avg[1000];
  double gua_avg[1000];
  double tot_avg[1000];
  for (int i = 0; i < (j-1); i++){
    double pos_temp = hpos->GetBinContent(i);
    double mor_temp = hmor->GetBinContent(i);
    double gua_temp = hgua->GetBinContent(i);
    double tot_temp = htot->GetBinContent(i);
    int contavg = 1;
    for (int k = 1; k < width; k++){
      if ( (i+k) < (j-1) ){
	pos_temp += hpos->GetBinContent(i+k);
	mor_temp += hmor->GetBinContent(i+k);
	gua_temp += hgua->GetBinContent(i+k);
	tot_temp += htot->GetBinContent(i+k);
	contavg++;
      }
    }
    pos_temp /= contavg;
    mor_temp /= contavg;
    gua_temp /= contavg;
    tot_temp /= contavg;
    pos_avg[i] = pos_temp;
    gua_avg[i] = gua_temp;
    mor_avg[i] = mor_temp;
    tot_avg[i] = tot_temp;
  }
  TGraph *ga1 = new TGraph(j-1, date, pos_avg);
  ga1->SetMarkerColor(kRed);
  ga1->SetLineColor(kRed);
  ga1->SetLineWidth(2);
  TGraph *ga2 = new TGraph(j-1, date, gua_avg);
  ga2->SetMarkerColor(kGreen+1);
  ga2->SetLineColor(kGreen+1);
  ga2->SetLineWidth(2);
  TGraph *ga3 = new TGraph(j-1, date, mor_avg);
  ga3->SetMarkerColor(kBlack);
  ga3->SetLineColor(kBlack);
  ga3->SetLineWidth(2);
  TGraph *ga4 = new TGraph(j-1, date, tot_avg);
  ga4->SetMarkerColor(kBlue);
  ga4->SetLineColor(kBlue);
  ga4->SetLineWidth(2);
  
  ga1->Draw("Lsame");
  ga2->Draw("Lsame");
  ga3->Draw("Lsame");
  ga4->Draw("Lsame");
  
  TLegend *leg1 = new TLegend(0.2,0.7,0.45,0.88);
  leg1->SetBorderSize(0);
  leg1->AddEntry(htot, "contagiati totali","F");
  leg1->AddEntry(hpos, "attualmente positivi","F");
  leg1->AddEntry(hgua, "guariti","F");
  leg1->AddEntry(hmor, "deceduti","F");
  leg1->Draw();
  c1->Update();
  c1->Print("covid-evoluzione-giornaliera.pdf");
  
  myapp->Run();
  return 0;
}
  
