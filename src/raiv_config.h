//=====================================================================
//
// raiv_config.h                                              Date: 2022/10/29
//
// Author : Mustafa abdelhamid
/*
 Description: 
     This is the header file containing model parameters 
     	- Warps and threads number
     	- Signals Number of bits
     	- Signal Encoding 
//=====================================================================
*/

#define NUM_THREADS 4
#define NUM_WARPS 4

#define PERF_CTR_BITS   44

#define UUID_BITS       44



#define INST_ALU_ADD         0b0000
#define INST_ALU_LUI         0b0010
#define INST_ALU_AUIPC       0b0011
#define INST_ALU_SLTU        0b0100
#define INST_ALU_SLT         0b0101
#define INST_ALU_SRL         0b1000
#define INST_ALU_SRA         0b1001
#define INST_ALU_SUB         0b1011
#define INST_ALU_AND         0b1100
#define INST_ALU_OR          0b1101
#define INST_ALU_XOR         0b1110
#define INST_ALU_SLL         0b1111
#define INST_ALU_OTHER       0b0111


#define EX_NOP          0x0
#define EX_ALU          0x1
#define EX_LSU          0x2
#define EX_CSR          0x3
#define EX_FPU          0x4
#define EX_GPU          0x5
#define EX_BITS         3

#define INST_OP_BITS    4
#define INST_MOD_BITS   3


#define INST_LUI        0b0110111
#define INST_AUIPC      0b0010111
#define INST_JAL        0b1101111
#define INST_JALR       0b1100111
#define INST_B          0b1100011 // branch instructions
#define INST_L          0b0000011 // load instructions
#define INST_S          0b0100011 // store instructions
#define INST_I          0b0010011 // immediate instructions
#define INST_R          0b0110011 // register instructions
#define INST_FENCE      0b0001111 // Fence instructions
#define INST_SYS        0b1110011 // system instructions

#define INST_FL         0b0000111 // float load instruction
#define INST_FS         0b0100111 // float store  instruction
#define INST_FMADD      0b1000011  
#define INST_FMSUB      0b1000111
#define INST_FNMSUB     0b1001011
#define INST_FNMADD     0b1001111 
#define INST_FCI        0b1010011 // float common instructions

#define INST_GPGPU      0b1101011
#define INST_GPU        0b1011011

#define INST_TEX       0b0101011

