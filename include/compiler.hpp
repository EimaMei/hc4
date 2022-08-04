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

enum compilerError {
    UNDEFINED_VARIABLE, MISSING_BRACKET_1, MISSING_BRACKET_2,
    UNDEFINED_FUNCTION
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

    void compileFile(std::string path, std::string folder);
    bool areBracketsBalanced(std::string path);
    
    void debugMode();
    void errorReport(int numOfLines, std::string path, std::string error, int waveyLength=line.size(), compilerError type=UNDEFINED_VARIABLE, textType msgType=COMPILER_ERROR);

    bool newEvent(bool& found, variable& eventVar, int i=-1, int numOfLines=0, std::string path="");
    void newVar(bool& found);

    int hashtags(std::string folder);
};