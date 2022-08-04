#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <fstream>

enum textType { COMPILER_CORRECT, COMPILER_WARNING, COMPILER_ERROR, COMPILER_DEBUG, COMPILER_TYPE, COMPILER_NOTE };
enum varType { STRING, INTEGER, FUNCTION, EVENT };

std::string currentPath();
bool createFolder(std::string path, bool errorChecks=true);
bool removeFolder(std::string path, bool errorChecks=true);
bool refreshFolder(std::string path, bool errorCheck=true);
bool fileExists(std::string path);

std::fstream createFile(std::string path, std::string content, std::ios_base::openmode args);
void moveFile(std::string oldPath, std::string newPath);
std::vector<std::filesystem::directory_entry> listFiles(std::string path, std::string extension="");

std::string colorText(std::string txt, textType type);
bool terminal(std::string cmd);
std::string printType(varType var);

bool find(std::string line, std::string str);
std::string removeSpaces(std::string& str);
std::vector<std::string> split(std::string str, std::string value);
bool isWhitespace(std::string s);
std::string replace(std::string str, char changeFrom, char changeTo);