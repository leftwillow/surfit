#
load libfreeflow[info sharedlibextension]

set deb -40.3844832
set press 12

set a_desc "func debit : "
set b_desc "debit resid : "
set c_desc "func press : "
set d_desc "press resid : "

puts $deb
puts $press


func_load map_hor_frac.dat

set a [expr 24*3600*[func_debit_rect 34000 57600 36000 59200 16.4 0.5 1e-6]]
set b [expr $a - $deb]
set c [func_getValue 34967 58314]
set d [ expr $press - $c ]
puts $a_desc$a
puts $c_desc$c
puts $b_desc$b
puts $d_desc$d 
set e [func_D1]
puts $e
func_del
puts " "

func_load map_ver_frac.dat

set a [expr 24*3600*[func_debit_rect 34000 57600 36000 59200 16.4 0.5 1e-6]]
set b [expr $a - $deb]
set c [func_getValue 34967 58314]
set d [ expr $press - $c ]
puts $a_desc$a
puts $c_desc$c
puts $b_desc$b
puts $d_desc$d 
set e [func_D1]
puts $e
func_del
puts " "
