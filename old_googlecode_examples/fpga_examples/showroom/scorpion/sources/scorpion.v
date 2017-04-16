// This example was created based on code written by Anton Moiseev,
// a teacher from Nizhny Novgorod State Technical University, Russia

module scorpion
(
    input            clock,
    input            reset,
    input            danger,
    input            use_posedge_danger,
    output reg       retreat,
    output reg       attack,
    output     [2:0] out_state
);
     
    parameter state_wait_for_1st_danger = 3'd0,
              state_1st_danger_action   = 3'd1,
              state_wait_for_2nd_danger = 3'd2,
              state_2nd_danger_action   = 3'd3,
              state_wait_for_3rd_danger = 3'd4,
              state_3rd_danger_action   = 3'd5;

    reg previous_danger;

    always @ (posedge clock)
    begin
        if (reset) 
            previous_danger <= 0;
        else
            previous_danger <= danger;
    end

    wire danger_indicator
        = use_posedge_danger ?
              danger & ~ previous_danger : danger;

    reg  timer_start;
    reg  timer_two_seconds;
    wire timer_finish;

    timer timer
    (
        .clock       ( clock             ),
        .reset       ( reset             ),
        .start       ( timer_start       ),
        .two_seconds ( timer_two_seconds ),
        .finish      ( timer_finish      )
    );

    reg [2:0] state, next_state;
	 assign out_state = state;

    always @ (*)
    begin
        timer_start       = 0;
        timer_two_seconds = 0;

        next_state        = state;

        case (state)
        state_wait_for_1st_danger:

            if (danger_indicator)
            begin
                timer_start  = 1;
                next_state   = state_1st_danger_action;
            end

        state_1st_danger_action: 

            if (timer_finish)
                next_state = state_wait_for_2nd_danger;

        state_wait_for_2nd_danger:

            if (danger_indicator)
            begin
                timer_start = 1;
                next_state  = state_2nd_danger_action;
            end

        state_2nd_danger_action:

            if (timer_finish)
                next_state = state_wait_for_3rd_danger;

        state_wait_for_3rd_danger:

            if (danger_indicator)
            begin
                timer_start       = 1;
                timer_two_seconds = 1;
                next_state        = state_3rd_danger_action;
            end

        state_3rd_danger_action:

            if (timer_finish)
                next_state = state_wait_for_1st_danger;

        endcase
    end

    wire next_retreat
        =    state == state_1st_danger_action
          || state == state_2nd_danger_action;

    wire next_attack
        =    state == state_3rd_danger_action;

    always @ (posedge clock)
    begin
        if (reset) 
        begin
             retreat <= 0;
             attack  <= 0;
             state   <= state_wait_for_1st_danger;
        end
        else
        begin
             retreat <= next_retreat;
             attack  <= next_attack;
             state   <= next_state;
        end
    end

endmodule
