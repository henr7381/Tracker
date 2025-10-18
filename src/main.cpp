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

	SensorModel cam0(Vector3D{-1, 0, 0}, Point3D{0, 0, 0}, 0);
	//SensorModel cam1(Vector3D{0, -1, 0}, Point3D{500, 500, 0}, 1);

	// For testing
	Ray rayIdx {};

   // Spawn Python child process with popen (writes to child's stdin)
   FILE* pipe = popen("python3 ./src/visualize_ray.py", "w");  // Replace with your Python script path
   if (!pipe) 
   {
      std::cerr << "Failed to open pipe to Python" << std::endl;
      return 1;
   }

	for ever
	{
		cam0.ProcessImage();

      if (!cam0.DetectedRay_W.empty())
      {
         rayIdx = cam0.DetectedRay_W.back();
      }

		while (!cam0.DetectedRay_W.empty()) 
		{
			cam0.DetectedRay_W.pop_back();
		}

      float rayLength = 2.f;
      Point3D endPoint {};
      endPoint.x = rayIdx.origin.x + (rayLength * rayIdx.vector.x);
      endPoint.y = rayIdx.origin.y + (rayLength * rayIdx.vector.y);
      endPoint.z = rayIdx.origin.z + (rayLength * rayIdx.vector.z);

      printf("Origin: (%lf, %lf, %lf) End: (%lf, %lf, %lf)\n",
      													rayIdx.origin.x,
      													rayIdx.origin.y,
      													rayIdx.origin.z,
      													endPoint.x,
      													endPoint.y,
      													endPoint.z );

      // Format and write the latest ray to the pipe
      std::string ray_str = std::to_string(rayIdx.origin.x) + " " +
                            std::to_string(rayIdx.origin.y) + " " +
                            std::to_string(rayIdx.origin.z) + " " +
                            std::to_string(endPoint.x) + " " +
                            std::to_string(endPoint.y) + " " +
                            std::to_string(endPoint.z) + "\n";
      fputs(ray_str.c_str(), pipe);
      fflush(pipe);  // Ensure data is sent immediately

      // Non-blocking check for key press to handle window events and exit
      int key = cv::pollKey(); // Non-blocking alternative to waitKey
      if (key == 27) 
      { // Press 'Esc' to exit
         break;
      }
	}

   pclose(pipe);  // Close the pipe when done
   cv::destroyWindow("Filtered FGMask");

#endif
	return 0;
}
