//  @author Muhammad Hakim
//  @date CS 251, Spring 2023.
//  @brief Project 1 - Gerrymandering, University of Illinois Chicago
//

#include "ourvector.h"
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <math.h> 

using namespace std;

// Data Structure to hold data which is a vector of structs plus the struct has a vector of structs.

struct district { // Structure for each district containing number of blue voters and number of red voters

    double numberBlueVoters;
    double numberRedVoters;
};

struct state {// Structure for each state containing the total number of voters, the state name, and a vector of districts 

    int numberVoters = 0;
    string stateName;
    ourvector<district> districtVector;

};
void readFile(const string& fileName, const string& fileName2, ourvector<state> &dataVector,string& dataLoad) {
    // Reads data from two input files, fills data into the vector of states and updates the dataLoad flag

    ifstream inFile (fileName);
    ifstream inFile2 (fileName2);
    string parse,AL;
    if (dataLoad == "Yes") {
        cout << "Already read data in, exit and start over." << endl;
        return;
    }

    // Check if the files were successfully opened

    if (!inFile.is_open()) {
        cout << "Invalid first file, try again.'" << fileName << "'\n";
        return;
    }
    if (!inFile2.is_open()) {
        cout << "Invalid second file, try again." << fileName2 << "'\n";
        return;
    }
    // Read data from the first file and parse
    while (getline(inFile,parse)) {
        district tempDistrict; state tempState;
        int commaPos = parse.find(',');
        tempState.stateName = parse.substr(0, commaPos);
        parse = parse.substr(commaPos + 1);
        while (!parse.empty()) {
            commaPos = parse.find(',');
            int key;
            AL = parse.substr(0, commaPos);
            if (AL == "AL") { // Exception for states with 1 distrcit
                key = 1;
            }
            else {
                key = stoi(AL);
            }
            parse = parse.substr(commaPos + 1);

            commaPos = parse.find(',');
            int value1 = stoi(parse.substr(0, commaPos));
            parse = parse.substr(commaPos + 1);

            commaPos = parse.find(',');
            int value2;
            if (commaPos == -1) {
                value2 = stoi(parse);
                parse.clear();
            }
            else {
                value2 = stoi(parse.substr(0, commaPos));
                parse = parse.substr(commaPos + 1);
            }
            tempDistrict.numberBlueVoters = value1;
            tempDistrict.numberRedVoters = value2;
            tempState.districtVector.push_back(tempDistrict);
        }
        dataVector.push_back(tempState);
    }
    // Read data from the second file and parse
    while (getline(inFile2, parse)) {
        int commaPos = parse.find(',');
        string stateCheck;
        int voters;
        stateCheck = parse.substr(0, commaPos);
        //system("pause");
        voters = stoi(parse.substr(commaPos + 1));
        for (auto &itr : dataVector) {
            if (itr.stateName == stateCheck) {
                itr.numberVoters = voters;
                cout << itr.stateName << ":" << itr.numberVoters << endl;
            }
        }
    }
    inFile.close();
    inFile2.close();
    dataLoad = "Yes";
    return;
}

void readOut(const string& fileName, const string& fileName2, ourvector<state>& dataVector) { // Function to print data that has been loaded in
    cout << endl << "Reading: " << fileName << endl ;
    for (auto& itr : dataVector) {
        cout << "..." << itr.stateName <<  "..." << itr.districtVector.size() << " districts total" << endl << "..." << endl;
    }
    cout <<  endl <<"Reading: " << fileName2 << endl;
    for (auto& itr : dataVector) {
        cout << "..." << itr.stateName << "..." << itr.numberVoters << " eligible voters" << endl << "..." << endl;
    }
}
void search(string& stateSet, const string findState, ourvector<state>& dataVector,const string dataLoad) { // Function to search state and if it exists
    if (dataLoad == "Yes") {
        string cap = findState;
        bool found = false;
        cap[0] = toupper(cap[0]);
        for (auto& itr : dataVector) {
            if (itr.stateName == cap) {
                stateSet = itr.stateName;
                found = true;
            }
        }
        if (found == false) {
            cout << "State does not exist, search again." << endl;
        }
    }
    else {
        cout << "No data loaded, please load data first." << endl; // Check for if file was not loaded
    }

    return;
}
void stat(const string dataLoad, const string stateSet, string searchState, ourvector<state>& dataVector) { // displays stats regarding gerrymandering
    string gerry = "No";
    double demVoters = 0, repVoters = 0, totalVoters = 0, overhalf,gap;
    int   eligibleVoters = 0;
    string cap = searchState;
    cap[0] = toupper(cap[0]);

    if (dataLoad == "No") {
        cout << "No data loaded, please load data first." << endl; // Check for if file was not loaded
        return;
    }
    if (stateSet == "N/A") {
        cout << "No state searched, please search state first." << endl; // Check for if state was searched was not loaded
        return;
    }
    for (auto& itr : dataVector) { //iterating over vectors to calculate gao efficiency
        if (itr.stateName == cap) {
            if (itr.districtVector.size() > 3) {
                for (auto& key : itr.districtVector) {
                    totalVoters = totalVoters + key.numberBlueVoters + key.numberRedVoters;
                    overhalf = ((key.numberBlueVoters + key.numberRedVoters)/2)+1;
                    if (key.numberBlueVoters > key.numberRedVoters) {
                        demVoters = demVoters + abs(overhalf - key.numberBlueVoters);
                        repVoters = repVoters + key.numberRedVoters;
                    }
                    else {
                        repVoters = repVoters + abs(overhalf - key.numberRedVoters);
                        demVoters = demVoters + key.numberBlueVoters;
                    }
                }
            eligibleVoters = itr.numberVoters;
            }
            else {
                cout << "Cannot determine Gerrymandering" << endl;
                return;
            }
        }
    }
    if (repVoters > demVoters) {
        gap = ((repVoters - demVoters) / totalVoters);
    }
    else {
        gap = ((demVoters - repVoters) / totalVoters);
    }
    if (gap >= 0.07) {
        gerry = "Yes";
    }
    cout << "Gerrymandered: " << gerry << endl;
    cout << "Wasted Democratic votes: " << demVoters << endl;
    cout << "Wasted Republican votes :" << repVoters << endl;
    cout << "Eligible voters :" << eligibleVoters << endl<< endl;


}
void plot( ourvector<state>& dataVector,string searchState) { // plotting histogram for each districts red and blue votes percentage in terms of R and D 
    int demVoters, repVoters;
    string cap = searchState;
    string plotter;
    cap[0] = toupper(cap[0]);
    for (auto& itr : dataVector) {
        int counter = 0;
        if (itr.stateName == cap) {
            for (auto& key : itr.districtVector) {
                counter++;
                if (key.numberBlueVoters > key.numberRedVoters) {
                    demVoters = (key.numberBlueVoters / (key.numberRedVoters + key.numberBlueVoters)) * 100;
                    repVoters = 100 - demVoters;
                }
                else {
                    repVoters = (key.numberRedVoters / (key.numberRedVoters + key.numberBlueVoters)) * 100;
                    demVoters = 100 - repVoters;
                }
                plotter = string(demVoters, 'D') + string(repVoters, 'R');
                cout << "District: " << counter << endl;
                cout << plotter << endl << "..." << endl;
            }
        }

    }
    cout << endl;
}

int main() {
    string command,fileName,fileName2;
    ourvector<state> dataVector;
    string dataLoad = "No";
    string stateSet = "N/A";
    string searchState;
    double gap = 0;

    cout << "Welcome to the Gerrymandering App!" << endl << endl;

    while(command != "exit") { // Menu for 5 commands which will loop over until exit command
        cout << "Data loaded? " << dataLoad << endl;
        cout << "State: " << stateSet << endl << endl;
        cout << "Enter command:";
        cin >> command;
        if (command == "load") {
            cin >> fileName;
            cin >> fileName2;
            cout << endl << "-----------------------------" << endl << endl;
            readFile(fileName, fileName2, dataVector, dataLoad);
            readOut(fileName, fileName2, dataVector);
        }
        if (command == "search") {
            cin >> searchState;
            search(stateSet, searchState,dataVector, dataLoad);
            cout << endl << "-----------------------------" << endl << endl;
        }
        if (command == "stat") {
            cout << endl << "-----------------------------" << endl << endl;
            stat(dataLoad, stateSet, searchState, dataVector);
        }
        if (command == "plot") {
            cout << endl << "-----------------------------" << endl << endl;
            plot(dataVector, searchState);
        }
        if (command == "exit") {
            return 0;
        }
        
    }
    
   
    return 0;
}