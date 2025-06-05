trace = [] # (... 轨迹数据)

import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.mplot3d import Axes3D
def plot_trace(trace):
    trace = np.array(trace)
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='3d')
    
    # Plot the trace
    ax.plot(trace[:, 0], trace[:, 1], trace[:, 2], marker='o', color='b', label='Trace Path')
    
    # Set labels
    ax.set_xlabel('X-axis')
    ax.set_ylabel('Y-axis')
    ax.set_zlabel('Z-axis')
    ax.set_title('3D Trace Plot')
    
    # Show grid
    ax.grid(True)
    
    # Show legend
    ax.legend()
    
    plt.show()
    
if __name__ == "__main__":
    plot_trace(trace)