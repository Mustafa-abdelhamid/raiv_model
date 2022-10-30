//=====================================================================
//
// decoder.c                                              Date: 2022/10/29
//
// Author : Mustafa abdelhamid
//=====================================================================


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "decoder.h"

// registers naming conventioal ( Application program interface)
char registers_abi [32] [5] = {
	"zero", 	//x0
	"ra",		//x1
	"sp",		//x2
	"gp",		//x3
	"tp",		//x4
	"t0", 		//x5
	"t1",		//x6
	"t2",		//x7
	"s0",		//x8
	"s1",		//x9
	"a0",	 	//x10
	"a1",		//x11
	"a2",		//x12
	"a3",		//x13
	"a4",		//x14
	"a5", 		//x15
	"a6",		//x16
	"a7",		//x17
	"s2",		//x18
	"s3",		//x19
	"s4",		//x20
	"s5",		//x21
	"s6",		//x22
	"s7",		//x23
	"s8",	 	//x24
	"s9",		//x25
	"s10",		//x26
	"s11",		//x27
	"t3",		//x28
	"t4", 		//x29
	"t5",		//x30
	"t6"		//x31

} ;

char *get_regName(uint16_t regNum) {
	return registers_abi[regNum] ;
}

uint32_t extendSign ( uint16_t signal , uint8_t signalWidth)  {

	uint32_t extendMask= 0xFFFFFFFF;
	extendMask = extendMask << signalWidth ;
	bool checkPolarity = ( signal >> (signalWidth-1) ) ;
	if (!checkPolarity) 
		return (signal) ;
	else 
		return (extendMask | signal) ;
	/*
	#ifdef DEBUG
	printf("signal: 0x%04x\n", signal);
	printf("extendMask: 0x%04x\n", extendMask);
	printf("extended signal: 0x%08x\n", extendedSignal);
	#endif
	*/
}

//====================================================================================================================

void decode (instr_fetch_rsp fetched_instruction,
 perf_decode_if *decoder_performance, decode_if *decoded_signals , wstall_if *stalled_warps , join_if *join_inst ) 
{	
	//************************** pass undecoded signals ***********************************// 
	decoded_signals -> uuid = fetched_instruction.uuid;
	decoded_signals -> wid = fetched_instruction.wid ;
        decoded_signals -> tmask =fetched_instruction.tmask;
        decoded_signals -> PC= fetched_instruction.pc;
        
	stalled_warps -> wid = fetched_instruction.wid ;
	
	join_inst -> wid = fetched_instruction.wid ;
		
//************************** initialize undecoded signals  ***********************************//  
// NOTE performance counter ISN'T initialized ... need to be initialized in main 
	decoded_signals -> ex_type=0;  
        decoded_signals -> op_type=0;
        decoded_signals -> op_mod=0;   
        decoded_signals -> wb=0;
        decoded_signals -> use_PC=0;
        decoded_signals -> use_imm=0;
        decoded_signals -> imm=0;
        decoded_signals -> rd=0;
        decoded_signals -> rs1=0;
        decoded_signals -> rs2=0;
        decoded_signals -> rs3=0;
		
	stalled_warps -> valid = 0 ;
	stalled_warps -> stalled = 0 ;
		
	join_inst -> valid = 0 ;
//**************************************************************************************************//
//***************************** EXTRACT Informations from instruction ***********************************//	
	// for  signal [x:y] ... signal>>(y) & mask of ones equal Width 
	
	uint8_t opcode 	= 	fetched_instruction.instr & 0x7F ; 		//instr [6:0] 
	uint8_t func2  	= 	(fetched_instruction.instr >> 25)& 0x3 ;	//instr[26:25];
	uint8_t func3  	= 	(fetched_instruction.instr >> 12)& 0x7 ;	//instr[14:12];
	uint8_t func7  	= 	(fetched_instruction.instr >> 25)& 0x7F ;	//instr[31:25];
	uint16_t u_12  	= 	(fetched_instruction.instr >> 20)& 0xFFF ;	//instr[31:20];
	uint8_t rd  		= 	(fetched_instruction.instr >> 7)& 0x1F ;	//instr[11:7];
	uint8_t rs1  		= 	(fetched_instruction.instr >> 15)& 0x1F ;	//instr[19:15];
	uint8_t rs2  		= 	(fetched_instruction.instr >> 20)& 0x1F ;	//instr[24:20];
	uint8_t rs3  		= 	(fetched_instruction.instr >> 27)& 0x1F ;	//instr[31:27];
	
	// immediate values 
	// 20 bit U tybe immediate 
	uint32_t upper_imm  = 	(fetched_instruction.instr >> 12)& 0xFFFFF ;	//instr[31:12];
	
	/* ALU immediate in case of shift operation
	(we need only 5 bits: max shift is 32   so riscv make use of the rest of imm bits)
	------------------------------------------------------------------------------------------------------	
	instr	||	func3			|| imm			||	description 			||
	------	||---------------------------	||------------------	||-----------------------------------	||
	slli	||  	0x1			|| imm [11:5] = 0x00 	||	rd = rs1 << imm[4:0](rs2)	||
	------	||---------------------------	||--------------------	||-----------------------------------	||
	slri	||  	0x5			|| imm [11:5] = 0x00 	||	rd = rs1 >> imm[0:4]		||
	------	||---------------------------	||--------------------	||-----------------------------------	||
	srla	||	0x5			|| imm [11:5] = 0x20	||	rd = rs1 >> imm[0:4]		||
	--------------------------------------------------------------------------------------------------	||
	*/
	uint16_t alu_imm ;
    if (func3==5 || func3==1 ) {
		alu_imm= u_12 & 0x01F; 
	}
	else {
		alu_imm=u_12;
	}
	
	// s type immediate {func7, rd};
	uint16_t s_imm = func7 ; 
	s_imm = (s_imm <<5) | rd;
    //******************** 
	//b_imm   = {instr[31], instr[7], instr[30:25], instr[11:8], 1'b0};
	uint16_t b_imm = func7 ;
	// jal_imm   = {instr[31], instr[19:12], instr[20], instr[30:21], 1'b0};
	uint32_t jal_imm = func7 ;
	/*
	#ifdef DEBUG 
	printf("Instruction :0x%08x\n", fetched_instruction.instr);  
	printf("Opcode: 0x%02x\n", opcode);
	printf("func2 :0x%01x\n", func2);  
	printf("func3: 0x%01x\n", func3);
	printf("func7 :0x%02x\n", func7);  
	printf("rd :0x%02x\n", rd);  
	printf("rs1: 0x%02x\n", rs1);
	printf("rs2: 0x%02x\n", rs2);
	printf("rs3: 0x%02x\n", rs3);
	
	printf("Imm: 0x%03x\n", u_12);
	printf("alu_imm: 0x%03x\n", alu_imm);
	#endif
	*/
    
	switch(opcode) {

    case INST_I  :
        //ex_type = #EX_ALU;
	decoded_signals -> ex_type=EX_ALU;  
        //decoded_signals -> op_type=0;
        decoded_signals -> op_mod=0;   
        //decoded_signals -> wb=0;
        decoded_signals -> use_PC=0;
        decoded_signals -> use_imm=1;
        decoded_signals -> imm=extendSign(alu_imm,12);
        decoded_signals -> rd= rd;
        decoded_signals -> rs1=rs1;
        decoded_signals -> rs2=0;
        decoded_signals -> rs3=0;
		
	stalled_warps -> valid = 0 ;
	stalled_warps -> stalled = 0 ;
		
	join_inst -> valid = 0 ;
	
	if (rd !=0)
		decoded_signals -> wb=1;
	else 
		decoded_signals -> wb=0;
	
	char *rd_name= get_regName(rd);
	char *rs1_name= get_regName(rs1);	
	switch (func3) {
		case 0x0: 
			// intented for deassemble output 
			printf("addi %s , %s , %d \n",rd_name,rs1_name,decoded_signals -> imm);
			decoded_signals -> op_type= INST_ALU_ADD;
			break;
		case 0x1:
			printf("SLLI");
			decoded_signals -> op_type= INST_ALU_SLL;
			break;
		case 0x2:
			printf("SLTI");
			decoded_signals -> op_type= INST_ALU_SLT;
			break;
		case 0x3:
			printf("SLTU");
			decoded_signals -> op_type= INST_ALU_SLTU;
			break;
		case 0x4:
			printf("XORI");
			decoded_signals -> op_type= INST_ALU_XOR;
			break;
		case 0x5: 
			printf("SRAI");
			decoded_signals -> op_type= INST_ALU_SRA;
			break;
		case 0x6:
			printf("ORI");
			decoded_signals -> op_type= INST_ALU_OR;
			break;
		case 0x7:
			printf("ANDI");
			decoded_signals -> op_type= INST_ALU_AND;
			break;	
	}
	
	break; 
	
   
   
   default : 
		printf("NOP");
	}
 
}

