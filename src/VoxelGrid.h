// This is my custom dynamically configurable voxel grid object


#ifndef VOXELGRID_H
#define VOXELGRID_H


// Includes
#include <cstdint>
#include <vector>


// User includes
#include "Vector.h"


// Neccesary defines


class VoxelGrid
{
public:
    VoxelGrid(uint16_t N, float darkenFactor);  
    void walkRay(Ray input);

#ifndef TESTBUILD
private:
#endif
    std::vector<float> voxel_grid;
    uint16_t vectorSize {};

    double darkenFactor {1.f};
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

};


#endif
