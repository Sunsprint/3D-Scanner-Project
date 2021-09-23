import numpy as np
import matplotlib.pyplot as plt
import itertools


data = {'minYaw': 0, 'maxYaw': 90, 'yawStep': 5, 'minPitch': 135, 'maxPitch': 180, 'pitchStep': 5, 'pointExists': [[False, True, True, True, True, True, True, True, True, True], [False, True, True, True, True, True, True, True, True, True], [False, True, True, True, True, True, True, True, True, True], [False, True, True, True, True, True, True, True, True, True], [False, True, True, True, True, True, True, True, True, True], [False, False, False, False, True, True, True, False, False, False], [False, False, False, False, False, False, False, True, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, False, False, False, False, False], [False, False, False, False, False, True, False, False, False, False], [False, False, False, False, False, False, True, True, True, False]]}
yaw_range = range(data['minYaw'], data['maxYaw'] + data['yawStep'], data['yawStep'])
pitch_range = range(data['minPitch'], data['maxPitch'] + data['pitchStep'], data['pitchStep'])


def main():
    vis = []
    invis = []
    for yawi, yaw in enumerate(yaw_range):
        for pitchi, pitch in enumerate(pitch_range):
            z = 20
            y = z * np.sin(-np.radians(pitch - 180))
            x = z * np.sin(np.radians(yaw - 45))
            if data['pointExists'][yawi][pitchi]:
                vis += [[x, y, z]]
            else:
                invis += [[x, y, z]]
    vis = np.array(vis)
    invis = np.array(invis).reshape(-1, 3)
    print(vis.shape)
    print(invis.shape)

    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    ax.scatter(vis[:, 0], vis[:, 2], vis[:, 1], 'ob')
    ax.scatter(invis[:, 0], invis[:, 2], invis[:, 1], 'or')
    plt.show()


if __name__ == '__main__':
    main()
