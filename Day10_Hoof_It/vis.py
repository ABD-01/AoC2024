# Define a custom colormap where low values are blue/green and high values are red/orange
from matplotlib.colors import Normalize
import matplotlib.cm as cm
import numpy as np
import matplotlib.pyplot as plt

# Given topographic map as a string
topo_map_str = """89010123
78121874
87430965
96549874
45678903
32019012
01329801
10456732"""

# with open("input.txt") as f:
    # topo_map_str = f.read().strip()

# Convert the topographic map into a 2D NumPy array
topo_map = np.array([list(map(int, list(row))) for row in topo_map_str.split("\n")])


# Normalize height values for coloring
norm = Normalize(vmin=np.min(topo_map), vmax=np.max(topo_map))
colormap = cm.get_cmap('RdYlBu_r')  # Reversed Red-Yellow-Blue colormap

# Create a new 3D plot for the box-style visualization
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Plot each box (voxel-like) at its height with corresponding color
for i in range(topo_map.shape[0]):
    for j in range(topo_map.shape[1]):
        height = topo_map[i, j]
        color = colormap(norm(height))  # Get color based on height
        
        # Draw a 3D bar at (j, i) with height 'height'
        ax.bar3d(j, i, 0, 1, 1, height, color=color, shade=True)

# Labels and title
ax.set_xlabel('X Coordinate')
ax.set_ylabel('Y Coordinate')
ax.set_zlabel('Elevation')
ax.set_title('3D Topographic Map Visualization')

# Show the plot
plt.show()
