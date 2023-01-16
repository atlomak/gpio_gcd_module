//'timescale 1 ns/10 ps


module GpioEmu_tb;

reg n_reset = 1;
reg [15:0] saddress = 0;
reg srd = 0;
reg swr = 0;
reg clk = 0;
reg gpio_latch = 0;

reg [31:0] gpio_in = 0;
wire [31:0] gpio_out;
reg [31:0] sdata_in = 'h0;
wire [31:0] sdata_out;
wire [31:0] gpio_in_s_insp;

localparam period = 40;

initial begin
    $dumpfile("GpioEmu.vcd");
    $dumpvars(0, GpioEmu_tb);
end

initial begin 
   forever begin
   #2 clk = ~clk;
end
end

initial begin
//reset
    # 5 n_reset = 0;
    # 5 n_reset = 1;



// FIXTURE: GCD test values, result 3
    
    //Save A1 value 39
    sdata_in = 32'h27;
    saddress = 16'hf8;
    swr = 1;
    # 5;
    swr = 0;
    //Save A2 value 9
    sdata_in = 32'h9;
    saddress = 16'hfc;
    swr = 1;
    # 5;
    swr = 0;
    #period;

// ADDRESS TEST: read W value 
    saddress = 16'h100;
    srd = 1;
    # 5;
    srd = 0;
    
// ADDRESS TEST: read S value
    saddress = 16'h100;
    srd = 1;
    # 5;
    srd = 0;

// ADDRESS TEST: read A1 value
    saddress = 16'hf8;
    srd = 1;
    # 5;
    srd = 0;

// ADDRESS TEST: read A2 value
    saddress = 16'hfc;
    srd = 1;
    # 5;
    srd = 0;

// END
    # 1000 $finish;
end

gpioemu e1(n_reset, saddress, srd, swr, sdata_in, sdata_out,
    gpio_in, gpio_latch, gpio_out, clk, gpio_in_s_insp);

initial begin
    $monitor("At %t, saddress = %h , sdata_out = %h (%0d)", $time, saddress,
        sdata_out, sdata_out);
end

endmodule