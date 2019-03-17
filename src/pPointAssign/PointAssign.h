/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
#include "MBUtils.h"


class PointAssign : public AppCastingMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();

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
   void m_assign_alternating(std::string point);
   void m_assign_regionally(std::string point);
   void postViewPoint(double x, double y, std::string label, std::string color);
   std::string point;
   std::string m_visit_point;
   int m_id;
   int m_x;
   int m_y;
   std::string m_color;

 private: // Configuration variables
   bool assign_by_region;
   std::string m_boat1_name;
   std::string m_boat2_name;

 private: // State variables
};

#endif 
