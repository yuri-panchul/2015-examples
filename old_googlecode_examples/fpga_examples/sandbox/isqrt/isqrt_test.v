`ifndef COMMENTED_OUT

`ifndef ISQRT
`define ISQRT isqrt_1
`endif

module isqrt_test;

    reg         clock;
    reg         reset_n;
    reg         run;
    reg  [31:0] x;
    wire        ready;
    wire [15:0] y;

    initial
    begin
        clock = 1;

        forever
            # 50 clock = ! clock;
    end

    initial
    begin
        reset_n <= 0;
        repeat (20) @(posedge clock);
        reset_n <= 1;
    end

    `ISQRT isqrt
    (
        clock,
        reset_n,
        run,
        x,
        ready,
        y
    );

    task check (input [31:0] x, input [31:0] y);
    begin
        $display ("%m sqrt (%d) = %d    %d <= %d < %d",
            x, y, y ** 2, x, (y + 1) ** 2);

        if (y ** 2 > x)
            $display ("%m Error! y ** 2 > x");

        if (x >= (y + 1) ** 2)
            $display ("%m Error! x >= (y + 1) ** 2");
    end
    endtask

    task test (input [31:0] i);
    begin
        x   <= i;
        run <= 1;

        @(posedge clock);

        run <= 0;

        @(posedge clock);

        while (! ready)
            @(posedge clock);

        check (x, y);
    end
    endtask

    integer i;

    initial
    begin
        $dumpvars;

        @(posedge reset_n);
        @(posedge clock);

        for (i = 0; i < 256; i = i + 1)
            test (i);

        for (i = 0; i < 256; i = i + 1)
            test ('hFFFF_FFFF - i);

        for (i = 0; i < 256; i = i + 1)
            test ($random);

        $finish;
    end

endmodule

`endif
