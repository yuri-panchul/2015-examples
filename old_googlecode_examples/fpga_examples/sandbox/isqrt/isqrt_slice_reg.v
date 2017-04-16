module isqrt_slice_reg
(
    input             clock,
    input             reset_n,
    input             run,
    input      [31:0] ix,
    input      [31:0] iy,
    output reg        ready,
    output reg [31:0] ox,
    output reg [31:0] oy
);

    parameter [31:0] m = 32'h4000_0000;

    wire [31:0] cox, coy;

    isqrt_slice_comb #(m) i (ix, iy, cox, coy);

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
        begin
            ready <= 0;
            ox    <= 0;
            oy    <= 0;
        end
        else if (run)
        begin
            ready <= 1;
            ox    <= cox;
            oy    <= coy;
        end
        else
        begin
            ready <= 0;
        end
    end

endmodule
