module isqrt_pipe_16_gen
(
    input         clock,
    input         reset_n,
    input         run,
    input  [31:0] x,
    output        ready,
    output [15:0] y
);

    localparam [31:0] m = 32'h4000_0000;

    wire [31:0] wx [0:16], wy [0:16];
    wire wr [0:16];

    assign wr [0] = run;
    assign wx [0] = x;
    assign wy [0] = 0;

    assign ready = wr [16];
    assign y     = wy [16];

    generate
        genvar i;

        for (i = 0; i < 16; i = i + 1)
        begin : u
            isqrt_slice_reg #(m >>  i * 2) inst
            (
                clock, reset_n,
                wr [i    ], wx [i    ], wy [i    ],
                wr [i + 1], wx [i + 1], wy [i + 1]
            );
        end
    endgenerate

endmodule
