/************************************************************/
/*    NAME: Vincent Drevelle, Simon Rohou                                              */
/*    ORGN: MIT                                             */
/*    FILE: Joystick.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef Joystick_HEADER
#define Joystick_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class Joystick : public AppCastingMOOSApp
{
 public:
   Joystick();
   ~Joystick() {};

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
