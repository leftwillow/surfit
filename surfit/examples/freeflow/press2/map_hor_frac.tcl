#
load libfreeflow[info sharedlibextension]

clear_data
set tol 5e-7

set map_name map_hor_frac

pnts_read PZab_2001.txt press2001 5 4 3 5 " \t"
# recalc to MPa
pnts_mult_real 0.1013 press2001

cntr_read lines/P_2001_236_hor.txt P_2001_236_hor
# recalc to MPa
cntr_mult_real 0.1013

pnts_read Q_2001.txt debits2001 5 4 3 5 " \t"
pnts_mult_real -1 debits2001

curv_read VNK1.txt VNK1 2 2 1
curv_read VNK2.txt VNK2 2 2 1
curv_read VNK3.txt VNK3 2 2 1
area_read ZAM1.txt ZAM1 2 2 1
curv_read ZAM2.txt ZAM2 2 2 1
area_read ZAM5.txt ZAM5 2 2 1
curv_read lines/frac_236_hor.txt frac_236_hor

grid_get 28300 41400 50 45500 64700 50

points press2001
isoline 21.4 VNK1
isoline 21.4 VNK2
isoline 21.4 VNK3
area undef ZAM1
area undef ZAM5
lcm_simple 16.4 0.5 1e-6
contour_add 1 P_2001_236_hor 
prod_points debits2001
fault_add ZAM2
prod_isoline -0.000467413 frac_236_hor

cmofs

grid_unload
file_save map_hor_frac.dat