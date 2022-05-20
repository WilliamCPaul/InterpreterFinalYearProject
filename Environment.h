#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Instancer.h"
class Environment
{
public:
	Environment();
	~Environment();
	void run();


private:
	bool automaticLine;
	int currentLineNumber;
	std::vector<std::pair<int, std::string>> currentLines;
};

