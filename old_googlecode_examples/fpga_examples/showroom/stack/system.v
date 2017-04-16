`include "defines.vh"

module system
(
    input            clock,
    input      [7:0] switches,
    input      [3:0] buttons,
    output reg [7:0] leds,
    output     [6:0] seven_segments,
    output           dot,
    output     [3:0] anodes
);

    wire reset = buttons [3];
    wire clock_for_debouncing;
    wire clock_for_display;

    clock_divider clock_divider
    (
        clock,
        reset,
        clock_for_debouncing,
        clock_for_display
    );

    wire push, pop;

    debouncer debouncer2
        ( clock, clock_for_debouncing, reset, buttons [2], push );

    debouncer debouncer1
        ( clock, clock_for_debouncing, reset, buttons [1], pop  );

    wire [ 7:0] data;
    wire [15:0] result;
    wire        overflow;
    wire [ 3:0] error;

    assign data = switches;

    display display
    (
        clock_for_display,
        reset,
        result,
        overflow,
        error,
        seven_segments,
        dot,
        anodes
    );

    wire [`word_width - 1:0] write_data;
    wire [`word_width - 1:0] read_data;

    assign write_data = data;
    assign result     = read_data;

    assign overflow   = 0;
    assign error      = 0;

    stack stack
    (
        clock,
        reset,
        push,
        pop,
        write_data,
        read_data
    );

    always @(posedge clock_for_display)
        leds <= reset ? 0 : switches;

endmodule
