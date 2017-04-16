module basys2
(
    input        mclk,
    input  [7:0] sw,
    input  [3:0] btn,
    output [7:0] Led,
    output [6:0] seg,
    output       dp,
    output [3:0] an
);

    wire [9:0] inputs;
    wire [9:0] outputs;

    assign inputs = { 2'b0, sw };
    assign Led    = outputs [7:0];

    top
    (
        .clock   ( mclk      ),
        .reset_n ( btn [3]   ),
        .buttons ( btn [2:0] ),
        .inputs  ( inputs    ),
        .outputs ( outputs   )
    );

endmodule
