/************************************************************/
/*    NAME: ENSTA Bretagne                                  */
/*    ORGN: MIT                                             */
/*    FILE: SimModem.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "SimModem.h"

using namespace std;

//---------------------------------------------------------
// Constructor

SimModem::SimModem()
{
  // Default values:

  m_beacon_x = 0.;
  m_beacon_y = 0.;
  m_beacon_z = 0.;

  m_robot_x = 0.;
  m_robot_y = 0.;
  m_robot_z = 0.;

  computeRange();
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SimModem::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

    if(key == "NAV_X") 
      m_robot_x = msg.GetDouble();

    else if(key == "NAV_Y") 
      m_robot_y = msg.GetDouble();

    else if(key == "NAV_Z") 
      m_robot_z = msg.GetDouble();

    else if(key != "APPCAST_REQ") // handle by AppCastingMOOSApp
     reportRunWarning("Unhandled Mail: " + key);
  }

  return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SimModem::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SimModem::Iterate()
{
  AppCastingMOOSApp::Iterate();

  computeRange(); // range is computed according to AppTick parameter
  Notify("RANGE", m_range);

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SimModem::OnStartUp()
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
    string param = toupper(biteStringX(line, '='));
    string value = line;

    bool handled = false;

    if(param == "BEACON_X")
    {
      m_beacon_x = atof(value.c_str());
      handled = true;
    }

    else if(param == "BEACON_Y")
    {
      m_beacon_y = atof(value.c_str());
      handled = true;
    }

    else if(param == "BEACON_Z")
    {
      m_beacon_z = atof(value.c_str());
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);
  }
  
  registerVariables();
  computeRange();
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void SimModem::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_Z", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool SimModem::buildReport() 
{
  m_msgs << "\nBeacon: \n";
  ACTable actab_beacon(3);
  actab_beacon << "X | Y | Z";
  actab_beacon.addHeaderLines();
  actab_beacon << m_beacon_x << m_beacon_y << m_beacon_z;
  m_msgs << actab_beacon.getFormattedString() << endl;

  m_msgs << "\nRobot: \n";
  ACTable actab_robot(3);
  actab_robot << "X | Y | Z";
  actab_robot.addHeaderLines();
  actab_robot << m_robot_x << m_robot_y << m_robot_z;
  m_msgs << actab_robot.getFormattedString() << endl;

  m_msgs << "\nRange: " << m_range;

  return(true);
}


//------------------------------------------------------------
// Procedure: buildReport()

void SimModem::computeRange() 
{
  m_range = sqrt(pow(m_robot_x - m_beacon_x, 2)
               + pow(m_robot_y - m_beacon_y, 2) 
               + pow(m_robot_z - m_beacon_z, 2));
}