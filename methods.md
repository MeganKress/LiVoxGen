---
layout: page
title: Methods
---

# Methods in the Creation of VoxGen

<hr>

## Custom Classes
 
### Point


The simple Point class contains the x, y, and z coordinates for a point.
 
### Voxel


The Voxel class contains the x, y, and z coordinates for the center of the voxel
along with the number of points in the voxel and a vector of the points.
This class also contains two functions: inVox() and trimVox(). inVox() takes
in as arguments a point or 3D coordinates and returns whether or not that
point is within the voxel. trimVox() is called on a voxel in VoxGen because
the kdtree library’s function to determine the points within a given range of
a point returns points in a sphere around the voxel, and the Voxel class only
wants points within the rectangular prism that is the voxel.

 

 
### VoxCol


The VoxCol class simply contains the x and y center coordinates for a given
voxel column and a vector of voxels (Voxel objects) that are within that
column.
 

 
### VoxData


VoxData is a class that summarizes the voxel information for a given LAS file.
It includes a vector of all the voxel columns (VoxCol objects), the number
of points in all the voxels, the number of voxel columns, and the number of
voxels in each voxel column.
VoxData also includes methods to read and write files.
 

 
### vgpar


vgpar requires a text document parameter file of relevant information for the
VoxGen program (see below).
The vgpar class constructor takes in a string of the text file location, and
the class includes functions getString() and getNum(), which utilize a map
to obtain a value referenced in the paragraph.
For example, getString(“filter”) returns the string “n,” which is then used
as user input in VoxGen to determine that the user does not wish to filter
points from the LAS file.
 
<hr>

## Functions
 
- Voxelize: VoxGen takes in user input in the form of a text file. Based on the LAS file and filters specified, VoxGen divides the points from the LAS file into voxels of specified dimensions.

- Output R File: If the user chooses to create an R file, VoxGen will create an R source file, which will input various matrices into the R workspace that contain voxel information.

- VoxData File: VoxGen reads and writes text files to create or save VoxData object information.

- Voxel Column Metrics Flat File: VoxGen writes a text file in which each row represents data corresponding to a voxel column’s metrics.

- Voxel Column Histogram Flat File: VoxGen writes a text file in which each row has the number of points in each voxel column’s voxels.


<hr>
 
## Quality Assurance
 
 

QGIS was used to check the accuracy of the output lidar metric flat files.The flat file was loaded into a spreadsheet program, and four metric raster layers were loaded into QGIS. A random number generated selected whichrows from the flat file to examine. ”ZoomToCoordinates” found the point inQGIS, and the ”Identify Features” tool displayed the metric values at that point. All of the 100 centroids had data in the flat file that matched the observed data in QGIS.

 

An R script (VoxGenTest.R) was utilized to check the ac-curacy of the output voxel histogram flat files. The script loads the .txt file conversion of a given split LAS file along with the output histogram flat file.Next, based on the centers of each voxel in the flat file, the script determineshow many points in the LAS file lie within the voxel. The script then counts the number of mismatches between numbers of points. Running this script gave an accuracy of 100% for 1.48 million voxels.

 
