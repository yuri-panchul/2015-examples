module isqrt_comb
(
    input             clock,
    input             reset_n,
    input             run,
    input      [31:0] x,
    output reg        ready,
    output reg [15:0] y
);

    localparam [31:0] m = 32'h4000_0000;

    wire [31:0] wx [0:16], wy [0:16];

    assign wx [0] = x;
    assign wy [0] = 0;

    isqrt_slice_comb #(m >>  0 * 2) i00 (wx [ 0], wy [ 0], wx [ 1], wy [ 1]);
    isqrt_slice_comb #(m >>  1 * 2) i01 (wx [ 1], wy [ 1], wx [ 2], wy [ 2]);
    isqrt_slice_comb #(m >>  2 * 2) i02 (wx [ 2], wy [ 2], wx [ 3], wy [ 3]);
    isqrt_slice_comb #(m >>  3 * 2) i03 (wx [ 3], wy [ 3], wx [ 4], wy [ 4]);
    isqrt_slice_comb #(m >>  4 * 2) i04 (wx [ 4], wy [ 4], wx [ 5], wy [ 5]);
    isqrt_slice_comb #(m >>  5 * 2) i05 (wx [ 5], wy [ 5], wx [ 6], wy [ 6]);
    isqrt_slice_comb #(m >>  6 * 2) i06 (wx [ 6], wy [ 6], wx [ 7], wy [ 7]);
    isqrt_slice_comb #(m >>  7 * 2) i07 (wx [ 7], wy [ 7], wx [ 8], wy [ 8]);
    isqrt_slice_comb #(m >>  8 * 2) i08 (wx [ 8], wy [ 8], wx [ 9], wy [ 9]);
    isqrt_slice_comb #(m >>  9 * 2) i09 (wx [ 9], wy [ 9], wx [10], wy [10]);
    isqrt_slice_comb #(m >> 10 * 2) i10 (wx [10], wy [10], wx [11], wy [11]);
    isqrt_slice_comb #(m >> 11 * 2) i11 (wx [11], wy [11], wx [12], wy [12]);
    isqrt_slice_comb #(m >> 12 * 2) i12 (wx [12], wy [12], wx [13], wy [13]);
    isqrt_slice_comb #(m >> 13 * 2) i13 (wx [13], wy [13], wx [14], wy [14]);
    isqrt_slice_comb #(m >> 14 * 2) i14 (wx [14], wy [14], wx [15], wy [15]);
    isqrt_slice_comb #(m >> 15 * 2) i15 (wx [15], wy [15], wx [16], wy [16]);

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
        begin
            ready <= 0;
            y     <= 0;
        end
        else if (run)
        begin
            ready <= 1;
            y     <= wy [16];
        end
        else
        begin
            ready <= 0;
        end
    end

endmodule
