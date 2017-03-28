#ifndef runtime_h_INCLUDED
#define runtime_h_INCLUDED

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "dbg.h"
#include "ops.h"

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define T0 4
#define T1 5
#define T2 6
#define T3 7
#define PC 8
#define LR 9
#define SR 10

// Status register masks.
#define SR_ZERO 0
#define SR_EQ 1

// Given a runtime and register index, access that register.
#define RTREG(RT, REG) (((reg *) RT) [REG])

// Increment the program counter of the given runtime.
#define INCPC(RT) (RTREG(RT, PC)++);

// Compare the given register for a zero and set status register flag.
#define CMPZ(RT, REG) if(!RTREG(RT, REG)) { RTREG(RT, SR) |= (1 << SR_ZERO); }

// Check the EQ flag of the status register.
#define CMPEQ(RT) ((RTREG(RT, SR) & (1 << SR_EQ)) ? 1 : 0)

// Compare the given registers in the given runtime.
#define CMPREG(RT, R1, R2) (RTREG(RT, R1) == RTREG(RT, R2))

typedef uint16_t reg;

typedef struct _runtime {
	reg a0;
	reg a1;
	reg a2;
	reg a3;
	reg t0;
	reg t1;
	reg t2;
	reg t3;
	reg pc;
	reg lr;
	reg sr;
	reg sp;
	uint16_t *ram_base;
	uint16_t *prg_base;
} runtime;

runtime *create_runtime();
void apply_opcode(runtime *rt, op *opcode);

#endif // runtime_h_INCLUDED

