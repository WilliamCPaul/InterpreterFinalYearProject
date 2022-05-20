#pragma once
#include <vector>
#include <map>
#include <string>
#include "Interpreter.h"
class Instancer
{
public:
	Instancer(std::vector<std::pair<int, std::string>> lines);
	~Instancer();
	std::string run();

private:
	std::vector<std::pair<int, std::string>> programToRun;
};

