/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: SimpleSIVIA.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef SimpleSIVIA_HEADER
#define SimpleSIVIA_HEADER

#include "vibes.h"
#include "contractors.h"

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class SimpleSIVIA : public AppCastingMOOSApp
{
 public:
   SimpleSIVIA();
   ~SimpleSIVIA();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   void registerVariables();

 protected: // Own functions
   void sivia();
   void ctcOut(ibex::IntervalVector& x);
   void ctcIn(ibex::IntervalVector& x);

 private: // Configuration variables
    double m_sivia_precision;
    double m_beacon1_x, m_beacon1_y;
    double m_beacon2_x, m_beacon2_y;
    ibex::Interval m_range_uncertainty;

 private: // State variables
    ibex::IntervalVector *m_box;
    ibex::Interval m_beacon1_range, m_beacon2_range;
};

#endif 
