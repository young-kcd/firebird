
#ifndef _FirebirdConfig_H
#define _FirebirdConfig_H

#include <iostream>
#include <fstream>
#include <string>


class FirebirdConfig {
public:

    static string getSysString(const string& key);
    static int    getSysInt(const string& key);
    static void   loadSysConfig();
    static const string getSysConfigFile();
    static void setSysConfigFile(const string& newFile);


    FirebirdConfig() {}
    virtual const string getConfigFile()= 0;
    virtual void setConfigFile(const string& newFile)= 0;

    virtual bool getIsLoadedFlg()= 0;
    virtual void setIsLoadedFlg(bool newFlg)= 0;

    virtual void loadConfig()= 0;
    virtual void checkLoadConfig()= 0;
    virtual string getString(const string& key) = 0;
    virtual int    getInt(const string& key) = 0;

    virtual ~FirebirdConfig() {}

};

#endif
