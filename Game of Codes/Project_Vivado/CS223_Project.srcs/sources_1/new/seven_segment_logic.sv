`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.11.2017 14:03:43
// Design Name: 
// Module Name: sevenSegDisp
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


module seven_segment_logic(
        input logic clk, reset, posedgeKeyValid,
        input logic [3:0] keyValue, motorValue,
        output logic [6:0] digit,
        output logic [3:0] an,
        output logic dp 
    );
    
    // Internal wires and registers
    logic isCorrect;
    logic [3:0] score;
    
    // Combinational comparator module takes keyValue and motorValue and outputs a single bit for true or false
    
    assign isCorrect = ~(keyValue[3] ^ motorValue[3]) &  ~(keyValue[2] ^ motorValue[2]) &  ~(keyValue[1] ^ motorValue[1]) &  ~(keyValue[0] ^ motorValue[0]);
    
    // Score counter checks comparator results if posedgeKeyValid is 1, then increments the score by 1, else keeps the previous score
    always_ff @(posedge clk)
        begin
            if (reset) // If game is reset, reset the score
                score <= 4'b0000;  
            else if (posedgeKeyValid) // If an input is taken, evaluate the score
                begin
                    if (isCorrect == 1)
                        begin
                            if (score == 4'b1001) // If score goes over 9, reset to 0
                                score <= 4'b0000;
                            else
                                score <= score + 1;
                        end
                    else
                        begin
                            if (score == 4'b0000) // If score is 0, keep it 0
                                score <= 4'b0000;
                            else
                                score <= score - 1;
                        end
                end
            else // Else keep the score
                score <= score;
        end
    
    // Score registers' memory is fed into given sevSeg module
    SevSeg_4digit sevSegController(
        clk, score, 4'b0, 4'b0, 4'b0, 
        digit[6], digit[5], digit[4], digit[3], digit[2], digit[1], digit[0],
        dp, an);
    
endmodule
