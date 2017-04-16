module de0_nano
(
    input         clock,
    output [7: 0] port_e,
    input  [3: 0] port_f,
    output [3: 0] port_d,
    output [1:12] display,
    output [7: 0] leds
);


    top top (clock, port_e, port_f, port_d, display, leds);

endmodule
