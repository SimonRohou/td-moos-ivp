/************************************************************/
/*    NAME: ENSTA Bretagne                                  */
/*    ORGN: MIT                                             */
/*    FILE: SimModem.h                                      */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef SimModem_HEADER
#define SimModem_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class SimModem : public AppCastingMOOSApp
{
 public:
   SimModem();
   ~SimModem() {};

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

   // La méthode ci-dessous est ajoutée pour calculer
   // la distance entre la balise et le robot
   void computeRange();

 private: // Configuration variables
   double m_beacon_x, m_beacon_y, m_beacon_z;

 private: // State variables
   double m_robot_x, m_robot_y, m_robot_z;
   double m_range;
};

#endif 