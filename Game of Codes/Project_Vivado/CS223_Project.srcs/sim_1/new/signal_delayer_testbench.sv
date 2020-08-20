`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 17.12.2017 12:58:27
// Design Name: 
// Module Name: signal_delayer_testbench
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


module signal_delayer_testbench( );

    // Internal logic
    logic clk, signal, signal_delayed;
    
    // Device under test
    signal_delayer_1bit dut(clk, signal, signal_delayed);
    
    // Test code
    initial
        begin
            clk = 0;
            signal = 0;
            
            
                        
            for (int i = 0; i < 64; i = i + 1)
                begin
                    if (i === 32)
                        signal = 1;
                    if (i === 35)
                        signal = 0;
                
                    #0.001;
                end
        end
        
    always
        begin
            clk = ~clk;
            #0.001;
        end
        
endmodule
