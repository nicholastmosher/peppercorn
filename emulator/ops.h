#ifndef ops_h_INCLUDED
#define ops_h_INCLUDED

#include <stdlib.h>
#include <stdint.h>

#define OP_MOV 0
#define OP_MOVI 1
#define OP_ALU 2
#define OP_LD 3
#define OP_ST 4
#define OP_BRANCH 5

#define FUNC_UNUSED 0
#define FUNC_ALU_ADD 0
#define FUNC_ALU_SUB 1
#define FUNC_ALU_OR 2
#define FUNC_ALU_AND 3
#define FUNC_ALU_CMP 4
#define FUNC_BRANCH 0
#define FUNC_BEQ 1
#define FUNC_BZ 2

// Register IDs.
typedef uint8_t reg_index;
typedef uint8_t addr;

typedef struct _op {
	uint8_t rand2 : 4;
	uint8_t rand1 : 4;
	uint8_t func : 4;
	uint8_t op : 4;
} op;

typedef struct _op_immediate {
	uint8_t op : 4;
	uint8_t func : 4;
	uint8_t immediate; // Full 8 bits.
} op_im;

// Generator functions for reg_indexular ops.
op *create_mov(reg_index dest, reg_index src);
op *create_alu_or(reg_index dest, reg_index src);
op *create_alu_and(reg_index dest, reg_index src);
op *create_alu_add(reg_index dest, reg_index src);
op *create_alu_sub(reg_index dest, reg_index src);
op *create_cmp(reg_index dest, reg_index src);

// Generator functions for op_immediates.
op_im *create_mov_im(reg_index dest, addr address);
op_im *create_branch(addr address);
op_im *create_beq(addr address);
op_im *create_bz(addr address);
op_im *create_st(reg_index dest, addr address);
op_im *create_ld(reg_index src, addr address);

#endif // ops_h_INCLUDED

