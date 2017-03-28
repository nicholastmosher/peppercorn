/* Peppercorn assembler. */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* The current line being assembled. */
char pcl[256];
/* The binary file output. */
FILE *bf;

#define OPCODE(o) ((o & 0xF) << 4)
#define FUNC(f) ((f & 0xF) << 0)

#define PC_PANIC(...) fprintf(stderr, "%s -> ", pcl); \
                      fprintf(stderr, ##__VA_ARGS__); \
                      exit(EXIT_FAILURE);

/* OPS */
#define MOV_OP 0
#define MOVI_OP 1
#define ALU_OP 2
#define LD_OP 3
#define ST_OP 4
#define B_OP 5
/* ALU */
#define ADD_FUNC 0
#define SUB_FUNC 1
#define OR_FUNC 2
#define AND_FUNC 3
#define CMP_FUNC 4
/* BRANCH */
#define B_FUNC 0
#define BEQ_FUNC 1
#define BZ_FUNC 2

/* Returns the opcode and function for a given instruction mnemonic. */
uint8_t pc_get_of(char *i) {
    /* Return the opcode corresponding to each instruction mnemonic. */
    if (!strcmp(i, "mov")) {
        return OPCODE(MOV_OP);
    } else if (!strcmp(i, "movi")) {
        return OPCODE(MOVI_OP);
    } else if (!strcmp(i, "add")) {
        return OPCODE(ALU_OP) | FUNC(ADD_FUNC);
    } else if (!strcmp(i, "sub")) {
        return OPCODE(ALU_OP) | FUNC(SUB_FUNC);
    } else if (!strcmp(i, "or")) {
        return OPCODE(ALU_OP) | FUNC(OR_FUNC);
    } else if (!strcmp(i, "and")) {
        return OPCODE(ALU_OP) | FUNC(AND_FUNC);
    } else if (!strcmp(i, "cmp")) {
        return OPCODE(ALU_OP) | FUNC(CMP_FUNC);
    } else if (!strcmp(i, "ld")) {
        return OPCODE(LD_OP);
    } else if (!strcmp(i, "st")) {
        return OPCODE(ST_OP);
    }  else if (!strcmp(i, "b")) {
        return OPCODE(B_OP) | FUNC(B_FUNC);
    } else if (!strcmp(i, "beq")) {
        return OPCODE(B_OP) | FUNC(BEQ_FUNC);
    } else if (!strcmp(i, "bz")) {
        return OPCODE(B_OP) | FUNC(BZ_FUNC);
    }
    /* If we get here, an invalid instruction mnemonic was provided. */
    PC_PANIC("Invalid instruction '%s'.\n", i);
}

/* Returns the register file number for a given register mnemonic. */
uint8_t pc_get_reg(char *r) {

    /* The modifiable register string. */
    char *rs = r;
    /* The register number. */
    uint8_t rn = 0;

    /* Get rid of the $. */
    rs ++;

    /* See if the register is a special register. */
    if (!strcmp(rs, "pc")) {
        rn = 0x08;
        goto done;
    } else if (!strcmp(rs, "lr")) {
        rn = 0x09;
        goto done;
    } else if (!strcmp(rs, "sr")) {
        rn = 0x0a;
        goto done;
    } else if (!strcmp(rs, "sp")) {
        rn = 0x0b;
        goto done;
    }

    /* The first character is the register type. */
    char rtc = *rs ++;
    /* The next character is the register number. */
    char rnc = *rs;
    /* Ensure that the register is in the valid range. */
    if (rnc < '0' || rnc > '4') {
        PC_PANIC("Invalid register '%s'. Register numbers must be in range 0-3.\n", r);
    }
    /* Get the register number. */
    rn = rnc - 0x30;

    /* If the register is a temporary register, add 4 to the register number. */
    if (rtc == 'a') {
        rn += 0;
    } else if (rtc == 't') {
        rn += 4;
    } else {
        PC_PANIC("Invalid register type '%c'.", rtc);
    }

done:
    return rn;

error:
    PC_PANIC("Invalid register '%s'.\n", r);
}

uint8_t pc_get_imm(char *imm) {
    return (uint8_t)atoi(imm) & 0xFF;
}

int pc_assemble_line(char *line) {
    /* Set the current line. */
    strcpy(pcl, line);

    /* Break the instruction into its components. */
    char *ic[8];
    /* Index into the instruction components array. */
    int i = 0;
    /* Pointer to the subcomponent of the instruction. */
    char *sc;
    /* Iterate through the line and separate the subcomponents. */
    sc = strtok(line, " ");
    while (sc) {
        ic[i ++] = sc;
        sc = strtok(NULL, " \n");
    }

    /* Check if the line is a label. */
    if (line[0] == '_') {
        printf("label: %s\n", line);
        return 0;
    }

    /* Create the instruction */
    uint16_t instruction = 0;
    /* Get the opcode and func. */
    uint8_t of = pc_get_of(ic[0]);
    /* Extract the opcode. */
    uint8_t op = (of >> 4) & 0xF;
    /* Load the opcode into the instruction. */
    instruction |= (of & 0xFF) << 8;

    switch (op) {
        /* MOV */
        case MOV_OP:
        /* ADD, SUB, OR, AND, CMP */
        case ALU_OP:
            /* Load $rd. */
            instruction |= (pc_get_reg(ic[1]) & 0xF) << 4;
            /* Load $rs. */
            instruction |= pc_get_reg(ic[2]) & 0xF;
            break;
        /* MOVI */
        case MOVI_OP:
        /* LD */
        case LD_OP:
        /* ST */
        case ST_OP:
            instruction |= (pc_get_reg(ic[1]) & 0xF) << 8;
            instruction |= pc_get_imm(ic[2]) & 0xFF;
            break;
        /* B, BEQ, BZ */
        case B_OP:
            instruction |= pc_get_imm(ic[1]) & 0xFF;
            break;
        default:
            PC_PANIC("Invalid opcode '0x%02x'.\n", op);
            break;
    }

    /* Write the instruction to the file. */
    fputc(instruction & 0xFF, bf);
    fputc((instruction >> 8) & 0xFF, bf);

    return 0;
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Please provide a path to the assembly source file.\n");
        goto usage;
    } else if (argc < 3) {
        fprintf(stderr, "Please provide a path to the output binary file.\n");
        goto usage;
    }

    /* Load the assembly file. */
    char *sfp = argv[1];
    FILE *sf = fopen(sfp, "r");
    if (!sf) {
        fprintf(stderr, "Failed to load source file for compilation.\n");
        return 1;
    }

    /* Open the binary file for writing. */
    char *bfp = argv[2];
    bf = fopen(bfp, "w");
    if (!bf) {
        fprintf(stderr, "Failed to open binary file for writing.\n");
        return 1;
    }

    /* The line. */
    char *l;
    /* The line buffer. */
    char lb[256];

    /* Parse a line of assembly from the source file. */
    while ((l = fgets(lb, sizeof(lb), sf))) {
        /* Assemble the source file. */
        pc_assemble_line(l);
    }

    return 1;

usage:

    fprintf(stderr, "\npas [source.s] [output.bin]\n");

    return 1;
}
