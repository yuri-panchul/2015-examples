module system
(
    input        clock,
    input  [7:0] switches,
    input  [3:0] buttons,
    output [7:0] leds,
    output [6:0] seven_segments,
    output       dot,
    output [3:0] anodes
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

    wire       danger;
    wire       use_posedge_danger;
    wire       retreat;
    wire       attack;
    wire [2:0] out_state;

    assign use_posedge_danger = switches [0];

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

    debouncer debouncer
    (
        clock,
        clock_for_debouncing,
        reset,
        buttons [2],
        danger
    );

    wire [15:0] number;
    wire        overflow = 0;
    wire [ 3:0] error    = 0;

    assign number [15:12] = danger;
    assign number [11: 8] = retreat;
    assign number [ 7: 4] = attack;
    assign number [ 3: 0] = out_state;

    display display
    (
        clock_for_display,
        reset,
        number,
        overflow,
        error,
        seven_segments,
        dot,
        anodes
    );

    assign leds = 0;

endmodule
