`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 17.12.2017 12:13:25
// Design Name: 
// Module Name: signal_delayer_1bit
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


module signal_delayer_1bit(
        input logic clk, signal,
        output logic counting, signal_delayed
    );
    
    // Logic for signal was receiver in the last delay period
    logic enable;
    
    assign enable = signal | counting;
    
    // Logic for counter is active
    // logic counting; // Made into an output
    
    // Down counter register
    logic [26:0] counter;
    
    always_ff @(posedge clk)
        begin
            if (enable == 1 && counter != 27'b0)
                begin
                    counter <= counter - 1;
                    counting <= 1'b1;
                end
            else
                begin
                    counter <= 27'b111_1111_1111_1111_1111_1111_1111;
                    counting <= 1'b0;
                end
        
        end
        
    assign signal_delayed = 
        ~(counter[26] | counter[25] | counter[24]
        | counter[23] | counter[22] | counter[21] | counter[20]
        | counter[19] | counter[18] | counter[17] | counter[16]
        | counter[15] | counter[14] | counter[13] | counter[12]
        | counter[11] | counter[10] | counter[9]  | counter[8]
        | counter[7]  | counter[6]  | counter[5]  | counter[4]
        | counter[3]  | counter[2]  | counter[1]  | counter[0]);
        
endmodule
