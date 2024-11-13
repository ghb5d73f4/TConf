// -*- coding: utf-8 -*-
// -*- file-name: TConf.h -*-
// -*- mode: c++ -*-
// -*- created: 2012-10-29T11:21:55.411225 -*-
// -*- author: Greg Henning  -*-
//

#ifndef TCONF_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <sstream>


using namespace std;

class TConf
{
 public:
  ~TConf();
  TConf(const char* conffile, const int e=0);

  const int ReadInt(const char* name, const int def=0);
  const float ReadFloat(const char* name, const float def=0.0);
  const char* ReadString(const char* name, const char* def="");
  const int ReadFlag(const char* name);

  const int ReadListInt(const char* name, const int idx, const int def=0.0);
  const float ReadListFloat(const char* name, const int idx, const float def=0.0);
  const char* ReadListString(const char* name, const int idx, const char* def="");
  int echo;
 private:
  void AddFile(const char *f);
  void Dump();
  void SelfCheck();
  char* fconffile;
  stringstream inf;
};

#define TCONF_H
#endif
