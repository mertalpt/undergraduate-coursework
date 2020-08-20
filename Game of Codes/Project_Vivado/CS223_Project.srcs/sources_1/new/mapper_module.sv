`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.12.2017 12:14:25
// Design Name: 
// Module Name: mapperModule
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


module key_mapper(
        input logic [3:0] keyValue,
        output logic [3:0] mappedValue
    );
    
    /*
     * Mapping of the movements:
     * SL: 11
     * SR: 00
     * LL: 10
     * LR: 01
     */
     
    always_comb
        begin
            case (keyValue)                
                /* D */ 4'b0000: mappedValue = 4'b0101; // SR-SR
                /* # */ 4'b0001: mappedValue = 4'b0111; // SR-LR
                /* 0 */ 4'b0010: mappedValue = 4'b0110; // SR-LL
                /* * */ 4'b0011: mappedValue = 4'b0100; // SR-SL
                /* C */ 4'b0100: mappedValue = 4'b1101; // LR-SR
                /* 9 */ 4'b0101: mappedValue = 4'b1111; // LR-LR
                /* 8 */ 4'b0110: mappedValue = 4'b1110; // LR-LL
                /* 7 */ 4'b0111: mappedValue = 4'b1100; // LR-SL
                /* B */ 4'b1000: mappedValue = 4'b1001; // LL-SR
                /* 6 */ 4'b1001: mappedValue = 4'b1011; // LL-LR
                /* 5 */ 4'b1010: mappedValue = 4'b1010; // LL-LL
                /* 4 */ 4'b1011: mappedValue = 4'b1000; // LL-SL
                /* A */ 4'b1100: mappedValue = 4'b0001; // SL-SR
                /* 3 */ 4'b1101: mappedValue = 4'b0011; // SL-LR
                /* 2 */ 4'b1110: mappedValue = 4'b0010; // SL-LL
                /* 1 */ 4'b1111: mappedValue = 4'b0000; // SL-SL
                default: mappedValue = 4'b0000; // No value for default
            endcase
        end
    
endmodule
