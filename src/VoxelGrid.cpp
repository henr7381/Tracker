// Voxel Grid implementation


// Includes
#include <cmath>


// User includes
#include "VoxelGrid.h"


VoxelGrid::VoxelGrid(uint16_t N, float darkenFactor)
	: voxel_grid(N*N*N, 0.f)
{
	this -> darkenFactor += darkenFactor;
	vectorSize = N;
}


void VoxelGrid::walkRay(Ray input)
{
	// Set step direction
	stepX = (input.vector.x > 0) ? 1 : -1;
	stepY = (input.vector.y > 0) ? 1 : -1;
	stepZ = (input.vector.z > 0) ? 1 : -1;

	// Deltas (checking for infinity)
	stepDx = (input.vector.x == 0.f) ? INFINITY : ( 1.f / (stepX * input.vector.x) );
	stepDy = (input.vector.y == 0.f) ? INFINITY : ( 1.f / (stepY * input.vector.y) );
	stepDz = (input.vector.z == 0.f) ? INFINITY : ( 1.f / (stepZ * input.vector.z) ); 

	// Initial tMAX
	tMAXx = (input.vector.x == 0.f) ? INFINITY : ( ((input.origin.x + ((stepX > 0) ? 1 : 0)) - input.origin.x) / input.vector.x );
	tMAXy = (input.vector.y == 0.f) ? INFINITY : ( ((input.origin.y + ((stepY > 0) ? 1 : 0)) - input.origin.y) / input.vector.y );
	tMAXz = (input.vector.z == 0.f) ? INFINITY : ( ((input.origin.z + ((stepZ > 0) ? 1 : 0)) - input.origin.z) / input.vector.z ); 

	boundx = (input.vector.x > 0) ? vectorSize : 0;
	boundx = (input.vector.y > 0) ? vectorSize : 0;
	boundx = (input.vector.z > 0) ? vectorSize : 0;

	
}