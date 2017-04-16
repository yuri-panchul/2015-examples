module top
(
    input             clock,
    input      [7: 0] port_e,
    input      [3: 0] port_f,
    output reg [3: 0] port_d,
    output reg [1:12] display,
    output reg [7: 0] leds
);

wire reset_n = port_f [0];

reg [1:0] a1, a2, a3, a4;
reg [2:0] b1, b2;
reg [3:0] c;

always @*
begin
    a1 = port_e [7] + port_e [6];
    a2 = port_e [5] + port_e [4];
    a3 = port_e [3] + port_e [2];
    a4 = port_e [1] + port_e [0];

    b1 = a1 + a2;
    b2 = a3 + a4;

    c = b1 + b2;
end

always @(posedge clock or negedge reset_n)
begin
    if (! reset_n)
    begin
        port_d  <= 0;
        leds    <= 0;
    end
    else
    begin
        port_d <= c;
        leds   <= c;
    end
end
	 
endmodule
