module srec_parser
(
    input         clock,
    input         reset_n,
    input  [ 7:0] char_data,
    input         char_ready

    output        error,
    output [31:0] write_address,
    output [ 7:0] write_byte,
    output        write_enable
);

    localparam [4:0]
        WAITING_S          = 5'd0,
        GET_TYPE           = 5'd1,
        GET_COUNT_7_4      = 5'd2,
        GET_COUNT_3_0      = 5'd3,
        GET_ADDRESS_31_28  = 5'd4,
        GET_ADDRESS_27_24  = 5'd5,
        GET_ADDRESS_23_20  = 5'd6,
        GET_ADDRESS_19_16  = 5'd7,
        GET_ADDRESS_15_12  = 5'd8,
        GET_ADDRESS_11_08  = 5'd9,
        GET_ADDRESS_07_04  = 5'd10,
        GET_ADDRESS_03_00  = 5'd11,
        GET_BYTE_7_4       = 5'd12,
        GET_BYTE_3_0       = 5'd13,
        CHECK_SUM_7_4      = 5'd14,
        CHECK_SUM_3_0      = 5'd15,
        CR                 = 5'd16,
        LF                 = 5'd17
        ;

reg  [3:0] nibble       , reg_nibble       ;
reg        nibble_error , reg_nibble_error ;

wire [7:0] byte_data  = { reg_nibble, nibble };
reg  [7:0] reg_byte_data;
wire       byte_error = nibble_error | reg_nibble_error;

always @(posedge clock)
begin
    reg_nibble       <= nibble;
    reg_nibble_error <= nibble_error;
    reg_byte_data    <= byte_data;
end

always @*
begin
    nibble       = 0;
    nibble_error = 0;

    if (char_data >= "0" && char_data <= "9")
        nibble = char_data - "0";
    else if (char_data >= "A" && char_data <= "F")
        nibble = char_data - "A";
    else
        nibble_error = 1;
end

reg [ 4:0] state          , reg_state          ; 
reg        combined_error , reg_combined_error ;
reg [ 7:0] rec_type       , reg_rec_type       ;
reg [ 7:0] count          , reg_count          ;
reg [31:0] address        , reg_address        ;
reg        write          , reg_write          ;

assign error          = reg_combined_error;
assign write_address  = reg_address;
assign write_byte     = reg_byte_data;
assign write_enable   = reg_write;

always @*
begin
    state          = reg_state + 1      ; 
    combined_error = reg_combined_error ;
    rec_type       = reg_rec_type       ;
    count          = reg_count          ;
    address        = reg_address        ;
    write          = 0                  ;

    case (reg_state)

    WAITING_S:

        if (char_data != "S")
            combined_error = 1;

    GET_TYPE:

        rec_type = char_data;

    GET_COUNT_7_4, GET_COUNT_3_0:
    begin
        count = (count << 4) | nibble_data;
        combined_error = combined_error | nibble_error;
    end

    GET_ADDRESS_31_28 , GET_ADDRESS_27_24,
    GET_ADDRESS_23_20 , GET_ADDRESS_19_16,
    GET_ADDRESS_15_12 , GET_ADDRESS_11_08,
    GET_ADDRESS_07_04 , GET_ADDRESS_03_00:
    begin
        address = (address << 4) | nibble_data;
        combined_error   = combined_error | nibble_error;
    end

    GET_BYTE_7_4:

        ;

    GET_BYTE_3_0:
    begin
        address = address + 1;
        write   = 1;

        combined_error = combined_error | byte_error;

        count = count - 1;

        if (count > 1)
            state = GET_BYTE_7_4;
    end

    CHECK_SUM_7_4, CHECK_SUM_3_0:

        combined_error = combined_error | nibble_error;

    CR:
        combined_error = combined_error | (char_data != 8'h0D);    

    LF:
    begin
        combined_error = combined_error | (char_data != 8'h0A);
        state = WAITING_S;
    end

    endcase

end
