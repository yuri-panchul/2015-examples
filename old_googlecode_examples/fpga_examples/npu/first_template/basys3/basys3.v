module basys3
(
    input         clk,

    input         btnC,
    input         btnU,
    input         btnL,
    input         btnR,
    input         btnD,

    input  [15:0] sw,

    output [15:0] led,

    output [ 6:0] seg,
    output        dp,
    output [ 3:0] an
);

    assign led [ 0] = sw [ 0];
    assign led [ 1] = sw [ 1];
    assign led [ 2] = sw [ 2];
    assign led [ 3] = sw [ 3];
    assign led [ 4] = sw [ 4];
    assign led [ 5] = sw [ 5];
    assign led [ 6] = sw [ 6];
    assign led [ 7] = sw [ 7];
    assign led [ 8] = sw [ 8];
    assign led [ 9] = sw [ 9];
    assign led [10] = sw [10];
    assign led [11] = sw [11];
    assign led [12] = sw [12];
    assign led [13] = sw [13];
    assign led [14] = sw [14];
    assign led [15] = sw [15];

    assign seg = 7'b0;
    assign dp  = 1'b0;
    assign an  = 4'hf;

endmodule
