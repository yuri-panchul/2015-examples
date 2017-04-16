module isqrt_pipe_gen_for_for
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
    localparam n_comb_per_stage   = n_slices_per_stage - 1;

    localparam [31:0] m = 32'h4000_0000;

    wire [31:0] wx [0:n_slices], wy [0:n_slices];
    wire wr [0:n_slices];

    assign wr [0] = run;
    assign wx [0] = x;
    assign wy [0] = 0;

    assign ready = wr [n_slices];
    assign y     = wy [n_slices];

    generate
        genvar i, j;

        for (i = 0; i < n_slices; i = i + n_slices_per_stage)
        begin : u
            for (j = i; j < i + n_slices_per_stage - 1; j = j + 1)
            begin : v
                isqrt_slice_comb #(m >> j * 2) inst
                    (wx [j], wy [j], wx [j + 1], wy [j + 1]);
            end

            isqrt_slice_reg #(m >> (i + n_slices_per_stage - 1) * 2) inst
            (
                clock,
                reset_n,
                wr [ i + n_slices_per_stage - 1 ],
                wx [ i + n_slices_per_stage - 1 ],
                wy [ i + n_slices_per_stage - 1 ],
                wr [ i + n_slices_per_stage     ],
                wx [ i + n_slices_per_stage     ],
                wy [ i + n_slices_per_stage     ]
            );

            assign wr [i + n_slices_per_stage - 1] = wr [i];
        end
    endgenerate

endmodule
