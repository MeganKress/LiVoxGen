---
layout: page
title: About LiVoxGen
---
## About
LiVoxGen is an open source C++ program. Its purpose is to allow processing of LiDAR data into voxels
(volumetric pixels).

More information can be found at the [repository on Github](https://github.com/MeganKress/LiVoxGen/).

Voxels are commonly used in the medical field and in video game graphics,
but their use in geographic surfaces is only beginning to be explored with
the availability of higher computational power.

The input files in VoxGen, LAS files contain data from LiDAR point
clouds. Each point has a set of properties described by the LAS file. 

VoxGen takes in a LAS file along with various metric files (in the form of TIFF) that correspond to that
LAS file and parses the data in two ways:
- Voxelization: With dimensions chosen by the user, VoxGen splits the
bounded LAS file into volumetric pixels (voxels) and outputs a file
with the the center coordinates of each voxel and the points within
each voxel.
- Determination of Metrics for Voxel Column Centroids: For each col-
umn of voxels, VoxGen outputs a file with the column’s centroid xy-
coordinates along with corresponding metrics for that coordinate (eg.
densities, percentiles, tree cover, etc.).

### Setup
Before installing LiVoxGen, ensure that you have the required [libraries](http://megankress.github.io/LiVoxGen//libraries.html).

LiVoxGen may be installed by simply downloading the .zip folder from the [repository on Github](https://github.com/MeganKress/LiVoxGen/archive/master.zip).

After extracting the content from the folder into the selected directory, type "make" to create the 
LiVoxGen executable program.

### Additional Programs

The VoxGen repository contains three additional programs to aid in the processing of LiDAR data.

1. MinMaxZ: 

The purpose of MinMaxZ is to obtain the minimum and maximum z values
for all LAS files in a certain acqusition. This allows one to run VoxGen
with consistent voxel heights for all segments. MinMaxZ is executed on the
directory containing all LAS files for a given acquisition, and it outputs a text
file “range.txt” that contains the bounds for all LAS files in the directory.

2. VGProj: 

VGProj simply takes in a LAS file as an argument and prints its Spatial Reference. The user may then document the projection for a given acquisition,
which is essential for reprojecting the lidar metric flat files when processing
them with a shapefile or raster with a different spatial reference.

3. SplitLas: 

SplitLas tiles LAS files into manageable smaller LAS files so that VoxGen
may be run more efficiently.

### Acknowledgements
LiVoxGen was created under the guidance and with the assistance of Dr. Andrew Finley of the Michigan State University Forestry department.

Jacopo Rabolini created LiVoxGen website [template](http://www.jacoporabolini.com/emerald/).
