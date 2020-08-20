`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 16.12.2017 15:18:11
// Design Name: 
// Module Name: edge_detector
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


module posedge_detector(
    input clk, in,
    output out
    );
    
    // Register
    logic in_delayed;
    
    // Take old in into in_delayed
    always_ff @(posedge clk)
        begin
            in_delayed <= in;
        end
        
    // Main logic for edge detection
    assign out = in & ~in_delayed;
    
endmodule
