
#ifndef FIREBIRDCONFIGFILE_H
#define FIREBIRDCONFIGFILE_H

#include <functional>
#include <map>
#include "../include/fb_string.h"

#include "FirebirdConfig.h"

class FirebirdConfigFile : public FirebirdConfig
{
	class key_compare : public std::binary_function<string, string, bool>
	{
	public:
		key_compare() : isCaseSensitive(CASE_SENSITIVITY) {}
		bool operator()(const string& x, const string& y) const;
	private:
		bool isCaseSensitive;
		void uppercase(string& x) const;
	};

public:
    FirebirdConfigFile();

    const string getConfigFile() { return configFile; }
    void setConfigFile(const string& newFile) { configFile = newFile; }

    bool getIsLoadedFlg() { return isLoadedFlg; }
    void setIsLoadedFlg(bool newFlg) { isLoadedFlg = newFlg; }

    virtual void loadConfig();
    virtual void checkLoadConfig();
    virtual bool doesKeyExist(const string& key);
    virtual string getString(const string& key);
    virtual int getInt(const string& key);
    virtual bool getBoolean(const string& key);

private:
    string configFile;
    bool isLoadedFlg;
    typedef std::map<string, string, key_compare> mymap_t;

    mymap_t parameters;

};

#endif	// FIREBIRDCONFIGFILE_H
