#
load libsurfit[info sharedlibextension]

# resulting surface must have name "map_wtask"
set map_name map_wtask

# reading surfit_task (scattered data points)
# from 3-column text file
task_read task.txt task

# converting surfit_task to surfit_wtask
task_to_wtask 0.6

set tol 5e-7

# setting surface behavoiur between scattered data points
completer_set 1 2

## settings for very fast calculation (and unprepossessing result)
#set tol 4e-6
#completer_set 1 0

# constructing surfit_grid
grid_auto 150 150

# running CMOFS algorithm
solve

# unloading surfit_grid from memory
grid_unload

# saving results to binary ROFF file
file_save map_wtask.dat

wtask_to_task
func_resid task_residuals.txt 

task_read task_residuals.txt residuals 4 1 2 4
set task_mean [task_mean]
putlog "residuals mean value is $task_mean"
putlog "residuals standard deviation is [task_std $task_mean]"
putlog "func_D1 is [func_D1]"

