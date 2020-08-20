`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 17.12.2017 11:14:49
// Design Name: 
// Module Name: idle_detector_4bit
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


module idle_detector_4bit(
        input logic clk,
        input logic [3:0] signal,
        output logic idle
    );
    
    // Internal logic
    logic didChange; // Keeps track of signal change per clock
    
    // Register for delayed signal
    logic [3:0] signal_delayed;
    
    // Register for certainty of idleness
    logic [26:0] certainty;
    
    // Implement change logic
    assign didChange = (signal[3] ^ signal_delayed[3]) | (signal[2] ^ signal_delayed[2]) | (signal[1] ^ signal_delayed[1]) | (signal[0] ^ signal_delayed[0]);
    
    // Main logic
    always_ff @(posedge clk)
        begin
            signal_delayed <= signal;
        
            if (didChange) // If there is a change, keep certainty at maximum
                certainty <= 27'b111_1111_1111_1111_1111_1111_1111;
            else if (~idle) // If there is not a change and idle is not 1, decrement
                certainty <= certainty - 1;
            else // If there is not a change and idle is 1, keep certainty at minimum
                certainty <= 27'b0;
        end
        
    assign idle = 
        ~(certainty[26] | certainty[25] | certainty[24] | certainty[23] 
        | certainty[22] | certainty[21] | certainty[20] | certainty[19] 
        | certainty[18] | certainty[17] | certainty[16] | certainty[15]
        | certainty[14] | certainty[13] | certainty[12] | certainty[11] 
        | certainty[10] | certainty[9]  | certainty[8]  | certainty[7] 
        | certainty[6]  | certainty[5]  | certainty[4]  | certainty[3] 
        | certainty[2]  | certainty[1]  | certainty[0]);
endmodule
