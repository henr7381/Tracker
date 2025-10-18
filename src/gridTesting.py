import numpy as np
import pyvista as pv

# Create a 3D voxel grid (500^3; use uint8 to save memory ~125MB)
grid_size = 300
volume_data = np.zeros((grid_size, grid_size, grid_size), dtype=np.uint8)

# Populate with example data (e.g., a sphere; replace with your voxel logic)
x, y, z = np.indices((grid_size, grid_size, grid_size))
center = grid_size // 2
radius = 10
volume_data = ((x - center)**2 + (y - center)**2 + (z - center)**2 <= radius**2).astype(np.uint8)

# Create PyVista ImageData grid for volume
grid = pv.ImageData(dimensions=(grid_size, grid_size, grid_size))
grid.point_data["values"] = volume_data.ravel(order="F")  # Use point_data, not cell_data

# Plot with volume rendering (GPU accelerated)
p = pv.Plotter(off_screen=False)  # Interactive window; use True for headless
p.add_volume(grid, scalars="values", opacity="sigmoid", cmap="viridis")  # Opacity transfer for visibility
p.show_bounds(grid="front", location="outer", all_edges=True)
p.show()