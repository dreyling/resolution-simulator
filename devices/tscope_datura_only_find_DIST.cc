// Simon Spannagel (DESY) January 2016

#include "assembly.h"
#include "propagate.h"
#include "materials.h"
#include "constants.h"
#include "log.h"

using namespace std;
using namespace gblsim;
using namespace unilog;

int main(int argc, char* argv[]) {

  /*
   * Telescope resolution simulation for the DATURA telescope at the DESY TB21 beam line
   * Six MIMOSA26 planes with 150mm spacing, intrinsic sensor resolution 3.4um
   * DUT with variable thickness (scan)
   */

  
  Log::ReportingLevel() = Log::FromString("INFO");

  for (int i = 1; i < argc; i++) {
    // Setting verbosity:
    if (std::string(argv[i]) == "-v") { 
      Log::ReportingLevel() = Log::FromString(std::string(argv[++i]));
      continue;
    } 
  }
  
  //----------------------------------------------------------------------------
  // Preparation of the telescope and beam properties:

  // MIMOSA26 telescope planes consist of 50um silicon plus 2x25um Kapton foil only:
  double MIM26 = 50e-3 / X0_Si + 50e-3 / X0_Kapton;
  // The intrinsic resolution has been measured to be around 3.4um:
  double RES = 3.4e-3;

  // M26  M26  M26      DUT      M26  M26  M26
  //  |    |    |        |        |    |    |
  //  |    |    |        |        |    |    |
  //  |<-->|    |<------>|<------>|    |    |
  //   DIST      DUT_DIST DUT_DIST
  
  // Distance between telescope planes in mm:
  // double DIST = 20;
  // Distance of telescope arms and DUT assembly:
  double DUT_DIST = 1000;

  // Beam energy 5 GeV electrons/positrons at DESY:
  double BEAM = 5.0;



  //----------------------------------------------------------------------------
  // Build the trajectory through the telescope device:


  for(int DIST = 20; DIST <= 150 ; DIST += 10) {

    // Build a vector of all telescope planes:
    std::vector<plane> datura;
    double position = 0;
    
    // Upstream telescope arm:
    for(int i = 0; i < 3; i++) {
      datura.push_back(plane(position,MIM26,true,RES));
      position += DIST;
    }

    // Downstream telescope arm:
    position = 2*DIST + 2*DUT_DIST;
    for(int i = 0; i < 3; i++) {
      datura.push_back(plane(position,MIM26,true,RES));
      position += DIST;
    }


    // Prepare the DUT (no measurement, just scatterer
    //plane dut(2*DIST+DUT_DIST, dut_x0, false);

    // Duplicate the planes vector and add the current DUT:
    //std::vector<plane> planes = datura;
    //planes.push_back(dut);

    // Build the telescope:
    //telescope mytel(planes, BEAM);
    telescope mytel(datura, BEAM);

    // Get the resolution at plane-vector position (x):
    LOG(logRESULT) << "The pointing resolution for a plane distance of " << DIST << " mm is " << mytel.getResolution(3) << " mum";
  }
  
  return 0;
}
