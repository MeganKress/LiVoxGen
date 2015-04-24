require(data.table)

# Load the txt file of the LAS file as pts
pts <- "/media/megan/backup/Data/20140730_farmers_loop/las/las/txt/c0r0_9.txt"

# Load the histogram flat file as vox
vox <- "/home/megan/Downloads/Hist/histff_20140730_farmers_loop_c0r0-9"

# Convert data sets into data frames
pts <- fread(pts)
setnames(pts,c("x","y","z"))
pts <- as.data.frame(pts)

vox <- fread(vox)

setnames(vox, as.character(vox[1, ]))
vox <- vox[2:nrow(vox),]
vox <- as.data.frame(vox)

# Set the min and max z values for the voxels
minZ = 138.57
maxZ = 268.67

mistakes <- 0

h = 0.3
hh = 0.15
w = 13
hw = 6.5

voxct <- ncol(vox) - 4
rows <- nrow(vox)

randI <- sample(1:rows,1000)
points <- 0

for(index in 1:rows)
{
  
  cat(index,"out of", rows,"\n")
  
  x <- as.numeric(vox$x[index])
  y <- as.numeric(vox$y[index])
  
  TF <- pts$x > x - hw & pts$x <= x + hw
  pts1 <- subset(pts, TF)
  TF <- pts1$y > y - hw & pts1$y <= y + hw
  pts1 <- subset(pts1, TF)
  
  ptCt <- 0
  for(j in 5:ncol(vox))
  {
    ptCt <- ptCt + vox[index, j]
    
  }
  points <- points + ptCt

  
    # Test the point count for each voxel
    # against the number of points from
    # LAS file in that range
  
    z <- minZ
    for(i in 1:voxct)
    {
      z <- z + hh
      
      TF <- pts1$z > z - hh & pts1$z <= z + hh
      pts1.2 <- subset(pts1, TF)
      
      if(nrow(pts1.2) > 0)
      {
        if(nrow(pts1.2) != vox[index,i + 4])
        {
          cat("Mismatch\n", nrow(pts1.2), " ", vox[index,i+4],"\n")
          mistakes <- mistakes + 1
        }
        
      }
      
      
      z <- z + hh
    }
  
}

print(points)
print(mistakes)