#include "firebird.h"

#ifdef _MSC_VER
#pragma warning (disable: 4786)	// debug identifier truncated
#endif

#include "FirebirdConfigFile.h"
#include "../jrd/gds.h"
#include "../include/fb_macros.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <fstream>
#include <iostream>




//-----------------------------------------------------------------------------
//
//
//
FirebirdConfigFile::FirebirdConfigFile() 
{
    isLoadedFlg = false;
#pragma FB_COMPILER_MESSAGE("Error! This is a *nix only thing!")
    configFile = "/etc/firebird.conf";
}

//-----------------------------------------------------------------------------
//
//
//

void stripLeadingWhiteSpace(FirebirdConfig::string& s)
{
	if (!s.size()) {
		return;
	}

	const Firebird::string::size_type startPos = s.find_first_not_of(" \t");
	if (startPos == Firebird::string::npos) {
		s.erase();	// nothing but air
	} else if (startPos) {
		s = s.substr(startPos);
	}
}

//-----------------------------------------------------------------------------
//
//
//

void stripTrailingWhiteSpace(FirebirdConfig::string& s)
{
	if (!s.size()) {
		return;
	}

	Firebird::string::size_type endPos = s.find_last_not_of(" \t");
	if (endPos != std::string::npos) {
		// Note that endPos is the index to the last non-ws char
		// why we have to inc. it
		++endPos;
		s = s.substr(0, endPos);
	}
}

//-----------------------------------------------------------------------------
//
//
//

void stripComments(FirebirdConfig::string& s)
{
	// Note that this is only a hack. It won't work in case inputLine
	// contains hash-marks embedded in quotes! Not that I know if we
	// should care about that case.
	const Firebird::string::size_type commentPos = s.find('#');
	if (commentPos != Firebird::string::npos) {
		s = s.substr(0, commentPos);
	}
}


//-----------------------------------------------------------------------------
//
//
//

bool FirebirdConfigFile::doesKeyExist(const FirebirdConfig::string& key) {

    checkLoadConfig();

    Firebird::string data = getString(key);

    return !data.empty();
}


//-----------------------------------------------------------------------------
//
//
//

FirebirdConfig::string FirebirdConfigFile::getString(const FirebirdConfig::string& key) {

    checkLoadConfig();

    mymap_t::iterator lookup;

    lookup = parameters.find(key);

    if (lookup != parameters.end()) {
      return lookup->second;
    }

    return FirebirdConfig::string();
}

//-----------------------------------------------------------------------------
//
//
//

int    FirebirdConfigFile::getInt(const FirebirdConfig::string& key) {

    checkLoadConfig();

    Firebird::string data = getString(key);

    if (data.empty()) {
        return 0;
    }

    return atoi(data.data());
}


//-----------------------------------------------------------------------------
//
//
//

bool FirebirdConfigFile::getBoolean(const FirebirdConfig::string& key) {

    checkLoadConfig();

    string data = getString(key);

    if (data.empty()) {
        return false;
    }

    int result =  atoi(data.data());

    if (result != 0) {
      return true;
    }

    return false;

}

//-----------------------------------------------------------------------------
//
//
//

FirebirdConfig::string parseKeyFrom(const FirebirdConfig::string& inputLine,
		FirebirdConfig::string::size_type& endPos) {

    endPos = inputLine.find_first_of("= \t");
    if (endPos == Firebird::string::npos) {
        return inputLine;
    }

    return inputLine.substr(0, endPos);
}



//-----------------------------------------------------------------------------
//
//
//

FirebirdConfig::string parseValueFrom(FirebirdConfig::string inputLine,
			FirebirdConfig::string::size_type initialPos) {

    if (initialPos == Firebird::string::npos) {
        return Firebird::string();
    }

    // skip leading white spaces
    unsigned int startPos = inputLine.find_first_not_of("= \t", initialPos);
    if (startPos == Firebird::string::npos) {
        return Firebird::string();
    }

    stripTrailingWhiteSpace(inputLine);
    return inputLine.substr(startPos);
    
}


//-----------------------------------------------------------------------------
//
//
//

void FirebirdConfigFile::checkLoadConfig() {
  if (!isLoadedFlg) {
    loadConfig();
  }
}

//-----------------------------------------------------------------------------
//
//
//

void FirebirdConfigFile::loadConfig() {

    isLoadedFlg = true;

    std::ifstream configFile(configFile.c_str());

    if (!configFile) {
        // config file does not exist, a warning message would be nice.
        return;
    }
    string inputLine;
    
    while (!configFile.eof() ) {
		std::getline(configFile, inputLine);

		stripComments(inputLine);
		stripLeadingWhiteSpace(inputLine);
		
		if (!inputLine.size()) {
			continue;	// comment-line or empty line
		}

        //        cout << "read \"" << inputLine << "\"\n";

        if (inputLine.find('=') == string::npos) {
            std::cerr << "illegal line \"" << inputLine << "\"" << std::endl;
            continue;
        }

        string::size_type endPos;

        string key   = parseKeyFrom(inputLine, endPos);
        string value = parseValueFrom(inputLine, endPos);

        std::cout << "adding \"" << key << "\" \"" << value << "\"" << std::endl;
//        parameters.insert(pair<string, string>(key, value));
// Just to display yet another template function
        parameters.insert(std::make_pair(key, value));
    }
}

