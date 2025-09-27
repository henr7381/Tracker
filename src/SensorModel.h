// This is my custom vector implementation


#ifndef SENSORMODEL_H
#define SENSORMODEL_H


// Includes
#include <cstdint>
#include <variant>
#include <opencv2/opencv.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudabgsegm.hpp>


// User includes
#include "Vector.h"
#include "MathHelp.h"


using Result = std::variant<Ray, int>;


class SensorModel
{
public:
	SensorModel(Vector3D worldPointingVector, Point3D origin);
	void MainLoop();
	Result detectedPixel(uint16_t x, uint16_t y);
	void ProcessImage();

#ifndef TESTBUILD
private:
#endif
	static constexpr uint16_t Xres = 2560;		// in pixels
	static constexpr uint16_t Yres = 1440;		// in pixels

	static constexpr double Horz_fov = 85;		// in DEG
	static constexpr double Vert_fov = 54.6;	// in DEG

	static constexpr double principal_X = Xres / 2.0;
	static constexpr double principal_Y = Yres / 2.0;
	static constexpr double focal_length = principal_X / std::tan(toRads(Horz_fov) / 2.0);

	Point3D CameraLocation_W {};
	Vector3D CameraFrameX_W {};
	Vector3D CameraFrameY_W {};
	Vector3D CameraFrameZ_W {};
	Vector3D DetectedVector_L {};
	Ray DetectedRay_W {};
};


#endif
