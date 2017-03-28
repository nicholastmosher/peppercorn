#include <stdio.h>
#include "peppercorn.h"

int main(void) {

	log_info("create_runtime()");
	runtime *rt = create_runtime();
	FILE *binary = fopen("main.bin", "rb");
	if(!binary) { fprintf(stderr, "Failed to open file."); return 1; }
	size_t size;
	fseek(binary, 0, SEEK_END);
	size = ftell(binary);
	fseek(binary, 0, SEEK_SET);
	fread(rt->prg_base, sizeof(char), size, binary);

	log_info("prg_base: %p", rt->prg_base);
	log_info("Beginning binary execution.");
	op *opcode;
	while (1) {
		opcode = (op *) rt->prg_base + (rt->pc - 0x400);
		rt->pc++;
		log_info("executing opcode at %p", opcode);
		apply_opcode(rt, opcode);
	}

	return 0;
}
