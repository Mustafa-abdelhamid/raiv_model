//=====================================================================
//
// decoder.h                                              Date: 2022/10/29
//
// Author : Mustafa abdelhamid
//
//=====================================================================
/*
  Description:
=================	 
	1. it contains new defined data types used as input and output from decode stage 
		instr_fetch_rsp, perf_decode_if, decode_if , wstall_if, join_if
	2. Functions, used in decoder, prototype
		signExtend : extend a signal of width [1 --> 16] to a 32 bit signal 
		decode: raiv decoder model function  	
*/
// -------------------------------------------------------------------------
// INCLUDES
// -------------------------------------------------------------------------
#include <stdio.h>
#include <stdint.h>
#include <raiv_config.h>


#define DEBUG



typedef struct instr_fetch_rsp
{
    	//unsigned valid : 1;
    	//unsigned ready : 1;
	
    	uint64_t uuid : UUID_BITS;
	unsigned tmask : NUM_THREADS ;
	
	unsigned wid : NUM_WARPS;
    
	unsigned pc : 32;
    	unsigned instr : 32;
	
} instr_fetch_rsp;

typedef struct perf_decode_if
{
    	uint64_t loads : PERF_CTR_BITS;
    	uint64_t stores : PERF_CTR_BITS;
	
    	uint64_t branches : PERF_CTR_BITS;
	
	
} perf_decode_if;    

typedef struct decode_if
{
	//unsigned valid:1;
	uint64_t uuid: UUID_BITS;
	unsigned wid: NUM_WARPS;
        unsigned tmask: NUM_THREADS;
        unsigned PC:32;
        unsigned ex_type: EX_BITS;  
        unsigned op_type:INST_OP_BITS;
        unsigned op_mod:INST_MOD_BITS;   
        unsigned wb:1;
        unsigned use_PC:1;
        unsigned use_imm:1;
        unsigned imm:32;
        unsigned rd:6; //6 because of floating point regs 
        unsigned rs1:6;
        unsigned rs2:6;
        unsigned rs3:6;
        //unsigned  ready:1;
	
	
} decode_if; 
 
typedef struct wstall_if
{
    unsigned valid : 1;
    unsigned wid : NUM_WARPS;
    unsigned stalled : 1;
	
} wstall_if; 

typedef struct join_if
{
    unsigned valid : 1;
    unsigned wid : NUM_WARPS;
	
} join_if; 

int main()
{
    instr_fetch_rsp fetched_instruction;
    perf_decode_if decoder_performance; 
    decode_if decoded_signals; 
    wstall_if stalled_warps ;
    join_if join_inst; 

    fetched_instruction.instr = 0x00000002 ;

    decode (fetched_instruction,&decoder_performance,&decoded_signals,&stalled_warps,&join_inst); 
	return 0;
}

//============================= FUNCTION ============================================
void extendSign ( uint16_t signal , uint8_t signalWidth, uint32_t *extendedSignal) ;

void decode (instr_fetch_rsp fetched_instruction, perf_decode_if *decoder_performance, decode_if *decoded_signals , wstall_if *stalled_warps , join_if *join_inst ) 

