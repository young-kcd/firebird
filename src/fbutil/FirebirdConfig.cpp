#include "firebird.h"
#include "FirebirdConfig.h"
#include "FirebirdConfigFile.h"

#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif



//-----------------------------------------------------------------------------
//
//  The instance of the static class variable.
//


static FirebirdConfigFile sysConfig;
//static FirebirdConfigReg  sysConfig;  // I would expect win32 to have this one.

//-----------------------------------------------------------------------------
//
//
//

string FirebirdConfig::getSysString(const string& key) {
  return sysConfig.getString(key);
}

//-----------------------------------------------------------------------------
//
//
//

int    FirebirdConfig::getSysInt(const string& key) {
  return sysConfig.getInt(key);
}


//-----------------------------------------------------------------------------
//
//
//
void FirebirdConfig::loadSysConfig() {
    sysConfig.loadConfig();
}

//-----------------------------------------------------------------------------
//
//
//
const string FirebirdConfig::getSysConfigFile() {
    return sysConfig.getConfigFile();
}

//-----------------------------------------------------------------------------
//
//
//
void FirebirdConfig::setSysConfigFile(const string& newFile) {
    sysConfig.setConfigFile(newFile);
}

