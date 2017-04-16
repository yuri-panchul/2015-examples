module timer
# (parameter clocks_per_second = 50_000_000)
(
    input      clock,
    input      reset,
    input      start,
    input      two_seconds,
    output reg finish
);

    reg [31:0] counter;

    always @(posedge clock)
    begin
        if (reset)
        begin
            counter <= 0;
            finish  <= 0;
        end
        else
        begin
            if (start)
                counter <= two_seconds ?
                    2 * clocks_per_second : clocks_per_second;
            else if (counter != 0)
                counter <= counter - 1;
            
            finish <= (counter == 1);
        end
    end

endmodule
