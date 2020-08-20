`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 07.12.2017 15:47:59
// Design Name: 
// Module Name: rng_testbench
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


module rng_testbench( );

// Internal logic
logic clk, reset;
logic [1:0] speed, direction;
logic [7:0] sRegisters, dRegisters;

// Module to test
rng_4bit dut(clk, reset, speed, direction, sRegisters, dRegisters);

// Testing
initial
begin
    clk = 0;
    reset = 0;

    #0.1;   
    
    reset = 1;
    
    #0.1;
    
    clk = 1;
    
    #0.1;
    
    reset = 0;
    
end

always
begin
            #0.1;
            clk <= ~clk;
            
end
endmodule
