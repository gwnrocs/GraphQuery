#include "MainConsole.h"
#include <iostream>
#include <string>

int main() {
    MainConsole* mainConsole = new MainConsole();

    mainConsole->drawConsole();

    delete mainConsole;
    return 0;
}