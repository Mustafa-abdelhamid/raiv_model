
#include <stdio.h>
#include <stdint.h>

#include "decoder.h"


int main()
{
    instr_fetch_rsp fetched_instruction;
    perf_decode_if decoder_performance; 
    decode_if decoded_signals; 
    wstall_if stalled_warps ;
    join_if join_inst; 

    fetched_instruction.instr = 0x00000FF2 ;

    decode (fetched_instruction,&decoder_performance,&decoded_signals,&stalled_warps,&join_inst); 
	return 0;
}
