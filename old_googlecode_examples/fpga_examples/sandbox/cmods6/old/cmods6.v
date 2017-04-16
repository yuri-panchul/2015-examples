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

    // input        DEPP_ASTB,   // Address strobe
    // input        DEPP_DSTB,   // Data strobe
    // input        DEPP_WRITE,  // Write enable (write operation = 0, read operation = 1)
    // output       DEPP_WAIT,   // Ready 
    // inout  [7:0] DBUS,

    // General purpose I/O

    output [7:0] PORTA,
    output [7:0] PORTB,
    output [6:0] PORTC,
    output [7:0] PORTD,
    output [7:0] PORTE,
    output [6:0] PORTF
);

    assign LED_0 =   CLK_LFC;
    assign LED_1 = ~ CLK_LFC | BTN_0;
    assign LED_2 = ~ CLK_LFC;
    assign LED_3 =   CLK_LFC | BTN_1;

    assign PORTB = 'hff;
    assign PORTC = 'h7f;
    assign PORTD = 'hff;
    assign PORTE = 'hff;

    wire       clock              = CLK;
    wire       reset              = BTN_0;
    wire [1:0] clock_divider_mode = { BTN_1, BTN_1 };

    wire       clock_for_debouncing;
    wire       clock_for_display;

    clock_divider clock_divider
    (
        .clock                 ( clock                ),
        .reset                 ( reset                ),
        .clock_for_debouncing  ( clock_for_debouncing ),
        .clock_for_display     ( clock_for_display    )
    );

    reg        [15:0] number;
    wire              overflow = 0;
    wire       [ 3:0] error    = 0;

    wire       [ 6:0] seven_segments;
    wire              dot;
    wire       [ 3:0] anodes;

    display display
    (
        .clock           ( clock_for_display ),
        .reset           ( reset             ),
        .number          ( number            ),
        .overflow        ( overflow          ),
        .error           ( error             ),
        .seven_segments  ( seven_segments    ),
        .dot             ( dot               ),
        .anodes          ( anodes            )
    );

    assign PORTA [0] = ~ seven_segments [4]; // E  1
    assign PORTA [1] = ~ seven_segments [3]; // D  2
    assign PORTA [2] = ~ dot;                // .  3
    assign PORTA [3] = ~ seven_segments [2]; // C  4
    assign PORTA [4] = ~ seven_segments [6]; // G  5
    assign PORTA [5] = ~ seven_segments [1]; // B  7
    assign PORTA [6] = ~ seven_segments [5]; // F 10
    assign PORTA [7] = ~ seven_segments [0]; // A 11

    assign PORTF = anodes;

    always @(posedge CLK_LFC)
    begin
        if (reset)
            number = 0;
        else
            number = number + 1;
    end

endmodule
