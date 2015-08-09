module edge_sampler
(
    input  clock,
    input  reset_n,
    input  rx,
    output reg [11:0] counter1,
    output reg [11:0] counter2
);

    parameter width = 256;

    reg [width - 1:0] shift_reg;

    always @ (posedge clock or negedge reset_n)
        if (! reset_n)
            shift_reg <= { width { 1'b1 } };
        else
            shift_reg <= (shift_reg << 1) | rx;

    always @ (posedge clock or negedge reset_n)
        if (! reset_n)
        begin
            counter1 <= 0;
            counter2 <= 0;
        end
        else
        begin
            if (shift_reg [1] == 0 && shift_reg [0] == 1)
                counter1 <= counter1 + 1'b1;

            if (   shift_reg [width - 1]   == 0
                && shift_reg [width - 2:0] == { (width - 1) { 1'b1 } } )
            begin
                counter2 <= counter2 + 1'b1;
            end
        end

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

module de0_cv_user
(
    input         CLOCK_50,
    input         RESET_N,
    input  [ 3:0] KEY,
    input  [ 9:0] SW,
    output [ 9:0] LEDR,
    output [ 6:0] HEX0,
    output [ 6:0] HEX1,
    output [ 6:0] HEX2,
    output [ 6:0] HEX3,
    output [ 6:0] HEX4,
    output [ 6:0] HEX5,
    inout  [35:0] GPIO_0,
    inout  [35:0] GPIO_1
);

    wire uart_rx = GPIO_1 [35];

    assign LEDR = { 10 { uart_rx } };
    
    wire [11:0] counter1, counter2;

    edge_sampler
    (
        .clock    ( CLOCK_50 ),
        .reset_n  ( RESET_N  ),
        .rx       ( uart_rx  ),
        .counter1 ( counter1 ),
        .counter2 ( counter2 )
    );                     

    single_digit_display digit_0 (counter1 [ 3:0], HEX0);
    single_digit_display digit_1 (counter1 [ 7:4], HEX1);
    single_digit_display digit_2 (counter1 [11:8], HEX2);
    single_digit_display digit_3 (counter2 [ 3:0], HEX3);
    single_digit_display digit_4 (counter2 [ 7:4], HEX4);
    single_digit_display digit_5 (counter2 [11:8], HEX5);

endmodule
