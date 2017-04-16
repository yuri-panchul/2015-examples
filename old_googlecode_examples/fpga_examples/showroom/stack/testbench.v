`include "defines.vh"

module testbench;

    reg  clock;
    reg  reset;
    reg  push;
    reg  pop;

    reg  [`word_width - 1:0] write_data;
    wire [`word_width - 1:0] read_data;

    stack stack
    (
        clock,
        reset,
        push,
        pop,
        write_data,
        read_data
    );

    //----------------------------------------------------------------

    initial
    begin
        clock = 0;

        forever
            # 5 clock = ! clock;
    end

    //----------------------------------------------------------------

    task dump;
    begin

        $write   ("push=%h pop=%h",
                   push,   pop);

        $display (" write_data=%h read_data=%h",
                    write_data,   read_data);

    end
    endtask

    task t_reset;
    begin

        reset       <= 1; repeat (10) @(posedge clock);

        push        <= 0;
        pop         <= 0;
        write_data  <= 0;

        reset       <= 0; repeat (10) @(posedge clock);

        $write ("After reset    "); dump;

    end
    endtask

    task t_push (input [`word_width - 1:0] value);
    begin

        write_data  <= value;
        push        <= 1;             @(posedge clock);
        push        <= 0; repeat (10) @(posedge clock);

        $write ("After push %x  ", value); dump;

    end
    endtask

    task t_pop;
    begin

        pop         <= 1;             @(posedge clock);
        pop         <= 0; repeat (10) @(posedge clock);

        $write ("After pop      "); dump;

    end
    endtask

    //----------------------------------------------------------------

    integer i;

    initial
    begin
        t_reset;

        for (i = 0; i < 20; i = i + 1)
            t_push (i);

        for (i = 0; i < 20; i = i + 1)
            t_pop;

        $finish;
    end

endmodule
