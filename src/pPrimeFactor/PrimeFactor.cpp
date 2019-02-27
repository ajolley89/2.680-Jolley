/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "PrimeFactor.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PrimeFactor::PrimeFactor()
{
  m_num_recieved = 1;
  m_num_calc = 1;

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
      m_input_string = msg.GetString();
      uint64_t m_input_int = strtoul(m_input_string.c_str(), NULL, 0);
      PrimeEntry m_new_entry;
      m_new_entry.setOriginalVal(m_input_int);
      m_new_entry.setRecievedIndex(m_num_recieved);
      m_num_recieved++;

      m_list_of_entries.push_back(m_new_entry);
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
  list<PrimeEntry>::iterator p;
  for (p=m_list_of_entries.begin(); p!=m_list_of_entries.end();) {
    
    PrimeEntry& current_entry = *p; //pull the first entry off the list and make it the current_entry

    current_entry.factor(100000); //perform 100,000 steps towards calculating primes.

    if(current_entry.done()){ //if all the primes for the current_entry have been determined
      current_entry.setCalculatedIndex(m_num_calc);
      m_num_calc++;
      Notify("PRIME_RESULT", current_entry.getReport()); //Format and publish a report of primes and other data
      p = m_list_of_entries.erase(p); //Remove the completed entry from the list
    }
    p++; //incriment to the next item on the list
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
   Register("PRIME_RESULT",0);
}

