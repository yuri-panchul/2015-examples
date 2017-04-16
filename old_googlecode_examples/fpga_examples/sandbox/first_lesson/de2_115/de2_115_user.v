module de2_115_user_1
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

    assign LEDG [3:0] = KEY;
    assign LEDG [4]   = KEY [0] & KEY [1];
    assign LEDG [5]   = KEY [0] | KEY [1];
    assign LEDG [6]   = KEY [0] ^ KEY [1];
    assign LEDG [7]   = ~ KEY [0];
    assign LEDG [8]   = ~ KEY [1];

    assign LEDR = SW;

    assign HEX0 = 7'h0;
    assign HEX1 = 7'h7f;
    assign HEX2 = 7'h0;
    assign HEX3 = 7'h7f;
    assign HEX4 = 7'h0;
    assign HEX5 = 7'h7f;
    assign HEX6 = 7'h0;
    assign HEX7 = 7'h7f;

endmodule

//--------------------------------------------------------------------

module clock_divider
(
    input  clock_50_MHz,
    input  resetn,
    output clock_1_49_Hz
);

    // 50 MHz / 2 ** 25 = 1.49 Hz

    reg [24:0] counter;

    always @ (posedge clock_100_MHz)
    begin
        if (! resetn)
            counter <= 0;
        else
            counter <= counter + 1;
    end

    assign clock_1_49_Hz = counter [24];

endmodule


module counter
(
    input             clock,
    input             resetn,
    output reg [15:0] count
);

    always @ (posedge clock or negedge resetn)
    begin
        if (! resetn)
            count <= 0;
        else
            count <= count + 1;
    end

endmodule


module de2_115_user_1
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
    wire resetn = ! KEY [0];

    clock_divider clock_divider
    (
        .clock_50_MHz  (clk),
        .resetn        (resetn),
        .clock_1_49_Hz (clock)
    );

    counter counter
    (
        .clock  ( clock  ),
        .resetn ( resetn ),
        .count  ( led    )
    );

    assign seg = ~ 7'b0;
    assign dp  = 1'b0;
    assign an  = 4'b0;

endmodule

//--------------------------------------------------------------------

module counter_with_load
(
    input             clock,
    input             resetn,

    input             load,
    input      [15:0] load_data,
    output reg [15:0] count
);

    always @ (posedge clock or negedge resetn)
    begin
        if (! resetn)
            count <= 0;
        else if (load)
            count <= load_data;
        else
            count <= count + 1;
    end

endmodule

module basys3_3
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

    clock_divider clock_divider
    (
        .clock_100_MHz (clk),
        .resetn        (resetn),
        .clock_1_49_Hz (clock)
    );

    counter_with_load counter_with_load
    (
        .clock      ( clock  ),
        .resetn     ( resetn ),
        .load       ( btnC   ),
        .load_data  ( sw     ),
        .count      ( led    )
    );

    assign seg = ~ 7'b0;
    assign dp  = 1'b0;
    assign an  = 4'b0;

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
            // data <= (data >> 1) | in;
    end
    
    assign out = data [0];

endmodule

module basys3_4
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

    clock_divider clock_divider
    (
        .clock_100_MHz (clk),
        .resetn        (resetn),
        .clock_1_49_Hz (clock)
    );

    wire out;

    shift_register shift_register
    (
        .clock      ( clock  ),
        .resetn     ( resetn ),
        .in         ( btnC   ),
        .out        ( out    ),
        .data       ( led    )
    );

    assign seg = out ? 7'b1111001 : 7'b1000000;
    assign dp  = 1'b1;
    assign an  = 4'b1110;

endmodule

//--------------------------------------------------------------------

module mux_2_1
(
    input  [3:0] d0,
    input  [3:0] d1,
    input        sel,
    output [3:0] y
);

    assign y = sel ? d1 : d0;

endmodule

module mux_4_1_imp_1
(
    input  [3:0] d0, d1, d2, d3,
    input  [1:0] sel,
    output [3:0] y
);

    assign y = sel [1]
        ? (sel [0] ? d3 : d2)
        : (sel [0] ? d1 : d0);

endmodule

module mux_4_1_imp_2
(
    input      [3:0] d0, d1, d2, d3,
    input      [1:0] sel,
    output reg [3:0] y
);

    always @*
        case (sel)
        2'b00: y = d0;
        2'b01: y = d1;
        2'b10: y = d2;
        2'b11: y = d3;
        endcase

endmodule

module mux_4_1_imp_3
(
    input  [3:0] d0, d1, d2, d3,
    input  [1:0] sel,
    output [3:0] y
);

    wire [3:0] w01, w23;

    mux_2_1 i0 (.d0 ( d0  ), .d1 ( d1  ), .sel (sel [0]), .y ( w01 ));
    mux_2_1 i1 (.d0 ( d2  ), .d1 ( d3  ), .sel (sel [0]), .y ( w23 ));
    mux_2_1 i2 (.d0 ( w01 ), .d1 ( w23 ), .sel (sel [1]), .y ( y   ));

endmodule

module basys3_5
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

    mux_4_1_imp_1
    (
        .d0  ( sw [ 3: 0]     ),
        .d1  ( sw [ 7: 4]     ),
        .d2  ( sw [11: 8]     ),
        .d3  ( sw [15:12]     ),
        .sel ( { btnC, btnR } ),
        .y   ( led [3:0]      )
    ); 
        
    mux_4_1_imp_2
    (
        .d0  ( sw [ 3: 0]     ),
        .d1  ( sw [ 7: 4]     ),
        .d2  ( sw [11: 8]     ),
        .d3  ( sw [15:12]     ),
        .sel ( { btnC, btnR } ),
        .y   ( led [7:4]      )
    ); 
        
    mux_4_1_imp_3
    (
        .d0  ( sw [ 3: 0]     ),
        .d1  ( sw [ 7: 4]     ),
        .d2  ( sw [11: 8]     ),
        .d3  ( sw [15:12]     ),
        .sel ( { btnC, btnR } ),
        .y   ( led [11:8]     )
    ); 

    assign led [15:12] = 0;

    assign seg = ~ 7'b0;
    assign dp  = 1'b1;
    assign an  = 4'b0;

endmodule

//--------------------------------------------------------------------

module decoder_3_8_imp_1
(
    input      [2:0] a,
    output reg [7:0] y
);

    always @*
    case (a)
    3'b000: y = 8'b00000001;
    3'b001: y = 8'b00000010;
    3'b010: y = 8'b00000100;
    3'b011: y = 8'b00001000;
    3'b100: y = 8'b00010000;
    3'b101: y = 8'b00100000;
    3'b110: y = 8'b01000000;
    3'b111: y = 8'b10000000;
    endcase

endmodule

module decoder_3_8_imp_2
(
    input      [2:0] a,
    output reg [7:0] y
);

    always @*
    begin
        y = 0;
        y [a] = 1;
    end

endmodule

module decoder_3_8_imp_3
(
    input  [2:0] a,
    output [7:0] y
);

    assign y = 1 << a;

endmodule

module basys3_6
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

    decoder_3_8_imp_1 decoder_3_8_imp_1 (.a (sw [3:0]), .y (led [ 7:0]));
    decoder_3_8_imp_2 decoder_3_8_imp_3 (.a (sw [3:0]), .y (led [15:8]));

    assign seg = ~ 7'b0;
    assign dp  = 1'b1;
    assign an  = 4'b0;

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
                next_state <= S0;
            else
                next_state <= S1;

        S1:
            if (a)
                next_state <= S0;
            else
                next_state <= S1;

        default:

            next_state <= S0;

        endcase

    // output logic

    assign y = (a & state == S1);

endmodule

//--------------------------------------------------------------------

module basys3  // _7
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

    clock_divider clock_divider
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

*/
