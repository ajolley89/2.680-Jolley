/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle_ajolley.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef CommunicationAngle_ajolley_HEADER
#define CommunicationAngle_ajolley_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <cmath>

double const surface_sound_speed = 1480; // meters/second
double const sound_speed_gradient = 0.016; // (meters/sectod)/meter
double const water_depth = 6000; // meters


class CommunicationAngle_ajolley : public AppCastingMOOSApp
{
 public:
   CommunicationAngle_ajolley();
   ~CommunicationAngle_ajolley();


 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   double defineCircle();
   double CalcTL();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   std::string m_collaborator_name;
   double m_own_x;
   double m_own_y;
   double m_own_depth;
   double m_own_heading;
   double m_collaborator_x;
   double m_collaborator_y;
   double m_collaborator_depth;
   double m_theta_0;
   double m_theta_max;
   double m_transmission_loss;
   double m_connectivity_x;
   double m_connectivity_y;
   double m_connectivity_depth;
   double m_own_range_from_center;
   double m_own_depth_from_center;
   double m_max_range;
   double m_radius;
   double c_z0;
   double m_horizontal_range;

 private: // Configuration variables

 private: // State variables
};

#endif 
