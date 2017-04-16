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

    wire [7: 0] buf_port_e;
    wire [7: 5] buf_port_d_in;
    wire [3: 0] buf_port_d_out;

    // ibuf8 ibuf8_inst ( port_e         , buf_port_e     );
    // ibuf4 ibuf4_inst ( port_d_in      , buf_port_d_in  );
    // obuf4 obuf4_inst ( buf_port_d_out , port_d_out     );

    // Alternatively, we can simply rely
    // on default insertion of I/O cells
    //
    assign buf_port_e    = port_e;
    assign buf_port_d_in = port_d_in;
    assign port_d_out    = buf_port_d_out;

    top top_inst
    (
        .clock      ( multiplied_clock ),
        .port_e     ( buf_port_e       ),
        .port_d_in  ( buf_port_d_in    ),
        .port_d_out ( buf_port_d_out   ),
        .display    ( display          ),
        .leds       ( leds             )
    );

endmodule
