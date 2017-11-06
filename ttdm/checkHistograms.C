void loadHistograms( TString campaign, TString process, TString systematic);

void printRatio();

void checkHistograms(){


	printRatio(); 

	//loadHistograms( "LES"    , "TT", "MET");
	//loadHistograms( "summerEnd", "TT", "QCD_TT");

	//loadHistograms( "julia"    , "signal", "QCD_signal");
	//loadHistograms( "julia"    , "DY", "QCD_DY");
	//loadHistograms( "julia"    , "TTV", "QCD_TTV");
	//loadHistograms( "julia"    , "TT", "QCD_TT");

	//loadHistograms( "julia"    , "ST", "ST_xs");
	/*loadHistograms( "julia"    , "WW", "WW_xs");
	loadHistograms( "julia"    , "WZ", "WZ_xs");
	loadHistograms( "julia"    , "VZ", "VZ_xs");
	loadHistograms( "julia"    , "VVV","VVV_xs");*/

}


void loadHistograms( TString campaign, TString process, TString systematic){

	TString path = ( campaign == "julia" ) ? "170913_julia_w-syst_ISCOPY/" : "170904_justMET50SF_new-binning_with-syst/";

	path = "/afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/" + path + "ttDM0001pseudo00010/";

	TFile* f = new TFile( path + "simple-shapes-TH1-final.root", "read" );

	TH1F*  h_nm = (TH1F*) f -> Get( process );
	TH1F*  h_up = (TH1F*) f -> Get( process + "_" + systematic + "Up"   );
	TH1F*  h_do = (TH1F*) f -> Get( process + "_" + systematic + "Down" );


	cout << "---------------------------------------------------" << endl;
	cout << "campaign: " << campaign << "\t process: " << process << "\t systematic: " << systematic << endl;
	cout << "---------------------------------------------------" << endl;

	for( int i = 0; i <= h_nm -> GetNbinsX()+1; i++ ){ 

		float ratio_up = ( h_nm -> GetBinContent( i ) == 0 ) ? -1.00 : h_up -> GetBinContent( i )/h_nm -> GetBinContent( i ) ;
		float ratio_do = ( h_nm -> GetBinContent( i ) == 0 ) ? -1.00 : h_do -> GetBinContent( i )/h_nm -> GetBinContent( i ) ;

		cout << i << "\t" << ratio_up << "\t" << ratio_do << "\t" << endl;

	}

	cout << "---------------------------------------------------" << endl;

	cout << " " << endl;

	for( int i = 0; i <= h_nm -> GetNbinsX()+1; i++ ){ 

		if( h_nm -> GetBinContent( i ) == 0 && ( h_up -> GetBinContent( i ) != 0 || h_do -> GetBinContent( i ) != 0 ) ){

			cout << "WARNING: "<< h_up -> GetBinContent( i ) << "\t" << h_nm -> GetBinContent( i ) << endl; 

		}

	}


}

void printRatio(){

	TString path = "/afs/cern.ch/user/j/jgarciaf/www/txt-files/datacards/171106_LES/"; 
	
	TFile* f = new TFile( path + "04_TTTo2L2Nu.root", "read" );

	TString MET = "metPfType1"; 

	TH1F*  h0 = (TH1F*) f -> Get( MET              );
	TH1F*  h1 = (TH1F*) f -> Get( MET + "MuonEnUp" );
	TH1F*  h2 = (TH1F*) f -> Get( MET + "MuonEnDn" );
	TH1F*  h3 = (TH1F*) f -> Get( MET + "ElecEnUp" );
	TH1F*  h4 = (TH1F*) f -> Get( MET + "ElecEnDn" );

	int ngroup = 10; 

	h0 -> Rebin(ngroup);
	h1 -> Rebin(ngroup);
	h2 -> Rebin(ngroup);
	h3 -> Rebin(ngroup);
	h4 -> Rebin(ngroup);

	TH1F* hh1 = (TH1F*) h0 -> Clone("MuUp");
	TH1F* hh2 = (TH1F*) h0 -> Clone("MuDo");
	TH1F* hh3 = (TH1F*) h0 -> Clone("ElUp");
	TH1F* hh4 = (TH1F*) h0 -> Clone("Eldo");

	for( int i = 0; i <= h0->GetNbinsX()+1; i++ ){ 

		float b0 = h0 -> GetBinContent( i );	float e0 = h0 -> GetBinError( i ); 	
		float b1 = h1 -> GetBinContent( i );	float e1 = h1 -> GetBinError( i );	
		float b2 = h2 -> GetBinContent( i );	float e2 = h2 -> GetBinError( i );
		float b3 = h3 -> GetBinContent( i );	float e3 = h3 -> GetBinError( i );
		float b4 = h4 -> GetBinContent( i );	float e4 = h4 -> GetBinError( i );

		float r1 = ( b0 == 0 ) ? 1. : b1/b0;  float s1 = ( b0 == 0 || b1 == 0 ) ? 0.0 : r1*(e1/b1+e0/b0) ;
		float r2 = ( b0 == 0 ) ? 1. : b2/b0;  float s2 = ( b0 == 0 || b2 == 0 ) ? 0.0 : r2*(e2/b2+e0/b0) ;
		float r3 = ( b0 == 0 ) ? 1. : b3/b0;  float s3 = ( b0 == 0 || b3 == 0 ) ? 0.0 : r3*(e3/b3+e0/b0) ;
		float r4 = ( b0 == 0 ) ? 1. : b4/b0;  float s4 = ( b0 == 0 || b4 == 0 ) ? 0.0 : r4*(e4/b4+e0/b0) ;

		hh1 -> SetBinContent( i, r1 ); hh1 -> SetBinError( i, s1 ); 
		hh2 -> SetBinContent( i, r2 ); hh2 -> SetBinError( i, s2 ); 
		hh3 -> SetBinContent( i, r3 ); hh3 -> SetBinError( i, s3 ); 
		hh4 -> SetBinContent( i, r4 ); hh4 -> SetBinError( i, s4 ); 

	}
	
	TCanvas* c1 = new TCanvas("c1", "c1");

	hh1 -> SetMaximum(1.05); hh1 -> SetMinimum(0.95); 
	hh1 -> GetXaxis() -> SetRange( 2, 1000 ); 

	hh1 -> SetStats(false);

	hh1 -> SetTitle("MET_{Muon Scale Up} / MET_{nominal}");

	hh1 -> GetYaxis() -> SetTitle("ratio");
	hh1 -> GetXaxis() -> SetTitle("MET [GeV]");

	//hh1 -> SetLineColor(kBlue); 	//hh1 -> SetLineWidth(2.0); 
	//hh2 -> SetLineColor(kBlue);  	hh2 -> SetLineWidth(2.0);  

	hh1 -> Draw("");
	//hh2 -> Draw("same");
	//hh3 -> Draw("same");
	//hh4 -> Draw("same");

	TLine *line = new TLine(0, 1, 3000, 1);

	line->SetLineWidth(2.0);
	line->SetLineColor(kRed);
	line->SetLineStyle(1);
	line->Draw("same");
	
	c1 -> SetLogx();
}

