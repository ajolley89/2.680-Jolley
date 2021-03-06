/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: TempPassing.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef TempPassing_HEADER
#define TempPassing_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "TempEntry.h"
#include <list>
#include <string>

class TempPassing : public AppCastingMOOSApp
{
 public:
   TempPassing();
   ~TempPassing();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
  TempEntry m_new_entry;
  list<TempEntry> m_temperature_queue;
  unsigned int m_received_index;
  unsigned int m_sent_time;


};

#endif 
