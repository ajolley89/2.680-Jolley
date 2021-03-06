/************************************************************/
/*    NAME: Austin Jolley                                   */
/*    ORGN: MIT                                             */
/*    FILE: TempEntry.cpp                                  */
/*    DATE:                                                 */
/************************************************************/

#include "TempEntry.h"
using namespace std;

//-----------------------------------------------------------
// Constructor
TempEntry::TempEntry()
{
    m_sent_flag = false; 
}

//-----------------------------------------------------------
// Destructor
TempEntry::~TempEntry()
{
}

void TempEntry::parseMessage(string str)
{
  vector<string> svector = parseString(str, ',');
  unsigned int i, vsize = svector.size();
  for(i=0; i<vsize; i++) {
    string param = biteStringX(svector[i], '=');
    string str_value = svector[i];
    double value = stod(str_value);
    if(param == "vname")
      setVname(str_value);
    else if(param == "utc")
      setUTC(value);
    else if(param == "x")
      setX(value);
    else if(param == "y")
      setY(value);
    else if(param == "temp")
      setTemp(value);
  }
}

void TempEntry::setVname(string v)
{
    m_vname = v;
    if(m_vname == "archie"){
        m_other_vname = "betty";
    }
    else if(m_vname == "betty") {
        m_other_vname = "archie";
    }
}

string TempEntry::genMessage()
{
    string message;
    message = "vname=" + m_other_vname + ",utc=" + to_string(m_utc) + ",x=" + to_string(m_x) + ",y=" + to_string(m_y) + ",temp=" + to_string(m_temp);
    return(message);
}
