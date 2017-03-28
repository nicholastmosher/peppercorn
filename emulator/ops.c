#include "ops.h"

op *create_mov(reg_index dest, reg_index src) {
	op *movop = malloc(sizeof(op));
	movop->op = OP_MOV;
	movop->func = FUNC_UNUSED;

	// Least-significant 4 bits of dest and src.
	movop->rand1 = dest & 0xF;
	movop->rand2 = src & 0xF;
	return movop;
}

op *create_alu_or(reg_index dest, reg_index src) {
	op *orop = malloc(sizeof(op));
	orop->op = OP_ALU;
	orop->func = FUNC_ALU_OR;

	// Least-significant 4 bits of dest and src.
	orop->rand1 = dest & 0xF;
	orop->rand2 = src & 0xF;
	return orop;
}

op *create_alu_and(reg_index dest, reg_index src) {
	op *andop = malloc(sizeof(op));
	andop->op = OP_ALU;
	andop->func = FUNC_ALU_AND;

	// Least-significant 4 bits of dest and src.
	andop->rand1 = dest & 0xF;
	andop->rand2 = src & 0xF;
	return andop;
}

op *create_alu_add(reg_index dest, reg_index src) {
	op *addop = malloc(sizeof(op));
	addop->op = OP_ALU;
	addop->func = FUNC_ALU_ADD;

	// Least-significant 4 bits of dest and src.
	addop->rand1 = dest & 0xF;
	addop->rand2 = src & 0xF;
	return addop;
}

op *create_alu_sub(reg_index dest, reg_index src) {
	op *subop = malloc(sizeof(op));
	subop->op = OP_ALU;
	subop->func = FUNC_ALU_SUB;

	// Least-significant 4 bits of dest and src.
	subop->rand1 = dest & 0xF;
	subop->rand2 = src & 0xF;
	return subop;
}

op *create_cmp(reg_index dest, reg_index src) {
	op *cmpop = malloc(sizeof(op));
	cmpop->op = OP_BRANCH;
	cmpop->func = FUNC_ALU_CMP;
	cmpop->rand1 = dest & 0xF;
	cmpop->rand2 = src & 0xF;
	return cmpop;
}

op_im *create_branch(addr address) {
	op_im *bop = malloc(sizeof(op_im));
	bop->op = OP_BRANCH;
	bop->func = FUNC_BRANCH;
	bop->immediate = address;
	return bop;
}

op_im *create_beq(addr address) {
	op_im *beqop = malloc(sizeof(op));
	beqop->op = OP_BRANCH;
	beqop->func = FUNC_BEQ;
	beqop->immediate = address;
	return beqop;
}

op_im *create_bz(addr address) {
	op_im *bzop = malloc(sizeof(op));
	bzop->op = OP_BRANCH;
	bzop->func = FUNC_BZ;
	bzop->immediate = address;
	return bzop;
}

op_im *create_st(reg_index dest, addr address) {
	op_im *opst = malloc(sizeof(op));
	opst->op = OP_ST;
	opst->func = dest & 0xF;
	opst->immediate = address;
	return opst;
}

op_im *create_ld(reg_index src, addr address) {
	op_im *opld = malloc(sizeof(op));
	opld->op = OP_LD;
	opld->func = src & 0xF;
	opld->immediate = address;
	return opld;
}
