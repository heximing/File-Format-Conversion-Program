// CIT237-C1
// Homework
// Project 2
// File Format Conversion Program
// Ximing He
// July 20, 2015
//
///////////////////////////


#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int parseLineToArray(string, char, vector<string> &); // for Pile-to-CSV
int parseLineToArray2(string, char, vector<string> &); // for CSV-to-Pile
char getDelimiter(char);

int main()
{
	int num; // store the user input for choosing a function in this program

	cout << "This program has 2 functions." << endl;
	cout << "1. Convert a pipe-delimited text file to an equivalent CSV-format file." << endl;
	cout << "2. Convert a CSV-format file to an equivalent pipe-delimited text file." << endl;
	cout << "Please choose a function. (enter a number): ";
	cin >> num;
	while (cin.fail() || num != 1 && num != 2) // no extraction took place
	{
		cin.clear(); // reset the state bits back to goodbit so we can use ignore()
		cin.ignore(1000, '\n'); // clear out the bad input from the stream
		cout << "invalid input!!" << endl;
		cout << "Please choose a function. (enter a number): ";
		cin >> num;
	}

	if (num == 1) // Convert a pipe-delimited text file to an equivalent CSV-format file
	{
		char delimiter = '|';
		delimiter = getDelimiter(delimiter); // let the user choose their own delimiter
		string filename;
		ifstream inFile;
		cout << "Enter file name: ";
		cin.ignore(20, '\n'); // clear the new line charactor in the keyboard buff
		getline(cin, filename);
		inFile.open(filename.c_str());
		if (!inFile)
		{
			cout << "Can't read file." << endl;
			inFile.close();
			system("pause");
			return 0;
		}
		ofstream outFile;
		outFile.open("PipeToCSV.txt"); // change the output file name if needed
		string inputRecord;
		while (getline(inFile, inputRecord)) // if there is something input
		{
			vector<string> dataField;
			int parsedFieldCount = parseLineToArray(inputRecord, delimiter, dataField);
			if (parsedFieldCount > 0) // if there is at least 1 field
			{
				for (int i = 0; i < parsedFieldCount; i++)
				{
					int doubleQuotePosition = dataField[i].find('\"', 0);
					// if there are double-quotes, replace them by double double-quotes
					while (doubleQuotePosition != -1)
					{
						dataField[i].insert(doubleQuotePosition + 1, 1, '\"');
						doubleQuotePosition = dataField[i].find('\"', doubleQuotePosition + 2);
					}
					// if there are commas OR double-quotes, put double-quote for the field
					if (dataField[i].find(',', 0) != -1 || dataField[i].find('\"', 0) != -1)
					{
						dataField[i].insert(0, 1, '\"');
						dataField[i].insert(dataField[i].size(), 1, '\"');
					}
					outFile << dataField[i] << ",";
				}// (end for)
				outFile << endl;
			}// (end if)

		}// (end while)

		inFile.close();
		outFile.close();
	}
	else if (num == 2) // Convert a CSV - format file to an equivalent pipe - delimited text file
	{
		char delimiter = ',';
		string filename;
		ifstream inFile;
		cout << "Enter file name: ";
		cin.ignore(20, '\n'); // clear the new line charactor in the keyboard buff
		getline(cin, filename);
		inFile.open(filename.c_str());
		if (!inFile)
		{
			cout << "Can't read file." << endl;
			inFile.close();
			system("pause");
			return 0;
		}
		ofstream outFile;
		outFile.open("CSVtoPipe.txt"); // change the output file name if needed
		string inputRecord;
		while (getline(inFile, inputRecord)) // if there is something input
		{
			vector<string> dataField;
			int parsedFieldCount = parseLineToArray2(inputRecord, delimiter, dataField);
			if (parsedFieldCount > 0) // if there is at least 1 field
			{
				for (int i = 0; i < parsedFieldCount; i++)
				{
					// if there are commas OR double-quotes, erase the outside double-quote for the field
					if (dataField[i].find(',', 0) != -1 || dataField[i].find('\"', 0) != -1)
					{
						dataField[i].erase(0, 1);
						dataField[i].erase(dataField[i].size() - 1, 1);
					}

					int doubleQuotePosition = dataField[i].find("\"\"", 0);
					// if there are double-double-quotes, replace them by double-quotes
					while (doubleQuotePosition != -1)
					{
						dataField[i].erase(doubleQuotePosition, 1);
						doubleQuotePosition = dataField[i].find("\"\"", doubleQuotePosition + 1);
					}
					outFile << dataField[i];
					if (i < parsedFieldCount - 1)
						outFile << "|";
				}// (end for)
				outFile << endl;
			}// (end if)

		}// (end while)

		inFile.close();
		outFile.close();
	}
	else
	{
		cout << "Something weird happend. Check the code!!" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}

	system("pause");
	return 0;
}

int parseLineToArray(string inputText, char inputDelim,
	vector<string> &outputDataValue)
{
	//	Local Variables:
	int nextFieldPosition = 0;		//	Start position of data field.
	int foundPosition;				//  Position where delimiter was found,
	int parsedFieldCount = 0;		//  Number of data fields parsed so far,
	bool endOfLine = false;			//	Flag -- end of input line detected,
	string dataField;				//  Copy of the data field.

	cout << "Enter 'parseLineToArray' function, " << endl
		<< "inputText = (" << inputText << ")" << endl
		<< "NextFieldPosition = " << nextFieldPosition
		<< endl;

	while (!endOfLine)
	{
		foundPosition = inputText.find(inputDelim, nextFieldPosition);
		if (foundPosition == -1)
		{
			// (Here if the string 'find' function reached the end of the input string.)
			endOfLine = true;
			cout << "delimiter not found for inputText = " << inputText << endl
				<< " (search began at position " << nextFieldPosition << ")."
				<< endl;

			foundPosition = inputText.length();

			if (foundPosition > nextFieldPosition)
			{
				dataField = inputText.substr(nextFieldPosition,
					foundPosition - nextFieldPosition);
				cout << "dataField = " << dataField << endl;
				parsedFieldCount++;
			}
		}
		else
		{
			// (Here if 'inputDelim' was found in the input string.)
			// Copy the data field contents from the input string to 'dataField'
			// local variable.
			dataField = inputText.substr(nextFieldPosition, foundPosition - nextFieldPosition);
			parsedFieldCount++;

			// Adjust 'nextFieldPosition' as preparation for finding NEXT delimiter.
			nextFieldPosition = foundPosition + 1;
			cout << "dataField = " << dataField << ", nextFieldPosition = "
				<< nextFieldPosition << endl;
		}// (end if)

		if (parsedFieldCount > 0)
		{
			outputDataValue.push_back(dataField);
		}
	} // (end while)

	//	Display the found data fields, as a debug aid:
	for (int i = 0; i < parsedFieldCount; i++)
	{
		cout << "outputDataValue[" << i << "] = " << outputDataValue[i] << endl;
	} // (end for)	

	return parsedFieldCount;
} // (end function parseLineToArray )

char getDelimiter(char ch)
{
	cout << "Now you can choose to set the delimiter, or use the default one." << endl;
	cout << "The default delimiter is: \"" << ch << "\". Do you want to change it? (Y/N) ";
	char temp; // to store Y or N
	cin.ignore(20, '\n'); // clear the new line charactor in the keyboard buff
	cin.get(temp);

	while (!(toupper(temp) == 'Y' || toupper(temp) == 'N'))
	{
		cout << "You have to choose Y or N here. No tricky play." << endl;
		cout << "The default delimiter is: \"" << ch << "\". Do you want to change it? (Y/N) ";
		if (temp != '\n')
			cin.ignore(20, '\n'); // clear the new line charactor in the keyboard buff
		cin.get(temp);
	}

	if (toupper(temp) == 'Y')
	{
		cout << "Please enter a delimiter charactor: ";
		cin.ignore(20, '\n'); // clear the new line charactor in the keyboard buff
		cin.get(ch);
		while (ch == '\n' || !isprint(ch))
		{
			cout << "Delimiter can't be new line character or other wierd characters." << endl;
			cout << "Please enter a delimiter charactor again: ";
			if (ch != '\n')
				cin.ignore(20, '\n'); // clear the new line charactor in the keyboard buff
			cin.get(ch);
		}
		cout << "The delimiter is: \"" << ch << "\" now." << endl;
	}
	else if (toupper(temp) == 'N')
	{
		cout << "The delimiter is: \"" << ch << "\" now." << endl;
	}
	else
	{
		cout << "Something wierd happened. Please check the code!!" << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	return ch;
}

int parseLineToArray2(string inputText, char inputDelim,
	vector<string> &outputDataValue)
{
	//	Local Variables:
	int nextFieldPosition = 0;		//	Start position of data field.
	int foundPosition;				//  Position where delimiter was found,
	int parsedFieldCount = 0;		//  Number of data fields parsed so far,
	bool endOfLine = false;			//	Flag -- end of input line detected,
	string dataField;				//  Copy of the data field.

	cout << "Enter 'parseLineToArray' function, " << endl
		<< "inputText = (" << inputText << ")" << endl
		<< "NextFieldPosition = " << nextFieldPosition
		<< endl;

	while (!endOfLine)
	{
		foundPosition = inputText.find(inputDelim, nextFieldPosition);
		if (foundPosition == -1)
		{
			cout << "Line end. continue..." << endl;
			endOfLine = true;
			continue;
		}
		else
		{
			// (Here if 'inputDelim' was found in the input string.)
			// Copy the data field contents from the input string to 'dataField'
			// local variable.
			dataField = inputText.substr(nextFieldPosition, foundPosition - nextFieldPosition);
			parsedFieldCount++;

			// Adjust 'nextFieldPosition' as preparation for finding NEXT delimiter.
			nextFieldPosition = foundPosition + 1;

			// from here, check if dataField is complete
			cout << endl << "from here, check if dataField is complete" << endl;
			bool complete = false;
			int foundDoubleQuote = dataField.find('\"', 0);
			if (foundDoubleQuote != -1)
			{
				cout << "Double quote found, which means dataField might not be complete" << endl;
				cout << "Now start looking for extra data field by looking for another double quote" << endl;
				cout << "First, check if there is another double quote in \"dataField\"." << endl;
				foundDoubleQuote = dataField.find('\"', 1);
				while (foundDoubleQuote != -1)
				{
					cout << "another double quote found, but is it a double-double quote?" << endl;
					if (dataField.size() == foundDoubleQuote + 1) // if the double quote is the last charactor
					{
						cout << "It is not a double-double, which means dataField is complete." << endl;
						foundDoubleQuote = -1;
						complete = true;
					}
					else if (dataField.at(foundDoubleQuote + 1) == '\"')
					{
						cout << "Unfortunately, it is a double-double quote." << endl;
						cout << "Skip it and continue..." << endl;
						foundDoubleQuote = dataField.find('\"', foundDoubleQuote + 2);
					}
					else
					{
						cout << "Something wired happend. Check the code!!" << endl;
						system("pause");
						exit(EXIT_FAILURE);
					}
				}
				if (!complete)
				{
					cout << "Can't find double quote in dataField, which means dataField is NOT complete" << endl;
					cout << "Now look for extra data in \"inputText\"" << endl;
					foundPosition = inputText.find('\"', nextFieldPosition);
					while (foundPosition != -1)
					{
						cout << "another double quote found, but is it a double-double quote?" << endl;
						if (inputText.at(foundPosition + 1) == '\"')
						{
							cout << "Unfortunately, it is a double-double quote." << endl;
							cout << "Skip it and continue..." << endl;
							foundPosition = inputText.find('\"', foundPosition + 2);
						}
						else
						{
							cout << "It is not a double-double, which means dataField would be complete here." << endl;
							cout << "First, store extra data into dataField..." << endl;
							dataField.append(inputText, nextFieldPosition - 1, foundPosition - nextFieldPosition + 2);
							cout << "Now dataField is complete." << endl;
							nextFieldPosition = foundPosition + 2;
							foundPosition = -1;
						}
					}
				}

				
			}
			else
			{
				cout << "Double quote NOT found. dataField is complete" << endl;
			}

			cout << "dataField = " << dataField << ", nextFieldPosition = "
				<< nextFieldPosition << endl;
		}// (end if)

		if (parsedFieldCount > 0)
		{
			outputDataValue.push_back(dataField);
		}
	} // (end while)

	//	Display the found data fields, as a debug aid:
	for (int i = 0; i < parsedFieldCount; i++)
	{
		cout << "outputDataValue[" << i << "] = " << outputDataValue[i] << endl;
	} // (end for)	

	return parsedFieldCount;
} // (end function parseLineToArray )