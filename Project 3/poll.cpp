//Electoral Engineering
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

/** FUNCTIONS DECLARATION */
//see below main() for description and implementation
int myStoi(string input);
bool isValidStateCode(string stateCode);
bool hasCorrectSyntax(string pollData);
int countVotes(string pollData, char party, int& voteCount);

/**MY MAIN*/
int main()
{
  string s = "TX38RCA55DMs6rnY29dUT10R";
 int x = 10;
  cout <<  hasCorrectSyntax(s) << endl;
  cout << countVotes(s,'R', x) << endl;
  cout << x;
}

/** FUNCTIONS IMPLEMENTATION */

//myStoid returns int version of string input (converts input to int)
int myStoi(string input){
    int output =0 ;
    for(char c : input){
       output = output*10 + (c-'0');
       }
    return output;
}

//  isValidUppercaseStateCode
// Return true if the argument is a two-letter state code, or
// false otherwise.
bool isValidStateCode(string stateCode)
{
  for(char& c : stateCode) c = toupper(c);
  const string codes =
    "AL.AK.AZ.AR.CA.CO.CT.DE.DC.FL.GA.HI.ID.IL.IN.IA.KS."
      "KY.LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC."
      "ND.OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
  return (stateCode.size() == 2  &&
      stateCode.find('.') == string::npos  &&  // no '.' in stateCode
      codes.find(stateCode) != string::npos);  // match found
}


//hasCorrectSyntax returns true if pollData is a syntactically proper chain of state forecasts (a poll data string)
bool hasCorrectSyntax(string pollData){
  //TX38RCA55DMs6rnY29dUT06L
  //TX38R CA55D MS6R NY29D UT06L (is wrong syntax but just to show what a state forecast looks like)
  string analyzing = "";
  int cut; //how much to cut when cutting digit(s) out
 
  while(pollData.length()>0){
        //state
     if(pollData.length() < 4) return false; //min length of forcast is SS0P, where SS is state, 0 is votes, P is party = 4
      analyzing = pollData.substr(0,2);
      pollData = pollData.substr(2);
      if(!isValidStateCode(analyzing) ) return false;
        //votes
      cut = 1;
      if(! isdigit(pollData[0]) ) return false;
      if(isdigit(pollData[1]) ) cut = 2;
      pollData = pollData.substr(cut);
       //party
      if( pollData.length() < 1 ) return false; //checks if there is a character for party
      if( !isalpha(pollData[0] ) ) return false;
      pollData = pollData.substr(1);
  }
 
  return true;
  }


/*  **  countVotes(string pollData, char party, int& voteCount) **
If the parameter pollData is not a poll data string, this function returns 1.
If pollData is a poll data string in which at least one state forecast predicts zero electoral votes for that state, this function returns 2.
If party is not a letter, this function returns 3.
(If more than one of these situations occur, returns 1 or 2 or 3)
If any of the preceding situations occur, voteCount is left unchanged.
If none of those situations occurs, then the function returns 0
after setting voteCount to the total number of electoral votes
that pollData predicts the candidate of the party indicated by party will get.
*/
int countVotes(string pollData, char party, int& voteCount){
  int reset = voteCount;
  voteCount = 0;
 
  if(!hasCorrectSyntax(pollData) ) {voteCount = reset; return 1;}
  if(!isalpha(party)) {voteCount = reset; return 3;}
 
 
  while(pollData.length()>0){
      
        //state
      pollData = pollData.substr(2);
      
        //votes
      int cut = 1;
      if(isdigit(pollData[1]) ) cut = 2;
      int votes = myStoi( pollData.substr(0,cut) );
      pollData = pollData.substr(cut);
      if(votes == 0) {
          voteCount = reset;
          return 2;
          }
       //party
     if(toupper(pollData[0]) == toupper(party) ) //to compare party ignoring case
         voteCount += votes; //convert votes from string to int and add to voteCount
      pollData = pollData.substr(1);
  }
 
 
  return 0;
 
}











