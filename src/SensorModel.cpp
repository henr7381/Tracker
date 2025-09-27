// Vector implementation


// Includes
#include <stdio.h>
#include <cmath>


// User includes
#include "SensorModel.h"


SensorModel::SensorModel(Vector3D worldPointingVector, Point3D origin)
{
	CameraLocation_W = origin;

	Vector3D up {0, 0, 1};

	CameraFrameZ_W = worldPointingVector.normalize();

	if((std::abs(CameraFrameZ_W.x - up.x) < 1e-8  &&
	    std::abs(CameraFrameZ_W.y - up.y) < 1e-8  &&
	    std::abs(CameraFrameZ_W.z - up.z) < 1e-8) ||
	   (std::abs(CameraFrameZ_W.x + up.x) < 1e-8  &&
	    std::abs(CameraFrameZ_W.y + up.y) < 1e-8  &&
	    std::abs(CameraFrameZ_W.z + up.z) < 1e-8))
	{
		up = {0, 1, 0};
	}

	CameraFrameX_W = up.cross(CameraFrameZ_W);
	CameraFrameX_W = CameraFrameX_W.normalize();

	CameraFrameY_W = CameraFrameZ_W.cross(CameraFrameX_W);
	CameraFrameY_W = CameraFrameY_W.normalize();	
}


void SensorModel::ProcessImage()
{

	uint16_t x = 0;
	uint16_t y = 0;

	Result processResult = detectedPixel(x, y);

	if (std::holds_alternative<Ray>(processResult))
	{
		printf("success\n");
	}
	else
	{
		printf("error");
	}

	return;
}


Result SensorModel::detectedPixel(uint16_t x, uint16_t y)
{ 
	DetectedVector_L.x = x - principal_X;
	DetectedVector_L.y = y - principal_Y;
	DetectedVector_L.z = focal_length;

	DetectedVector_L = DetectedVector_L.normalize();

	const double rotMat[3][3] = {{CameraFrameX_W.x, CameraFrameY_W.x, CameraFrameZ_W.x},
				     {CameraFrameX_W.y, CameraFrameY_W.y, CameraFrameZ_W.y},
				     {CameraFrameX_W.z, CameraFrameY_W.z, CameraFrameZ_W.z}};
	
	DetectedRay_W.vector = DetectedVector_L.matrixMultiply(rotMat);
	DetectedRay_W.vector = DetectedRay_W.vector.normalize();
	DetectedRay_W.origin = this -> CameraLocation_W;

	return (DetectedRay_W);
}


void SensorModel::MainLoop()
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened())
	{
		printf("Stream open error\n");
		return;
	}

	cv::Ptr<cv::cuda::BackgroundSubtractorMOG2> subtractor = cv::cuda::createBackgroundSubtractorMOG2(500, 16, true);

	while (true)
	{
		cv::Mat frame;
		if(!cap.read(frame))
		{
			printf("Failed to capture a frame\n");
			break;
		}

		cv::cuda::GpuMat gpu_frame;
		gpu_frame.upload(frame);

		cv::cuda::GpuMat gpu_fgmask;
		subtractor -> apply(gpu_frame, gpu_fgmask, -1);

		cv::Mat fgmask;
		gpu_fgmask.download(fgmask);

		cv::Mat foreground;
		cv::bitwise_and(frame, frame, foreground, fgmask);

		cv::imshow("Original frame", frame);
		cv::imshow("Foreground Mask", fgmask);
		cv::imshow("Foreground", foreground);

		if (cv::waitKey(1) == 'q')
		{
			break;
		}
	}

	cap.release();
	cv::destroyAllWindows();
}
