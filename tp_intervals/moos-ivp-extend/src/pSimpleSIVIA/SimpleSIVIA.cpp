/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: SimpleSIVIA.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "SimpleSIVIA.h"

using namespace std;
using namespace ibex;

//---------------------------------------------------------
// Constructor

SimpleSIVIA::SimpleSIVIA()
{
  vibes::beginDrawing();
  vibes::newFigure("SIVIA");
  vibes::setFigureProperties(
            vibesParams("figure", "SIVIA",
                        "x", 100,
                        "y", 100,
                        "width", 500,
                        "height", 500));
  vibes::axisLimits(-10,10,-10,10);

  m_sivia_precision = 0.1;
  m_box = new IntervalVector(2, Interval(-10,10));
  sivia();
}

//---------------------------------------------------------
// Destructor

SimpleSIVIA::~SimpleSIVIA()
{
  delete m_box;
  vibes::endDrawing();
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SimpleSIVIA::OnNewMail(MOOSMSG_LIST &NewMail)
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

     if(key == "RANGE")
     {
       if(msg.GetSource() == "uSimModem_Beacon1")
         m_beacon1_range = msg.GetDouble() + m_range_uncertainty;

       if(msg.GetSource() == "uSimModem_Beacon2")
         m_beacon2_range = msg.GetDouble() + m_range_uncertainty;

       sivia();
     }

     else if(key != "APPCAST_REQ") // handle by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SimpleSIVIA::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SimpleSIVIA::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SimpleSIVIA::OnStartUp()
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

    if(param == "SIVIA_PRECISION")
    {
      handled = true;
      m_sivia_precision = atof(value.c_str());
    }

    if(param == "BEACON1_X")
    {
      handled = true;
      m_beacon1_x = atof(value.c_str());
    }

    if(param == "BEACON1_Y")
    {
      handled = true;
      m_beacon1_y = atof(value.c_str());
    }

    if(param == "BEACON2_X")
    {
      handled = true;
      m_beacon2_x = atof(value.c_str());
    }

    if(param == "BEACON2_Y")
    {
      handled = true;
      m_beacon2_y = atof(value.c_str());
    }

    if(param == "RANGE_UNCERTAINTY")
    {
      handled = true;
      double radius = atof(value.c_str());
      m_range_uncertainty = Interval(-radius, radius);
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();

  Notify("NAV_X", 0.);
  Notify("NAV_Y", 0.);
  Notify("NAV_Z", 0.);

  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void SimpleSIVIA::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("RANGE", 0);
}

//------------------------------------------------------------
// Procedure: buildReport()

bool SimpleSIVIA::buildReport() 
{
  m_msgs << "\nConfiguration: \n";
  ACTable actab_config(1);
  actab_config << "Precision";
  actab_config.addHeaderLines();
  actab_config << m_sivia_precision;
  m_msgs << actab_config.getFormattedString() << endl;

  m_msgs << "\nBeacon1: \n";
  ACTable actab_beacon1(2);
  actab_beacon1 << "X | Y";
  actab_beacon1.addHeaderLines();
  actab_beacon1 << m_beacon1_x << m_beacon1_y;
  m_msgs << actab_beacon1.getFormattedString() << endl;

  m_msgs << "\nBeacon2: \n";
  ACTable actab_beacon2(2);
  actab_beacon2 << "X | Y";
  actab_beacon2.addHeaderLines();
  actab_beacon2 << m_beacon2_x << m_beacon2_y;
  m_msgs << actab_beacon2.getFormattedString() << endl;

  return(true);
}

//------------------------------------------------------------
// Procedure: ctcOut()

void SimpleSIVIA::ctcOut(IntervalVector& x)
{
  sepInRing(x[0], x[1], m_beacon1_x, m_beacon1_y, m_beacon1_range, true);
  sepInRing(x[0], x[1], m_beacon2_x, m_beacon2_y, m_beacon2_range, true);
}

//------------------------------------------------------------
// Procedure: ctcIn()

void SimpleSIVIA::ctcIn(IntervalVector& x)
{
  IntervalVector a(x);
  IntervalVector b(x);
  sepInRing(a[0], a[1], m_beacon1_x, m_beacon1_y, m_beacon1_range, false);
  sepInRing(b[0], b[1], m_beacon2_x, m_beacon2_y, m_beacon2_range, false);
  x = a | b;
}

//------------------------------------------------------------
// Procedure: sivia()

void SimpleSIVIA::sivia()
{
  IntervalVector X(2, Interval(-10,10));
  list<IntervalVector> L;
  L.push_back(*m_box);

  while(!L.empty())
  {
    X = L.front(); L.pop_front();

    vibes::drawBox(X[0].lb(), X[0].ub(), X[1].lb(),X[1].ub(), "lightGray[cyan]");
    ctcOut(X);
    vibes::drawBox(X[0].lb(), X[0].ub(), X[1].lb(),X[1].ub(), "lightGray[red]");
    ctcIn(X);
    vibes::drawBox(X[0].lb(), X[0].ub(), X[1].lb(),X[1].ub(), "lightGray[yellow]");

    if(X.volume() > 0.1)
    {
      IntervalVector X1(2), X2(2);
      bisect(X, X1, X2);
      L.push_back(X1); L.push_back(X2);
    }
  }
}