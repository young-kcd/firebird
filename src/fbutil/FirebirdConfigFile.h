
#ifndef FIREBIRDCONFIGFILE_H
#define FIREBIRDCONFIGFILE_H

#include <map>
#include "../include/fb_string.h"

#include "FirebirdConfig.h"


class FirebirdConfigFile : public FirebirdConfig
{
public:
    FirebirdConfigFile();

    const string getConfigFile() { return configFile; }
    void setConfigFile(const string& newFile) { configFile = newFile; }

    bool getIsLoadedFlg() { return isLoadedFlg; }
    void setIsLoadedFlg(bool newFlg) { isLoadedFlg = newFlg; }

    virtual void loadConfig();
    virtual void checkLoadConfig();
    virtual string getString(const string& key);
    virtual int    getInt(const string& key);
    virtual bool    getBoolean(const string& key);

private:
    string configFile;
    bool isLoadedFlg;
    typedef std::map<string, string> mymap_t;

    mymap_t parameters;

};

#endif	// FIREBIRDCONFIGFILE_H
