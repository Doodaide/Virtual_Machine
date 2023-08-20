#ifndef HEADER_H_
#define HEADER_H_

#include <stdint.h>

// Structure stuff:
enum commands;
enum VirtualRoutines;

struct heapMemoryNode
{
    char * contents; // array of 64 
    struct heapMemoryNode * next;
    unsigned char linked;
    unsigned char index;
    unsigned char allocated;
    /*
    linked = 0 -> empty block 
    linked = 1 -> full block 
    linked = 2 -> linked to 1 more block 
    */
};

unsigned char getOffset(uint32_t address);

struct heapMemoryNode* startBank();

struct heapMemoryNode * getHeapMem(uint32_t address, struct heapMemoryNode * head);

struct heapMemoryNode * getNextBank(struct heapMemoryNode * n);

// ? getInstructionsSet.c: 

int32_t signExtend(uint32_t i, uint32_t len, uint32_t pos, uint32_t neg);

void getErrored(int32_t * PC, uint32_t * reg, char* virtualMem, int32_t op, struct heapMemoryNode * head);

uint32_t getOpCode(uint32_t opCode, int32_t * PC, uint32_t * reg);

int32_t getRegister(uint32_t instStart, int32_t * PC, uint32_t * reg, char* virtMem, unsigned char start
, struct heapMemoryNode * head);

int32_t getFunc3(uint32_t instStart, int32_t * PC, uint32_t * reg, char * virtMem
, struct heapMemoryNode * head);

int32_t getFunc7(uint32_t instStart, int32_t * PC, uint32_t * reg, char* virtMem
, struct heapMemoryNode * head);

int32_t getImmI(uint32_t instStart);

int32_t getImmU(uint32_t instStart);

int32_t getImmS(uint32_t instStart);

int32_t getImmSB(uint32_t instStart);

int32_t getImmUJ(uint32_t instStart);

void virtMemAccess(uint32_t * reg, char * start, int32_t address, int32_t pos, 
struct heapMemoryNode * head);
// ? OperationSet.c:

void add_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs);

void addi_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs);

void sub_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs); 

void lui_OP(int32_t rd, int32_t imm, uint32_t * regs);              

void xor_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs); 

void xori_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs);

void or_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs);  

void ori_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs); 

void and_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs); 

void andi_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs);

void sll_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs);

void srl_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs); 

// ! FiX bIt shifting
void sra_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs);   // !13 Probably

// ! NEED TO CHECK MEMORY ACCESS FROM HERE
void lb_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs, char* M, struct heapMemoryNode * head); // *14

void lh_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs, char* M, struct heapMemoryNode * head); // *15

void lw_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs, char* M, struct heapMemoryNode * head); // *16

void lbu_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs,char * M, struct heapMemoryNode * head); // *17

void lhu_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs,char * M, struct heapMemoryNode * head);// *18

void sb_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, char* M, struct heapMemoryNode * head);

void sh_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, char* M, struct heapMemoryNode * head);

void sw_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, char* M, struct heapMemoryNode * head);
// ! TO HERE
void slt_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs,char * M);

void slti_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs,char * M);

void sltu_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs,char * M);

void sltiu_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs,char * M);
// ! PROBABLY CHECK THIS, POINTER ARITHMETIC MAY BE WRONG
void beq_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC);

void bne_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC);

// ! Treat as signed
void blt_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC);

// ! Treat as unsigned
void bltu_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC);

void bge_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC);

void bgeu_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC);

void jal_OP(int32_t imm, int32_t rd, uint32_t * regs, int32_t * PC);

void jalr_OP(int32_t rd, int32_t rs1, int32_t imm ,uint32_t * regs, int32_t * PC);


// ? heapMemoryFunctions.c

unsigned char addBanks(struct heapMemoryNode** head, unsigned char pos);

void clearNode(struct heapMemoryNode * n);

void clearHeap(struct heapMemoryNode* heapBank);

uint32_t malloc_OP(uint32_t size, struct heapMemoryNode* virtualMemory);

int32_t free_OP(struct heapMemoryNode * toFree);

#endif 