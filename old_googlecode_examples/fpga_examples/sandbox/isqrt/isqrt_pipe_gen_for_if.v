module aaa2 // isqrt_pipe_gen_for_if
(
    input         clock,
    input         reset_n,
    input         run,
    input  [31:0] x,
    output        ready,
    output [15:0] y
);

    parameter n_pipe_stages = 16;

    localparam n_slices           = 16;
    localparam n_slices_per_stage = n_slices / n_pipe_stages;

    localparam [31:0] m = 32'h4000_0000;

    wire [31:0] wx [0:n_slices], wy [0:n_slices];
    wire wr [0:n_slices];

    assign wr [0] = run;
    assign wx [0] = x;
    assign wy [0] = 0;

    assign ready = wr [n_slices];
    assign y     = wy [n_slices];

    generate
        genvar i;

        for (i = 0; i < n_slices; i = i + n_slices_per_stage)
        begin : u
            if (i % n_slices_per_stage != n_slices_per_stage - 1)
            begin
                isqrt_slice_comb #(m >> i * 2) inst
                (
                    wx [i    ], wy [i    ],
                    wx [i + 1], wy [i + 1]
                );
            end
            else
            begin
                isqrt_slice_reg #(m >> i * 2) inst
                (
                    clock,
                    reset_n,
                    wr [i    ], wx [i    ], wy [i    ],
                    wr [i + 1], wx [i + 1], wy [i + 1]
                );

                assign wr [i] = wr [i - i % n_slices_per_stage];
            end
        end
    endgenerate

endmodule
