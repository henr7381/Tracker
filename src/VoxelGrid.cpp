// Voxel Grid implementation


// Includes


// User includes
#include "VoxelGrid.h"


VoxelGrid::VoxelGrid(uint16_t N, uint8_t darkenFactor)
{
	this -> darkenFactor = darkenFactor;
	this -> vectorSize = N;
	this -> memSize = N * N * N * sizeof(uint8_t);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) 
	{
        printf("shm_open");
    }

    if (ftruncate(shm_fd, memSize) == -1) 
	{
        printf("ftruncate");
    }

    voxel_grid = (uint8_t*)mmap(nullptr, memSize, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (voxel_grid == MAP_FAILED) 
	{
        printf("mmap");
    }

    update_sem = sem_open(SEM_NAME, O_CREAT, 0666, 0);
    if (update_sem == SEM_FAILED) 
	{
        printf("sem_open");
    }

	std::string command = "python3 ./src/RenderingUtility.py " + std::to_string(vectorSize);
	int result = std::system(command.c_str());
	if (result != 0) 
	{
		printf("Warning: Python script execution failed with code %u\n", result);
	}
}


void VoxelGrid::walkRay(Ray input)
{
	sem_wait(update_sem);

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

	// Determine bounds
	boundx = (input.vector.x > 0) ? vectorSize : 0;
	boundy = (input.vector.y > 0) ? vectorSize : 0;
	boundz = (input.vector.z > 0) ? vectorSize : 0;

	// Mark our current voxel
	markVoxel(input.origin);

	// For stepping
	uint16_t idx_x = input.origin.x;
	uint16_t idx_y = input.origin.y;
	uint16_t idx_z = input.origin.z;

	// Main loop
	while (true)
	{
		if (tMAXx < tMAXy)
		{
			if (tMAXx < tMAXz)
			{
				idx_x += stepX;
				if (idx_x == boundx)
				{
					break;
				}
				tMAXx += stepDx;
			}
			else
			{
				idx_z += stepZ;
				if (idx_z == boundz)
				{
					break;
				}
				tMAXz += stepDz;
			}
		}
		else
		{
			if (tMAXy < tMAXz)
			{
				idx_y += stepY;
				if (idx_y == boundy)
				{
					break;
				}
				tMAXy += stepDy;
			}
			else
			{
				idx_z += stepZ;
				if (idx_z == boundz)
				{
					break;
				}
				tMAXz += stepDz;
			}
		}

		if ( ((0 <= idx_x) && (idx_x < vectorSize)) && 
			 ((0 <= idx_y) && (idx_y < vectorSize)) && 
			 ((0 <= idx_z) && (idx_z < vectorSize))	)
			{
				markVoxel(Point3D{(double)idx_x, (double)idx_y, (double)idx_z});		// Kinda hate this, bit it is what it is
			}
			else
			{
				break;
			}
	}

	sem_post(update_sem);
}


void VoxelGrid::markVoxel(Point3D p)
{
	voxel_grid[(uint16_t)p.x + 
		       (vectorSize * (uint16_t)p.y) + 
		       (vectorSize * vectorSize * (uint16_t)p.z)] += darkenFactor;	// Also kinda hate this, bit it is what it is
}