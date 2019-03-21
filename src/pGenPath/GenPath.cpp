/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "GenPath.h"

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
  m_sorted = false;
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

      if(key == "VISIT_POINT") {
        m_sorted = false; //if new points are recieved, they are not sorted
        m_visit_point = msg.GetString();
        if(m_visit_point != "firstpoint" && m_visit_point != "lastpoint"){
          m_parse_string(m_visit_point);
          m_waypoints.add_vertex(m_x, m_y);
        }
      }

      else if(key=="NAV_X"){
        m_nav_x = msg.GetDouble();
      }

      else if(key=="NAV_Y"){
        m_nav_y = msg.GetDouble();
      }
      
      else if(key=="GENPATH_REGENERATE"){
        m_sorted = false;
        m_waypoints.add_vertex(m_start_x, m_start_y); 
        Notify("COLLECT", "true");
      }
      
#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     else if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}


void GenPath::m_parse_string(string point)
{
  string str = point;
  vector<string> myvector = parseString(str, ',');
  for(unsigned int i=0; i<myvector.size(); i++){
    string param = biteStringX(myvector[i], '=');
    string value = myvector[i];
    if(tolower(param) == "id"){
      m_id = stoi(value);}
    else if(tolower(param) == "x"){
      m_x = stoi(value);}
    else if(tolower(param) == "y"){ 
      m_y = stoi(value);}
  }
  return;
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   registerVariables();
   Notify("UTS_PAUSE", "false");
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!

  

  if(m_sorted == false){ //if the waypoints have not been sorted yet, sort them
    SortWaypoints();
  }
  if(m_sorted == true){ //if they are sorted, begin pulling points off the list as they are visited
    int closest_waypoint = m_waypoints.closest_vertex(m_nav_x, m_nav_y); //find the vertext closest to the current position
    double closest_x = m_waypoints.get_vx(closest_waypoint);
    double closest_y = m_waypoints.get_vy(closest_waypoint);
    double distance = sqrt(pow((closest_x-m_nav_x),2)+pow((closest_y-m_nav_y),2));
    if(distance < visit_radius){
      m_waypoints.delete_vertex(closest_waypoint);
    }
  }

  AppCastingMOOSApp::PostReport();
  return(true);
}

void GenPath::SortWaypoints()
{
  XYSegList sorted_waypoints;
  XYSegList working_waypoints = m_waypoints;
  int closest_index = working_waypoints.closest_vertex(m_start_x, m_start_y); //find the point closest to our starting location
  double next_x = working_waypoints.get_vx(closest_index);
  double next_y = working_waypoints.get_vy(closest_index);
  working_waypoints.delete_vertex(closest_index);//remove vertex from the list so we dont double back
  sorted_waypoints.add_vertex(next_x, next_y); //make that point the begining of our sorted seglist
  for(int i=1; i<m_waypoints.size(); i++){
     int closest_index = working_waypoints.closest_vertex(next_x, next_y);//find the next closest vertex
     next_x = working_waypoints.get_vx(closest_index);
     next_y = working_waypoints.get_vy(closest_index);
     sorted_waypoints.add_vertex(next_x, next_y);//add the next closest vertex to the sorted list
     working_waypoints.delete_vertex(closest_index);//remove vertex from the list so we dont double back
  }
  string color;
  if(next_x < 88){
   color = "red";
  }
  if(next_x>88){
   color = "yellow";
  }

  update_str = "points = ";
  update_str += sorted_waypoints.get_spec();
  update_str += " # visual_hints = edge_color = " + color + ", vertex_color = " + color;
  

  Notify("WAYPOINT_UPDATE_" + m_vehicle_name, update_str);
  m_sorted = true;
}
//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
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
    else if(param == "vname") {
      m_vehicle_name = toupper(value);
      handled=true;
    }    
    else if(param == "start_pos") {
      string m_start_pos = value;
      m_start_x = stoi(MOOSChomp(m_start_pos, ","));
      m_start_y = stoi(m_start_pos);
      m_waypoints.add_vertex(m_start_x, m_start_y);
      handled=true;
    }       
    else if(param == "visit_radius") {
      visit_radius = stod(value);
      handled=true;
    }    

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void GenPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("VISIT_POINT",0);
  Register("NAV_X",0);
  Register("NAV_Y",0);
  Register("GENPATH_REGENERATE");
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool GenPath::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  m_msgs << m_vehicle_name << " Waypoints Remaining: " << m_waypoints.size()<<endl;
  m_msgs << "WAYPOINT_UPDATE = " << update_str<<endl;

  return(true);
}




