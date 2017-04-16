module top
(
    input             clock,
    output reg [7: 0] port_e,
    input      [3: 0] port_f,
    output reg [3: 0] port_d,
    output reg [1:12] display,
    output reg [7: 0] leds
);

wire reset_n = port_f [0];

always @(posedge clock or negedge reset_n)
begin
    if (! reset_n)
        port_e <= 0;
    else
        port_e <= port_e + 1;
end
	 
endmodule
