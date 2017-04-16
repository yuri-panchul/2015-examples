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

wire [3:0] digit = 4'b1111;

wire a, b, c, d, e, f, g, dp;

assign { a, b, c, d, e, f, g, dp }
    = port_e;

always @(posedge clock or negedge reset_n)
begin
    if (! reset_n)
    begin
        port_d   <= 0;
        display  <= 0;
        leds     <= 0;
    end
    else
    begin
        port_d <= port_f;
        
        { display [6], display [8], display [9], display [12] }
            <= ~ digit;
        
        { display [11], display [ 7], display [4], display [2],
          display [ 1], display [10], display [5], display [3] }
            <= { a, b, c, d, e, f, g, dp };
        
        leds <= { a, b, c, d, e, f, g, dp };
    end
end
	 
endmodule
