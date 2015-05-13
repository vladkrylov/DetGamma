#ifndef D0STYLE_h 
#define D0STYLE_h

#include "TStyle.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TF1.h"
#include "TGraph.h"


/// ... Global attributes go here ... 
static void global_style() {

   gStyle->SetCanvasColor(0);
   gStyle->SetCanvasBorderMode(0);
   gStyle->SetPadColor(0);
   gStyle->SetPadBorderMode(0);
   gStyle->SetFrameBorderMode(0);

   gStyle->SetTitleColor(0);   // 0
   gStyle->SetTitleBorderSize(1);
   gStyle->SetTitleX(0.149);
   gStyle->SetTitleY(1.00);
   gStyle->SetTitleW(0.5);
   gStyle->SetTitleH(0.06);

   gStyle->SetLabelOffset(1e-04);
   gStyle->SetLabelSize(0.2);

   gStyle->SetStatColor(0);
   gStyle->SetStatBorderSize(2);
   gStyle->SetStatX(0.948);
   gStyle->SetStatY(0.952);
   gStyle->SetStatW(0.28);
   gStyle->SetStatH(0.2);

   gStyle->SetOptStat(111110) ;

   gStyle->SetErrorX(0.0);   // Horizontal error bar size
   //   gStyle->SetPaperSize(10.,12.);   // Printout size

   gStyle->SetPalette(1) ;
}

/// ... Canvas attributes ...
static void canvas_style(TCanvas *c,
                  float left_margin=0.15,
		  float right_margin=0.05,
		  float top_margin=0.05,
		  float bottom_margin=0.15,
		  int canvas_color=0,
                  int frame_color=0) {

  c->SetLeftMargin(left_margin);
  c->SetRightMargin(right_margin);
  c->SetTopMargin(top_margin);
  c->SetBottomMargin(bottom_margin);
  c->SetFillColor(canvas_color);
  c->SetFrameFillColor(frame_color);
  gStyle->SetTitleFillColor(frame_color);

  c->SetBorderMode(0);
  c->SetBorderSize(1);
  c->SetFrameBorderMode(0);
  gPad->SetRightMargin(0.13) ;
}

/// ... 1D histogram attributes; (2D to come) ...
static void h1_style(TH1 *h,
	    int line_width=3,
	    int line_color=1,
	    int line_style=1, 
	    int fill_style=1001,
	    int fill_color=18,
	    float y_min=-1111.,
	    float y_max=-1111.,
	    int ndivx=510,
	    int ndivy=510,
	    int marker_style=20,
	    int marker_color=1,
	    float marker_size=1.2,
	    int optstat=1) {

  h->SetLineWidth(line_width);
  h->SetLineColor(line_color);
  h->SetLineStyle(line_style);
  h->SetFillColor(fill_color);
  h->SetFillStyle(fill_style);
   h->SetMaximum(y_max);
   h->SetMinimum(y_min);
   h->GetXaxis()->SetNdivisions(ndivx);
   h->GetYaxis()->SetNdivisions(ndivy);
   h->GetYaxis()->SetTitleOffset(1.2);
   h->GetXaxis()->SetTitleColor(1);
   h->GetYaxis()->SetTitleColor(1);

   h->SetMarkerStyle(marker_style);
   h->SetMarkerColor(marker_color);
   h->SetMarkerSize(marker_size);
   h->SetStats(optstat);

   h->SetLabelFont(62,"X");       // 42
   h->SetLabelFont(62,"Y");       // 42
   h->SetLabelOffset(0.000,"X");  // D=0.005
   h->SetLabelOffset(0.005,"Y");  // D=0.005
   h->SetLabelSize(0.055,"X");
   h->SetLabelSize(0.055,"Y");
   h->SetTitleOffset(0.95,"X");
   h->SetTitleOffset(1.1,"Y");
   h->SetTitleSize(0.06,"X");
   h->SetTitleSize(0.06,"Y");
   //   h->SetTitle(0);
}

/// ... 2D histogram attributes; 
static void h2_style(TH1 *h,
	    int line_width=3,
	    int line_color=1,
	    int line_style=1, 
	    int fill_style=1001,
	    int fill_color=50,
	    float y_min=-1111.,
	    float y_max=-1111.,
	    int ndivx=510,
	    int ndivy=510,
	    int marker_style=20,
	    int marker_color=1,
	    float marker_size=1.2,
	    int optstat=0) {

  h->SetLineWidth(line_width);
  h->SetLineColor(line_color);
  h->SetLineStyle(line_style);
  h->SetFillColor(fill_color);
  h->SetFillStyle(fill_style);
   h->SetMaximum(y_max);
   h->SetMinimum(y_min);
   h->GetXaxis()->SetNdivisions(ndivx);
   h->GetYaxis()->SetNdivisions(ndivy);
   h->GetXaxis()->SetTitleColor(1);
   h->GetYaxis()->SetTitleColor(1);

   h->SetMarkerStyle(marker_style);
   h->SetMarkerColor(marker_color);
   h->SetMarkerSize(marker_size);
   h->SetStats(optstat);

   h->SetLabelFont(62,"X");       // 42
   h->SetLabelFont(62,"Y");       // 42
   h->SetLabelFont(62,"Z");       // 42
   h->SetLabelOffset(0.000,"X");  // D=0.005
   h->SetLabelOffset(0.005,"Y");  // D=0.005
   h->SetLabelOffset(0.0,"Z");  // D=0.005
   h->SetLabelSize(0.055,"X");
   h->SetLabelSize(0.055,"Y");
   h->SetLabelSize(0.055,"Z");
   h->SetTitleOffset(0.8,"X");
   h->SetTitleOffset(0.9,"Y");
   h->SetTitleOffset(0.0,"Z");
   h->SetTitleSize(0.06,"X");
   h->SetTitleSize(0.06,"Y");
   h->SetTitleSize(0.04,"Z");
   h->SetTitle(0);

}

static void func_style(TF1 *func) {

  func->SetLineWidth(3);
  func->SetLineColor(1);
  func->SetLineStyle(1);
  func->SetFillStyle(1);

  func->GetXaxis()->SetNdivisions(510);
  func->GetYaxis()->SetNdivisions(510);
  func->GetYaxis()->SetTitleOffset(1.2);
  func->GetXaxis()->SetTitleColor(1);
  func->GetYaxis()->SetTitleColor(1);
  func->GetXaxis()->SetLabelFont(62);     
  func->GetYaxis()->SetLabelFont(62);     
  func->GetXaxis()->SetLabelOffset(0.000);
  func->GetYaxis()->SetLabelOffset(0.005);
  func->GetXaxis()->SetLabelSize(0.055);
  func->GetYaxis()->SetLabelSize(0.055);
  func->GetXaxis()->SetTitleOffset(0.8);
  func->GetYaxis()->SetTitleOffset(0.9);
  func->GetXaxis()->SetTitleSize(0.06);
  func->GetYaxis()->SetTitleSize(0.06);


}


static void graph_style(TGraph *h,
			int line_width=3,
			int line_color=1,
			int line_style=1, 
			int fill_style=1001,
			int fill_color=18,
			float y_min=-1111.,
			float y_max=-1111.,
			int ndivx=510,
			int ndivy=510,
			int marker_style=20,
			int marker_color=1,
			int optstat=1) {
  
  h->SetLineWidth(line_width);
  h->SetLineColor(line_color);
  h->SetLineStyle(line_style);
  h->SetFillColor(fill_color);
  h->SetFillStyle(fill_style);
  h->SetMaximum(y_max);
   h->SetMinimum(y_min);
   h->GetXaxis()->SetNdivisions(ndivx);
   h->GetYaxis()->SetNdivisions(ndivy);
   h->GetYaxis()->SetTitleOffset(1.2);
   h->GetXaxis()->SetTitleColor(1);
   h->GetYaxis()->SetTitleColor(1);

   h->SetMarkerStyle(marker_style);
   h->SetMarkerColor(marker_color);
   h->SetMarkerSize(1.2);

   h->GetXaxis()->SetLabelFont(62);       // 42
   h->GetYaxis()->SetLabelFont(62);       // 42
   h->GetXaxis()->SetLabelOffset(0.000);  // D=0.005
   h->GetYaxis()->SetLabelOffset(0.005);  // D=0.005
   h->GetXaxis()->SetLabelSize(0.055);
   h->GetYaxis()->SetLabelSize(0.055);
   h->GetXaxis()->SetTitleOffset(0.95);
   h->GetYaxis()->SetTitleOffset(1.1);
   h->GetXaxis()->SetTitleSize(0.06);
   h->GetYaxis()->SetTitleSize(0.06);
   //   h->SetTitle(0);
}
#endif
