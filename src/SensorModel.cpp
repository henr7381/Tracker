// Vector implementation


// Includes
#include <stdio.h>
#include <cmath>


// User includes
#include "SensorModel.h"
#include <chrono>


SensorModel::SensorModel(Vector3D worldPointingVector, Point3D origin, uint8_t cameraStream)
    : cap(pipeline, cv::CAP_GSTREAMER)
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
    
    if (!cap.isOpened()) 
    {
        printf("Stream {%u} open error\n", cameraStream);
    }


    cv::namedWindow("Filtered FGMask", cv::WINDOW_NORMAL); // WINDOW_NORMAL allows resizing

    // Set the window size (width, height in pixels)
    cv::resizeWindow("Filtered FGMask", 800, 600); // Example: 800x600 pixels
}


void SensorModel::ProcessImage()
{
    // Capture frame
    if (!cap.read(frame)) 
    {
        printf("Failed to capture a frame\n");
        return;
    }

    // Upload frame to GPU
    gpu_frame.upload(frame);

    // Apply background subtraction
    subtractor->apply(gpu_frame, gpu_fgmask, -1);
    
    // Morphology on GPU foreground mask to clean up the salt/pepper noise
    erodeFilter->apply(gpu_fgmask, gpuCleanMask);
    dilateFilter->apply(gpuCleanMask, gpuCleanMask);  // Erode then dilate

    // Download to CPU for detailed stats and filtering
    gpuCleanMask.download(fgmask);

    // Connected components with stats on CPU
    int nLabels = cv::connectedComponentsWithStats(fgmask, labels, stats, centroids, 8, CV_32S);

    // Detect and draw/filter blobs
    for (int i = 1; i < nLabels; ++i) // Skip background (label 0)
    {  
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        if (area >= minBlobArea) 
        {
            // Use centroids for tracking
            Result processResult = detectedPixel(centroids.at<double>(i, 0), centroids.at<double>(i, 1));

            if (std::holds_alternative<Ray>(processResult))
            {
                if ((i - 1) < MAXOBJECTS)
                {
                    DetectedRay_W.push_back(std::get<Ray>(processResult));
                }
            }
            else
            {
                printf("error");
            }
        }
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
	
    Ray retval {};

	retval.vector = DetectedVector_L.matrixMultiply(rotMat);
	retval.vector = retval.vector.normalize();
	retval.origin = this -> CameraLocation_W;

	return (retval);
}

void SensorModel::resetRays()
{
    DetectedRay_W.clear();
}


void SensorModel::MainLoop()
{


    while (true) 
    {

    }

    // Release resources
    cap.release();
    cv::destroyAllWindows();
}
