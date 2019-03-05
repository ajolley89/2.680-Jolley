/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: CommunicationAngle_ajolley.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "CommunicationAngle_ajolley.h"

using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle_ajolley::CommunicationAngle_ajolley()
{
  
}

//---------------------------------------------------------
// Destructor

CommunicationAngle_ajolley::~CommunicationAngle_ajolley()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle_ajolley::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

  if(key == "COLLABORATOR_NAME") {                        //Gets the name of our collaboator and registers for pertinent variables
    m_collaborator_name = msg.GetString();
    Register(m_collaborator_name + "_NAV_X", 0);
    Register(m_collaborator_name + "_NAV_Y", 0);
    Register(m_collaborator_name + "_NAV_DEPTH", 0);
    Register(m_collaborator_name + "_NAV_HEADING", 0);
    Register(m_collaborator_name + "_NAV_SPEED", 0);
  }
  if(key == "NAV_X") {
    m_own_x = msg.GetDouble();
  }
  if(key == "NAV_Y") {
    m_own_y = msg.GetDouble();
  }
    if(key == "NAV_DEPTH") {
    m_own_depth = msg.GetDouble();
  }
    if(key == "NAV_HEADING") {
    m_own_heading = msg.GetDouble();
  }
    if(key == m_collaborator_name + "_NAV_X") {
    m_collaborator_x = msg.GetDouble();
  }
  if(key == m_collaborator_name + "_NAV_Y") {
    m_collaborator_y = msg.GetDouble();
  }
  if(key == m_collaborator_name + "_NAV_DEPTH") {
    m_collaborator_depth = msg.GetDouble();
  }
 /* if(key == m_collaborator_name + "_NAV_HEADING") {
    m_collaborator_heading = msg.GetDouble();
  }
  if(key == m_collaborator_name + "_NAV_SPEED") {
    m_collaborator_speed = msg.GetDouble();
  } */
  
#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle_ajolley::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool CommunicationAngle_ajolley::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!


   double angle = defineCircle();
   CalcTL();

   if(m_theta_0 > m_theta_max){ //if a satisfactory circle was not found, output NaN and find a good path
      Notify("ACOUSTIC_PATH", "elev_angle=NaN,transmission_loss=NaN,id=ajolley@mit.edu");
      //FindPath();
      //Notify("CONNECTIVITY_LOCATION","x=" + to_string(m_connectivity_x) + ",y=" + to_string(m_connectivity_y) + ",depth=" + to_string(m_connectivity_depth) + ",id=ajolley@mit.edu.")
    }
   
   if(m_theta_0 < m_theta_max){ //if we found a happy circle
      m_connectivity_x = m_own_x;
      m_connectivity_y = m_own_y;
      m_connectivity_depth = m_own_depth;
      Notify("ACOUSTIC_PATH", "elev_angle=" + to_string(angle) +",transmission_loss=" + to_string(CalcTL()) + ",id=ajolley@mit.edu");
      Notify("CONNECTIVITY_LOCATION","x=" + to_string(m_connectivity_x) + ",y=" + to_string(m_connectivity_y) + ",depth=" + to_string(m_connectivity_depth) + ",id=ajolley@mit.edu.");
    }


  AppCastingMOOSApp::PostReport();
  return(true);
}

double CommunicationAngle_ajolley::defineCircle()
{
  m_theta_0 = 0;
  c_z0 = surface_sound_speed + sound_speed_gradient * m_own_depth; //calc speed of sound at the depth of the self (c(z_0))
  m_theta_max = (180/PI)* acos(c_z0 / (water_depth*sound_speed_gradient + surface_sound_speed)); // calculated steepest transmission angle that will not impact the sea floor
  m_horizontal_range = sqrt(pow(m_own_x - m_collaborator_x,2) + pow(m_own_y - m_collaborator_y,2) );
   m_own_range_from_center = (pow(m_collaborator_depth,2)/(2*m_horizontal_range)) +
    (surface_sound_speed*m_collaborator_depth/(sound_speed_gradient*m_horizontal_range)) + ((m_horizontal_range)/2) -
    (pow(m_own_depth,2)/(2*m_horizontal_range)) - (m_own_depth*surface_sound_speed/(m_horizontal_range*sound_speed_gradient));
  m_own_depth_from_center = (surface_sound_speed / sound_speed_gradient) + m_own_depth; //our vertical distance from the circle center = depth + circle height  
  m_radius = sqrt( pow(m_own_depth_from_center,2) + pow(m_own_range_from_center,2));
  m_theta_0 = acos(c_z0/(sound_speed_gradient*m_radius));
   //m_theta_0 = atan( ( (m_horizontal_range*sound_speed_gradient) / (2*c_z0) ) + ( (surface_sound_speed*m_collaborator_depth) / (m_horizontal_range) ) + ( (pow(m_collaborator_depth,2)*sound_speed_gradient) / (2*m_horizontal_range*c_z0) ) - ( (surface_sound_speed*m_own_depth) / (c_z0*m_horizontal_range) ) - ( (pow(m_own_depth,2)*sound_speed_gradient) / (2*m_horizontal_range*c_z0)));
   //m_radius = c_z0 / (sound_speed_gradient*cos(m_theta_0)); //calculate m_radius of the transmission path
  m_max_range = m_radius*(1+cos(PI/2-m_theta_0)); //calculate the maximum transmission range for the given transmission angle

   //Notify("TROUBLESHOOT1", "m_theta_0 = " + to_string(m_theta_0) + ", m_radius=" + to_string(m_radius) + ", c_z0=" + to_string(c_z0) + ", own_range_from_center =" + to_string(m_own_range_from_center));

  return((180/PI)*m_theta_0);

}


double CommunicationAngle_ajolley::CalcTL()
{
  double r_i = m_horizontal_range;
  double delta_theta = 0.00001;
  double s_i = (asin(((r_i / m_radius) - sin(m_theta_0)))+m_theta_0)*m_radius;
  double grazing_angle = m_theta_0 - (s_i / m_radius);
  double r_i1= m_radius*(sin(m_theta_0+delta_theta)+sin(s_i/m_radius - (m_theta_0+delta_theta)));
  double J_s = (r_i / sin(grazing_angle)) * (r_i1 - r_i)/delta_theta;
  double c_zs = c_z0 + sound_speed_gradient * m_own_depth;
  double pressure_s = sqrt( (c_zs * cos(m_theta_0)) / (c_z0 * J_s) );
  double TL = -20*log10(pressure_s);
  return (TL);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle_ajolley::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void CommunicationAngle_ajolley::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("VEHICLE_NAME", 0);
  Register("COLLABORATOR_NAME", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_DEPTH", 0);
  Register("NAV_HEADING", 0);
  Register("NAV_SPEED", 0);

  
}


//------------------------------------------------------------
// Procedure: buildReport()

bool CommunicationAngle_ajolley::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}




