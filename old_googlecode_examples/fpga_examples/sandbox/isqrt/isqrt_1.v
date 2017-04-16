module isqrt_1
(
    input             clock,
    input             reset_n,
    input             run,
    input      [31:0] x,
    output reg        ready,
    output reg [15:0] y
);

    reg [31:0] m, tx, ty, b;

    always @*
    begin
        m  = 31'h4000_0000;
        tx = x;
        ty = 0;
    
        repeat (16)
        begin
            b  = ty |  m;
            ty = ty >> 1;
            
            if (tx >= b)
            begin
                tx = tx - b;
                ty = ty | m;
            end
            
            m = m >> 2;
        end
    end

    always @(posedge clock or negedge reset_n)
    begin
        if (! reset_n)
        begin
            ready <= 0;
            y     <= 0;
        end
        else if (run)
        begin
            ready <= 1;
            y     <= ty [15:0];
        end
        else
        begin
            ready <= 0;
        end
    end

endmodule
