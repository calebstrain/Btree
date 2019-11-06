/*

IMPORTANT: Sample B-tree functions are from the book Data Structures using C++ by D.S. Malik

Program to implement B-tree insert, inorder traversal, and to print a specified level.
Caleb Strain - 3/30/19

*/

#include <iostream>
#include <fstream>
#include <string>
#include "ArgumentManager.h"
#include "Btree.h"
#include <sstream>
#include <cassert>
using namespace std;

int getOrder(string s)
{
	stringstream ss(s);
	string temp = "";

	getline(ss, temp, '=');
	getline(ss, temp, '\n');

	return stoi(temp);
}

int getLevel(string s)
{
	stringstream ss(s);
	string temp = "";

	getline(ss, temp, ' ');
	getline(ss, temp, '\n');

	return stoi(temp);
}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Usage: sort input=input1.txt output=output1.txt command=command1.txt" << std::endl;
		return -1;
	}
	ArgumentManager am(argc, argv);
	const string inputFileName = am.get("input");
	const string outputFileName = am.get("output");
	const string commandFileName = am.get("command");

	/*const string inputFileName = ("in.txt");
	const string commandFileName = ("command.txt");
	const string outputFileName = ("out.txt");*/

	ifstream f(inputFileName);
	ifstream ifs(commandFileName);
	ofstream ofs(outputFileName);

	string command;
	int level;
	while (1) // use this loop to ignore empty lines at the beginning of the file, assuming the degree will be the first command
	{
		getline(ifs, command);
		if (command.empty())
			continue;
		else
			break;
	}

	const int order = getOrder(command);
	Btree tree(order); 

	int data;
	while (f >> data)
		tree.insert(data);

	bool first = true;
	while (!ifs.eof()) 
	{
		getline(ifs, command);
		if (command.empty())
			continue;
		else if(!first)
			ofs << endl;
		if (command[0] == 'L') // Level x. "No errors in command"
		{
			level = getLevel(command);
			tree.printLevel(ofs, level);
		}
		else if (command[0] == 'I') // Inorder Traversal. "No errors in command"
			tree.inOrder(ofs);
		first = false;
	}

	return 0;
}

