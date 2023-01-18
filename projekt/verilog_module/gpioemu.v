/* verilator lint_off UNUSED */
/* verilator lint_off MULTIDRIVEN */
module gpioemu(
	n_reset, 							//magistrala z CPU
	saddress[15:0], srd, swr,
	sdata_in[31:0], sdata_out[31:0],
	gpio_in[31:0], gpio_latch, 			//styk z GPIO - in
	gpio_out[31:0], 					//styk z GPIO = out
	clk, 								
	gpio_in_s_insp[31:0]);

// Interface

input n_reset;
input srd;
input swr;
input gpio_latch;
input clk;

input [15:0] saddress;
input [31:0] sdata_in;
input [31:0] gpio_in;

output [31:0] sdata_out;
output [31:0] gpio_out;
output [31:0] gpio_in_s_insp;

// DEBUG REGs

reg [31:0] gpio_in_s;

// GCD REGs

reg start;
reg [31:0] A1;
reg [31:0] A2;
reg [31:0] W;
reg [31:0] S;
reg [31:0] a,b;
reg [31:0] sdata_out_s;

// OPS COUNTER REG

reg [31:0] counter;



always@(posedge n_reset)
	begin
		S <= 32'b0;
		a <= 32'b0;
		b <= 32'b0;
		W <= 32'b0;
		counter <= 32'b0;
	end
always@(posedge clk)
	begin
		if(!S[3]) begin
			if(start) begin
				S[3] <= 1'b1;
				a <= A1;
				b <= A2;
			end
			else begin
				S[3] <= 1'b0;
			end
		end
		else
			if(a != b) begin

				if(a < b)
					b <= b - a;
				else	
					a <= a - b;
			end
			else begin
				W <= a;
				S[3] <= 1'b0;
				start <= 1'b0;
			end
	end

// ADDRESS ACCESS LOGIC

	always@(posedge srd) begin
		if(saddress == 16'hf8) begin
			sdata_out_s <= A1;  // A1 
		end
		if(saddress == 16'hfc) begin
			sdata_out_s <= A2; // A2
		end
		if(saddress == 16'h100) begin
			sdata_out_s <= W;	// W
		end
		if(saddress == 16'h104) begin
			sdata_out_s <= S; // S
		end
	end
	
	always@(posedge swr) begin
		if(saddress == 16'hf8) begin
			A1 <= sdata_in;
		end
		if(saddress == 16'hfc) begin
			A2 <= sdata_in;
			start <= 1'b1;
			counter <= counter + 1'b1;
		end
	end

always@(posedge gpio_latch)
	begin
		gpio_in_s <= gpio_in;
	end

// BINDINGS
assign sdata_out = sdata_out_s;
assign gpio_out = counter;
assign gpio_in_s_insp = gpio_in_s;

endmodule
