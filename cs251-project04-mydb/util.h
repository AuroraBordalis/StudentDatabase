/*util.h*/

//
// Utility functions for myDB project
//
// <<YOUR NAME>>
// U. of Illinois, Chicago
// CS 251: Fall 2019
// Project #04
//

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "avl.h"

using namespace std;

void EchoData(string tablename, int recordSize, int numColumns);

vector<string> GetRecord(string tablename, streamoff pos, int numColumns);

vector<streamoff> LinearSearch(string tablename, int recordSize, int numColumns, string matchValue, int matchColumn);

bool StoreMetaData(string tablename, int &recordSize, int &numColumns, vector<int> &isIndex, vector<string> &colName);

void BuildIndexTree(string tablename, int recordSize, int colIndex, vector<avltree<string,streamoff>> &indexTrees);

void display(bool displayAll, vector<string> &values, vector<string> &colName, int dataColNumber);

bool isData(int numColumns, vector<string> &colName, vector<string> &tokens, int tokenNum, int &columnNumber);

bool error(vector<string> &tokens, string tablename);