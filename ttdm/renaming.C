#include "ttdm.h"

const int nbranch = 8; 

TString branchID[nbranch] = { "channel", "darkpt", "dphillmet", "eventW", "eventW_Toppt", "eventW_genmatched", "mt2ll", 
				"metPfType1ElecEnDn" };//, "metPfType1ElecEnUp", "metPfType1ElecEnDn", "metPfType1MuonEnUp", "metPfType1MuonEnDn" }

void renaming2( TString process );

void renaming(){

	Assign();

	for( int i = 0; i < nscalar; i++ ){

		renaming2( scalarID[i] );

		renaming2( pseudoID[i] );

	}

	
	for( int i = 0; i < nprocess; i++ ){

		if ( i == Wg || i == Zg || i == data || i == fakes || i == ttDM ) continue; 

		renaming2( processID[i] ); 

	}
}

void renaming2( TString process ){ 

	cout << "===============" << endl; 
	cout << " "<<process<<" " << endl; 
	cout << "===============" << endl; 

	float      old_v[nbranch]; 
	float      new_v[nbranch]; 
	TBranch*   new_b[nbranch];


	TFile* old_f = new TFile("/eos/user/j/jgarciaf/minitrees/julia_LES/TTDM/" + process + ".root" , "read"  );

	TTree* old_t = (TTree*) old_f -> Get( "latino" );

	old_t -> SetBranchStatus( "*", 0 );


	for( int i = 0; i < nbranch; i++ ){

		old_t -> SetBranchStatus( branchID[i], 1 ); old_t -> SetBranchAddress( branchID[i], &old_v[i] );

	}


	TFile* new_f = new TFile("/eos/user/j/jgarciaf/minitrees/julia_Eledo/TTDM/" + process + ".root", "new"); 

	TTree* new_t = new TTree( "latino", "branch renamed" );

	for( int i = 0; i < nbranch; i++ ){

		if ( i <  nbranch - 1 ) new_b[i] = new_t -> Branch( branchID[i], &new_v[i], branchID[i] + "/F" ); 

		if ( i == nbranch - 1 ) new_b[i] = new_t -> Branch( "metPfType1", &new_v[i], "metPfType1/F" ); 
	}

		
	int nentries = old_t -> GetEntries();

	for( int ievt = 0; ievt < nentries; ievt++ ){

		if( ievt%10000 == 0 ) cout << "  >> " << ievt << " event" << endl;

		old_t -> GetEntry( ievt ); 

		for( int i = 0; i < nbranch; i++ ){

			new_v[i] = old_v[i];

		}

		new_t -> Fill(); 

	}

	new_f -> Write("");

	old_f -> Close();		
	new_f -> Close(); 

}
