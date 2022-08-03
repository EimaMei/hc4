// HC4 is not responsible for any data loss
#include <iostream>
#include <filesystem>
#include <string.h>
#include "../include/functions.hpp"
#include "../include/compiler.hpp"
using namespace hc4;

bool debug=false;
std::string filename;

void compileFile(std::string path, std::string folder) {
    std::ifstream file(path);
	int numOfLines=0;
	int amountOfBrackets=0;

    while (std::getline(file, line)) {
		numOfLines++;
		if (stop) break;
		
		bool found=false;

        if (find(line, "#include")) {
            std::string f = folder+split(line, "\"")[1];

            compileFile(f, folder);
			continue;
        }
        else if (find(line, "#")) {
            std::string n;
            for(int i = 0; i < line.size(); i++) {
                if (line.at(i) == '#') break;
                n += line.at(i);
            }
            line=n;
        }

        if (!isWhitespace(line)) {

            words = split(line, " ");
			std::string noSpace;
            for (int i=0; i<words.size(); i++) { noSpace += words[i] + " "; };
			equals= split(noSpace, "=");

			if (find(line, "{")) { amountOfBrackets++; found=true; }
			if (find(line, "}")) { 
				amountOfBrackets--; found=true;
				fileBuffer2 << "}\n";
			}

            if (words[0] == "func") {
				found=true;
            }
            else if (words[0] == "event") { variable v; newEvent(found, v); } 
			else if (!variables.empty()) {
				for (int i=0; i<variables.size(); i++) {
					if (find(line, variables[i].name)) {
						switch (variables[i].type) {
							case EVENT: newEvent(found, variables[i], i); found = true; break;
							case STRING: continue;
						}
						
						if (found) break;
					}
				}
			}
            
			if (find(line, "=") && !found) {
				if (refNum != -1) {
					if (find(line, "title") && variables[refNum].value2.size() > variables[refNum].name.size() && amountOfBrackets > 0 ) {
						newEvent(found, variables[refNum], -2);
						continue;
					} 
				}
				else newVar(found);
            }

			if (debug) std::cout << colorText(path+":"+std::to_string(numOfLines)+": (bracketCount - "+std::to_string(amountOfBrackets)+")", COMPILER_DEBUG) << std::endl;
        }
		else { continue; }
		
		if (!found) {
			errorReport(numOfLines, path, amountOfBrackets);
			break;
		}
    }
	fileBuffer2 << "}";

    if (!stop) std::cout << colorText("Finished compiling "+std::to_string(numOfLines)+" lines from ", COMPILER_CORRECT) << "'" << colorText(path, COMPILER_WARNING) << "'" << colorText(" successfully", COMPILER_CORRECT) << std::endl;
}

int main(int argc, char** argv) { 
	if (argc <= 1) { std::cout << colorText("Error", COMPILER_ERROR) << ": No input files provided" << std::endl; return 0;  }
	else {
		for (int i=1; i<argc; i++) {
			std::string arg = (std::string)argv[i];
			if (arg == "-debug") { debug=true; continue; }
			else { filename=arg; } 
		}
	}
	refreshFolder("build", false);

    if (std::filesystem::exists(filename) && !std::filesystem::is_directory(filename)) {
        std::replace( filename.begin(), filename.end(), '\\', '/' );

        std::vector<std::string> list = split(filename, "/");
        std::string folder;
        for (int i=0; i<list.size()-1; i++) folder += list[i]+"/"; 

        compileFile(filename, folder);

        if (fileExists("build/interface/new_eventspictures.gfx")) {
            std::fstream gfxFile = createFile("build/interface/new_eventspictures.gfx", "", std::ios::app); 
            gfxFile << "\n}";
        }
		if (debug) { debugMode();}
    }
	else if (std::filesystem::is_directory(filename)) {
        std::cout << colorText("Error: ", COMPILER_ERROR) << colorText(argv[1], COMPILER_WARNING) << ": Must specify a file, can't compile a directory" << std::endl; 
    }
    else {
        std::cout << colorText("Error: ", COMPILER_ERROR) << colorText(argv[1], COMPILER_WARNING) << ": No such file or directory" << std::endl; 
    }
    return 0;
}