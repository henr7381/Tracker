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

    // Create background subtractor
    cv::Ptr<cv::cuda::BackgroundSubtractorMOG2> subtractor = cv::cuda::createBackgroundSubtractorMOG2(500, 16, true);

    // Initialize variables
    cv::Mat frame, fgmask, labels, stats, centroids;
    cap.read(frame); // Read first frame to get dimensions
    cv::cuda::GpuMat gpu_frame, gpu_fgmask, gpuCleanMask;
   
    uint16_t minBlobArea = 1000;  // Ignore blobs smaller than this (needs tuned)
    cv::Mat erodeKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::Mat dilateKernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::Ptr<cv::cuda::Filter> erodeFilter = cv::cuda::createMorphologyFilter(cv::MORPH_ERODE, CV_8UC1, erodeKernel);
    cv::Ptr<cv::cuda::Filter> dilateFilter = cv::cuda::createMorphologyFilter(cv::MORPH_DILATE, CV_8UC1, dilateKernel);

    while (true) 
    {
        // Capture frame
        if (!cap.read(frame)) 
        {
            printf("Failed to capture a frame\n");
            break;
        }

        // Upload frame to GPU
        gpu_frame.upload(frame);

        // Apply background subtraction
        subtractor->apply(gpu_frame, gpu_fgmask, -1);
        
        // Morphology on GPU foreground mask to clean up the salt/pepper noise
        erodeFilter->apply(gpu_fgmask, gpuCleanMask);
        dilateFilter->apply(gpuCleanMask, gpuCleanMask);  // Erode then dilate

/*
        // Update accumulated mask (bitwise OR)
        cv::cuda::bitwise_or(gpuCleanMask, gpu_accumulated_mask, gpu_accumulated_mask);

        // Download masks for display (optional)
        cv::Mat fgmask;
        gpu_fgmask.download(fgmask);

        cv::Mat accumulated_mask;
        gpu_accumulated_mask.download(accumulated_mask);
*/
	
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
                // Significant blob found: Get bounding box
                int x = stats.at<int>(i, cv::CC_STAT_LEFT);
                int y = stats.at<int>(i, cv::CC_STAT_TOP);
                int w = stats.at<int>(i, cv::CC_STAT_WIDTH);
                int h = stats.at<int>(i, cv::CC_STAT_HEIGHT);
                cv::rectangle(frame, cv::Rect(x, y, w, h), cv::Scalar(0, 255, 0), 2);

                // Centroid for tracking if needed
                double cx = centroids.at<double>(i, 0);
                double cy = centroids.at<double>(i, 1);
                // Use for further processing, e.g., output or tracking
                std::cout << "Blob " << i << ": Area=" << area << ", Center=(" << cx << "," << cy << ")" << std::endl;
            }
        }
	printf("--\n");

	/*
        cv::imshow("Original with Blobs", frame);
        cv::imshow("Clean Mask", fgmask);
        */

        // Break loop on 'q' key press
        if (cv::waitKey(1) == 'q') 
        {
            break;
        }
    }

    // Release resources
    cap.release();
    cv::destroyAllWindows();
}
