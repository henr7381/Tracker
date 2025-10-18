import sys
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import numpy as np

# Set up interactive 3D plot
plt.ion()  # Interactive mode for live updates
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.set_xlim([-10, 10])
ax.set_ylim([-10, 10])
ax.set_zlim([-10, 10])
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# Initial empty ray plot (will update)
origin = np.array([0, 0, 0])
direction = np.array([1, 0, 0])
ray_line, = ax.plot([origin[0], origin[0] + direction[0]],
                    [origin[1], origin[1] + direction[1]],
                    [origin[2], origin[2] + direction[2]], 'r-', linewidth=2)

# Loop to read from stdin and update plot
for line in sys.stdin:
    try:
        parts = list(map(float, line.strip().split()))
        if len(parts) != 6:
            continue
        origin = np.array(parts[:3])
        direction = np.array(parts[3:])

        # Update the ray line with new data
        ray_line.set_data_3d([origin[0], origin[0] + direction[0]],
                             [origin[1], origin[1] + direction[1]],
                             [origin[2], origin[2] + direction[2]])
        
        # Redraw the plot
        fig.canvas.draw()
        fig.canvas.flush_events()
    except ValueError:
        print("Invalid ray data received", file=sys.stderr)