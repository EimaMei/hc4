#include "../../include/functions.hpp"
#include "../../include/compiler.hpp"

void hc4::debugMode() {
    std::cout << colorText("\n============ DEBUG INFORMATION ============\n", COMPILER_DEBUG) << "Variables:\n";
		for (int i=0; i<variables.size(); i++) { 
			std::cout <<	"  " << colorText(variables[i].name, COMPILER_CORRECT) << " (" << colorText(printType(variables[i].type), COMPILER_TYPE) << "):\n    " <<
							colorText("Value 1: ", COMPILER_WARNING) << colorText(variables[i].value, COMPILER_CORRECT) << "\n    " <<
							colorText("Value 2: ", COMPILER_WARNING) << colorText(variables[i].value2, COMPILER_CORRECT)  << std::endl; 
		}
}

int hc4::newEvent(bool& found, variable &eventVar, int i/*=-1*/) {
	if (find(line, "event.new")) {
		variable var;
    	var.name=words[1];
		var.type=EVENT;
    	createFolder("build/events", false);
		createFolder("build/localisation", false);
		createFolder("build/gfx/event_pictures", false);
		createFolder("build/interface", false);

		fileBuffer1 = createFile("build/localisation/new_events_l_english.yml", "\xEF\xBB\xBFl_english:\n  ", std::ios::app); 

    
        removeSpaces(equals[2]);

        var.value = split(equals[2], "\"")[0];
        std::string n = split(var.value, ".")[0];

        fileBuffer2 = createFile("build/events/new_"+n+".txt", "add_namespace = "+n+"\n", std::ios::app);
        fileBuffer2 << "\n\ncountry_event = {\n" <<
					  "    id = " << var.value << "\n" <<
					  "    title = " << var.value << ".t\n" <<
					  "    desc = " << var.value << ".d\n\n" <<
					  "    is_triggered_only = yes\n";
		variables.insert(variables.end(), var);
		found=true;
    }
	if (i != -1) {
		if (equals[1][0] == ' ') equals[1] = equals[1].substr(1,  equals[1].size()-1);
		if (equals[1][equals[1].size()-1] == ' ') equals[1].pop_back();

		if (!find(equals[1], "\"")) {
			for (int ii=0; ii<variables.size(); ii++) { 
				if (variables[ii].name == equals[1]) {  equals[1]=variables[ii].value; break;  }
			} 
		}
		std::string msg;
		bool writeToLoc = true;

		if (i == -2) {
			std::string option = split(eventVar.value2, ".")[2];
			msg = eventVar.value + "." + option + ":0 " + equals[1] + "\n  ";
		}

		if (find(line, eventVar.name+".title")) {
			msg = eventVar.value + ".t:0 " + equals[1] + "\n  "; 
		}

		else if (find(line, eventVar.name+".description")) {
			msg = eventVar.value + ".d:0 " + equals[1] + "\n  "; 
		}

		else if (find(line, eventVar.name+".picture")) {
            bool search=true;
            std::string path = replace(equals[1], '/', '\\');
			std::string ddspath;

            for (int n=0; n<mentionedPaths.size(); n++) { if (mentionedPaths[n] == path) { search=false; break;  } }

            std::vector<std::string> list = split(path, "\\");
            std::string filename = split(list[list.size()-1], ".")[0];

            if (search) {
				for (int ii=0; ii<list.size()-1; ii++) { ddspath += list[ii] + "\\"; }

				terminal("tools\\texconv.exe "+equals[1]+" -ft dds -f DXT1 -m 1 -y && mv "+ddspath+filename+".dds\" build\\gfx\\event_pictures\\");

                std::fstream gfxFile = createFile("build/interface/new_eventspictures.gfx", "spriteTypes = {\n", std::ios::app); 
                gfxFile <<  "    spriteType = {\n" <<
                            "        name = \"GFX_event_"+filename+"\"\n" <<
                            "        texturefile = \"gfx/event_pictures/"+filename+".dds\"\n"
                            "    }";
				gfxFile.close();
            }
            fileBuffer2 << "    picture = GFX_event_" << filename << "\n";

			writeToLoc=false;
            mentionedPaths.insert(mentionedPaths.end(), path);
		}

		else if (find(line, eventVar.name+".option")) {
			refNum=i;
			eventVar.value2 = removeSpaces(equals[0]);
			std::string option = split(eventVar.value2, ".")[2];

			fileBuffer2 << "\n    option = {\n        name = " << eventVar.value << "." << option << "\n    ";
		}

		if (writeToLoc) fileBuffer1 << msg;
	}

	return 0;
}

void hc4::errorReport(int numOfLines, std::string path, int amountOfBrackets) {
	if (find(line, "}") && amountOfBrackets >= 0) {}
	else if (!isWhitespace(line)) {
		std::cout << colorText("Error: ", COMPILER_ERROR) << colorText(path+":"+std::to_string(numOfLines), COMPILER_WARNING) << ": Undefined error\n";

		std::string buf;
		std::string wavey="";
		for (int i=0; i< 4-int(std::to_string(numOfLines).size()); i++) buf += " ";
		
		for (int i=0; i<line.size()-1; i++) {
			if (line.at(i) == ' ' && isWhitespace(wavey)) wavey += ' ';
			else {
				if (isWhitespace(wavey)) wavey += "^";
				wavey+="~";
			}
		}
		std::cout << buf << numOfLines << " | "<< line << "\n" << buf << "   | " << colorText(wavey, COMPILER_ERROR) << "\n" << buf << "   |" <<  std::endl;
		stop=true;
	}
}
//  41 
void hc4::newVar(bool& found) {
	variable var;

	var.name=equals[0];
	var.value=equals[1].substr(1,  equals[1].size()-1);

	if (var.name.back() == ' ') var.name.pop_back();
    if (find(line, "\"")) { var.type=STRING; }
    else { var.type=INTEGER; }

    variables.insert(variables.end(), var);
	found=true;
}

namespace hc4 {
	std::vector<variable> variables;
    std::vector<std::string> mentionedPaths;

	std::string line=line;

    std::fstream fileBuffer1;
    std::fstream fileBuffer2;
    std::fstream fileBuffer3;

    int refNum=-1;
    bool stop=false;

    std::vector<std::string> words;
    std::vector<std::string> equals;
};