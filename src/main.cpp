// This is the main file for the tracker program


// Includes
#include <stdio.h>
#include <cmath>

// User includes
#include "SensorModel.h"
#include "Test.h"


#define ever (;;)


int main(int argc, char** argv)
{
#ifdef TESTBUILD
	test2();
#else

	// I need to start deciding on some physical sizes. Since I'm doing things in terms of voxels, I kind of want to say
	
	//		Voxel pitch: 100 mm^3 (1dm accuracy)
	
	// Then we only have to worry about placing the sensors with decimeter accuracy. 

	const int N = 500;		// Voxel grid size (50m^3)

    std::vector<float> voxel_grid(N*N*N, 0.f);

	SensorModel cam0(Vector3D{-1, 0, 0}, Point3D{500, 0, 0}, 0);
	//SensorModel cam1(Vector3D{0, -1, 0}, Point3D{500, 500, 0}, 1);

	bool newline = false;

	for ever
	{
		cam0.ProcessImage();
		//cam1.ProcessImage();

		while (!cam0.DetectedRay_W.empty()) 
		{
			newline = true;
			Ray rayIdx = cam0.DetectedRay_W.front();
			cam0.DetectedRay_W.pop_front();

			printf("Origin: (%lf, %lf, %lf)	Ray: (%lf, %lf, %lf)\n",
																rayIdx.origin.x,
																rayIdx.origin.y,
																rayIdx.origin.z,
																rayIdx.vector.x,
																rayIdx.vector.y,
																rayIdx.vector.z );
		}

		if (newline)
		{
			printf("\n");
			newline = false;
		}
		
	}

#endif
	return 0;
}
