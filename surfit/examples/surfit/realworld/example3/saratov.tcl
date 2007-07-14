#
# This script was generated with funner v2.2

# 
# load plugins
# 
load libsurfit[info sharedlibextension]
load libsurfit_io[info sharedlibextension]

# remove all previous data and gridding rules
clear_data 

# set name of surface
set map_name "saratov" 

# set solver
set_solver "cg" 

# set tolerance for cg solver
set tol 1e-007

##
## load initial data 
##

# load isolines from Surfer BLN file
cntr_load_bln "isolines.bln"

# load faults from Surfer BLN file
curv_load_bln "faults.bln"

##
## construct grid 
##
grid 10 10
 
##
## create gridding rules
##

# fault lines
fault "fault*"

# resulting surface should tend to be constant or plane 
completer 1 50

# add "resulting surface should interpolate contours"
contours_add 3 "iso*"

##
## run gridding algorithm
##
surfit 

##
## save results 
##

# save surface to surfit datafile 
surf_save "saratov.dat" $map_name 

# plot resulting surface in EPS
surf_plot "saratov.eps"