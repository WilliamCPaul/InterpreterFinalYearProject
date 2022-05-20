#include "Environment.h"
#include <fstream>
#include <algorithm>
using namespace std;
Environment::Environment()
{
	automaticLine = true;
	currentLineNumber = 10;
	currentLines = std::vector<std::pair<int, std::string>>();
}

Environment::~Environment()
{

}

//The main loop of the entire program. It handles all interaction with the user alongside setting up the Instancer and run it
void Environment::run()
{
	std::string userInput;
	bool done = false;

	cout << "Welcome to the Altered Tiny Basic Interpreter application written by William Chantrill Paul." << endl << "If you need help going around, please type $h and press enter." << endl;
	cout << "Before beginning to use the software, do you intend to manually type in the line numbers?" << endl <<"This decision will also matter for any file you import. Y for yes and N for no." << endl;
	
	userInput = "";
	getline(cin, userInput);
	if (userInput.find("y") != std::string::npos || userInput.find("Y") != std::string::npos) {
		automaticLine = false;
	}

	cout << "Thank you, you may now begin using the software" << endl;
	while (!done) {
		if (automaticLine==true) {
			cout << "WCPB" + to_string(currentLineNumber)+": ";
		}
		getline(cin, userInput);
		if (userInput.at(0) == '$') {//Enviromnent Function Calls
			std::ifstream file;
			std::string fileline;
			Instancer inst(currentLines);
			switch (userInput.at(1)) {
			case 'a':
			case 'A'://Toggle Automatic Line Generation
				automaticLine = !automaticLine;
				currentLineNumber = currentLines.size() * 10 + 10;
				break;
			case 'f':
			case 'F'://Read into the array the lines of code from a local file
				cout << userInput.substr(3, std::string::npos) + ".wcp" << endl;
				file=ifstream(userInput.substr(3, std::string::npos)+".wcp");
				if (!file) {
					cout << "Error trying to find or read the following file:" + userInput.substr(3, std::string::npos) << endl;
				}
				while (getline(file, fileline)) {
					if (automaticLine) {
						currentLines.push_back(pair<int, string>(currentLineNumber, fileline));
						currentLineNumber += 10;
					}
					else {
						if (fileline.at(0) != 'L') {
							cout << "With line numbers not being automatic, each line must start with the letter L afterwhich a series of numbers" << endl;
							break;
						}
						currentLines.push_back(pair<int, string>(std::stoi(fileline.substr(1, fileline.length() + fileline.find_first_of(" "))), fileline.substr(fileline.find_first_of(" "))));
					}
				}
				break;
			case 'r':
			case 'R'://Run the current code stored in the vector variable
				inst.run();
				break;
			case 'q':
			case 'Q'://Quit the program
				done = true;
				break;

			case 'h':
			case 'H'://Help function to offer some help to the user
				cout << "To use this software, you have 2 different types of lines you can submit." << endl << "1. You can submit lines that are meant to be saved and then later run or 2 run commands for this environment" << endl;
				cout << "For the environment functions, you simply as you did earlier add a dollar sign and then a letter, uppercase or lowercase does not matter." << endl << "Your current avaiable commands are " << endl;
				cout << "$A to be able to change between automatic or manual feed line numbers" << endl << "$F alongside a filename to be able to import a file. The filename will automatically add the .wcp at the end and will only accept local files" << endl;
				cout << "$R will simply run your program" << endl << "$L will print a list of all the lines of code that you have written thus far, whichever line number you picked will be shown here" << endl << "$Q will simply quit the program" << endl;
				cout << "As for the language itself, it is very near to how simple BASIC programs are fun. You are able to define variables with numbers and strings, for which the system to pick out the variable type based on context" << endl;
				cout << "<Variable Name> = <Expression> to create and initialize variables, print <Expression> to print out mathematics or the contents of a variable" << endl << "goto <label> will take the program back to the line number explicitly typed or a named label"<< endl;
				cout << "For control a if <condition> then <label name/line number> will evaluate a condition then if it evalutuates to a non zero number it will jump to the line number or label location" << endl;
				break;

			case 'l':
			case 'L'://Lists all the code written thus far
				for (int i = 0; i < currentLines.size(); i++) {
					cout << std::to_string(currentLines.at(i).first) + ": " + currentLines.at(i).second;
					cout << endl;
				}
				break;
			default://Ensures any other symbols or letters are kept in mind and offers an error to the user
				cout << "Error: Command " + userInput.at(1);
				cout << " does not exist" << endl;
			}
		}
		else {
			if (automaticLine) {//Pushes the line into the array
				currentLines.push_back(pair<int, string>(currentLineNumber, userInput));
				currentLineNumber += 10;
			}
			else {
				if (userInput.at(0) != 'L') {//Pushes the line into the array while keeping in mind the the custom line number
					cout << "With line numbers not being automatic, each line must start with the letter L afterwhich a series of numbers" << endl;
					break;
				}
				currentLines.push_back(pair<int, string>(std::stoi(userInput.substr(1, userInput.length() + userInput.find_first_of(" "))), userInput.substr(userInput.find_first_of(" "))));
			}
		}
	}
}
