module starter3e
(
    input        CLK_50MHZ,
    input  [3:0] SW,
    input        BTN_EAST,
    input        BTN_NORTH,
    input        BTN_SOUTH,
    input        BTN_WEST,
    output [7:0] LED
);

    wire [9:0] outputs;
    assign LED = outputs [7:0];

    top top
    (
        .clock   ( CLK_50MHZ                         ),
        .reset_n ( BTN_NORTH                         ),
        .buttons ( { BTN_WEST, BTN_SOUTH, BTN_EAST } ),
        .inputs  ( { 2'b0, SW, SW }                  ),
        .outputs ( outputs                           )
    );

endmodule
