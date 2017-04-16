module testbench;

    parameter clocks_per_second = 10;

    reg        clock;
    reg        reset;
    reg        danger;
    reg        use_posedge_danger;
    wire       retreat;
    wire       attack;
    wire [2:0] out_state;

    scorpion scorpion
    (
        clock,
        reset,
        danger,
        use_posedge_danger,
        retreat,
        attack,
        out_state
    );

    defparam scorpion.timer.clocks_per_second = clocks_per_second;

    initial
    begin
        clock = 0;

        forever
            # 5 clock = ! clock;
    end

    integer cycle; initial cycle = 0;

    always @(posedge clock)
        cycle <= cycle + 1;

    always @(posedge clock)
    begin
        $display
        (
            "cycle=%d reset=%b state=%d danger=%b retreat=%b attack=%b",
            cycle,
            reset,
            scorpion.state,
            danger,
            retreat,
            attack
        );
    end

    task test;
    begin
        $display ("*** Test with use_posedge_danger=%b ***",
            use_posedge_danger);

        danger <= 0;

        reset  <= 0; repeat (10) @(posedge clock);
        reset  <= 1; repeat (10) @(posedge clock);
        reset  <= 0; repeat (10) @(posedge clock);

        danger <= 0; repeat (     clocks_per_second     ) @(posedge clock);
        danger <= 1; repeat ( 5 * clocks_per_second     ) @(posedge clock);
        danger <= 0; repeat ( 5 * clocks_per_second     ) @(posedge clock);
        danger <= 1; repeat (     clocks_per_second / 2 ) @(posedge clock);
        danger <= 0; repeat (     clocks_per_second     ) @(posedge clock);
        danger <= 1; repeat (     clocks_per_second / 2 ) @(posedge clock);
        danger <= 0; repeat (     clocks_per_second     ) @(posedge clock);
        danger <= 1; repeat (     clocks_per_second / 2 ) @(posedge clock);
        danger <= 0; repeat (     clocks_per_second     ) @(posedge clock);
    end
    endtask

    initial
    begin
        use_posedge_danger <= 0;

        test;

        use_posedge_danger <= 1;

        test;

        $finish;
    end

    initial $dumpvars;

endmodule
