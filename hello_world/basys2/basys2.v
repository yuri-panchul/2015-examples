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

    assign Led [0] = btn [0];
    assign Led [1] = btn [1];
    assign Led [2] = btn [0] & btn [1];
    assign Led [3] = btn [0] | btn [1];
    assign Led [4] = btn [0] ^ btn [1];
    assign Led [5] = 0;
    assign Led [6] = 0;
    assign Led [7] = 0;

    assign seg = 0;
    assign dp  = 0;
    assign an  = 0;

endmodule
