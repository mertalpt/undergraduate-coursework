`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.12.2017 11:24:35
// Design Name: 
// Module Name: movement_memory
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


module movement_memory(
        input logic clk, reset, load,
        input logic [3:0] movementIn,
        output logic [3:0] movementMem
    );
    
    always_ff @(posedge clk)
        begin
            if (reset)
                movementMem <= 4'b0;
            else if (load == 1)
                movementMem <= movementIn;
            else
                movementMem <= movementMem;
        end
        
endmodule
