/*main.cpp*/

//
// myDB project using AVL trees
//
// <<Shawn John>>
// UIN: 663331719
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #04
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>

#include "avl.h"
#include "util.h"

using namespace std;


//
// tokenize
//
// Given a string, breaks the string into individual tokens (words) based
// on spaces between the tokens.  Returns the tokens back in a vector.
//
// Example: "select * from students" would be tokenized into a vector
// containing 4 strings:  "select", "*", "from", "students".
//
vector<string> tokenize(string line)
{
  vector<string> tokens;
  stringstream  stream(line);
  string token;

  while (getline(stream, token, ' '))
  {
    tokens.push_back(token);
  }

  return tokens;
}



int main()
{
  string tablename; // = "students";

  cout << "Welcome to myDB, please enter tablename> ";
  getline(cin, tablename);

  cout << "Reading meta-data..." << endl;
 
	//store the column name and its respective value into two vectors
	vector<int> isIndex;
	vector<string> colName;
	
	//numbers to hold recordSize and the number of columns 
	int recordSize;
	int numColumns;
	
	bool metaSuccess; //checks if reading the meta data was succesful 

	/* Function uses pass by reference for all except tablename so it will compute the vectors  and integers and pass them back 
	 * it will return false if it couldn't do anything because it was an invalid meta file */
	
	metaSuccess = StoreMetaData(tablename, recordSize, numColumns, isIndex, colName); //function in util.cpp
	if(metaSuccess == false){
		return 0;
	}
	
  cout << "Building index tree(s)..." << endl;
  
	//store a vector that in each position has an avl tree
	vector<avltree<string,streamoff>> indexTrees;
  
	//loops through the columns and builds trees if those columns are indexed  
	for(int column = 0; column<numColumns; column++){
		
		avltree<string,streamoff> tree; //iniitialize an empty avl tree 
		
		//checks if the column can be indexed if it can then insert into the avltree using the function
		if(isIndex[column] == 1){
			
			BuildIndexTree(tablename,recordSize,column,indexTrees); //function in util.cpp
			
			cout<<"Index column: "<<colName[column]<<endl;
			cout<<"  Tree size: "<< (indexTrees.back()).size()<<endl;
			cout<<"  Tree height: "<<(indexTrees.back()).height()<<endl;
				
		} //end of if
		
		else{
			
			/*if the item can't be indexed just push an empty avltree to keep it consistent with 
			 * isIndex vector, such that if there is a 1 at isIndex[someposition] there should be 
			 * an non-empty tree in indexTrees[someposition]. And if there is a 0 at isIndex[different position]
			 * then there should be an empty tree at indexTrees[different position] */
			indexTrees.push_back(tree);
			
		} // end of else
		
	} // end of for loop
	
  //
	
  //
  // Main loop to input and execute queries from the user:
  //
  string query;
  
  cout << endl;
  cout << "Enter query> ";
  getline(cin, query);

  while (query != "exit")
	{
		
    vector<string> tokens = tokenize(query);
	
		
		//checks if the inputted string has any errors and returns true if there is any errors 
		//it also does the needed cout statements in the function
		bool isError = error(tokens, tablename); //function in util.cpp
		

		//checks if there is an error and if there is user has to enter a new query
		if(isError == true){
			
			cout << endl;
      cout << "Enter query> ";
      getline(cin, query);
			continue;
		}
		
		/* booleans to determine if the select column and where column where found 
		 * determine if the where column is an indexed column 
		 * and determine if all information needs to be displayed */
		
    bool queryFound = false;
		bool dataFound = false;
		bool indexed = false;
		bool displayAll = false;
		
		/* finds the column number of the select and where columns */
		
		int searchColNumber = 0;
		int dataColNumber = 0;
		
		//finds the value we are looking for or the value after the "="
		string searchVal = tokens[7];
		
		// pointer to some streamoff which is what is returned in the search avltree function
		streamoff* pos;
		
		// stores the values of each column of one record 
		vector<string> values;
		
		//a vector of streamoff matches if the data isn't indexed there could be mutiple data that fits the 
		// paramaters 
		vector<streamoff> matches;
		
		
		
		//checks if all data needs to be displayed 
		if(tokens[1] == "*"){
			queryFound = true;
			displayAll = true;
		}
		
		// else find which data needs to be retrieved 
		else{
			
			//function returns boolean to determine if the select column  was found and changes dataColNumber by pass by reference
			// which finds which column to display 
			queryFound = isData(numColumns, colName, tokens, 1, dataColNumber);	
		}
		
		//function returns boolean to determine if the where column was found and changes searchColNumber 
		// by pass by reference which is used to search for the data 
		dataFound = isData(numColumns, colName, tokens, 5, searchColNumber);
		
		//only execute if both data was found 
		if((dataFound==true) && (queryFound==true)){
			
			//check if the where column is indexed
			if(isIndex[searchColNumber]==1){
				indexed = true;
				
				//find the position of beginning of the record in the avltree of where column number 
				// since the indexTrees have the same size as the numColumns it should contain a non-empty tree 
				// at same columnNumber whose indexes are 1 
				pos = (indexTrees[searchColNumber]).search(searchVal);
				
				//checks if record was indeed found 
				if(pos == NULL){
					cout<<"Not found..." << endl;
					
					cout<<endl;
					cout << "Enter query> ";
					getline(cin, query);
					continue;
				}
				
			}// end of if(isIndex[]...)
			
			//if it is not indexed that means there could be matches so linear search and find the matches 
			else{
				matches = LinearSearch(tablename, recordSize, numColumns,searchVal, searchColNumber);
				
				//checks if there were any records actual found 
				if(matches.size()==0){
					cout<<"Not found..." << endl;
					
					cout<<endl;
					cout << "Enter query> ";
					getline(cin, query);
					continue;
				}
				
			} // end of else 207
		
			//checks if indexed was true 
			if (indexed == true){
				
				//gets the values of all column based on the position given 
				// had to be derefenced because avltree return pointer to position 
				
				values = GetRecord(tablename, *pos, numColumns); //function in util.cpp
			
				
				//displays the values determined on the select columnNumber 
				
				display(displayAll, values, colName, dataColNumber); //function in util.cpp
			
			} //end of if line 230 
			
			//if it wasn't indexed there could be multiple matches 
			//so loop through and display all select columns that matches 
			else{
				for(int i = 0; i<matches.size(); i++){
					
					//value gets updated for every position in matches 
					values = GetRecord(tablename, matches[i],numColumns); //function in util.cpp
					
					// display for every postion in matche 
					display(displayAll, values, colName, dataColNumber); //function in util.cpp
					
				}// end of for 
				
			} //end of else line 246
			
		} //end of if both dataFound and queryFound
		
		//if  dataFound or queryFound was false means there was some error 
		else{
			
			//checks if the select column was false 
			if(queryFound == false){
				cout<<"Invalid select column, ignored..."<<endl;
				
			}
			//checks if the where column was false 
			else if(dataFound == false){
				cout<<"Invalid where column, ignored..."<<endl;
			}
		}
		


    cout << endl;
    cout << "Enter query> ";
    getline(cin, query);
  }

  //
  // done:
  //
  return 0;
}
