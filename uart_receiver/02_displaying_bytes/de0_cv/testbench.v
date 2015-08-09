module testbench;

    reg        clock;
    reg        reset_n;
    reg        rx;
    wire [7:0] byte_data;
    wire       byte_ready;

    uart_receiver uart_receiver
    (
        clock,
        reset_n,
        rx,
        byte_data,
        byte_ready
    );

    defparam uart_receiver.clock_frequency = 500;
    defparam uart_receiver.baud_rate       = 10;

    initial
    begin
        clock = 1;

        forever # 50 clock = ! clock;
    end

    initial
    begin
        repeat (2) @(posedge clock);
        reset_n <= 0;
        repeat (2) @(posedge clock);
        rx      <= 1;
        repeat (2) @(posedge clock);
        reset_n <= 1;
    end

    task symbol (input _rx);
    begin
        rx <= _rx;
        repeat (uart_receiver.clock_cycles_in_symbol) @(posedge clock);
    end
    endtask

    integer i, j;

    initial
    begin
        $dumpvars;

        $monitor
        (
            "clock %b reset_n %b rx %b byte_data %h byte_ready %b",
            clock,
            reset_n,
            rx,
            byte_data,
            byte_ready
        );

        @(posedge reset_n);
        @(posedge clock);
 
        symbol (1);
 
        for (i = 0; i < 10; i = i + 1)
        begin
            symbol (0);

            for (j = 0; j < 8; j = j + 1)
                symbol (~ (j & 1));

            repeat (3) symbol (1);
        end

        $finish;
    end

endmodule
