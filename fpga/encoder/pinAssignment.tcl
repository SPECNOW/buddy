set_location_assignment PIN_112 -to A1
set_location_assignment PIN_113 -to B1
set_location_assignment PIN_114 -to	out1_fwrd
set_location_assignment PIN_115 -to out1_bwrd
set_location_assignment PIN_116 -to Xor1

set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to A1
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to B1
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to Xor1
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to out1_fwrd
set_instance_assignment -name IO_STANDARD "3.3-V LVTTL" -to out1_bwrd
