module clock_divider_100_MHz_to_1_49_Hz
(
    input  clock_100_MHz,
    input  resetn,
    output clock_1_49_Hz
);

    // 100 MHz / 2 ** 26 = 1.49 Hz

    reg [25:0] counter;

    always @ (posedge clock_100_MHz)
    begin
        if (! resetn)
            counter <= 0;
        else
            counter <= counter + 1;
    end

    assign clock_1_49_Hz = counter [25];

endmodule


//--------------------------------------------------------------------

module shift_register
(
    input             clock,
    input             resetn,
    input             in,
    output            out,
    output reg [15:0] data
);

    always @ (posedge clock or negedge resetn)
    begin
        if (! resetn)
            data <= 16'hABCD;
        else
            data <= { in, data [15:1] };
            // data <= (data >> 1) | (in << 15);
    end
    
    assign out = data [0];

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
                next_state = S0;
            else
                next_state = S1;

        S1:
            if (a)
                next_state = S2;
            else
                next_state = S1;

        S2:
            if (a)
                next_state = S0;
            else
                next_state = S1;

        default:

            next_state = S0;

        endcase

    // output logic

    assign y = (state == S2);

endmodule

//--------------------------------------------------------------------

module pattern_fsm_mealy
(
    input  clock,
    input  resetn,
    input  a,
    output y
);

    parameter S0 = 0, S1 = 1;

    reg state, next_state;

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
                next_state = S0;
            else
                next_state = S1;

        S1:
            if (a)
                next_state = S0;
            else
                next_state = S1;

        default:

            next_state = S0;

        endcase

    // output logic

    assign y = (a & state == S1);

endmodule

//--------------------------------------------------------------------

module basys3
(
    input         clk,

    input         btnC,
    input         btnU,
    input         btnL,
    input         btnR,
    input         btnD,

    input  [15:0] sw,

    output [15:0] led,

    output [ 6:0] seg,
    output        dp,
    output [ 3:0] an
);

    wire clock;
    wire resetn = ! btnU;

    clock_divider_100_MHz_to_1_49_Hz clock_divider
    (
        .clock_100_MHz (clk),
        .resetn        (resetn),
        .clock_1_49_Hz (clock)
    );

    wire [15:0] shift_data_out;

    shift_register shift_register
    (
        .clock      ( clock          ),
        .resetn     ( resetn         ),
        .in         ( btnC           ),
        .out        (                ),
        .data       ( shift_data_out )
    );

    wire fsm_in = shift_data_out [8];
    assign led = shift_data_out;

    wire moore_out;

    pattern_fsm_moore pattern_fsm_moore
    (
        .clock  ( clock     ),
        .resetn ( resetn    ),
        .a      ( fsm_in    ),
        .y      ( moore_out )
    );

    wire mealy_out;

    pattern_fsm_mealy pattern_fsm_mealy
    (
        .clock  ( clock     ),
        .resetn ( resetn    ),
        .a      ( fsm_in    ),
        .y      ( mealy_out )
    );

    assign seg [0] = ~  moore_out;
    assign seg [1] = ~  moore_out;
    assign seg [2] = ~  mealy_out;
    assign seg [3] = ~  mealy_out;
    assign seg [4] = ~  mealy_out;
    assign seg [5] = ~  moore_out;
    assign seg [6] = ~ (moore_out | mealy_out);

    assign dp  = 1'b1;
    assign an  = 4'b1110;

endmodule
