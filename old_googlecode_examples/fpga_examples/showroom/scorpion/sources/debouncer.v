module debouncer
(
    input      clock,
    input      clock_for_debouncing,
    input      reset,
    input      button,
    output reg debounced_button
);

    reg [2:0] samples;

    always @(posedge clock_for_debouncing)
    begin
        if (reset)
            samples <= 0;
        else
            samples <= { samples [1:0], button };
    end

    always @(posedge clock)
    begin
        if (reset)
            debounced_button <= 0;
        else
            debounced_button <= & samples;
    end

endmodule
