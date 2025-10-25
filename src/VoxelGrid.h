// This is my custom dynamically configurable voxel grid object


#ifndef VOXELGRID_H
#define VOXELGRID_H


// Includes
#include <cstdint>
#include <cmath>
#include <vector>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <cstring>
#include <stdio.h>
#include <string>
#include <cstdlib>


// User includes
#include "Vector.h"


class VoxelGrid
{
public:
    VoxelGrid(uint16_t N, uint8_t darkenFactor);  
    void walkRay(Ray input);

#ifndef TESTBUILD
private:
#endif
    //std::vector<uint8_t> voxel_grid {0};
    uint8_t* voxel_grid;        // Pointer to the shared memory array
    uint16_t vectorSize {};
    uint16_t memSize {}; 

    uint8_t darkenFactor {};
    int16_t stepX {};
    int16_t stepY {};
    int16_t stepZ {};
    double stepDx {};
    double stepDy {};
    double stepDz {};
    double tMAXx {};
    double tMAXy {};
    double tMAXz {};
    int16_t boundx {};
    int16_t boundy {};
    int16_t boundz {};

    const char* SHM_NAME = "/voxelGrid";
    const char* SEM_NAME = "/voxelGridSemaphore";
    sem_t* update_sem {};

    void markVoxel(Point3D p);
};


#endif
