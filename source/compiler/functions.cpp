#include "../../include/functions.hpp"

#ifdef __WIN32__
#include <windows.h>
#include <shellapi.h>
#endif

std::string currentPath() {
    #ifdef __WIN32__
    TCHAR szFileName[MAX_PATH];
    GetCurrentDirectory (MAX_PATH, szFileName);

    return szFileName;
    #endif
}

bool createFolder(std::string path, bool errorChecks/*=true*/) {
    if (std::filesystem::create_directories(path)) { 
        return true; 
    }
    else if (errorChecks) { 
        std::cout << colorText("Error: ", COMPILER_ERROR) << "Failed to create " << colorText(path, COMPILER_WARNING) << " at " << colorText(currentPath(), COMPILER_WARNING) << std::endl; 
    }
    return false;
}

bool removeFolder(std::string path, bool errorChecks/*=true*/) {
    if (std::filesystem::remove_all(path)) { 
        return true; 
    }
    else if (errorChecks) { 
        std::cout << colorText("Error: ", COMPILER_ERROR) << "Failed to remove '" << colorText(path, COMPILER_WARNING) << "' at '" << colorText(currentPath(), COMPILER_WARNING) << std::endl; 
    }
    return false;
}

bool refreshFolder(std::string path, bool errorChecks/*=true*/) {
    if (std::filesystem::create_directories(path)) { 
        return true; 
    }
    else if (!errorChecks)  {
        removeFolder(path);
        if (createFolder(path)) return true;
    }
    else { 
        std::cout << colorText("Error: ", COMPILER_ERROR) << "Failed to refresh " << colorText(path, COMPILER_WARNING) << " at " << colorText(currentPath(), COMPILER_WARNING) << std::endl; 
    }
    return false;
}

std::vector<std::filesystem::directory_entry> listFiles(std::string path, std::string extension/*=""*/) {
    std::vector<std::filesystem::directory_entry> list;

    for (const auto & file : std::filesystem::directory_iterator(path)) { 
        if ( extension == "" || (extension != "" && file.path().extension() == extension) ) {
            list.insert(list.end(), file);
        }
    }

    return list;
}

std::string colorText(std::string txt, textType type) {
    int clr;
    switch (type) {
        case COMPILER_CORRECT: clr = FOREGROUND_GREEN; break;
        case COMPILER_WARNING: clr = FOREGROUND_RED | FOREGROUND_GREEN; break;
        case COMPILER_ERROR: clr = FOREGROUND_RED|FOREGROUND_INTENSITY; break;
        case COMPILER_DEBUG: clr = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE; break;
        case COMPILER_TYPE: clr = FOREGROUND_INTENSITY | FOREGROUND_BLUE; break;
        case COMPILER_NOTE: clr = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
        default: clr = type; break;
    }

    #ifdef __WIN32__
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, clr);
    std::cout << txt;
    SetConsoleTextAttribute(hConsole, 7);
    #endif

    return ""; //If version not defined, return nothing
}

bool isWhitespace(std::string s) {
    for(int index = 0; index < s.length(); index++){
        if(!std::isspace(s[index]))
            return false;
    }
    return true;
}

std::vector<std::string> split(std::string str, std::string value) {
    std::vector<std::string> list;
    char *token = strtok((char*)str.c_str(), value.c_str());
    
    while (token != NULL) {
        list.insert(list.end(), token);
        token = strtok(NULL, value.c_str());
    }
    
    return list;
}

bool find(std::string line, std::string str) {
    return (line.find(str) != std::string::npos);
}

std::string removeSpaces(std::string& str) {
	str.erase(remove(str.begin(), str.end(),' '), str.end());

    return str;
}


std::fstream createFile(std::string path, std::string content, std::ios_base::openmode args) {
    std::fstream newFile;
    if (!std::filesystem::exists(path)) {
        newFile.open(path, args);
        newFile << content;
    } else { newFile.open(path, args); }
    
    return newFile;
}

std::string replace(std::string str, char changeFrom, char changeTo) {
    std::replace( str.begin(), str.end(), changeFrom, changeTo);

    return str;
}

bool fileExists(std::string path) {
    return std::filesystem::exists(path);
}

bool terminal(std::string cmd) {
    std::string c = "/c "+cmd;
    bool b = ShellExecuteA(0,"open", "cmd", c.c_str(), NULL, 0L);
    return b;    
}

void moveFile(std::string oldPath, std::string newPath) {
    MoveFileA(oldPath.c_str(), newPath.c_str());
}

std::string printType(varType var) {
    switch (var) {
        case STRING: return "String";
        case INTEGER: return "Int";
        case EVENT: return "Event";
        case FUNCTION: return "Function";
    }

    return "";
}