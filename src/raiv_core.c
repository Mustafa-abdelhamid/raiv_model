
#include <stdio.h>
#include <stdint.h>

#include "decoder.h"


int main()
{   int CLK_NUM=1;
    instr_fetch_rsp fetched_instruction;
    perf_decode_if decoder_performance; 
    decode_if decoded_signals; 
    wstall_if stalled_warps ;
    join_if join_inst; 

    fetched_instruction.instr = 0xF00F0E93 ;
    
    for (int i=0;i<CLK_NUM;i++) {
    	decode (fetched_instruction,&decoder_performance,&decoded_signals,&stalled_warps,&join_inst);
    }
     
	return 0;
}
