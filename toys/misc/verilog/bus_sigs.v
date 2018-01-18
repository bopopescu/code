///////////////////////////////////////////////////////
//
// 2-input bus mux
//
// multi-bit bus - vector signals
// single-bits are scalars
//
//
///////////////////////////////////////////////////////

module bus_sigs
    // Port list
            (
                // Inputs
                in_1,
                in_2,
                in_3,
                // Outputs
                out_1,
            );

    // Port I/O declarations

    input   [3:0]   in_1;
    input   [3:0]   in_2;
    input           in_3;

    output  [3:0]   out_1;

    wire    [3:0]   in_3_bus;

    // -------------- Design implementation -----------

    assign in_3_bus = {4{in_3}};
    assign out_1 = (~in_3_bus & in_1) | (in_3_bus & in_2);

endmodule

