module top
(
    input         clock,
    input  [9: 0] frequency_in_mhz,
    input  [7: 0] port_e,
    input  [7: 5] port_d_in,
    output [3: 0] port_d_out,
    output [1:12] display,
    output [7: 0] leds
);

    wire reset_n = port_d_in [5];

    reg [7:0] data0;
    reg       tag0;

    reg [7:0] data;
    reg       tag;

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
        begin
            data0 <= 0;
            tag0  <= 0;
    
            data  <= 0;
            tag   <= 0;
        end
        else
        begin
            data0 <= port_e;
            tag0  <= port_d_in [6];
    
            data  <= data0;
            tag   <= tag0;
        end
    end

    // Datapath

    reg [7:0] add_a, add_b;
    reg [7:0] mul_a, mul_b;

    wire [7:0] add_result = add_a + add_b;
    wire [7:0] mul_result = mul_a * mul_b;

    // Main state machine - sequential

    reg [7:0] r_add_result;
    reg [7:0] r_mul_result;

    reg [1:0] r_state;
    reg       r_first_tag;
    reg       r_prev_tag;
    reg [3:0] r_result;

    reg [2:0] state;
    reg       first_tag;
    reg       prev_tag;
    reg [3:0] result;

    assign port_d_out = r_result;

    always @(*)
    begin
        state      = r_state;
        first_tag  = r_first_tag;
        prev_tag   = r_prev_tag;
        result     = r_result;

        add_a      = 0;
        add_b      = 0;
        mul_a      = 0;
        mul_b      = 0;
    
        case (r_state)
    
        0:
            if (first_tag || tag != prev_tag)
            begin
                first_tag  = 0;
                prev_tag   = tag;
    
                mul_a      = data;
                mul_b      = data;
    
                state      = 1;
            end
    
        1:
            begin
                add_a      = r_mul_result;
                add_b      = 3;
    
                state      = 2;
            end
    
        2:
            begin
                mul_a      = r_add_result;
                mul_b      = r_add_result;

                state      = 3;
            end
		  
        3:
            begin
                result     = r_mul_result;
                state      = 0;
            end
    
        endcase
    end
    
    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
        begin
            r_add_result  <= 0;
            r_mul_result  <= 0;

            r_state       <= 0;
            r_first_tag   <= 1;
            r_prev_tag    <= 0;
    
            r_result      <= 0;
        end
        else
        begin
            r_add_result  <= add_result;
            r_mul_result  <= mul_result;

            r_state       <= state;
            r_first_tag   <= first_tag;
            r_prev_tag    <= prev_tag;

            r_result      <= result;
        end
    end

    display_driver display_driver_inst
    (
        .clock          ( clock                    ),
        .reset_n        ( reset_n                  ),
        .number         ( { data, 4'b0, r_result } ),
        .digit_enables  ( 4'b1101                  ),
        .out            ( display                  )
    );

endmodule
