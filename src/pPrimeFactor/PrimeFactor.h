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
#include "PrimeEntry.h"

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
   string m_input_string;
   uint64_t m_input_int;
   PrimeEntry m_new_entry;
   int m_num_recieved;
   int m_num_calc;
   list<PrimeEntry> m_list_of_entries;

 private: // Configuration variables

 private: // State variables
};

#endif 
