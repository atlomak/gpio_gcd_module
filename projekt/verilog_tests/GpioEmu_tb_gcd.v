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



// GCD Module test expected 422
    
    //Save A1 value 422
    sdata_in = 32'h1a6;
    saddress = 16'hf8;
    swr = 0;
    swr = 1;
    #5;

    //Save A2 value 844
    sdata_in = 32'h34c;
    saddress = 16'hfc;
    swr = 0;
    swr = 1;
    #period;
// GCD Module test expected 3
    
    //Save A1 value 39
    sdata_in = 32'h27;
    saddress = 16'hf8;
    swr = 0;
    swr = 1;
    #5;
    //Save A2 value 9
    sdata_in = 32'h9;
    saddress = 16'hfc;
    swr = 0;
    swr = 1; 
    #period;
// GCD Module test expected 4
    
    //Save A1 value 4
    sdata_in = 32'h4;
    saddress = 16'hf8;
    swr = 0;
    swr = 1;
    #5;
    //Save A2 value 216
    sdata_in = 32'hd8;
    saddress = 16'hfc;
    swr = 0;
    swr = 1; 
    #period;
// END
    # 1000 $finish;
end

gpioemu e1(n_reset, saddress, srd, swr, sdata_in, sdata_out,
    gpio_in, gpio_latch, gpio_out, clk, gpio_in_s_insp);

initial begin
    $monitor("At %t, gpio_out = %h (%0d), sdata_in = %h (%0d)", $time, gpio_out, gpio_out,
        sdata_in, sdata_in);
end

endmodule