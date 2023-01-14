module gpioemu(
	n_reset, 							//magistrala z CPU
	saddress[15:0], srd, swr,
	sdata_in[31:0], sdata_out[31:0],
	gpio_in[31:0], gpio_latch, 			//styk z GPIO - in
	gpio_out[31:0], 					//styk z GPIO = out
	clk, 								//sygnał opcjonalny - zegar 1KHz
	gpio_in_s_insp[31:0]);

// Interface

input reset;
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

// GCD REGs

reg clk_en;
reg start;
reg [31:0] A1;
reg [31:0] A2,
reg [31:0] W
reg [31: 0] S;
reg [31:0] a,b;


always@(posedge clk, posedge reset)
	if(reset) begin
		S[3] <= 1'b0;
		a <= 32'b0;
		b <= 32'b0;
		W <= 32'b0;
	end
	else begin
		if(clk_en) begin
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
					S[3] <= 1'b1;
					if(a < b)
						b <= b - a;
					else	
						a <= a - b;
				end
				else begin
					W <= a;
					S[3] <= 1'b0;
				end
		end
	end
				end
		end
	end
endmodule
