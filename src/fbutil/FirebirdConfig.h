
#ifndef _FirebirdConfig_H
#define _FirebirdConfig_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>


class FirebirdConfig {
public:
  static string getSysString(const string& key);
  static int    getSysInt(const string& key);
  static void   loadSysConfig();

  void loadConfig();
  string getString(const string& key);
  int    getInt(const string& key);

private:
  typedef pair<string, string> mypair;
  typedef map<string, string> mymap_t;

  mymap_t parameters;
  
  static FirebirdConfig sysConfig;

};

#endif
