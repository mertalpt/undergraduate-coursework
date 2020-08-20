`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 25.11.2017 13:19:50
// Design Name: 
// Module Name: game
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


module game(
        input logic clk, reset, start,
        input logic [3:0] keyb_col,
        output logic [6:0] sevSegOut,
        output logic [3:0] keyb_row, an, motorOut, motorInternal,
        output logic dp, motorIdle, delayInProcess
    );
    
    // Internal wires
    logic initiateNext; // Start next turn
    logic initiateNextDelayed; // Delayed initiateNext signal
    //logic delayInProcess; // Turns to 1 when signal delayer is delaying a signal
    logic resetMain; // Resets the game when user input reset is 1 or at the posedge of start input
    //logic motorIdle; // Turns to 1 when motor has been idle for about 1 seconds
    logic posedgeStart; // Posedge of start
    logic negedgeStart; // Negedge of start
    logic posedgeKeyValid; // Posedge of keyValid
    logic negedgeKeyValid; // Negedge of keyValid
    logic keyValid; // Keypad input for key press
    logic keyValidControlled; // keyValid is set to 0 when motor is not idle
    logic [3:0] keyValues; // Keypad input for key value
    logic [3:0] mappedValues; // Mapped values of key values
    
    logic [3:0] rngInternal; // Rearranged output of RNG module    
    logic [1:0] motorSpeed, motorDirection; // Output of RNG module
    //logic [3:0] motorInternal; // Input of motor wrapper, comes from memory module
    
    // Connecting idle detector
    idle_detector_4bit motorIdleModule(clk, motorOut, motorIdle); // If motorOut didn't change for about 1 seconds, then motor is idle
    
    // Setting idle keypad logic
    assign keyValidControlled = keyValid & motorIdle & ~delayInProcess;
    
    // Logic for initiating next turn
    assign initiateNext = negedgeStart | negedgeKeyValid;
    
    // Connect delayer for initiateNext
    signal_delayer_1bit initiateNextDelayer(clk, initiateNext, delayInProcess, initiateNextDelayed);
    
    // Logic for resetting
    assign resetMain = posedgeStart | reset;
    
    // Connecting sedge detectors
    posedge_detector posedgeOfStart(clk, start, posedgeStart);
    negedge_detector negedgeOfStart(clk, start, negedgeStart);
    posedge_detector posedgeOfKeyValid(clk, keyValidControlled, posedgeKeyValid);
    negedge_detector negedgeOfKeyValid(clk, keyValidControlled, negedgeKeyValid);
    
    // Connecting Keypad module
    keypad4X4 keypad(clk, keyb_row, keyb_col, keyValues, keyValid);
    
    // Connecting Mapper module
    key_mapper mapper(keyValues, mappedValues);
    
    // Connecting Random Number Generator
    rng_4bit rndNumGenerator(clk, resetMain, motorSpeed, motorDirection); // Tested and working
    
    // Rearranging RNG output in a new bus
    assign rngInternal[3] = motorSpeed[0];
    assign rngInternal[2] = motorDirection[0];
    assign rngInternal[1] = motorSpeed[1];
    assign rngInternal[0] = motorDirection[1];
    
    // Register for the movement made
    movement_memory motorMemory(clk, resetMain, initiateNextDelayed, rngInternal, motorInternal);
    
    // Connecting Stepper Motor Wrapper
    /*
     *                 First Movement - Second Movement
     * motorSpeed[0]motorDirection[0] - motorSpeed[1]motorDirection[1]
     * Speed: 0-Short, 1-Long
     * Direction: 0-Left, 1-Right
     */
    steppermotor_wrapper motorControl(
        clk, 
        {motorInternal[0], motorInternal[2]}, 
        {motorInternal[1], motorInternal[3]}, 
        motorOut, initiateNextDelayed);
    
    // Connecting Seven Segment Display module
    seven_segment_logic sevSegDisplay(clk, resetMain, posedgeKeyValid, mappedValues, motorInternal, sevSegOut, an, dp);

endmodule
