# Tracker

This project is based off the work done by the YouTuber Consistently Inconsistent. Here is the link to the video that sparked by interest:

    https://www.youtube.com/watch?v=zFiubdrJqqI

You can find a link to his GitHub repo for the projet in the description of that video. I have also included a copy in the /ref directory of this repo. 


The original creator of the concept seems to think that the "2D pixel projected into a 3D voxel grid" is some revolutionary concept, as if it's something nobody has ever considered. That is not true, it's merely just an unorthodox form of DSP. Processing overlapping data streams to reduce noise is not a new concept. This method also has limited applications, and is pretty much only useful for reconstructing the approximate location of an object in 3D space from multiple 2D images (triangulation). Trying to operate on more complex datasets (radar, MRI, etc) that are not strictly a captured image of an object causes this method to fall apart, as does the need for precision. He also wants to avoid the applications that are related to defense, which is cringe.   

This "fork", if you will, is me throwing my hat in the ring. I intend to take multiple video streams and run a live collection and detection algorithm, instead of running a processing program on pre-captured data. I am writing and running this on an Nvidia Jetson Orin Nano, to leverage the onboard GPU for my image procesing. I have compiled the OpenCV library with CUDA support.

Some loose goals:

- Learn GPU accelerated methods of writing image processing programs in C++ (libraries, hardware interfaces)
- Learn about image processing methods (algorithms, image capturing methods)
- Write an effective algorithm and processing method to roughly track airborne objects in 3D space
- Create a portable and field deployable hardware set to enable effective tracking in any location

Current hardware:
- Nvidia Jetson Orin Nano
- 2k webcams (https://a.co/d/eWUQxvO)