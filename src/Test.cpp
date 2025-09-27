// This is where I'll keep tests


// Includes
#include <stdio.h>


// User includes
#include "Test.h"
#include "SensorModel.h"


#ifdef TESTBUILD


void test()
{
	printf("Beginning test =======================\n");

	SensorModel cam1 (Vector3D {0.577350, 0.577350, 0.577350}, Point3D {0, 0, 0});

	printf("Cam1 global location: (%lf, %lf, %lf,)\n", cam1.CameraLocation_W.x,
							   cam1.CameraLocation_W.y,
						           cam1.CameraLocation_W.z);

	printf("Cam1 global X vector: (%lf, %lf, %lf)\n", cam1.CameraFrameX_W.x,
							  cam1.CameraFrameX_W.y,
						          cam1.CameraFrameX_W.z);
	
	printf("Cam1 global Y vector: (%lf, %lf, %lf)\n", cam1.CameraFrameY_W.x,
							  cam1.CameraFrameY_W.y,
							  cam1.CameraFrameY_W.z);

	printf("Cam1 global Z vector: (%lf, %lf, %lf)\n", cam1.CameraFrameZ_W.x,
							  cam1.CameraFrameZ_W.y,
							  cam1.CameraFrameZ_W.z);
	
	printf("Cam1 processing pixel (1280, 720)\n");

	Result testResult = cam1.detectedPixel(1280, 720);
	Ray testValue = std::get<Ray>(testResult);

	printf("Resulting global location: (%lf, %lf, %lf,)\n", testValue.origin.x,
							        testValue.origin.y,
						                testValue.origin.z);

	printf("Resulting global vector: (%lf, %lf, %lf)\n", testValue.vector.x,
							     testValue.vector.y,
							     testValue.vector.z);
	printf("Cam1 processing pixel (0, 0)\n");

	testResult = cam1.detectedPixel(0, 0);
	testValue = std::get<Ray>(testResult);

	printf("Resulting global location: (%lf, %lf, %lf,)\n", testValue.origin.x,
							        testValue.origin.y,
						                testValue.origin.z);

	printf("Resulting global vector: (%lf, %lf, %lf)\n", testValue.vector.x,
							     testValue.vector.y,
							     testValue.vector.z);

	SensorModel cam2 (Vector3D {1, 0, 0}, Point3D {0, 0, 0});
	
	printf("Beginning cam2 main loop\n");

	cam2.MainLoop();

	printf("Ending cam2 main loop\n");
	


	printf("Ending test ==========================\n");
	return;
}


#endif
