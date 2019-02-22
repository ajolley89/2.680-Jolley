/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: PrimeFactor.h                                          */
/*    DATE:                                                 */
/************************************************************/

#ifndef PrimeFactor_HEADER
#define PrimeFactor_HEADER
#include <list>
#include <string>
#include "MOOS/libMOOS/MOOSLib.h"
#include <cstdlib>
#include <cstdint>
#include <sstream>
#include "MBUtils.h"

using namespace std;

class PrimeFactor : public CMOOSApp
{
 public:
   PrimeFactor();
   ~PrimeFactor();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   string m_New_Number;
   list<string> m_List_of_Numbers;
   string m_number_result;
   stringstream m_prime_factors;

 private: // Configuration variables

 private: // State variables
};

#endif 
