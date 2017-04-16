`ifdef COMMENTED_OUT

module isqrt_pipe_test;

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

    `define fifo_depth 4
    reg [31:0] fifo [0 : `fifo_depth - 1];
    integer fifo_next_index; initial fifo_next_index = 0;

    isqrt_pipe_gen_for_for isqrt
    (
        clock,
        reset_n,
        run,
        x,
        ready,
        y
    );

    defparam isqrt.n_pipe_stages = `fifo_depth;

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
        reg [31:0] oldest_x_in_fifo;
    begin
        x   <= i;
        run <= 1;

        oldest_x_in_fifo = fifo [fifo_next_index];
        fifo [fifo_next_index] = i;

        fifo_next_index = fifo_next_index + 1;

        if (fifo_next_index == `fifo_depth)
            fifo_next_index = 0;

        @(posedge clock);

        if (ready)
            check (oldest_x_in_fifo, y);
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
