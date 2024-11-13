// -*- coding: utf-8 -*-
// -*- file-name: TConf.cpp -*-
// -*- mode: c++ -*-
// -*- created: 2012-10-29T11:21:55.383022 -*-
// -*- author: Greg Henning  -*-
//

#include "TConf.h"

// This file defines a class to read a configuration file with items, flags and lists
// Typical conf file is like this:
/**
# comments with '#' at the start
# flags

flag flag_name

#numerical value (int)
int name = value
# numerical value (float)
float name = value

#string
string name = "value"

# Lsits are not defined yet
#list of int
int list name = {
1,
2,
3,
4,
}

#of string
string list name = {
one
two
three
}

 **/
TConf::TConf(const char* conffile, const int e)
{
  // Constructor: Initializes the TConf object with the configuration file path and echo flag
  // const char* conffile: file to read
  // const int e: echo flag (0 or 1).
  echo=e;
  fconffile = new char[500];
  strcpy(this->fconffile,conffile);

  stringstream inf;

  AddFile(fconffile);
  if (echo)
    Dump();
}

void TConf::SelfCheck()
{
  // Checks if the file stream is in a good state, and reloads the configuration if needed
  if (!inf.good())
    {
      inf.clear();//clear any bits set
      inf.str(string());
      if (echo)
	      cout << "# [CONFIG] Reloading file " << fconffile <<endl;
      AddFile(fconffile);
    }
}

TConf::~TConf()
{
  // Destructor: Cleans up resources when the object is destroyed
  if (echo)
    cout << "closing configuration file" <<endl;
  // Memory clean-up (commented out lines could be revisited if needed)
  //delete fconffile;
  //inf.close();
}

void TConf::AddFile(const char* f)
{
  // Adds a configuration file to be processed and read
  ifstream addedf (f, ifstream::in);
  char buffer[2048];
  char followfile[1024];
  while(addedf.good())
    {
      addedf.getline(buffer, 2048);
      if  (sscanf(buffer, " include \"%[^\t\n\"]", followfile)==1)
	    {
        // Check if the line includes another configuration file
	      if (echo)
	         cout << "Including file " << followfile <<endl;
	      AddFile(followfile);
	    }
      else
	      inf << buffer << endl; // Otherwise, add the line to the string stream
  }
}

void TConf::Dump()
{
  // Dumps the entire configuration content (useful for debugging)
  cout << inf.str() << endl;
}


// I provide a function that search thru the file and find the property
//               or to the default given
// for example: int number1 = ReadInt("number1", default_value=0) will set number1 to the value found in the conf file

const int TConf::ReadInt(const char* name, const int def)
 {
  // Reads an integer value from the configuration file, with a default fallback
   SelfCheck();
   int outval = def;
   char buffer[256];
   char nvar[256];
   // open the file from the beginning
   inf.seekg(0, inf.beg); //ifstream inf ( this->fconffile , ifstream::in );
   // start matching lines
   while (inf.good())
     {
       inf.getline(buffer, 256);
       if (sscanf(buffer, " int %s = %i", nvar, &outval)==2)
	 if (strcmp(nvar, name)==0 )
	   {
	     if (echo)
	       cout << "# [CONFIG] (int)" << name << " = " << outval <<endl;
	     return outval;
	   }
     }
   return def;
 }

// Similar logic is applied to ReadFloat, ReadString, ReadFlag, and the list-reading functions

const float TConf::ReadFloat(const char* name, const float def)
 {
   SelfCheck();
   float outval = def;
   char buffer[256];
   char nvar[256];
   // open the file from the beginning
   inf.seekg(0, inf.beg); // ifstream inf ( this->fconffile , ifstream::in );
   // start matching lines
   while (inf.good())
     {
       inf.getline(buffer, 256);
       //cout << "/ float // " << buffer <<endl;
       if (sscanf(buffer, " float %s = %f", nvar, &outval)==2)
	 if (strcmp(nvar, name)==0 )
	   {
	     if (echo)
	       cout << "# [CONFIG] (float)" << name << " = " << outval <<endl;
	     return outval;
	   }
     }
   return def;
 }

const int TConf::ReadFlag(const char* name)
 {
   SelfCheck();
   char buffer[256];
   char nvar[256];
   // open the file from the beginning
   inf.seekg(0, inf.beg); //ifstream inf ( this->fconffile , ifstream::in );
   // start matching lines
   while (inf.good())
     {
       inf.getline(buffer, 256);
       //cout << "/ flag // " << buffer <<endl;
       if (sscanf(buffer, " flag %s", nvar)==1)
	 if (strcmp(nvar, name)==0 )
	   {
	     if (echo)
	       cout << "# [CONFIG] (flag)" << name << " YES" <<endl;
	     return 1;
	   }
     }
   if (echo)
     cout << "# [CONFIG] (flag)" << name << " no" <<endl;
   return 0;
 }


const char* TConf::ReadString(const char* name, const char* def)
 {
   SelfCheck();
   char *outval;
   outval = (char*) calloc(1024, sizeof(char));
   char buffer[256];
   char nvar[256];
   // open the file from the beginning
   inf.seekg(0, inf.beg); //ifstream inf ( this->fconffile , ifstream::in );
   // start matching lines
   while (inf.good())
     {
       inf.getline(buffer, 256);
       if (sscanf(buffer, " string %s = \"%[^\t\n\"] ", nvar, outval)==2)
	 if (strcmp(nvar, name)==0 )
	   {
	     if (echo)
	       cout << "# [CONFIG] (string)" << name << " = " << outval <<endl;
	     return outval;
	   }
     }
   return def;
 }

// ReadListString, ReadListFloat and ReadListInt return the element of index `idx` in a list

const char* TConf::ReadListString(const char* name, const int idx, const char* def)
 {
   SelfCheck();
   char *outval;
   outval = (char*) calloc(256, sizeof(char));
   char buffer[256];
   char nvar[256];
   // open the file from the beginning
   inf.seekg(0, inf.beg); //ifstream inf ( this->fconffile , ifstream::in );
   // start matching lines
   while (inf.good())
     {
       int linebeginning = inf.tellg();
       inf.getline(buffer, 256);
       if (sscanf(buffer, " string list %s = {", nvar)==1)
	 if (strcmp(nvar, name)==0 )
	   {
	     // in the list of interrest
	     inf.seekg(linebeginning);
	     inf.getline(buffer, 256, '{');
	     int n = 0;
	     while (inf.good())
	       {
		 inf.getline(buffer, 256,',');
		 if (strcmp(buffer, "}")==0)
		   return def ;
		 if (n==idx)
		   {
		     if (sscanf(buffer, " \"%[^\t\n\"] ", outval)==1)
		       return outval;
		     else
		       return def;
		   }
		 n++;
	       }
	     return def;
	   }
     }
   return def;
 }

const float TConf::ReadListFloat(const char* name,
				 const int idx,
				 const float def)
 {
   SelfCheck();
   float outval = def;
   char buffer[256];
   char nvar[256];
   // open the file from the beginning
   inf.seekg(0, inf.beg); //ifstream inf ( this->fconffile , ifstream::in );
   // start matching lines
   while (inf.good())
     {
       int linebeginning = inf.tellg();
       inf.getline(buffer, 256);
       if (sscanf(buffer, " float list %s = {", nvar)==1)
	 if (strcmp(nvar, name)==0 )
	   {
	     // in the list of interrest
	     //cout << "% " << buffer <<endl;
	     inf.seekg(linebeginning);
	     inf.getline(buffer, 256, '{');//inf.ignore('{');
	     int n = 0;
	     while (inf.good())
	       {
		 inf.getline(buffer, 256, ',');
		 //cout << "** "<< buffer <<endl;
		 if (strcmp(buffer, "}")==0)
		   return def ;
		 if (n==idx && sscanf(buffer, " %f ", &outval)==1)
		   return outval;
		 n++;
	       }
	     return def;
	   }
     }
   return def;
 }


const int TConf::ReadListInt(const char* name, const int idx, const int def)
 {
   SelfCheck();
   int outval = def;
   char buffer[256];
   char nvar[256];
   // open the file from the beginning
   inf.seekg(0, inf.beg); //ifstream inf ( this->fconffile , ifstream::in );
   // start matching lines
   while (inf.good())
     {
       int linebeginning = inf.tellg();
       inf.getline(buffer, 256);
       if (sscanf(buffer, " int list %s = {", nvar)==1)
	 if (strcmp(nvar, name)==0 )
	   {
	     // in the list of interrest
	     inf.seekg(linebeginning);
	     inf.getline(buffer, 256, '{');
	     int n = 0;
	     while (inf.good())
	       {
		 inf.getline(buffer, 256, ',');
		 if (strcmp(buffer, "}")==0)
		   return def ;
		 if (n==idx && sscanf(buffer, "%i,", &outval)==1)
		   return outval;
		 n++;
	       }
	     return def;
	   }
     }
   return def;
 }
