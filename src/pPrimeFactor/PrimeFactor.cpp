/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PrimeFactor.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  m_New_Number = "0";
  m_number_result = "";
  m_prime_factors.str("");

}

//---------------------------------------------------------
// Destructor

PrimeFactor::~PrimeFactor()
{

}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PrimeFactor::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    string key = msg.GetKey();
    if(key=="NUM_VALUE") {
      m_New_Number = msg.GetString();
      m_List_of_Numbers.push_back(m_New_Number);
    }

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PrimeFactor::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PrimeFactor::Iterate()
{
  list<string>::iterator p;
  for (p=m_List_of_Numbers.begin(); p!=m_List_of_Numbers.end();) {
    
    string str = *p;
    m_prime_factors.str("");
    
    uint64_t original = strtoul(str.c_str(), NULL, 0);
    uint64_t working = original;

    while(working%2 == 0) {
      m_prime_factors << ":2";
      working /= 2;
    }

    for(int n=3; n<sqrt(original); n+=2) {
      while(working%n == 0) {
        m_prime_factors << ":" << to_string(n);
        working /= n;
      }
    }
    if(working != 1){m_prime_factors << ":" << to_string(working);}
    p = m_List_of_Numbers.erase(p);
    p++;

    string cleanstring = m_prime_factors.str(); //erases leading ":" from string
    m_prime_factors.str(cleanstring.erase(0,1));

    Notify("NUM_RESULT", to_string(original) + " = " +  m_prime_factors.str());
  }
    
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PrimeFactor::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PrimeFactor::RegisterVariables()
{
   Register("NUM_VALUE", 0);
}

