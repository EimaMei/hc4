#include "../../include/functions.hpp"
#include "../../include/compiler.hpp"

bool hc4::areBracketsBalanced(std::string path) {
	// Open the file
	std::ifstream file(path, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	// Read the entire file
	std::vector<char> buffer(size);
	file.read(buffer.data(), size);

	// Important variables
	std::string stack;
	int lineNum=0;
	int lineCount=0;
	std::string line;
	std::string lastBracketLine;
	int lastLineCount=0;
	bool hashtag=false;

	for (int i=0; i<buffer.size(); i++) {
		if (buffer[i] == '\n') {
			if (!hashtag) {
				line="";
				for (int n=lineNum+1; n<i; n++) { line += buffer[n]; }
				if (find(line, "{")) { lastBracketLine=line; lastLineCount=lineCount+1; }
			}
			lineCount++;
			lineNum=i+1;
			hashtag=false;
		}
		if (hashtag) continue;
		 
		if (buffer[i] == '#') { hashtag=true; continue; }
		else if (buffer[i] == '{') { stack.append("{"); }
		else {
			if (stack.empty() && buffer[i] == '}') {
				hc4::line=line;
				errorReport(lineCount, path, "Expected '{' before line "+std::to_string(lineCount) );
				return false;
			}
			if (!stack.empty() && buffer[i] == '}') stack.pop_back();
		}	
	}
	if (!stack.empty()) {
		hc4::line = line;
		errorReport(lineCount, path, "Expected '}' at the end of line "+std::to_string(lineCount), line.size(), MISSING_BRACKET_2);
		hc4::line = lastBracketLine;
		errorReport(lastLineCount, path, "Make sure to match this '{' with the missing '}' at line "+std::to_string(lastLineCount), line.size(), MISSING_BRACKET_2, COMPILER_NOTE); 
	}
	return stack.empty();
}

void hc4::debugMode() {
    std::cout << colorText("\n============ DEBUG INFORMATION ============\n", COMPILER_DEBUG) << "Variables:\n";
		for (int i=0; i<variables.size(); i++) { 
			std::cout <<	"  " << colorText(variables[i].name, COMPILER_CORRECT) << " (" << colorText(printType(variables[i].type), COMPILER_TYPE) << "):\n    " <<
							colorText("Value 1: ", COMPILER_WARNING) << colorText(variables[i].value, COMPILER_CORRECT) << "\n    " <<
							colorText("Value 2: ", COMPILER_WARNING) << colorText(variables[i].value2, COMPILER_CORRECT)  << std::endl; 
		}
}

bool hc4::newEvent(bool& found, variable &eventVar, int i/*=-1*/, int numOfLines/*=0*/, std::string path/*=""*/) {
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
		bool valid=false;
		if (equals[1][0] == ' ') equals[1] = equals[1].substr(1,  equals[1].size()-1);
		if (equals[1][equals[1].size()-1] == ' ') equals[1].pop_back();

		if (!find(equals[1], "\"")) {
			for (int ii=0; ii<variables.size(); ii++) { 
				if (variables[ii].name == equals[1]) {  equals[1]=variables[ii].value; valid=true; break;  }
			} 
		}
		else {valid=true;}

		std::string msg;
		bool writeToLoc = true;

		if (valid) {
			if (find(line, eventVar.name+".title")) {
				msg += eventVar.value + ".t:0 " + equals[1] + "\n  "; 
			}
			else if (find(line, "title")) {
				std::string option = split(eventVar.value2, ".")[2];
				msg += eventVar.value + "." + option + ":0 " + equals[1] + "\n  ";
			}

			else if (find(line, eventVar.name+".description")) {
				msg += eventVar.value + ".d:0 " + equals[1] + "\n  "; 
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

					terminal("tools\\texconv.exe "+equals[1]+" -ft dds -m 1 -y && mv "+ddspath+filename+".dds\" build\\gfx\\event_pictures\\");

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
		}

		if (find(line, eventVar.name+".option")) {
			refNum=i;
			valid=true;
			eventVar.value2 = removeSpaces(equals[0]);
			std::string option = split(eventVar.value2, ".")[2];

			fileBuffer2 << "\n    option = {\n        name = " << eventVar.value << "." << option << "\n    ";
		}

		if (!valid) {
			stop=true; 
			errorReport(numOfLines, path, "Undefined variable '"+equals[1]+"' ", equals[1].size()); 
			return false;
		}
		else found=true;

		if (writeToLoc) fileBuffer1 << msg;
	}

	return true;
}

void hc4::errorReport(int numOfLines, std::string path, std::string error, int waveyLength/*=line.size()*/, compilerError type/*=UNDEFINED_VARIABLE*/, textType msgType/*=COMPILER_ERROR*/) {
	switch (msgType) {
		case COMPILER_ERROR: colorText("Error: ", COMPILER_ERROR); break;
		case COMPILER_NOTE: colorText("Note: ", COMPILER_NOTE); break;
		case COMPILER_WARNING: colorText("Warning: ", COMPILER_DEBUG); break;

		default: break;
	}

	std::cout << colorText(path+":"+std::to_string(numOfLines), COMPILER_WARNING) << ": "+error+"\n";

	std::string buf,buf2;
	std::string wavey="";
	int n = std::to_string(numOfLines).size();
	for (int i=0; i< 6; i++) buf += ' ';
	buf2 = buf.substr(0,  buf.size()-n*2)+std::to_string(numOfLines)+buf.substr(0, n);

	if (line.size() > waveyLength) { for (int i=0; i<(line.size()-waveyLength); i++) { wavey += ' '; } }

	for (int i=0; i < waveyLength; i++) {
		if (type==UNDEFINED_VARIABLE) {
			if (line.at(i) == ' ' && isWhitespace(wavey)) wavey += ' ';
			else {
				if (isWhitespace(wavey)) { wavey += '^'; }
				else wavey+='~';
			}
		}
		else if ( type == MISSING_BRACKET_2) {
			if (i==waveyLength-2) { wavey += '^'; }
			else { wavey += ' '; }
		}
	}

	std::cout << buf2 << "| "<< line << "\n" << buf << "| " << colorText(wavey, msgType) << "\n" << buf << "|" <<  std::endl;
	stop=true;
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

int hc4::hashtags(std::string folder) {
	if (find(line, "#include")) {
        std::string f = folder+split(line, "\"")[1];

        compileFile(f, folder);
		return 1;
    }
    else if (find(line, "#")) {
        std::string n;
        for(int i = 0; i < line.size(); i++) {
            if (line.at(i) == '#') break;
            n += line.at(i);
        }
        line=n;
    }

	return 0;
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