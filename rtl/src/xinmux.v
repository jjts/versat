`timescale 1ns / 1ps
`include "xversat.vh"

module  xinmux (
                input [`N_W-1:0]         sel,
                input [2*`DATABUS_W-1:0] data_in,
                output reg [`DATA_W-1:0] data_out
                );

   always @ (*) begin
      integer 		i;
      for (i=0; i < `N; i = i+1)
	if (sel == i) 
	  data_out = data_in[(1+sel[`N_W-1])*`DATABUS_W-`DATA_W*i -1 -: `DATA_W];
   end

endmodule