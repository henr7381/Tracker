// This is my custom vector implementation


#ifndef SENSORMODEL_H
#define SENSORMODEL_H


// Includes
#include <cstdint>
#include <variant>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/core/cuda.hpp>
#include <opencv2/cudabgsegm.hpp>
#include <opencv2/cudaarithm.hpp>
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>


// User includes
#include "Vector.h"
#include "MathHelp.h"


using Result = std::variant<Ray, int>;

#define MAXOBJECTS 10


class SensorModel
{
public:
	SensorModel(Vector3D worldPointingVector, Point3D origin, uint8_t cameraStream);
	void MainLoop();
	Result detectedPixel(uint16_t x, uint16_t y);
	void ProcessImage();
	Ray* getRays();
	void resetRays();

	std::deque<Ray> DetectedRay_W;			// List of detections

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

	std::string pipeline = "v4l2src device=/dev/video0 ! image/jpeg, width=2560, height=1440, framerate=30/1 ! jpegdec ! videoconvert ! appsink";
	cv::VideoCapture cap;

	// Create background subtractor
    cv::Ptr<cv::cuda::BackgroundSubtractorMOG2> subtractor = cv::cuda::createBackgroundSubtractorMOG2(500, 16, true);

    // Initialize variables
    cv::Mat frame, fgmask, labels, stats, centroids;
    cv::cuda::GpuMat gpu_frame, gpu_fgmask, gpuCleanMask;
   
    uint16_t minBlobArea = 1000;  // Ignore blobs smaller than this (needs tuned)
    cv::Mat erodeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::Ptr<cv::cuda::Filter> erodeFilter = cv::cuda::createMorphologyFilter(cv::MORPH_ERODE, CV_8UC1, erodeKernel);
    cv::Ptr<cv::cuda::Filter> dilateFilter = cv::cuda::createMorphologyFilter(cv::MORPH_DILATE, CV_8UC1, dilateKernel);
};


#endif
