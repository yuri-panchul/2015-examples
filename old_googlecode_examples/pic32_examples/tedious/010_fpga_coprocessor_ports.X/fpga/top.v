module top
(
    input         clock,
    input  [7: 0] port_e,
    input  [3: 0] port_f,
    output [3: 0] port_d,
    output [1:12] display,
    output [7: 0] leds
);

assign port_d = { port_e [7], port_e [6], port_e [1], port_e [0] };
	 
endmodule
