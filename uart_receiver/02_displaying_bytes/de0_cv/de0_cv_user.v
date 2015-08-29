module uart_receiver
(
    input  clock,
    input  reset_n,
    input  rx,
    output reg [7:0] byte_data,
    output           byte_ready
);

    parameter  clock_frequency        = 50000000;
    parameter  baud_rate              = 115200;
    localparam clock_cycles_in_symbol = clock_frequency / baud_rate;

    // Finding edge for start bit

    reg prev_rx;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            prev_rx <= 1;
        else
            prev_rx <= rx;
    end

    wire start_bit_edge = prev_rx & ! rx;

    // Counter to measure distance between symbols

    reg [31:0] counter;
    reg        load_counter;
    reg [31:0] load_counter_value;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            counter <= 0;
        else if (load_counter)
            counter <= load_counter_value;
        else if (counter != 0)
            counter <= counter - 1;
    end

    wire counter_done = counter == 1;

    // Shift register to accumulate data

    reg       shift;
    reg [7:0] shifted_1;
    assign    byte_ready = shifted_1 [0];

    always @ (posedge clock or negedge reset_n)
    begin
        if (! reset_n)
        begin
            shifted_1 <= 0;
        end
        else if (shift)
        begin
            if (shifted_1 == 0)
                shifted_1 <= 8'b10000000;
            else
                shifted_1 <= shifted_1 >> 1;

            byte_data <= { rx, byte_data [7:1] };
        end
        else if (byte_ready)
        begin
            shifted_1 <= 0;
        end
    end

    reg idle, idle_r;

    always @*
    begin
        idle  = idle_r;
        shift = 0;

        load_counter        = 0;
        load_counter_value  = 0;

        if (idle)
        begin
            if (start_bit_edge)
            begin
                load_counter       = 1;
                load_counter_value = clock_cycles_in_symbol * 3 / 2;
           
                idle = 0;
            end
        end
        else if (counter_done)
        begin
            shift = 1;

            load_counter       = 1;
            load_counter_value = clock_cycles_in_symbol;
        end
        else if (byte_ready)
        begin
            idle = 1;
        end
    end

    always @ (posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            idle_r <= 1;
        else
            idle_r <= idle;
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

    wire uart_rx = GPIO_1 [31];

    assign LEDR = { 10 { uart_rx } };
    
    wire [7:0] new_byte_data;
    wire       new_byte_ready;

    uart_receiver uart_receiver
    (
        .clock      ( CLOCK_50       ),
        .reset_n    ( RESET_N        ),
        .rx         ( uart_rx        ),
        .byte_data  ( new_byte_data  ),
        .byte_ready ( new_byte_ready )
    );                     

    reg [7:0] byte_data;
    reg [7:0] byte_data_1;
    reg [7:0] byte_data_2;

    always @ (posedge CLOCK_50 or negedge RESET_N)
    begin
        if (! RESET_N)
        begin
            byte_data   <= 0;
            byte_data_1 <= 0;
            byte_data_2 <= 0;
        end
        else if (new_byte_ready)
        begin
            byte_data   <= new_byte_data;
            byte_data_1 <= byte_data;
            byte_data_2 <= byte_data_1;
        end
    end

    single_digit_display digit_0 (byte_data   [ 3:0], HEX0);
    single_digit_display digit_1 (byte_data   [ 7:4], HEX1);
    single_digit_display digit_2 (byte_data_1 [ 3:0], HEX2);
    single_digit_display digit_3 (byte_data_1 [ 7:4], HEX3);
    single_digit_display digit_4 (byte_data_2 [ 3:0], HEX4);
    single_digit_display digit_5 (byte_data_2 [ 7:4], HEX5);

endmodule
