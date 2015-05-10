module clock_divider_50_MHz_to_1_49_Hz
(
    input  clock_50_MHz,
    input  resetn,
    output clock_1_49_Hz
);

    // 50 MHz / 2 ** 25 = 1.49 Hz

    reg [24:0] counter;

    always @ (posedge clock_50_MHz)
    begin
        if (! resetn)
            counter <= 0;
        else
            counter <= counter + 1;
    end

    assign clock_1_49_Hz = counter [24];

endmodule

//--------------------------------------------------------------------

module shift_register_with_enable
(
    input             clock,
    input             resetn,
    input             in,
    input             enable,
    output            out,
    output reg [17:0] data
);

    always @ (posedge clock or negedge resetn)
    begin
        if (! resetn)
            data <= 18'b10_0000_0000_0000_0000;
        else if (enable)
            data <= { in, data [17:1] };
    end
    
    assign out = data [0];

endmodule

//--------------------------------------------------------------------

module single_digit_display
(
    input      [3:0] digit,
    output reg [6:0] seven_segments
);

    always @*
        case (digit)
        'h0: seven_segments = 'b1000000;  // a b c d e f g
        'h1: seven_segments = 'b1111001;
        'h2: seven_segments = 'b0100100;  //   --a--
        'h3: seven_segments = 'b0110000;  //  |     |
        'h4: seven_segments = 'b0011001;  //  f     b
        'h5: seven_segments = 'b0010010;  //  |     |
        'h6: seven_segments = 'b0000010;  //   --g--
        'h7: seven_segments = 'b1111000;  //  |     |
        'h8: seven_segments = 'b0000000;  //  e     c
        'h9: seven_segments = 'b0011000;  //  |     |
        'ha: seven_segments = 'b0001000;  //   --d-- 
        'hb: seven_segments = 'b0000011;
        'hc: seven_segments = 'b1000110;
        'hd: seven_segments = 'b0100001;
        'he: seven_segments = 'b0000110;
        'hf: seven_segments = 'b0001110;
        endcase

endmodule

//--------------------------------------------------------------------

// Smiling Snail FSM derived from David Harris & Sarah Harris

module pattern_fsm_moore
(
    input  clock,
    input  resetn,
    input  a,
    output y
);

    parameter [1:0] S0 = 0, S1 = 1, S2 = 2;

    reg [1:0] state, next_state;

    // state register

    always @ (posedge clock or negedge resetn)
        if (! resetn)
            state <= S0;
        else
            state <= next_state;

    // next state logic

    always @*
        case (state)

        S0:
            if (a)
                next_state <= S0;
            else
                next_state <= S1;

        S1:
            if (a)
                next_state <= S2;
            else
                next_state <= S1;

        S2:
            if (a)
                next_state <= S0;
            else
                next_state <= S1;

        default:

            next_state <= S0;

        endcase

    // output logic

    assign y = (state == S2);

endmodule


//--------------------------------------------------------------------

module de2_115_user
(
    input         CLOCK_50,
    input  [ 3:0] KEY,
    input  [17:0] SW,
    output [ 8:0] LEDG,
    output [17:0] LEDR,
    output [ 6:0] HEX0,
    output [ 6:0] HEX1,
    output [ 6:0] HEX2,
    output [ 6:0] HEX3,
    output [ 6:0] HEX4,
    output [ 6:0] HEX5,
    output [ 6:0] HEX6,
    output [ 6:0] HEX7
);

    wire clock;
    wire resetn = KEY [3];

    clock_divider_50_MHz_to_1_49_Hz clock_divider_50_MHz_to_1_49_Hz
    (
        .clock_50_MHz  (CLOCK_50),
        .resetn        (resetn),
        .clock_1_49_Hz (clock)
    );

    shift_register_with_enable shift_register_with_enable
    (
        .clock      (   clock    ),
        .resetn     (   resetn   ),
        .in         ( ~ KEY  [2] ),
        .enable     (   KEY  [1] ),
        .out        (   LEDG [7] ),
        .data       (   LEDR     )
    );

    single_digit_display digit_0
    (
        .digit          ( LEDR [ 3: 0] ),
        .seven_segments ( HEX0         )
    );

    single_digit_display digit_1
    (
        .digit          ( LEDR [ 7: 4] ),
        .seven_segments ( HEX1         )
    );

    single_digit_display digit_2
    (
        .digit          ( LEDR [11: 8] ),
        .seven_segments ( HEX2         )
    );

    single_digit_display digit_3
    (
        .digit          ( LEDR [15:12] ),
        .seven_segments ( HEX3         )
    );

    single_digit_display digit_4
    (
        .digit          ( { 2'b0 , LEDR [17:16] } ),
        .seven_segments ( HEX4                    )
    );

    pattern_fsm_moore pattern_fsm_moore
    (
        .clock  ( clock    ),
        .resetn ( resetn   ),
        .a      ( LEDG [7] ),
        .y      ( LEDG [6] )
    );

    assign LEDG [5:0] = 6'b11_1111;
    assign HEX5       = 7'h7f;
    assign HEX6       = 7'h7f;
    assign HEX7       = 7'h7f;

endmodule
