module display_driver
(
    input              clock,
    input              reset_n,
    input      [15: 0] number,
    input      [ 3: 0] digit_enables,
    output reg [ 1:12] out
);

    function [6:0] bcd_to_seg (input [3:0] bcd);

        case (bcd)
        'h0: bcd_to_seg = 'b1111110;  // a b c d e f g
        'h1: bcd_to_seg = 'b0110000;
        'h2: bcd_to_seg = 'b1101101;  //   --a--
        'h3: bcd_to_seg = 'b1111001;  //  |     |
        'h4: bcd_to_seg = 'b0110011;  //  f     b
        'h5: bcd_to_seg = 'b1011011;  //  |     |
        'h6: bcd_to_seg = 'b1011111;  //   --g--
        'h7: bcd_to_seg = 'b1110000;  //  |     |
        'h8: bcd_to_seg = 'b1111111;  //  e     c
        'h9: bcd_to_seg = 'b1111011;  //  |     |
        'ha: bcd_to_seg = 'b1110111;  //   --d-- 
        'hb: bcd_to_seg = 'b0011111;
        'hc: bcd_to_seg = 'b1001110;
        'hd: bcd_to_seg = 'b0111101;
        'he: bcd_to_seg = 'b1001111;
        'hf: bcd_to_seg = 'b1000111;
        endcase

    endfunction

    reg [15:0] counter;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            counter <= 0;
        else
            counter <= counter + 1;
    end

    reg [3:0] digit_select;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            digit_select <= 1;
        else if (counter == 0)
            digit_select <= { digit_select [0], digit_select [3:1] };
    end

    reg [1:12] next_out;
    reg [ 3:0] digit;
    
    reg a, b, c, d, e, f, g, dp;

    always @(*)
    begin
        next_out = out;

        if (digit_select [0])
            digit = number [3:0];
        else if (digit_select [1])
            digit = number [7:4];
        else if (digit_select [2])
            digit = number [11:8];
        else
            digit = number [15:12];

        { a, b, c, d, e, f, g } = bcd_to_seg (digit);
        dp = 0;

        next_out [ 1] = e;
        next_out [ 2] = d;
        next_out [ 3] = dp;
        next_out [ 4] = c;
        next_out [ 5] = g;
        next_out [ 6] = ~ (digit_select [0] & digit_enables [0]);
        next_out [ 7] = c; // b;
        next_out [ 8] = ~ (digit_select [1] & digit_enables [1]);
        next_out [ 9] = ~ (digit_select [2] & digit_enables [2]);
        next_out [10] = f;
        next_out [11] = a;
        next_out [12] = ~ (digit_select [3] & digit_enables [3]);
    end

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
            out <= 0;
        else
            out <= next_out;
    end

endmodule
