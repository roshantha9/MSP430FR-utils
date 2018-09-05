/*
 * checkpoint.c
 *
 *  Created on: Sep 4, 2018
 *      Author: Rosh
 */

#include "checkpoint.h"


//8K SRAM
#pragma NOINIT(backup_sram)
unsigned char backup_sram[8192];
//16 16bit register in CPU
#pragma NOINIT(backup_register)
#pragma location = 0x4400
unsigned char backup_register[32];


void Checkpoint_backup(){
    //backup all sram
    memcpy(backup_sram, ((void*) 0x1C00),   CHECKPOINT_SRAM_SIZE);
    //backup all registers
    asm(" MOV R1, &4402h");
    asm(" MOV R2, &4404h");
    asm(" MOV R3, &4406h");
    asm(" MOV R4, &4408h");
    asm(" MOV R5, &440ah");
    asm(" MOV R6, &440ch");
    asm(" MOV R7, &440eh");
    asm(" MOV R8, &4410h");
    asm(" MOV R9, &4412h");
    asm(" MOV R10, &4414h");
    asm(" MOV R11, &4416h");
    asm(" MOV R12, &4418h");
    asm(" MOV R13, &441ah");
    asm(" MOV R14, &441ch");
    asm(" MOV R15, &441eh");
    asm(" MOV R0, &4400eh"); // The stored PC will be at the end of this function
}


void Checkpoint_restore(){
    //restore all registers
    asm(" MOV &4402h, R1");
    asm(" MOV &4404h, R2");
    asm(" MOV &4406h, R3");
    asm(" MOV &4408h, R4");
    asm(" MOV &440ah, R5");
    asm(" MOV &440ch, R6");
    asm(" MOV &440eh, R7");
    asm(" MOV &4410h, R8");
    asm(" MOV &4412h, R9");
    asm(" MOV &4414h, R10");
    asm(" MOV &4416h, R11");
    asm(" MOV &4418h, R12");
    asm(" MOV &441ah, R13");
    asm(" MOV &441ch, R14");
    asm(" MOV &441eh, R15");
    //restore
    memcpy(((void*) 0x1C00), backup_sram,   0x2000);

}



