import numpy as np
import matplotlib.pyplot as plt
import itertools

# Data input
data = None  # Paste data here
yaw_range = range(data['minYaw'], data['maxYaw'] + 1, data['yawStep'])
pitch_range = range(data['minPitch'], data['maxPitch'] + 1, data['pitchStep'])


def main():
    # Calculate points
    points = []
    for yawi, yaw in enumerate(yaw_range):
        for pitchi, pitch in enumerate(pitch_range):
            dist = data['distances'][yawi][pitchi]
            if dist > 30:
                continue  # Filter out any points with distances more than 30cm
            yaw_adj = np.radians(yaw - (data['minYaw'] + data['maxYaw']) / 2)
            pitch_adj = np.radians(pitch - 30)
            z = dist * np.cos(yaw_adj) * np.cos(pitch_adj)
            y = dist * np.sin(pitch_adj)
            x = dist * np.cos(pitch_adj) * np.sin(yaw_adj)
            points += [[x, y, z]]
    points = np.array(points)

    # Plot points
    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    ax.scatter(points[:, 0], points[:, 2], points[:, 1], 'ob')
    plt.show()


if __name__ == '__main__':
    main()
