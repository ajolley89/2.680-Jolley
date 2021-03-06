/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
#include "XYSegList.h"

class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   void m_parse_string(std::string point);
   void SortWaypoints();
   int m_id;
   int m_x;
   int m_y;
   double m_start_x;
   double m_start_y;
   std::string point;
   std::string m_visit_point;
   XYSegList m_waypoints;
   bool m_sorted;

 private: // Configuration variables
   std::string m_vehicle_name;
   double visit_radius;

 private: // State variables
   double m_nav_x;
   double m_nav_y;
   std::string update_str;

};

#endif 
