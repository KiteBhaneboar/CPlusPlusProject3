#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cmath>


using namespace std;


struct BillingData { //create struct for reading and organizing file data
	int StartTime;
	int Minutes;
	int BillableMinutes;
	float Cost;
	bool IsLongDistance;
	bool IsNight;
};

BillingData call = { -1, -1, false, false }; //initializing to default values (-1 time and -1 call minutes will never happen, not a great one for bool since only true/false)


ifstream inFile;
string line;
char* lineToCharPtr; //defining char pointer to read data from the line
char* tokenPtr; //defining char pointer to read tokens from said line
int i, minutesUsed;
float totalCost;
const int NON_BILLABLE = 200;
const float BASE_COST = 39.99;

int main() {

	
	minutesUsed = 0; //initialize minutes counter
	//open file and read lines
	inFile.open("phoneminutes.txt");
	if (inFile.fail())
		cout << "No file has been found! Oops!" << endl;
	else {
		//print headers
		cout << "Cell Phone Charges - CASEY HILLSTROM" << endl << endl;
		cout << left << setw(6) << "Time" << setw(10) << "Duration" << setw(15) << "Type of Call" << setw(23) << "Minutes used so far" << setw(4) << "Cost" << endl;
		
		while (!inFile.eof()) {
			getline(inFile, line);
			i = 0; //set counter for reading line tokens

			lineToCharPtr = (char*)line.c_str(); //convert to char pointer because strtok function needs chr*
			
			tokenPtr = strtok(lineToCharPtr, " "); // read first token
			
			while (tokenPtr != NULL) { //while loop to get each token until we hit NULL (which means end of line), figured out how to put it in one line rather than do/while loop
				
				switch (i) {  //read and set tokens to variables
				case 0:
					call.StartTime = stoi(tokenPtr);
					break;
				case 1: call.Minutes = stoi(tokenPtr);
					break;
				case 2: call.IsLongDistance = strcmp(tokenPtr, "L") == 0; //string compare to figure out if string is "L" and if it is then that means long distance call
					break;

				}
				tokenPtr = strtok(NULL, " "); //keeps reading from where ever last token was found
				i++; //increments token counter
			}
			
			call.IsNight = call.StartTime >= 1800 || call.StartTime < 600; //sets when a call is night time call

			//calculations
			call.BillableMinutes = call.Minutes - (NON_BILLABLE - minutesUsed < 0 ? 0 : (NON_BILLABLE - minutesUsed > call.Minutes ? call.Minutes : NON_BILLABLE - minutesUsed)); //to figure out if minutes are billable or not, this was the hard math part
			call.Cost = call.IsNight ? 0 : (call.BillableMinutes * (call.IsLongDistance ? .69 : .29)); //calculate how much each minute over 200 is based on whether it's long distance or not
			
			if (!call.IsNight) {	//if call is not a night call, then accumulate call minutes in minutesUsed and then accumulate total cost if billable
				minutesUsed += call.Minutes;
				totalCost += call.Cost;
			}
			cout << fixed; //print values
			cout << right << setw(4) << setfill('0') << call.StartTime << "  " << left << setfill(' ') << setw(10) << call.Minutes << setw(15) << (call.IsLongDistance ? "L" : "A") << setw(23);
			if (!call.IsNight)
				cout << minutesUsed << setw(4) << setprecision(2) << call.Cost << endl;
			else cout << "Night call free" << endl;
			
			

		}
		cout << right << setw(64) << setfill(' ') << "Total Cost"  << " $ " << (BASE_COST + totalCost) << endl; //calculate and print total
		
		inFile.close();
	}

		


	return 0;
}