#pragma once
#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>

using namespace std;

namespace Colors {
	const string Reset = "\033[0m";
	const string Green = "\033[32m";
	const string Red = "\033[31m";
	const string Yellow = "\033[33m";
}

namespace Utils {
	void printAscii();
}


#endif