## CLOCK

set_property PACKAGE_PIN W5 [get_ports clk]							
	set_property IOSTANDARD LVCMOS33 [get_ports clk]
	create_clock -add -name sys_clk_pin -period 10.00 -waveform {0 5} [get_ports clk]

## RESET AND START

set_property PACKAGE_PIN W19 [get_ports start]						
	set_property IOSTANDARD LVCMOS33 [get_ports start]
set_property PACKAGE_PIN T17 [get_ports reset]						
	set_property IOSTANDARD LVCMOS33 [get_ports reset]

## KEYPAD PINS

set_property PACKAGE_PIN A15 [get_ports {keyb_row[0]}] 	 	 	 	 	 
 	set_property IOSTANDARD  LVCMOS33 [get_ports {keyb_row[0]}] 
set_property PACKAGE_PIN A17 [get_ports {keyb_row[1]}] 	 	 	 	 	 
 	set_property IOSTANDARD  LVCMOS33 [get_ports {keyb_row[1]}] 
set_property PACKAGE_PIN C15 [get_ports {keyb_row[2]}] 	 	 	 	 	 
 	set_property IOSTANDARD  LVCMOS33 [get_ports {keyb_row[2]}] 
set_property PACKAGE_PIN C16 [get_ports {keyb_row[3]}] 	 	 	 	 	 
 	set_property IOSTANDARD  LVCMOS33 [get_ports {keyb_row[3]}] 

set_property PACKAGE_PIN A14 [get_ports {keyb_col[0]}] 	 	 	 	 	 
 	set_property IOSTANDARD  LVCMOS33 [get_ports {keyb_col[0]}] 
set_property PACKAGE_PIN A16 [get_ports {keyb_col[1]}] 	 	 	 	 	 
 	set_property IOSTANDARD  LVCMOS33 [get_ports {keyb_col[1]}] 
set_property PACKAGE_PIN B15 [get_ports {keyb_col[2]}] 	 	 	 	 	 
 	set_property IOSTANDARD  LVCMOS33 [get_ports {keyb_col[2]}] 
set_property PACKAGE_PIN B16 [get_ports {keyb_col[3]}] 	 	 	 	 	 
 	set_property IOSTANDARD  LVCMOS33 [get_ports {keyb_col[3]}]
 	
## DELAY IN PROCESS INDICATOR

set_property PACKAGE_PIN V13 [get_ports {delayInProcess}]
    set_property IOSTANDARD LVCMOS33 [get_ports {delayInProcess}]

## MOTOR CONTROLLER PINS

# MOTOR IDLE INDICATOR

set_property PACKAGE_PIN L1 [get_ports {motorIdle}]
    set_property IOSTANDARD LVCMOS33 [get_ports {motorIdle}]

# LEDs from MEMORY

set_property PACKAGE_PIN  N3 [get_ports {motorInternal[3]}]
    set_property IOSTANDARD LVCMOS33 [get_ports {motorInternal[3]}]
set_property PACKAGE_PIN  P3 [get_ports {motorInternal[2]}]
    set_property IOSTANDARD LVCMOS33 [get_ports {motorInternal[2]}]
set_property PACKAGE_PIN  U3 [get_ports {motorInternal[1]}]
    set_property IOSTANDARD LVCMOS33 [get_ports {motorInternal[1]}]
set_property PACKAGE_PIN  W3 [get_ports {motorInternal[0]}]
    set_property IOSTANDARD LVCMOS33 [get_ports {motorInternal[0]}]
            
#Main Controls

#4 Phases
#step motor: Bb
set_property PACKAGE_PIN P18 [get_ports {motorOut[0]}]                    
     set_property IOSTANDARD  LVCMOS33 [get_ports {motorOut[0]}] 
#step motor: Ab
set_property PACKAGE_PIN N17 [get_ports {motorOut[1]}]                          
     set_property IOSTANDARD  LVCMOS33 [get_ports {motorOut[1]}] 
#step motor: B
set_property PACKAGE_PIN M18 [get_ports {motorOut[2]}]                          
     set_property IOSTANDARD  LVCMOS33 [get_ports {motorOut[2]}] 
#step motor: A
set_property PACKAGE_PIN K17 [get_ports {motorOut[3]}]                          
     set_property IOSTANDARD  LVCMOS33 [get_ports {motorOut[3]}] 
     
## SEVEN SEGMENT DISPLAY PINS

set_property PACKAGE_PIN W7 [get_ports {sevSegOut[6]}] 	 	 	 	 	 
 	set_property IOSTANDARD LVCMOS33 [get_ports {sevSegOut[6]}] 
set_property PACKAGE_PIN W6 [get_ports {sevSegOut[5]}] 	 	 	 	 	 
 	set_property IOSTANDARD LVCMOS33 [get_ports {sevSegOut[5]}] 
set_property PACKAGE_PIN U8 [get_ports {sevSegOut[4]}] 	 	 	 	 	 
 	set_property IOSTANDARD LVCMOS33 [get_ports {sevSegOut[4]}] 
set_property PACKAGE_PIN V8 [get_ports {sevSegOut[3]}] 	 	 	 	 	 
 	set_property IOSTANDARD LVCMOS33 [get_ports {sevSegOut[3]}] 
set_property PACKAGE_PIN U5 [get_ports {sevSegOut[2]}] 	 	 	 	 	 
 	set_property IOSTANDARD LVCMOS33 [get_ports {sevSegOut[2]}] 
set_property PACKAGE_PIN V5 [get_ports {sevSegOut[1]}] 	 	 	 	 	 
 	set_property IOSTANDARD LVCMOS33 [get_ports {sevSegOut[1]}] 
set_property PACKAGE_PIN U7 [get_ports {sevSegOut[0]}] 	 	 	 	 	 
 	set_property IOSTANDARD LVCMOS33 [get_ports {sevSegOut[0]}]
 	
set_property PACKAGE_PIN V7 [get_ports dp]  	 	 	 	  
 	set_property IOSTANDARD LVCMOS33 [get_ports dp]
 	
set_property PACKAGE_PIN U2 [get_ports {an[0]}] 	 	 	 	 	 
      set_property IOSTANDARD LVCMOS33 [get_ports {an[0]}] 
set_property PACKAGE_PIN U4 [get_ports {an[1]}]                          
      set_property IOSTANDARD LVCMOS33 [get_ports {an[1]}] 
set_property PACKAGE_PIN V4 [get_ports {an[2]}]                          
      set_property IOSTANDARD LVCMOS33 [get_ports {an[2]}] 
set_property PACKAGE_PIN W4 [get_ports {an[3]}]                          
      set_property IOSTANDARD LVCMOS33 [get_ports {an[3]}] 
