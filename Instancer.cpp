#include "Instancer.h"
Instancer::Instancer(std::vector<std::pair<int, std::string>> lines) {
	this->programToRun = lines;
}


Instancer::~Instancer()
{

}

std::string Instancer::run()
{
	Interpreter inter;
	std::string stringToConcat = "";
	for (int i = 0; i < programToRun.size(); i++) {//Turns the data gathered from the Environment object into a string that the lexer can take in...
		inter.getLabels()->insert(std::pair<std::string, int>("L"+std::to_string(programToRun.at(i).first), i));
		stringToConcat = stringToConcat + programToRun.at(i).second + "\n";
	}
	Lexer lex(stringToConcat);
	Parser pars(lex.tokenize(),inter.getVariables());//.. which is then parsed and converted into an array of statements...
	inter.interpret(pars.parse(inter.getLabels()));//.. which is then interpreted and ran
	return "";
}
