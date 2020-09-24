#include <iostream>
#include <cmath>
#include <vector>
#include "TMath.h"
#include "TVector3.h"
#include "TTree.h"
#include "/sphenix/u/skurdi/CMCalibration/PHG4TpcCentralMembrane.h"
R__LOAD_LIBRARY(build/.libs/libg4tpccentralmembrane)

//from phg4tpcsteppingaction.cc
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4Hitv1.h>
R__LOAD_LIBRARY(libphg4hit.so)


// all distances in mm, all angles in rad
// class that generates stripes and dummy hit coordinates
// stripes have width of one mm, length of one pad width, and are centered in middle of sector gaps

using namespace std;


int CheckStripeID() {
  StripesClass stripes;

  int stripeID, nbins, rsteps, phisteps; 
  double r, phi, x, y, xmod, ymod, phimod, rstepsize, phistepsize;

  nbins = 100;
  rsteps = 100;
  phisteps = 100;
  
  rstepsize = (stripes.end_CM - stripes.begin_CM)/rsteps;
  phistepsize = 2*TMath::Pi()/phisteps;
  
  //histogram from search
  TH2F *Pattern1 = new TH2F("Pattern1","Pattern1",nbins,-770.0,770.0,nbins,-770.0,770.0); // min n max just beyond extent of CM so it's easier to see
 
  for (r = stripes.begin_CM; r < stripes.end_CM; r = r + rstepsize){ // radii spanning full CM
    for (phi = 0.0; phi < 2*TMath::Pi(); phi = phi + phistepsize){ // angles spanning full CM
      
      x = r*cos(phi);
      y = r*sin(phi);

      result = stripes.getSearchResult(x, y);

      if(result == 1)
	Pattern1->Fill(x,y);
    }
  }

  vector<PHG4Hitv1*> Hits = stripes.PHG4Hits;

  const double mm = 1.0;
  const double cm = 10.0;
  
  vector<double> xhitS;
  vector<double> yhitS;
  
  //build tgraph from dummy hits
  for (int i = 0; i < Hits.size(); i++){
    xhitS.push_back(Hits[i]->get_x(0)*cm/mm); 
    yhitS.push_back(Hits[i]->get_y(0)*cm/mm);
    xhitS.push_back(Hits[i]->get_x(1)*cm/mm);
    yhitS.push_back(Hits[i]->get_y(1)*cm/mm);
    
  }

  int npts = 2*Hits.size();
  TGraph *gDummyHits = new TGraph(npts, &xhitS[0], &yhitS[0]);
  gDummyHits->SetMarkerColor(2);
  
  gStyle->SetOptStat(0);
  TCanvas *c=new TCanvas("a","CheckStripeID.cpp",500,500);
  Pattern1->Draw();
  gDummyHits->Draw("P");
  c->SaveAs("CheckStripeID.cpp"); 

  return 0;
}

  //loop thru hits again
  /*
  double xav, yav, xa, xb, ya, yb;
 for (int i = 0; i < Hits.size(); i++){
  
    //avg x0 n x1, y0 n y1 and use to draw stripeID
    xav = (Hits[i]->get_x(0)*cm/mm + Hits[i]->get_x(1)*cm/mm)/2;
    yav = (Hits[i]->get_y(0)*cm/mm + Hits[i]->get_y(1)*cm/mm)/2;
    //cout << "i: " << i << endl;
    xa = Hits[i]->get_x(0)*cm/mm ;
    xb= Hits[i]->get_x(1)*cm/mm ;
    ya= Hits[i]->get_y(0)*cm/mm ;
    yb = Hits[i]->get_y(1)*cm/mm ;
    //cout << "xav: " << xav << endl;
    //cout << "yav: " << yav << endl; 
    stripeID = stripes.getStripeID(xav, yav);
    TLatex *tex=new TLatex(xav,yav,"StripeID");
    tex->SetTextSize(0.005);
    tex->DrawLatex(xav,yav,Form("%d",stripeID));
    

    TLatex *tex=new TLatex(xav,yav,"Stripe");
    tex->SetTextSize(0.005);
    if(stripeID == -1){
      tex->DrawLatex(xav,yav,Form("%d",0));
    } else{
      tex->DrawLatex(xav,yav,Form("%d",1));
    }
}
  */
      
    //TLine *line=new TLine;
    //line->DrawLine(Hits[i]->get_x(0)*cm/mm ,Hits[i]->get_y(0)*cm/mm,Hits[i]->get_x(1)*cm/mm, Hits[i]->get_y(1)*cm/mm);
    //line->DrawLine(0, 0, 200, 200*((yb- ya)/(xb-xa)));
    
  
  
  /*for (r = stripes.begin_CM; r < stripes.end_CM; r = r + rstepsize){ // radii spanning full CM
    for (phi = 0.0; phi < 2.0*TMath::Pi()/9.0; phi = phi + phistepsize){ // angles spanning full CM
      
      x = r*cos(phi);
      y = r*sin(phi);
      
      
      
      //TLatex tex;
      
      //if(stripeID == 1)
      //Pattern1->Fill(x,y);
      cout << stripeID << endl;
    }
  }
  */
  
 
