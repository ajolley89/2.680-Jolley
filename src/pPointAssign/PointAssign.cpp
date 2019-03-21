/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "PointAssign.h"
#include "XYPoint.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
  assign_by_region = true;
  m_boat1_name = "";
  m_boat2_name = ""; 
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

    if(key == "VISIT_POINT"){
      m_visit_point= msg.GetString();

      if(m_visit_point == "firstpoint" || m_visit_point == "lastpoint") {
        Notify("VISIT_POINT_" + m_boat1_name, m_visit_point);
        Notify("VISIT_POINT_" + m_boat2_name, m_visit_point);
        continue;
      }
      m_parse_string(m_visit_point);

      if(assign_by_region == false){
        m_assign_alternating(m_visit_point);
      }
      
      if(assign_by_region == true){
        m_assign_regionally(m_visit_point);
      }

      postViewPoint(m_x, m_y, to_string(m_id), m_color);

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

void PointAssign::m_parse_string(string point)
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

void PointAssign::m_assign_alternating(string point)
{
  if(m_id%2 == 0){
    //Assign to HENRY
    Notify("VISIT_POINT_" + m_boat1_name, point);
    m_color = "red";
  }
  else{
    //Assign to GILDA;
    Notify("VISIT_POINT_"+m_boat2_name, point);
    m_color = "green";
  }
  return;
}

void PointAssign::m_assign_regionally(string point)
{
  if(m_x <88){
    //Assign to HENRY
    Notify("VISIT_POINT_"+m_boat1_name, point);
    m_color = "red";
  }
  else{
    //Assign to GILDA
    Notify("VISIT_POINT_"+m_boat2_name, point);
    m_color = "yellow";
  }
  return;
}

 void PointAssign::postViewPoint(double x, double y, string label, string color)
 {
   XYPoint point(x, y);
   point.set_label(label);
   point.set_color("vertex", color);
   point.set_param("vertex_size", "2");

   string spec = point.get_spec();
   Notify("VIEW_POINT", spec);
 } 

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
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

    if(param == "assign_by_region") {
      if(tolower(value) == "false") {
        assign_by_region = false;
      }
      if(tolower(value) == "true") {
        assign_by_region = true;
      }
      handled=true;
    }
    if(param == "vname" && m_boat1_name != "") {
      m_boat2_name = toupper(value);
      handled=true;
    }
    if(param == "vname" && m_boat1_name == "") {
      m_boat1_name = toupper(value);
      handled=true;
    }
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

void PointAssign::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("VISIT_POINT",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool PointAssign::buildReport() 
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




