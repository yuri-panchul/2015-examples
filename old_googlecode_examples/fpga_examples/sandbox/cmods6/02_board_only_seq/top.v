module fdce
(
    input      clk,
    input      rst,
    input      ce,
    input      d,
    output reg q
);

    always @ (posedge clk or posedge rst)
        if (rst)
            q <= 0;
        else if (ce)
            q <= d;

endmodule

module fdre
(
    input      clk,
    input      rst,
    input      ce,
    input      d,
    output reg q
);

    always @ (posedge clk)
        if (rst)
            q <= 0;
        else if (ce)
            q <= d;

endmodule

module caseab
(
    input            clk,
    input            rst,
    input            d,
	 input  [2:0]     sel,
	 output       reg a,
	 output       reg b
);

    always @ (posedge clk)
        if (rst)
		  begin
            a <= 0;
            b <= 0;
		  end
		  else
		  begin
		     // 
           case (sel)
	        1: a <= b;
			  2: b <= 0;
			  3: b <= 1;
			  4: a <= 0;
			  5: a <= b;
			  default: ;
			  endcase
		  end
			  
endmodule

module top
(
    input  clk,
    input  rst,
    input  ce,
    input  d,
    output q1,
	 output q2,
	 
	 input  [2:0] sel,
	 output       a,
	 output       b
);

    fdce fdce (clk, rst, ce, d, q1);
	 
    fdre fdre (clk, rst, ce, d, q2);

    caseab caseab (clk, rst, d, sel, a, b);

endmodule



/*


module top_3
(
    input  clk,

    input  btn_0,
    input  btn_1,

    output led_0,
    output led_1,
    output led_2,
    output led_3
);

    wire reset = btn_0;

    reg [3:0] r;

    always @ (posedge clk or posedge reset)
        if (reset)
            r <= 0;
        else
            r <= r + 1;
	 
    assign led_0 = r [0];
    assign led_1 = r [1];
    assign led_2 = r [2];
    assign led_3 = r [3];

endmodule















module top_2
(
    input  clk,

    input  btn_0,
    input  btn_1,

    output led_0,
    output led_1,
    output led_2,
    output led_3
);

    wire reset = btn_0;

    reg [3:0] r;

    always @ (posedge clk)
        if (reset)
            r <= 4'b0001;
        else
            r <= { r [0], r [3:1] };
	 
    assign led_0 = r [0];
    assign led_1 = r [1];
    assign led_2 = r [2];
    assign led_3 = r [3];

endmodule















module top_1
(
    input      clk,

    input      btn_0,
    input      btn_1,

    output     led_0,
    output     led_1,
    output reg led_2,
    output reg led_3
);

    assign led_0 = clk;

    reg reg_1;

    always @ (posedge clk)
        reg_1 <= btn_0;

    assign led_1 = reg_1;

    always @ (posedge clk)
        led_2 <= btn_0;

    always @ (posedge clk)
        if (btn_1)
            led_3 <= btn_0;

endmodule

*/
