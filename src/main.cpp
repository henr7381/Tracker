// This is the main file for the tracker program


// Includes
#include <stdio.h>
#include <cmath>


// User includes
#include "SensorModel.h"
#include "Test.h"
#include "VoxelGrid.h"


#define ever (;;)


int main(int argc, char** argv)
{
#ifdef TESTBUILD
	testGrid();
#else

	// I need to start deciding on some physical sizes. Since I'm doing things in terms of voxels, I kind of want to say
	//		Voxel pitch: 100 mm^3 (1dm accuracy)	
	// Then we only have to worry about placing the sensors with decimeter accuracy. 
    VoxelGrid V {300, 1};   // 30m^3 space
    
    // For sensor model output traversal
    Ray rayIdx {};       

    // Sensors
    SensorModel cam0(Vector3D{-1, 0, 0}, Point3D{0, 0, 0}, 0);
    //SensorModel cam1(Vector3D{0, -1, 0}, Point3D{500, 500, 0}, 1);

	for ever
	{
		cam0.ProcessImage();
      // Need to add second processor

		while (!cam0.DetectedRay_W.empty()) 
		{
         rayIdx = cam0.DetectedRay_W.back();
         cam0.DetectedRay_W.pop_back();
         V.walkRay(rayIdx);
      }

      // Second while for second sensor
   }

#endif
	return 0;
}
