#include <stdint.h>
#include "Header.h"
#define PC_Pos 32

void add_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs)   // * 1 Done
{
    *(regs + rd) = *(regs + rs1) + *(regs + rs2);
     
}

void addi_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs)  // * 2 Done
{
   *(regs + rd) = *(regs + rs1) + imm;
    
}

void sub_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs)   // * 3 Done
{
    *(regs + rd) = *(regs + rs1) - *(regs + rs2);
     
}

void lui_OP(int32_t rd, int32_t imm, uint32_t * regs)                // * 4 Done
{
    *(regs + rd) = (((imm >> 12) & 0xFFFFF)<<12) & 0xFFFFF000;
     
    
}

void xor_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs)   // * 5 Done
{
    *(regs + rd) = *(regs + rs1) ^ *(regs + rs2);
     
}

void xori_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs)  // * 6 Done
{
    *(regs + rd) = *(regs + rs1) ^ imm;
     
}

void or_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs)    // * 7 Done
{
    *(regs + rd) = *(regs + rs1) | *(regs + rs2);
}

void ori_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs)   // * 8 Done
{
    *(regs + rd) = *(regs + rs1) | imm;
     
}

void and_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs)   // * 9 Done
{
    *(regs + rd) = *(regs + rs1) & *(regs + rs2);
     
}

void andi_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs)  // *10 Done
{
    *(regs + rd) = *(regs+ rs1) & imm;
     
}

void sll_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs)   // *11 Done
{
    *(regs + rd) = *(regs + rs1) << *(regs + rs2);
     
}

void srl_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs)   // *12 Done
{
    *(regs + rd) = *(regs + rs1) >> *(regs + rs2);
     
}

// ! FiX bIt shifting
void sra_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs)   // !13 Probably Done
{
    // 32 bits in a full word. 'or' the truncated bits.
    regs[rd] = (regs[rs1] >> regs[rs2]) | (regs[rs1] << (32 - regs[rs2])); 
     
}

// ! NEED TO CHECK MEMORY ACCESS FROM HERE
void lb_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs, char* M, struct heapMemoryNode * head) // *14
{
    
    if(regs[rs1] + imm >= 0x0800 && regs[rs1] + imm < 0x0850)
    {
        virtMemAccess(regs, M, regs[rs1] + imm, rd, head);
        regs[rd] = signExtend(regs[rd], 8, 0x000000FF, 0xFFFFFF00);
        return;
    }
    else if(regs[rs1] + imm >= 0xb700)
    {
        struct heapMemoryNode * start = getHeapMem(regs[rs1] + imm, head);
        if(!start->allocated)
        {
            getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
        }
        regs[rd] = signExtend((*(start-> contents  + getOffset(regs[rs1] + imm))& 0x000000FF)
        , 8, 0x000000FF, 0xFFFFFF00);
        return;
    }
    else if(regs[rs1] + imm < 0x0800 && regs[rs1] + imm >= 0)
    {
        regs[rd] = signExtend(M[regs[rs1] + imm] & 0x000000FF, 8, 0x000000FF, 0xFFFFFF00);
        return;
    }
    getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
}

void lh_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs, char* M, struct heapMemoryNode * head) // *15
{
    if(regs[rs1] + imm >= 0x0800 && regs[rs1] + imm < 0x0850)
    {
        virtMemAccess(regs, M, regs[rs1] + imm, rd, head);
        regs[rd] = signExtend(regs[rd], 16, 0x0000FFFF, 0xFFFF0000);
        return;
    }
    else if(regs[rs1] + imm >= 0xb700 && regs[rs1] + imm < 0xD700-1)
    {
        //printf("Here3: Address = %x\n", regs[rs1] + imm);
        struct heapMemoryNode * start = getHeapMem(regs[rs1] + imm, head);
        if(!start->allocated)
        {
            getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
        }
        regs[rd] = signExtend((*(start-> contents + getOffset(regs[rs1] + imm))& 0x000000FF) | 
                               (*(start->contents + 1 + getOffset(regs[rs1] + imm)) << 8 & 0x0000FF00), 16, 0x0000FFFF, 0xFFFF0000);
        return;
    }
    else if(regs[rs1] + imm < 0x0800 && regs[rs1] + imm >= 0)
    {
        int32_t temp1 = (M[regs[rs1] + imm] & 0x000000FF); // 16 bit smashed into 32 bit, without sign extension
        int32_t temp2 = (M[regs[rs1] + imm + 1] << 8 & 0x0000FF00);
        regs[rd] = signExtend(temp1|temp2, 16, 0x0000FFFF, 0xFFFF0000);
        return;
    }
    getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
}

void lw_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs, char* M, struct heapMemoryNode * head) // *16
{
    
    if(regs[rs1] + imm >= 0x0800 && regs[rs1] + imm < 0x0850)
    {
        virtMemAccess(regs, M, regs[rs1] + imm, rd, head);
        return;
    }
    else if(regs[rs1] + imm >= 0xb700 && regs[rs1] + imm < 0xD700-2)
    {
        //printf("Here4: Address = %x\n", regs[rs1] + imm);
        struct heapMemoryNode * start = getHeapMem(regs[rs1] + imm, head);
        if(!start->allocated)
        {
            getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
        }
        
        regs[rd] = (*(start->contents + getOffset(regs[rs1] + imm)) & 0x000000FF) | 
                   (*(start->contents + 1 + getOffset(regs[rs1] + imm)) << 8 & 0x0000FF00)|
                   (*(start->contents + 2 + getOffset(regs[rs1] + imm)) << 16 & 0x00FF0000)|
                   (*(start->contents + 3 + getOffset(regs[rs1] + imm)) << 24 & 0xFF000000);
        return;
    }
    else if(regs[rs1] + imm < 0x0800 && regs[rs1] + imm >= 0)
    {
        regs[rd] = (M[regs[rs1] + imm] & 0x000000FF) |
           ((M[regs[rs1] + imm + 1] << 8)  & 0x0000FF00) | 
           ((M[regs[rs1] + imm + 2] << 16) & 0x00FF0000) | 
           ((M[regs[rs1] + imm + 3] << 24) & 0xFF000000); // 16 bit smashed into 32 bit, without sign extension
        return;
    }
    getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
}

// ! CHECK WHAT IT MEANS BY UNSIGNED
void lbu_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs,char * M, struct heapMemoryNode * head) // *17
{
    
    if(regs[rs1] + imm >= 0x0800 && regs[rs1] + imm < 0x0850)
    {
        virtMemAccess(regs, M, regs[rs1] + imm, rd, head);
        return;
    }
    else if(regs[rs1] + imm >= 0xb700)
    {
        //printf("Here5: Address = %x\n", regs[rs1] + imm);
        struct heapMemoryNode * start = getHeapMem(regs[rs1] + imm, head);
        if(!start->allocated)
        {
            getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
        }
        regs[rd] = *(start->contents + getOffset(regs[rs1] + imm))& 0x000000FF;
        return;
    }
    else if(regs[rs1] + imm < 0x0800 && regs[rs1] + imm >= 0)
    {
        regs[rd] = M[regs[rs1] + imm];
        return;
    }
    getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
}

void lhu_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs,char * M, struct heapMemoryNode * head) // *18
{
    uint32_t temp;
    if(regs[rs1] + imm >= 0x0800 && regs[rs1] + imm < 0x0850)
    {
        virtMemAccess(regs, M, regs[rs1] + imm, rd, head);
        return;
    }
    else if(regs[rs1] + imm >= 0xb700 && regs[rs1] + imm < 0xD700-1)
    {
        //printf("Here6: Address = %x\n", regs[rs1] + imm);
        struct heapMemoryNode * start = getHeapMem(regs[rs1] + imm, head);
        if(!start->allocated)
        {
            getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
        }
        regs[rd] = (*(start-> contents + getOffset(regs[rs1] + imm))& 0x000000FF) | 
                               (*(start->contents + 1 + getOffset(regs[rs1] + imm)) << 8 & 0x0000FF00);
        return;
    }
    else if(regs[rs1] + imm < 0x0800 && regs[rs1] + imm >= 0)
    {   // 16 bit smashed into 32 bit, without sign extension
        temp = (M[regs[rs1] + imm] & 0x000000FF) | 
               (M[regs[rs1] + imm + 1] << 8 & 0x0000FF00);
        regs[rd] = temp & 0x0000FFFF;
        return;
    }
    getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
}

void sb_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, char* M, struct heapMemoryNode * head)
{   
    if(regs[rs1] + imm >= 0x0800 && regs[rs1] + imm < 0x0850)
    {
        //printf("RS1 + IMM Inside: %d: %x\n", rs1+imm, rs1+imm);
        virtMemAccess(regs, M, regs[rs1] + imm, rs2, head);
        return;
    }
    else if(regs[rs1] + imm >= 0xb700)
    {
        //printf("Here7: Address = %x\n", regs[rs1] + imm);
        struct heapMemoryNode * start = getHeapMem(regs[rs1] + imm, head);
        if(!start->allocated)
        {
            getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
        }
        *(start->contents + getOffset(regs[rs1] + imm)) = regs[rs2] & 0xFF;
        return;
    }
    else if(regs[rs1] + imm < 0x0800 && regs[rs1] + imm >= 0)
    {
        M[regs[rs1] + imm] = regs[rs2] & 0xFF;
        return;
    }
    getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
}

void sh_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, char* M, struct heapMemoryNode * head)
{
    if(regs[rs1] + imm >= 0x0800 && regs[rs1] + imm < 0x0850)
    {
        virtMemAccess(regs, M, regs[rs1] + imm, rs2, head);
        return;
    }
    else if(regs[rs1] + imm >= 0xb700 && regs[rs1] + imm < 0xD700-1)
    {
        //printf("Here8: Address = %x\n", regs[rs1] + imm);
        struct heapMemoryNode * start = getHeapMem(regs[rs1] + imm, head);
        if(!start->allocated)
        {
            getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
        }
        *(start->contents + getOffset(regs[rs1] + imm)) = regs[rs2] & 0xFF;
        *(start->contents + 1 + getOffset(regs[rs1] + imm)) = (regs[rs2]>>8) & 0xFF;
        return;
    }
    else if(regs[rs1] + imm < 0x0800 && regs[rs1] + imm >= 0)
    {
        M[regs[rs1] + imm] = regs[rs2] & 0xFF;
        M[regs[rs1] + imm + 1] = (regs[rs2]>>8) & 0xFF;
        return;
    }
    getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head); 
}

void sw_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, char* M, struct heapMemoryNode * head)
{
    if(regs[rs1] + imm >= 0x0800 && regs[rs1] + imm < 0x0850)
    {
        virtMemAccess(regs, M, regs[rs1] + imm, rs2, head);
        return;
    }
    else if(regs[rs1] + imm >= 0xb700 && regs[rs1] + imm < 0xD700-2)
    {
        //printf("Here9: Address = %x\n", regs[rs1] + imm);
        struct heapMemoryNode * start = getHeapMem(regs[rs1] + imm, head);
        if(!start->allocated)
        {
            getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
        }
        *(start->contents + getOffset(regs[rs1] + imm)) = regs[rs2] & 0xFF;
        *(start->contents + 1 + getOffset(regs[rs1] + imm)) = (regs[rs2]>>8) & 0xFF;
        *(start->contents + 2 + getOffset(regs[rs1] + imm)) = (regs[rs2]>>16) & 0xFF;
        *(start->contents + 3 + getOffset(regs[rs1] + imm)) = (regs[rs2]>>24) & 0xFF;
        return;
    }
    else if(regs[rs1] + imm < 0x0800 && regs[rs1] + imm >= 0)
    {
        M[regs[rs1] + imm + 0] = regs[rs2] & 0xFF;
        M[regs[rs1] + imm + 1] = (regs[rs2]>>8) & 0xFF;
        M[regs[rs1] + imm + 2] = (regs[rs2]>>16) & 0xFF;
        M[regs[rs1] + imm + 3] = (regs[rs2]>>24) & 0xFF;
        return;
    }
    getErrored((int32_t *)(regs + PC_Pos), regs, M, 0, head);
     
}
// ! TO HERE

void slt_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs,char * M)
{
    regs[rd] = (regs[rs1] < regs[rs2]) ? 1 : 0;
     
}

void slti_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs,char * M)
{
    regs[rd] = (regs[rs1 < imm]) ? 1 : 0;
     
}

void sltu_OP(int32_t rd, int32_t rs1, int32_t rs2, uint32_t * regs,char * M)
{
    regs[rd] = (((uint32_t) regs[rs1]) < ((uint32_t) regs[rs2])) ? 1: 0;
}

void sltiu_OP(int32_t rd, int32_t rs1, int32_t imm, uint32_t * regs,char * M)
{
    regs[rd] = (((uint32_t) regs[rs1]) < ((uint32_t) imm)) ? 1: 0;
}
// ! PROBABLY CHECK THIS, POINTER ARITHMETIC MAY BE WRONG
void beq_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC)
{
    if(regs[rs1] == regs[rs2])
    {
        *PC = *PC + (imm);
    }
}

void bne_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC)
{
    // ! Think about cast
    if(regs[rs1] != regs[rs2])
    {
        *PC = *PC + (imm);
    }
}

// ! Treat as signed
void blt_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC)
{
    if((uint32_t)regs[rs1] < (uint32_t)regs[rs2]){
        *PC = *PC + (imm);
    }
}

// ! Treat as unsigned
void bltu_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC)
{
    if(((uint32_t) regs[rs1]) < ((uint32_t) regs[rs2])){
        *PC = *PC + (imm);
    }
}

void bge_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC)
{
    if((uint32_t)regs[rs1] >= (uint32_t)regs[rs2]){
        *PC = *PC + (imm);
    }
}

void bgeu_OP(int32_t imm, int32_t rs1, int32_t rs2, uint32_t * regs, int32_t * PC)
{
    if((uint32_t)regs[rs1] >= (uint32_t)regs[rs2]){
        *PC = *PC + (imm);
    }
}

void jal_OP(int32_t imm, int32_t rd, uint32_t * regs, int32_t * PC)
{
    *(regs + rd) = *(PC) + 4;
    *PC = *(PC) + (imm<<1);
     
}

void jalr_OP(int32_t rd, int32_t rs1, int32_t imm ,uint32_t * regs, int32_t * PC)
{
    regs[rd] = *(PC) + 4;
    *PC = regs[rs1] + imm;
}