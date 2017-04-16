module de0_nano
(
    input         clock,
    input  [7: 0] port_e,
    input  [7: 5] port_d_in,
    output [3: 0] port_d_out,
    output [1:12] display,
    output [7: 0] leds
);

    wire reset_n = port_d_in [5];

    wire multiplied_clock;
    wire locked;

    pll50_200 pll50_200_inst
    (
        .areset ( ~ reset_n          ),
        .inclk0 (   clock            ),
        .c0     (   multiplied_clock ),
        .locked (   locked           )
    );

    top top_inst
    (
        .clock      ( multiplied_clock ),
        .port_e     ( port_e           ),
        .port_d_in  ( port_d_in        ),
        .port_d_out ( port_d_out       ),
        .display    ( display          ),
        .leds       ( leds             )
    );

endmodule
