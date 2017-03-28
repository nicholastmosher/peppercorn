#include "runtime.h"

runtime *create_runtime() {
	runtime *rt = malloc(sizeof(runtime));
	rt->a0 = 0;
	rt->a1 = 0;
	rt->a2 = 0;
	rt->a3 = 0;
	rt->t0 = 0;
	rt->t1 = 0;
	rt->t2 = 0;
	rt->t3 = 0;
	rt->pc = 0x400;
	rt->lr = 0;
	rt->sr = 0;
	rt->ram_base = malloc(1024);
	rt->prg_base = malloc(1024);
	rt->sp = (uint16_t) rt->ram_base + 1024;
	return rt;
}

void apply_opcode(runtime *rt, op *opc) {

	log_info("apply_opcode: op %d, func %d, r1 %d, r2 %d.", opc->op, opc->func, opc->rand1, opc->rand2);

	// Temp pointer for handling op_ims.
	op_im *opi;

	// Determine opcode type and apply the correct operation.
	switch (opc->op) {
		// Apply MOV op.
		case OP_MOV:
			log_info("op_mov");
			RTREG(rt, opc->rand1) = RTREG(rt, opc->rand2);
			break;
		case OP_MOVI:
			log_info("op_movi");
			opi = (op_im *) opc;
			RTREG(rt, opi->func) = opi->immediate;
			break;
		case OP_ALU:
			log_info("op_alu");
			switch(opc->func) {
				case FUNC_ALU_OR:
					RTREG(rt, opc->rand1) |= RTREG(rt, opc->rand2);
					CMPZ(rt, opc->rand1);
					break;
				case FUNC_ALU_AND:
					RTREG(rt, opc->rand1) &= RTREG(rt, opc->rand2);
					CMPZ(rt, opc->rand1);
					break;
				case FUNC_ALU_ADD:
					RTREG(rt, opc->rand1) += RTREG(rt, opc->rand2);
					CMPZ(rt, opc->rand1);
					break;
				case FUNC_ALU_SUB:
					RTREG(rt, opc->rand1) -= RTREG(rt, opc->rand2);
					CMPZ(rt, opc->rand1);
					break;
				case FUNC_ALU_CMP:
					if (CMPREG(rt, opc->rand1, opc->rand2))
						RTREG(rt, SR) |= (1 << SR_EQ);
					else
						RTREG(rt, SR) &= ~(1 << SR_EQ);
					break;
				default:
					fprintf(stderr, "Func %d invalid for ALU.\n", opc->func);
					exit(1);
			}
			break;
		case OP_BRANCH:
			log_info("op_branch");
			opi = (op_im *) opc;
			switch(opc->func) {
				case FUNC_BRANCH:
					RTREG(rt, PC) = opi->immediate;
					break;
				case FUNC_BEQ:
					if (CMPEQ(rt)) RTREG(rt, PC) = opi->immediate;
					break;
				case FUNC_BZ:
					if (RTREG(rt, SR) & (1 << 0)) RTREG(rt, PC) = opi->immediate;
					break;
				default:
					fprintf(stderr, "Func %d invalid for BRANCH.\n", opc->func);
					exit(1);
			}
			break;
		case OP_ST:
			log_info("op_st");
			opi = (op_im *) opc;
			rt->ram_base [RTREG(rt, opi->immediate)] = RTREG(rt, opi->func);
			break;
		case OP_LD:
			log_info("op_ld");
			opi = (op_im *) opc;
			RTREG(rt, opi->func) = rt->ram_base [RTREG(rt, opi->immediate)];
			break;
		default:
			fprintf(stderr, "Unrecognized opcode %d.\n", opc->op);
			exit(1);
	}
}
