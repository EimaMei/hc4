#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>

struct variable {
    std::string name;
    std::string value;
    std::string value2;
    varType type;
};

namespace hc4 {
    extern std::vector<variable> variables;
    extern std::vector<std::string> mentionedPaths;

	extern std::vector<std::string> words;
	extern std::vector<std::string> equals;

    extern std::string line;
    
    extern std::fstream fileBuffer1;
    extern std::fstream fileBuffer2;
    extern std::fstream fileBuffer3;
    
    extern int refNum;
    extern bool stop;
    
    void debugMode();
    void errorReport(int numOfLines, std::string path, int amountOfBrackets);

    int newEvent(bool& found, variable& eventVar, int i=-1);
    void newVar(bool& found);
};