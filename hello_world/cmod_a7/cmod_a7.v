module cmod_a7
(
    input         CLK,         // Тактовый сигнал 12 MHz

    output [ 1:0] LED,         // Два светодиода

    output        RGB0_Red,    // Красная часть трехцветного светодиода
    output        RGB0_Green,  // Зеленая часть трехцветного светодиода
    output        RGB0_Blue,   // Синяя часть трехцветного светодиода

    input  [ 1:0] BTN,         // Две кнопки

    output [ 7:0] ja          // Pmod Header JA
//    output [48:1] pio          // GPIO, General-Purpose Input/Output
);

    assign LED [0] = BTN [0] & BTN [1];
    assign LED [1] = BTN [0] | BTN [1];

    assign ja  = 8'b0;
//    assign pio = 48'b0;

    wire reset_n = ! BTN [0];

    reg [25:0] counter;

    always @(posedge CLK or negedge reset_n)
    begin
        if (! reset_n)
            counter <= 26'b0;
        else
            counter <= counter + 26'b1;
    end

    assign RGB0_Red   = counter [23] | (counter [15:11] != 0);
    assign RGB0_Green = counter [24] | (counter [15:11] != 0);
    assign RGB0_Blue  = counter [25] | (counter [15:11] != 0);

endmodule
