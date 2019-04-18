/************************************************************/
/*    NAME: Austin Jolley                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_ZigLeg::BHV_ZigLeg(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  addInfoVars("NAV_X, NAV_Y, NAV_HEADING");
  addInfoVars("WPT_INDEX", "no_warning");

  // Set default state variable values.
  m_waypoint_index = 0;
  m_ownx = 0;
  m_owny = 0;
  m_ownheading = 0;
  heading_set = false;
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "foo") && isNumber(val)) {
    // Set local member variables here
    return(true);
  }
  else if (param == "bar") {
    // return(setBooleanOnString(m_my_bool, val));
  }
  else if (param == "zig_angle") {
    m_zig_angle = double_val;
    return(true);
  }
  else if (param == "zig_duration") {
    m_zig_duration = double_val;
    return(true);
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_ZigLeg::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;

  // Part 2: Obtain relevant data from the buffer; Post a warning if unavailable.
  bool ok1, ok2, ok3;
  m_ownx = getBufferDoubleVal("NAV_X", ok1);
  m_owny = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }

  unsigned int old_index = m_waypoint_index;
  m_waypoint_index = getBufferDoubleVal("WPT_INDEX", ok3);

  // Part 3: When waypoints change, start timer
  if(old_index != m_waypoint_index) {
    m_bufertime = getBufferCurrTime();
    
  }

  // Part 4: Impliment objective function from 5 seconds after shift for the duration set in config file
  if(getBufferCurrTime()-m_bufertime >5 && getBufferCurrTime()- m_bufertime < 5 + m_zig_duration) {

    if(!heading_set){
      bool ok3;
      m_ownheading = getBufferDoubleVal("NAV_HEADING", ok3);
      heading_set = true;
      if(!ok3){
        postWMessage("No ownship heading info in info_buffer.");
        return(0);
      }
    }

    ipf = buildFunctionWithZAIC();

  }
  else {
    ipf = 0;
    heading_set = false;
  }



  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_ZigLeg::buildFunctionWithZAIC() 
{ 
  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(m_ownheading + m_zig_angle);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(180.0);
  crs_zaic.setSummitDelta(0);  
  crs_zaic.setValueWrap(true);
  if(crs_zaic.stateOK() == false) {
    string warnings = "Course ZAIC problems " + crs_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();
  IvPFunction *ivp_function = crs_ipf;
  return(ivp_function);
}