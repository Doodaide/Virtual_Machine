#include <stdio.h> 
#include <stdlib.h>
#include <stdint.h>
#include "Header.h"

#define INST_MEM_SIZE 1024
#define DATA_MEM_SIZE 1024
#define VIRT_ROUT_MEM_SIZE 256
#define PC_Pos 32
#define counter 33

enum commands
{
    add = 1, addi, sub,
    lui, xor, xori, or,
    ori, and, andi, sll,
    srl, sra, lb, lh,
    lw, lbu, lhu, sb,
    sh, sw, slt, slti,
    sltu, sltiu, beq, bne, 
    blt, bltu, bge, bgeu, 
    jal, jalr,
};

enum VirtualRoutines
{
ConsoleWriteCharacter=0x0800, 
ConsoleWriteSignedInteger=0x0804,
ConsoleWriteUnsignedInteger=0x0808,
Halt=0x080c, 
ConsoleReadCharacter=0x0812,
ConsoleReadSignedInteger=0x0816,
DumpPC=0x0820,
DumpRegisterBanks=0x0824,
DumpMemoryWord=0x0828,
mallocHB=0x0830,
freeHB=0x0834,
HBStart = 0xb700
};

void registerDump(uint32_t * reg)
{
    printf("PC = 0x%08x;\n", *(reg + counter-1));
    for (size_t i = 0; i < counter-1; i++)
    {
        printf("R[%zu] = 0x%08x;\n", i, *(reg + i));
    }
    free(reg);
    exit(1);
}

void getErrored(int32_t * PC, uint32_t * reg, char* virtualMem, int32_t op, struct heapMemoryNode * head)
{
    if(op){
        printf("Instruction Not Implemented: 0x%02x%02x%02x%02x\n",
    ((*(virtualMem + (*(reg + counter - 1)+3)))&0xFF),
    ((*(virtualMem + (*(reg + counter - 1)+2)))&0xFF),
    ((*(virtualMem + (*(reg + counter - 1)+1)))&0xFF),
    (*(virtualMem + *(reg + counter - 1))&0xFF)
    );
    }
    else
    {
        printf("Illegal Operation: 0x%02x%02x%02x%02x\n",
    ((*(virtualMem + (*(reg + counter - 1)+3)))&0xFF),
    ((*(virtualMem + (*(reg + counter - 1)+2)))&0xFF),
    ((*(virtualMem + (*(reg + counter - 1)+1)))&0xFF),
    (*(virtualMem + *(reg + counter - 1))&0xFF)
    );
    }
    clearHeap(head);
    free(virtualMem);
    registerDump(reg);
}

struct heapMemoryNode * getHeapMem(uint32_t address, struct heapMemoryNode * head)
{
    /*
    address is an integer which refers to the "offset" from 
    0xb700. 
    address will be like 0xb980, which will be in a different 
    memory block. Hence, we just need to return the pointer 
    to the memory block start. 
    */
   uint32_t NodePos = (address - 0xb700)/64;
   struct heapMemoryNode * cursor = head;
   for (size_t i = 0; i < NodePos; i++)
   {
        cursor = getNextBank(cursor);
   }
   return cursor;
}

unsigned char getOffset(uint32_t address)
{
    return (address - 0xb700) % 64;
}

/*
reg :: register start address 
start :: Memory start address 
address :: Where to look in memory, memory offset
pos :: register offset
? Either returns 0 (no expected return value)

*/
void virtMemAccess(uint32_t * reg, char * start, int32_t address, int32_t pos, 
struct heapMemoryNode * head)
{
    /*
    look for virtual routine.
    */
    switch (address)
    {
    // Write a character to stdout
    case ConsoleWriteCharacter:
    {
        printf("Console Write Character Requested\n");
        printf("%c", *(reg + pos));
        return;
    }
    
    // Write a signed int to stdout
    case ConsoleWriteSignedInteger:
    {
        printf("Console Write Signed Integer Requested\n");
        printf("%d", *(reg + pos));
        return;
    }
    
    case ConsoleWriteUnsignedInteger:
    {
        printf("Console Write Unsigned Integer Requested\n");
        printf("%x", *(reg + pos));
        return;
    }

    case Halt:
    {
        printf("CPU Halt Requested\n");
        free(reg);
        free(start);
        clearHeap(head);
        exit(0);
        return;
    }

    case ConsoleReadCharacter:
    {
        printf("Read Character Requested\n");
        char c = getchar();
        reg[pos] = (uint32_t) (c & 0x000000FF);
        while (c != EOF && c != '\n'){
            c = getchar();
        }
        //scanf("%u", (reg + pos));
        
        return;
    }

    // Read 4 bytes of information in
    case ConsoleReadSignedInteger:
    {
        printf("Read Signed Integer requested\n");
        scanf("%d",(reg + pos));
        char c = getchar();
        while (c != EOF && c != '\n'){
            c = getchar();
        }
        return;
    }

    case DumpPC:
    {
        printf("Dump PC requested\n");
        printf("PC: %x", *(reg + PC_Pos));
        return;  
    }

    case DumpRegisterBanks:
    {
        printf("Dump Register Banks requested\n");
        registerDump(reg);
        return;
    }
    
    case DumpMemoryWord:
    {
        printf("Memory Word dump requested\n");
        printf("%x\n", *(start + *(reg + PC_Pos)));
        return;
    }

    case mallocHB:
    {
        printf("Memory malloc requested");
        *(reg + 28) = (malloc_OP(*(reg + pos), head));
        return;
    }
        

    case freeHB:
    {
        printf("Memory Free requested");
        if(free_OP(getHeapMem(*(reg + pos), head)))
        {
            getErrored((int32_t*)(reg + PC_Pos), reg, start, 0, head);
        }
        return;
    }
    }
    getErrored((int32_t *)(reg + PC_Pos), reg, start, 0, head);
    return;
    
}

int getCommand(char * instStart, uint32_t * registers, char * dataMem, int32_t * PC, struct heapMemoryNode * head)
{
    // * Implement Return number for each command (right now it's either 0 or 1 arbitrarily)
    uint32_t concatenatedInt = ((*instStart)     & 0x000000FF)| 
                      ((*(instStart + 1) << 8) &  0x0000FF00) | 
                      ((*(instStart + 2) << 16) & 0x00FF0000) | 
                      ((*(instStart + 3) << 24) & 0xFF000000);
    
    uint32_t res = getOpCode(concatenatedInt, PC, registers);
    int32_t rd;
    int32_t f3;
    int32_t rs1;
    int32_t rs2;
    int32_t f7;
    int32_t imm;
    int32_t * savedPC = PC;
    switch (res)
    {
    case 51: // R
    {
        rd = getRegister(concatenatedInt, PC, registers, dataMem, 7, head);
        f3 = getFunc3(concatenatedInt, PC, registers, dataMem, head);
        rs1 = getRegister(concatenatedInt, PC, registers, dataMem, 15, head);
        rs2 = getRegister(concatenatedInt, PC, registers, dataMem, 20, head);
        f7 = getFunc7(concatenatedInt, PC, registers, dataMem, head);
        switch (f3)
        {
        case 0:
            switch (f7)
            {   
            case 0:
                printf("Operation: add\nR[%d] = R[%d] + R[%d]\nR[%d] = %d + %d\n", rd, rs1, rs2, rd, registers[rs1], registers[rs2]);
                add_OP(rd, rs1, rs2, registers);
                break;
            case 32:
                printf("Operation: sub\nR[%d] = R[%d] - R[%d]\nR[%d] = %d - %d\n", rd, rs1, rs2, rd, registers[rs1], registers[rs2]);
                sub_OP(rd, rs1, rs2, registers);
                break;
            }
            break;

        case 1:
            printf("Operation: sll\nR[%d] = R[%d] << R[%d]\nR[%d] =%d << %d\n", rd, rs1, rs2, rd, registers[rs1],registers[rs2]);
            sll_OP(rd, rs1, rs2, registers);
            break;
        case 2:
            printf("Operation: slt\nR[%d] = (R[%d] < R[%d]) ? (1 : 0)\nrs1: %d\nrs2: %d\n", rd, rs1, rs2, registers[rs1],registers[rs2]);
            slt_OP(rd, rs1, rs2, registers, dataMem);
            break;  
        case 3:
            printf("Operation: sltu\nR[%d] = ((Unsigned)R[%d] < (Unsigned)R[%d]) ? (1 : 0)\nrs1: %d\nrs2: %d\n", rd, rs1, rs2, registers[rs1],registers[rs2]);
            sltu_OP(rd, rs1, rs2, registers, dataMem);
            break;    
        case 4:
            printf("Operation: xor\nR[%d] = R[%d] ^ R[%d]\nrs1: %d\nrs2: %d\n", rd, rs1, rs2, registers[rs1],registers[rs2]);
            xor_OP(rd, rs1, rs2, registers);
            break;
        case 5:
            switch (f7)
            {
            case 0:
                printf("Operation: srl\nR[%d] = R[%d] >> R[%d]\nrs1: %d\nrs2: %d\n", rd, rs1, rs2, registers[rs1], registers[rs2]);
                srl_OP(rd, rs1, rs2, registers);
                break;
            case 32:
                printf("Operation: sra\nR[%d] = R[%d] >> R[%d]\nrs1: %d\nrs2: %d\n", rd, rs1, rs2, registers[rs1], registers[rs2]);
                sra_OP(rd, rs1,rs2, registers);
                break;
            }  
            break;
        case 6:
            printf("Operation: or\nR[%d] = R[%d] | R[%d]\nrs1: %d\nrs2: %d\n", rd, rs1, rs2, registers[rs1], registers[rs2]);
            or_OP(rd, rs1, rs2, registers);
            break;
        case 7:
            printf("Operation: and\nR[%d] = R[%d] & R[%d]\nrs1: %d\nrs2: %d\n", rd, rs1, rs2, registers[rs1], registers[rs2]);
            and_OP(rd, rs1, rs2, registers);
            break;
        }
        break;
    }   
    
    case 19: // I
    {
        // Opcode 19
        rd = getRegister(concatenatedInt,  PC, registers, dataMem, 7, head);
        rs1 = getRegister(concatenatedInt, PC, registers, dataMem, 15, head);
        f3 = getFunc3(concatenatedInt, PC, registers, dataMem, head);
        imm = getImmI(concatenatedInt);
        switch (f3)
        {
        case 0:
            printf("Operation: addi\nR[%d] = R[%d] + %d\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            addi_OP(rd, rs1, imm, registers);
            break;

        case 2:
            printf("Operation: slti\nR[%d] = (R[%d] < %d) ? (1 : 0)\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            slti_OP(rd, rs1, imm, registers, dataMem);
            break;

        case 3:
            printf("Operation: sltiu\nR[%d] = ((Unsigned)R[%d] < (Unsigned)%d) ? (1 : 0)\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            sltiu_OP(rd, rs1, imm, registers, dataMem);
            break;     
        
        case 4:
            printf("Operation: xorii\nR[%d] = R[%d] ^ %d\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            xori_OP(rd, rs1, imm, registers);
            break;
            
        case 6:
            printf("Operation: ori\nR[%d] = R[%d] | %d\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            ori_OP(rd, rs1, imm, registers);
            break;
            
        case 7:
            printf("Operation: addi\nR[%d] = R[%d] & %d\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            andi_OP(rd, rs1, imm, registers);
            break;
            
        }
        break;
    }   
    case 3: // I
    {
        // Opcode 3
        rd = getRegister(concatenatedInt, PC, registers, dataMem, 7, head);
        rs1 = getRegister(concatenatedInt, PC, registers, dataMem, 15, head);
        imm = getImmI(concatenatedInt);
        f3 = getFunc3(concatenatedInt, PC, registers, dataMem, head);
        switch (f3)
        {
        case 0:
            printf("Operation: lb\nR[%d] = sext(R[%d] + %d)\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            lb_OP(rd, rs1, imm, registers, dataMem, head);
            break;

        case 1:
            printf("Operation: lh\nR[%d] = sext(R[%d] + %d)\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            lh_OP(rd, rs1, imm, registers, dataMem, head);
            break;

        case 2: 
            printf("Operation: lw\nR[%d] = R[%d] + %d\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            lw_OP(rd, rs1, imm, registers, dataMem, head);
            break;
 
        case 4:
            printf("Operation: lbu\nR[%d] = R[%d] + %d\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm, registers[rs1]);
            lbu_OP(rd, rs1, imm, registers, dataMem, head);
            break;

        case 5:
            printf("Operation: lhu\nR[%d] = R[%d] + %d\nimm: %d\n%x\nrs1: %d\n", rd, rs1, imm, imm, imm,registers[rs1]);
            lhu_OP(rd, rs1, imm, registers, dataMem, head);
            break;

        }
        break;
    }   
    case 103: // I
    {
        // Opcode 1100111
        
        rd = getRegister(concatenatedInt, PC, registers, dataMem, 7, head);
        rs1 = getRegister(concatenatedInt, PC, registers, dataMem, 15, head);
        imm = getImmI(concatenatedInt);
        printf("Operation: jalr\nR[%d] = PC(%d) + 4\nPC(%d) = R[%d] + %d\nimm: %d\n%x\nPC = %x\nrs1: %d\n", rd, *PC, *PC, rs1, imm, imm, imm, *PC, registers[rs1]);
        jalr_OP(rd, rs1, imm, registers, PC);
        return jalr;
    }     
    
    case 35: // S
    {
        //printf("Case S\n");
        f3 = getFunc3(concatenatedInt, PC, registers, dataMem, head);
        rs1 = getRegister(concatenatedInt, PC, registers, dataMem, 15, head);
        rs2 = getRegister(concatenatedInt, PC, registers, dataMem, 20, head);
        imm = getImmS(concatenatedInt);

        switch (f3)
        {
        case 0:
            printf("Operation: sb\nM[R[%d] + %d] = R[%d]\nimm: %d\n%x\nrs1: %d\nrs2: %d\n", rs1, imm, rs2, imm,imm, registers[rs1], registers[rs2]);
            sb_OP(imm, rs1, rs2, registers, dataMem, head);
            break;

        
        case 1:
            printf("Operation: sh\nM[R[%d] + %d] = R[%d]\nimm: %d\n%x\nrs1: %d\nrs2: %d\n", rs1, imm, rs2, imm,imm, registers[rs1], registers[rs2]);
            sh_OP(imm, rs1, rs2, registers, dataMem, head);
            break;

        case 2:
            printf("Operation: sw\nM[R[%d] + %d] = R[%d]\nimm: %d\n%x\nrs1: %d\nrs2: %d\n", rs1, imm, rs2, imm,imm, registers[rs1], registers[rs2]);
            sw_OP(imm, rs1, rs2, registers, dataMem, head);
            break;

        }
        break;
    }   
    
    case 99: // SB
    {
        f3 = getFunc3(concatenatedInt, PC, registers, dataMem, head);
        rs1 = getRegister(concatenatedInt, PC, registers, dataMem, 15, head);
        rs2 = getRegister(concatenatedInt, PC, registers, dataMem, 20, head);
        imm = getImmSB(concatenatedInt); 
        switch (f3)
        {
        case 0:
            printf("Operation: beq\nif(R[%d] == R[%d]) then PC(%d) = PC(%d) + (%d << 1)\nPC = %x\nrs1: %d\nrs2: %d\n", rs1, rs2, *PC, *PC, imm, *PC, registers[rs1], registers[rs2]);
            beq_OP(imm, rs1, rs2, registers, PC);
            break;
            
        case 1:
            printf("Operation: bne\nif(R[%d] != R[%d]) then PC(%d) = PC(%d) + (%d << 1)\nPC = %x\nrs1: %d\nrs2: %d\n", rs1, rs2, *PC, *PC, imm, *PC, registers[rs1], registers[rs2]);
            bne_OP(imm, rs1, rs2, registers, PC);
            break;

        case 4:
            printf("Operation: blt\nif(R[%d] < R[%d]) then PC(%d) = PC(%d) + (%d << 1)\nPC = %x\nrs1: %d\nrs2: %d\n", rs1, rs2, *PC, *PC, imm, *PC, registers[rs1], registers[rs2]);
            blt_OP(imm, rs1, rs2, registers, PC);
            break;

        case 6:
            printf("Operation: bltu\nif((Unsigned)R[%d] < (Unsigned)R[%d]) then PC(%d) = PC(%d) + (%d << 1)\nPC = %xrs1: %d\nrs2: %d\n", rs1, rs2, *PC, *PC, imm, *PC, registers[rs1], registers[rs2]);
            bltu_OP(imm, rs1, rs2, registers, PC);
            break;

        case 5:
            printf("Operation: bge\nif(R[%d] >= R[%d]) then PC(%d) = PC(%d) + (%d << 1)\nPC = %x\nrs1: %d\nrs2: %d\n", rs1, rs2, *PC, *PC, imm,*PC, registers[rs1], registers[rs2]);
            bge_OP(imm, rs1, rs2, registers, PC);
            break;

        case 7:
            printf("Operation: bgeu\nif(R[%d] >= R[%d]) then PC(%d) = PC(%d) + (%d << 1)\nPC = %x\nrs1: %d\nrs2: %d\n", rs1, rs2, *PC, *PC, imm, *PC, registers[rs1], registers[rs2]);
            bgeu_OP(imm, rs1, rs2, registers, PC);
            break;

        }
        break;
    }
    
    case 55: // U
    {
       
        rd = getRegister(concatenatedInt, PC, registers, dataMem, 7, head);
        imm = getImmU(concatenatedInt);
        printf("Operation: lui\nR[%d] = {31:12 = %x | 11:0 = 0}\nimm: %d\n%x\nrd: %d\n", rd, imm, imm, imm, registers[rd]);
        lui_OP(rd, imm, registers);
        return lui;
    }   
    
    case 111: // UJ
    {
        rd = getRegister(concatenatedInt, PC, registers, dataMem, 7, head);
        imm = getImmUJ(concatenatedInt);
        printf("Operation: jal\nR[%d] = PC(%d) + 4\nPC(%d) = PC(%d) + (%d << 1)\nPC = %x\nrd: %d\n", rd, *PC, *PC, *PC,imm, *PC, registers[rd]);
        jal_OP(imm, rd, registers, PC);
        break;
    }      
    default:
    {
        getErrored(savedPC, registers, dataMem, 1, head);
        break;
    }
    }
    return res;
}

int main(int argc, char const *argv[])
{
    FILE * fIn;
    fIn = fopen(argv[1], "rb");
    if(fIn == NULL)
    {
        perror("File not opened properly");
        return 1;
    }
    /*
    Heap Banks
    * Linked structure, 128 Nodes, that contain a bank attribute, with a link
    * to the next bank
    */
    struct heapMemoryNode * head = startBank();
    if(head == NULL)
    {
        fclose(fIn);
        return 1;
    }

    for (size_t i = 1; i < 128; i++)
    {
        if(addBanks(&head, i))
        {
            free(head);
            fclose(fIn);
            return 1;
        }
    }
    /*
    * VM memory, 
    ? Instruction memory - 1024 bytes (0 - 1023)
    ? Data memory - 1024 bytes (1024 - 2047)
    ? Virtual routines - 256 bytes (2048 - 2303)
    */
    char * virtualMemory = calloc(INST_MEM_SIZE + DATA_MEM_SIZE + VIRT_ROUT_MEM_SIZE, sizeof(char));
   
    // ! 32 registers + PC + counter to end it, all assigned to 0
    uint32_t * reg = (uint32_t * ) calloc(counter, sizeof(uint32_t));
    if(reg == NULL || virtualMemory == NULL)
    {
        perror("Memory Allocation Issues\n");
        clearHeap(head);
        return 1;
    }
    
    int32_t * pCounter = (int32_t *) (reg + 32);
    *pCounter = 0;
    
    fread(virtualMemory, sizeof(char) * (INST_MEM_SIZE + DATA_MEM_SIZE), 1, fIn);
    fclose(fIn);
    
    while(*pCounter != 0x080C)
    {
        printf("\n-------------------------------\n");
        printf("Instruction Start: \n");
        printf("PC(hex): %x\nPC(dec): %d\n", *pCounter, *pCounter);
        int32_t tempPC = *pCounter;
        *reg = 0;
        getCommand(virtualMemory + (*pCounter), reg, virtualMemory, pCounter, head);
        if(*pCounter == tempPC)
        {
            *pCounter += 4;
        }
        //* printf("\n-------------------------------\n");
        //* printf("\n--------------------------------\n");
        *reg = 0; // Ignore all writes to R0
    }
    free(reg);
    clearHeap(head);
    
    return 0; 
}