module gcd
(
		input reset,
		input clk,
		input clk_en,
		input start,
		input [31:0] dataa, datab,
		output reg done,
		output reg [31:0] result
);

reg [31:0] a,b;
reg working;

always@(posedge clk, posedge reset)
	if(reset) begin
		working <= 1'b0;
		done <= 1'b0;
		a <= 32'b0;
		b <= 32'b0;
		result <= 32'b0;
	end
	else begin
		done <= 1'b0;
		if(clk_en) begin
			if(!working) begin
				if(start) begin
					working <= 1'b1;
					a <= dataa;
					b <= datab;
				end
				else begin
					working <= 1'b0;
				end
			end
			else
				if(a != b) begin
					working <= 1'b1;
					if(a < b)
						b <= b - a;
					else	
						a <= a - b;
				end
				else begin
					result <= a;
					working <= 1'b0;
					done <= 1'b1;
				end
		end
	end
endmodule
