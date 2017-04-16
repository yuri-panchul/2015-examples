module top
(
    input        clock,
    input        reset_n,
    input  [2:0] buttons,
    input  [9:0] inputs,
    output [9:0] outputs
);

    wire [31:0] x;
    wire [15:0] y;

    assign x = inputs;
    assign outputs = y;

    isqrt_pipe_4 isqrt
    (
        .clock    ( clock       ),
        .reset_n  ( reset_n     ),
        .run      ( buttons [2] ),
        .x        ( x           ),
        .ready    (             ),
        .y        ( y           )
    );

endmodule
