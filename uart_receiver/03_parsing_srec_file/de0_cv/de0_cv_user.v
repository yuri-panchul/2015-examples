module de0_cv_user
(
    input         CLOCK_50,
    input         RESET_N,
    input  [ 3:0] KEY,
    input  [ 9:0] SW,
    output [ 9:0] LEDR,
    output [ 6:0] HEX0,
    output [ 6:0] HEX1,
    output [ 6:0] HEX2,
    output [ 6:0] HEX3,
    output [ 6:0] HEX4,
    output [ 6:0] HEX5,
    inout  [35:0] GPIO_0,
    inout  [35:0] GPIO_1
);

    wire uart_rx = GPIO_1 [31];

    wire [7:0] char_data;
    wire       char_ready;

    uart_receiver uart_receiver
    (
        .clock      ( CLOCK_50   ),
        .reset_n    ( RESET_N    ),
        .rx         ( uart_rx    ),
        .byte_data  ( char_data  ),
        .byte_ready ( char_ready )
    );                     

    wire        in_progress;
    wire        format_error;
    wire        checksum_error;
    wire [ 7:0] error_location;

    wire [31:0] write_address;
    wire [ 7:0] write_byte;
    wire        write_enable;

    srec_parser srec_parser
    (
        .clock           ( CLOCK_50       ),
        .reset_n         ( RESET_N        ),

        .char_data       ( char_data      ),
        .char_ready      ( char_ready     ), 

        .in_progress     ( in_progress    ),
        .format_error    ( format_error   ),
        .checksum_error  ( checksum_error ),
        .error_location  ( error_location ),

        .write_address   ( write_address  ),
        .write_byte      ( write_byte     ),
        .write_enable    ( write_enable   )
    );

    assign LEDR = { in_progress, format_error, checksum_error, error_location [6:0] };
    
    single_digit_display digit_0 ( write_byte    [ 3: 0] , HEX0 );
    single_digit_display digit_1 ( write_byte    [ 7: 4] , HEX1 );
    single_digit_display digit_2 ( write_address [ 3: 0] , HEX2 );
    single_digit_display digit_3 ( write_address [ 7: 4] , HEX3 );
    single_digit_display digit_4 ( write_address [11: 8] , HEX4 );
    single_digit_display digit_5 ( write_address [31:28] , HEX5 );

endmodule
