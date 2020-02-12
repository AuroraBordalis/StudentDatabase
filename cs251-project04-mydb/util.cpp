/*util.cpp*/

//
// Utility functions for myDB project
//
// <<Shawn John>>
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #04
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

#include "util.h"

using namespace std;


//
// EchoData
//
// Reads the contents of a table's .data file, and outputs the
// values for each record (one record per line).  Pass the table
// name, the record size, and the # of columns per record.
//
// Example: EchoData("students", 82, 5) would output the contents
// of "students.data".
//
void EchoData(string tablename, int recordSize, int numColumns)
{
  string   filename = tablename + ".data";
  ifstream data(filename, ios::in | ios::binary);

  if (!data.good())
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return;
  }

  //
  // Okay, read file record by record, and output each record of values:
  //
  data.seekg(0, data.end);  // move to the end to get length of file:
  streamoff length = data.tellg();

  streamoff pos = 0;  // first record at offset 0:
  string    value;

  while (pos < length)
  {
    data.seekg(pos, data.beg);  // move to start of record:

    for (int i = 0; i < numColumns; ++i)  // read values, one per column:
    {
      data >> value;
      cout << value << " ";
    }

    cout << endl;
    pos += recordSize;  // move offset to start of next record:
  }
	data.close();
}



//
// GetRecord
//
// Reads a record of data values and returns these values in a vector.
// Pass the table name, the file position (a stream offset), and the # 
// of columns per record.
//
// Example: GetRecord(�students�, 0, 5) would read the first student
// record in �students.data�.
// 

vector<string> GetRecord(string tablename, streamoff pos, int numColumns)
{
  vector<string> values;
	
  //
  // open the file, make sure it opened, seekg to the given position,
  // loop and input values using >>, store into vector, return vector
  //

	string filename = tablename + ".data";
	ifstream data(filename, ios::in | ios::binary);
	
	if (!data.good())
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return values;
  }
	
	string value;
	
	data.seekg(pos, data.beg);
	
	for(int i = 0; i < numColumns; ++i){
		data >> value;
	
		values.push_back(value);
	}
	data.close();
  return values;
}

/* funtion stores the metaData into the two vectors isIndex and colName which are passed by reference as well as changes 
 * recordSize and numColumns and passes them back. This function returns false if the file could not be open */
bool StoreMetaData(string tablename,int &recordSize, int &numColumns, vector<int> &isIndex, vector<string> &colName){
	
	
	string filename = tablename + ".meta";
	ifstream data(filename, ios::in | ios:: binary);
	
	if (!data.good()){
		
		cout<<"** Error: couldn't open data file " << filename << endl;
		return false;

	}
	
	string value;
	
	//store if the column is indexable either a 0 or 1 
	int numValue;
	
	//keep track of how many times we read from the file 
	int counter = 0; 
	
	//read in the meta data 
	while (data >> value){
		
		//if this is the first time reading into the file means the first value will have to be the recordSize
		if (counter == 0){
			
			//make the value into a number and assign recordSize to it  
			recordSize = std::stoi(value);
		}
		
		// if this is the second time reading into the file then it means it is the number of columns 
		// so make it into a number and assign numColumns to it  
		else if(counter == 1){
			numColumns = std::stoi(value);
		}
		
		//if neither means we have read it more than twice 
		else{
			
			//if the counter is even means where at a value in the meta data that is the column name so push it back 
			// into colName
			if(counter%2 == 0){
				colName.push_back(value);
			}
			
			//if it is not even then it is the number 0 or 1 so make value into a number and push that back into isIndex
			else{
				numValue = std::stoi(value);
				isIndex.push_back(numValue);
			}
		}
		
		++counter; // increment
	} //end of while 
	
	data.close();
	return true; //succesful storage of meta data return true 
	
}


//
// LinearSearch
//
// Searches the contents of a .data file record by record; the first 
// three parameters are the tablename, record size, and # of columns.
// The 4th parameter specifies the value to search for: matchValue.
// The 5th parameter is the record column to match against --- pass
// 1 for the first column, 2 for the 2nd column, and so on.  All matches
// are exact matches.
//
// Example: LinearSearch(�students�, 82, 5, "kim", 3) would search the 
// "students.data" file for all records whose 3rd column --- lastname ---
// matches "kim".  There are 2 matches (records 3 and 6), so their file 
// positions of 164 and 410 would be returned in the vector.
// 
vector<streamoff> LinearSearch(string tablename, int recordSize, int numColumns, string matchValue, int matchColumn)
{
  vector<streamoff>  matches;
	

  //
  // open the file, make sure it opened, loop through record by 
  // record looking for matches.  For each match found, add file
  // position to vector.
  //
  string   filename = tablename + ".data";
  ifstream data(filename, ios::in | ios::binary);

  if (!data.good())
  {
    cout << "**Error: couldn't open data file '" << filename << "'." << endl;
    return matches;
  }
		
  //
  // Okay, read file record by record, and output each record of values:
  //
  //
  data.seekg(0, data.end);  // move to the end to get length of file:
  streamoff length = data.tellg();

  streamoff pos = 0;  // first record at offset 0:
  string    value;
	
  if (numColumns < matchColumn){
			cout<< "the match column has to be less than or equal to the number of columns"<< endl;
			return matches;
	}
	
  while (pos < length)
  {
    data.seekg(pos, data.beg);  // move to start of record:

    for (int i = 0; i <= matchColumn; ++i)  // read values, one per column:
    {
      data >> value;

    }
		
		if(value == matchValue){
			matches.push_back(pos);
		}
		
    pos += recordSize;  // move offset to start of next record:
  }
	data.close();
  return matches;
}

/*builds tree from the .data file where key is the value at a certain column and the data value is the records position
 * and stores the tree into a vector */

void BuildIndexTree(string tablename, int recordSize, int colIndex, vector<avltree<string,streamoff>> &indexTrees){
	
	//gets the .data file 
	string filename = tablename + ".data";
	ifstream data(filename, ios::in | ios::binary);
	
	//checks if data can be opened 
	if (!data.good()){
		cout<<"**Eroor: couldn't open data file " << filename<< "."<<endl;
		return;
	}
	
	data.seekg(0, data.end);  // move to the end to get length of file:
  streamoff length = data.tellg();

  streamoff pos = 0;  // first record at offset 0:
  string    value;
	
	//create the tree 
	avltree<string,streamoff> tree;
	
	while (pos < length)
	{
		data.seekg(pos, data.beg);  // move to start of record:
		
		
		//read the value so that the last value read will be the value of the column who can be indexed 
		// this value will now be a key of the tree  
    for (int i = 0; i <= colIndex; ++i)  // read values, one per column:
    {
      data >> value;

    }
		
		//insert the value of the column and the records position 
		tree.insert(value,pos);
    pos += recordSize;  // move offset to start of next record:
  } // end of while 
	
	//push the now made tree to the back of the vector 
	indexTrees.push_back(tree);
	
	data.close();
}

//displays data based on if all data was wanted to display or only just one */
void display(bool displayAll, vector<string> &values, vector<string> &colName, int dataColNumber){
	
	// if a "*" was inputed then displayAll would be true and then it would display all column values in the record
	if(displayAll==true){
				for(int columnNumber = 0; columnNumber<values.size(); columnNumber++){
					cout<<colName[columnNumber]<<": "<<values[columnNumber]<<endl;
			
				}
			}
	
	// only display the column value of the column specified after select 
	else{
		cout<<colName[dataColNumber]<<": "<<values[dataColNumber]<<endl;
	}
	
}

/* Function that return a boolean to determine if the column specified is found in the vector colName */

bool isData(int numColumns, vector<string> &colName, vector<string> &tokens, int tokenNum, int &columnNumber){
	
	bool valFound = false;
	//loops through until some column equals the tokens column 
	for(int i = 0; i<numColumns; i++){
				
		if(colName[i]==tokens[tokenNum]){
			columnNumber = i;
			valFound = true;
			break;
		}
	}
	return valFound;
}


/* function that displays errors and returns true if there is any 
 * takes the vector as pass by reference to not create a copy */
bool error(vector<string> &tokens, string tablename){
	
	if((tokens[0]!= "select")){
		cout<<"Unknown query, ignored..."<<endl;
			return true;
	}
		
	else if((tokens[0]=="select")&&((tokens[2]!="from")||(tokens[4]!="where"))){
		cout<<"Invalid select query, ignored..."<<endl;
		return true;

	}
		
	else if(tokens[3] != tablename){
		cout<<"Invalid table name, ignored..."<<endl;
		return true;
	}
	
	else if(tokens.size()!= 8){
		cout<<"Unknown query, ignored..."<<endl;
		return true;
	}
	else{
		return false;
	}
	
}
