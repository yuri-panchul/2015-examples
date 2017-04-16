module cmods6
(
    input        CLK,         // FPGA_GCLK, 8MHz
    input        CLK_LFC,     // FPGA_LFC, 1 Hz

    output       LED_0,
    output       LED_1,
    output       LED_2,
    output       LED_3,

    input        BTN_0,
    input        BTN_1,

    // DEPP interface

    input        DEPP_ASTB,   // Address strobe
    input        DEPP_DSTB,   // Data strobe
    input        DEPP_WRITE,  // Write enable (write operation = 0, read operation = 1)
    output       DEPP_WAIT,   // Ready 
    inout  [7:0] DBUS,

    // General purpose I/O

    input  [7:0] PORTA,
    input  [7:0] PORTB,
    output [6:0] PORTC,
    input  [7:0] PORTD,
    input  [7:0] PORTE,
    output [6:0] PORTF
);

    isqrt_pipe_4 isqrt
    (
        .clock    ( CLK_LFC ),
        .reset_n  ( BTN_0   ),
        .run      ( BTN_1   ),
        .x        ( { PORTA, PORTB, PORTD, PORTE } ),
        .ready    ( LED_3   ),
        .y        ( { PORTC, PORTF, LED_1, LED_0 } )
    );

endmodule
