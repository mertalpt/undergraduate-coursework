`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.11.2017 13:09:41
// Design Name: 
// Module Name: rng_4bit
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


module rng_4bit(
    input logic clk, reset,
    output logic [1:0] rndSpeed, rndDirection
    );
    
    // Internal logic and registers
    logic [7:0] speedRegisters, directionRegisters;
    logic speedFeedback, directionFeedback;
    
    // Feedback with XORs
    assign speedFeedback = speedRegisters[7] ^ speedRegisters[6] ^ speedRegisters[5] ^ speedRegisters[4];
    assign directionFeedback = directionRegisters[7] ^ directionRegisters[5] ^ directionRegisters[4] ^ directionRegisters[3];
    
    always_ff @(posedge clk)
        begin
            if (reset) // Reset to 1, because this LSFR is built with XORs
                begin
                    speedRegisters <= 8'h01;
                    directionRegisters <= 8'h01;
                end
            else // Shift the registers and feed feedback into first register
                begin
                    speedRegisters <= {speedRegisters[6:0], speedFeedback};
                    directionRegisters <= {directionRegisters[6:0], directionFeedback};
                end
        end
    
    assign rndSpeed[1:0] = speedRegisters[1:0];
    assign rndDirection[1:0] = directionRegisters[1:0];
    
endmodule
