#include "firebird.h"

#ifdef _MSC_VER
#pragma warning (disable: 4786)	// debug identifier truncated
#endif

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

bool FirebirdConfig::doesSysKeyExist(const FirebirdConfig::string& key) {
  return sysConfig.doesKeyExist(key);
}

//-----------------------------------------------------------------------------
//
//
//

FirebirdConfig::string FirebirdConfig::getSysString(const FirebirdConfig::string& key) {
  return sysConfig.getString(key);
}

//-----------------------------------------------------------------------------
//
//
//

int    FirebirdConfig::getSysInt(const FirebirdConfig::string& key) {
  return sysConfig.getInt(key);
}

//-----------------------------------------------------------------------------
//
//
//

bool    FirebirdConfig::getSysBoolean(const FirebirdConfig::string& key) {
  return sysConfig.getBoolean(key);
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
const Firebird::string FirebirdConfig::getSysConfigFile() {
    return sysConfig.getConfigFile();
}

//-----------------------------------------------------------------------------
//
//
//
void FirebirdConfig::setSysConfigFile(const FirebirdConfig::string& newFile) {
    sysConfig.setConfigFile(newFile);
}

