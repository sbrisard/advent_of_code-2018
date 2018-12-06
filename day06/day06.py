import numpy as np
import scipy.ndimage

if __name__ == '__main__':
    target = np.loadtxt('input.txt', dtype=np.int, delimiter=',')
    xmin, ymin = np.amin(target, 0)
    xmax, ymax = np.amax(target, 0)
    x = np.arange(xmin, xmax+1)
    y = np.arange(ymin, ymax+1)
    # distance[k, i, j]: Manhatan distance between target[k] and (i, j)
    distance = (np.abs(x[None, :, None]-target[:, 0, None, None]) +
                np.abs(y[None, None, :]-target[:, 1, None, None]))
    closest_point = np.argmin(distance, axis=0)
    min_distance = np.amin(distance, axis=0)
    # Find points that are equally distant to at least two targets and set the
    # corresponding closest target to -1
    duplicate = np.sum(distance == min_distance, axis=0) > 1
    closest_point[duplicate] = -1
    area = scipy.ndimage.sum(np.ones_like(closest_point), closest_point,
                             np.arange(target.shape[0]))
    # Targets with infinite areas meet the boundaries
    on_boundary = np.ones_like(closest_point, dtype=np.bool)
    on_boundary[1:-1, 1:-1] = False
    infinite = np.unique(closest_point[on_boundary])
    area[infinite[infinite != -1]] = np.nan
    print('The size of the largest area that isn\'t infinite is: '
          '{}'.format(np.nanmax(area)))

    total_distance = np.sum(distance, axis=0)
    print('The size of the region containing all locations which have a total '
          'distance to all given coordinates of less than 10000 is: '
          '{}'.format(np.sum(total_distance < 10000)))

# The size of the largest area that isn't infinite is: 3894.0 The size
# of the region containing all locations which have a total distance
# to all given coordinates of less than 10000 is: 39398
