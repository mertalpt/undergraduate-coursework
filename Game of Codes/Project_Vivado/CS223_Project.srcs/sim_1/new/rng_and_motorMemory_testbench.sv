`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.12.2017 11:40:03
// Design Name: 
// Module Name: rng_and_motorMemory_testbench
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module rng_and_motorMemory_testbench( );

    // Internal wires
    logic clk, reset, load;
    logic [1:0] direction, speed;
    logic [3:0] internal, out;
    
    assign internal[3] = speed[0];
    assign internal[2] = direction[0];
    assign internal[1] = speed[1];
    assign internal[0] = direction[1];
    
    // Test modules
    rng_4bit rng(clk, reset, speed, direction);
    movement_memory dut(clk, reset, load, internal, out);
    
    // Test code
    
    initial
        begin
            reset = 0;
            load = 0;
            clk = 0;
            
            #0.1;
            
            reset = 1;
            clk = 1;
            
            #0.1;
            
            reset = 0;
            clk = 0;
            
            #0.1;
            
            for (int i = 0; i < 512; i++)
                begin
                    clk <= ~clk;
                    
                    if (i === 64)
                        load <= 1;
                    if (i === 66)
                        load <= 0;
                        
                    #0.1;
                end
        end
    
endmodule
