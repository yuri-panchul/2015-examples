module cmod_s6
(
    // input        CLK,         // FPGA_GCLK, 8MHz
    input        CLK_LFC,     // FPGA_LFC, 1 Hz

    output reg   LED_0,
    output reg   LED_1,
    output reg   LED_2,
    output reg   LED_3,

    input        BTN_0,
    input        BTN_1   // ,

    // DEPP interface

    // input        DEPP_ASTB,   // Address strobe
    // input        DEPP_DSTB,   // Data strobe
    // input        DEPP_WRITE,  // Write enable (write operation = 0, read operation = 1)
    // output       DEPP_WAIT,   // Ready 
    // inout  [7:0] DBUS,

    // General purpose I/O

    // output [7:0] PORTA,
    // output [7:0] PORTB,
    // output [6:0] PORTC,
    // output [7:0] PORTD,
    // output [7:0] PORTE,
    // output [6:0] PORTF
);
/*
    assign LED_0 = BTN_0;
    assign LED_1 = BTN_1;
    assign LED_2 = BTN_0 & BTN_1;
    assign LED_3 = BTN_0 | BTN_1;
*/
    reg [3:0] n;

    always @ (posedge CLK_LFC)
    begin
        if (BTN_1)
            n <= 0;
        else
            n <= n + 1;
    end

    always @*
    begin
        LED_0 = n [0];
        LED_1 = 1; // n [1];
        LED_2 = 0; // n [2];
        LED_3 = CLK_LFC; // n [3];
    end

endmodule
