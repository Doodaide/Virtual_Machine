#include <stdint.h>
#include "Header.h"

int32_t signExtend(uint32_t i, uint32_t len, uint32_t pos, uint32_t neg)
{
    int32_t extended = i;
    uint32_t sBit = (i >> (len - 1)) & 1;
    if (sBit)
    {
        return extended | neg;
    }
    return extended & pos;
}

uint32_t getOpCode(uint32_t opCode, int32_t * PC, uint32_t * reg)
{
    return opCode & 0x7F;
}

void checkWithBounds(int32_t toTest, int32_t * PC, uint32_t * reg, char* virtMem, uint32_t upper, uint32_t lower
, struct heapMemoryNode * head)
{
    if(toTest > upper || toTest < lower)
    {
        getErrored(PC, reg, virtMem, 1, head);
    }
}

int32_t getRegister(uint32_t instStart, int32_t * PC, uint32_t * reg, char* virtMem, unsigned char start
, struct heapMemoryNode * head)
{
    int8_t rd = (instStart >> start) & 0x1F;
    checkWithBounds(rd, PC, reg, virtMem, 32, 0, head);
    return rd;
}

int32_t getFunc3(uint32_t instStart, int32_t * PC, uint32_t * reg, char * virtMem
, struct heapMemoryNode * head)
{
    int8_t func3Start = (instStart >> 12) & 0x7;
    checkWithBounds(func3Start, PC, reg, virtMem, 7, 0, head);    
    return func3Start;
}

int32_t getFunc7(uint32_t instStart, int32_t * PC, uint32_t * reg, char* virtMem
, struct heapMemoryNode * head)
{
    int8_t func7 = (instStart >> 25) & 0x7F;
    if(func7 != 3 && func7 != 32 && func7 != 0)
    {
        getErrored(PC, reg, virtMem, 1, head);
    }
    return func7;
}

int32_t getImmI(uint32_t instStart)
{
    int32_t imm = (instStart >> 20) & 0xFFF;
    return signExtend(imm, 12, 0x00000FFF, 0xFFFFF000);
}

int32_t getImmU(uint32_t instStart)
{
    int32_t toRet = (((instStart >> 12) & 0xFFFFF) << 12) & 0xFFFFF000;
    return toRet;
}

int32_t getImmS(uint32_t instStart)
{
    int32_t ret = ((((instStart >> 25) & 0x7F) << 5) & 0b111111100000) |
                 ((instStart >> 7) & 0x1F);
    return signExtend(ret,12, 0x00000FFF, 0xFFFFF000);
}

int32_t getImmSB(uint32_t instStart)
{
    uint32_t start = (((instStart >> 8) & 0xF)  | 
    ((((instStart >> 25) & 0x3F) << 4) & 0x3F0)|
    ((((instStart >> 7) & 0x1) << 10) & 0x400) | 
    ((((instStart >> 31) & 0x1) <<11) & 0x800));
    int32_t extended = (start << 1) & 0x00001FFE; // Extends to 32 bits
    return signExtend(extended, 13, 0x00000FFF,0xFFFFF000);
}

int32_t getImmUJ(uint32_t instStart)
{
    int32_t start = (((instStart) >> 21) & 0x3FF) | 
                    ((((instStart >> 20) & 0x1) << 10) & 0x400) | 
                    ((((instStart >> 12) & 0xFF) << 11) & 0x7F800) | 
                    ((((instStart >> 31) & 0x1) << 19) & 0x80000);
    int32_t extended = start & 0xFFFFF; // Extends to 32 bits
    return signExtend(extended, 20, 0xFFFFF, 0xFFFF0000);
}