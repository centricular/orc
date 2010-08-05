
#include "config.h"

#include <orc/orc.h>
#include <orc/orcparse.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void output_code_emulate (OrcProgram *p, FILE *output);

int verbose = 0;
int error = 0;
int compat;

char *target = "sse";

#define ORC_VERSION(a,b,c,d) ((a)*1000000 + (b)*10000 + (c)*100 + (d))
#define REQUIRE(a,b,c,d) do { \
  if (ORC_VERSION((a),(b),(c),(d)) > compat) { \
    fprintf(stderr, "Feature used that is incompatible with --compat\n"); \
    exit (1); \
  } \
} while (0)

void help (void)
{
  printf("Usage:\n");
  printf("  generate-emulation [OPTION...]\n");
  printf("\n");
  printf("Help Options:\n");
  printf("  -h, --help              Show help options\n");
  printf("\n");
  printf("Application Options:\n");
  printf("  -o, --output FILE       Write output to FILE\n");
  printf("\n");

  exit (0);
}

int
main (int argc, char *argv[])
{
  char *output_file = NULL;
  char *input_file = NULL;
  char *include_file = NULL;
  FILE *output;
  int i;
  OrcOpcodeSet *opcode_set;

  orc_init ();

  for(i=1;i<argc;i++) {
    if (strcmp (argv[i], "--output") == 0 ||
        strcmp(argv[i], "-o") == 0) {
      if (i+1 < argc) {
        output_file = argv[i+1];
        i++;
      } else {
        help();
      }
    } else if (strncmp(argv[i], "-", 1) == 0) {
      printf("Unknown option: %s\n", argv[i]);
      exit (1);
    } else {
      if (input_file == NULL) {
        input_file = argv[i];
      } else {
        printf("More than one input file specified: %s\n", argv[i]);
        exit (1);
      }
    }
  }

  if (output_file == NULL) {
    output_file = "out.c";
  }

  output = fopen (output_file, "w");
  if (!output) {
    printf("Could not write output file: %s\n", output_file);
    exit(1);
  }

  fprintf(output, "\n");
  fprintf(output, "/* autogenerated by generate-emulation */\n");
  fprintf(output, "\n");

  fprintf(output, "#ifdef HAVE_CONFIG_H\n");
  fprintf(output, "#include \"config.h\"\n");
  fprintf(output, "#endif\n");
  fprintf(output, "#include <math.h>\n");
  fprintf(output, "#include <orc/orc.h>\n");
  if (include_file) {
    fprintf(output, "#include <%s>\n", include_file);
  }
  fprintf(output, "\n");
  fprintf(output, "%s", orc_target_get_asm_preamble ("c"));
  fprintf(output, "\n");

  opcode_set = orc_opcode_set_get ("sys");

  for(i=0;i<opcode_set->n_opcodes;i++){
    char s[40];
    OrcProgram *program;
    OrcStaticOpcode *opcode = opcode_set->opcodes + i;

    program = orc_program_new ();

    sprintf(s, "emulate_%s", opcode->name);
    orc_program_set_name (program, s);

    if (opcode->flags & ORC_STATIC_OPCODE_ACCUMULATOR) {
      orc_program_add_accumulator (program, opcode->dest_size[0], "d1");
    } else {
      orc_program_add_destination (program, opcode->dest_size[0], "d1");
    }
    if (opcode->dest_size[1]) {
      orc_program_add_destination (program, opcode->dest_size[1], "d2");
    }
    if (opcode->src_size[1]) {
      orc_program_add_source (program, opcode->src_size[0], "s1");
      if (opcode->flags & ORC_STATIC_OPCODE_SCALAR) {
        orc_program_add_parameter (program, opcode->src_size[1], "s2");
      } else {
        orc_program_add_source (program, opcode->src_size[1], "s2");
      }
    } else {
      if (opcode->flags & ORC_STATIC_OPCODE_SCALAR) {
        orc_program_add_parameter (program, opcode->src_size[0], "s1");
      } else {
        orc_program_add_source (program, opcode->src_size[0], "s1");
      }
    }
    if (opcode->src_size[1]) {
      orc_program_append_str (program, opcode->name, "d1", "s1", "s2");
    } else {
      if (opcode->dest_size[1]) {
        orc_program_append_dds_str (program, opcode->name, "d1", "d2", "s1");
      } else {
        orc_program_append_ds_str (program, opcode->name, "d1", "s1");
      }
    }

    output_code_emulate (program, output);
  }

  fclose (output);

  if (error) exit(1);

  return 0;
}


const char *varnames[] = {
  "d1", "d2", "d3", "d4",
  "s1", "s2", "s3", "s4",
  "s5", "s6", "s7", "s8",
  "a1", "a2", "a3", "d4",
  "c1", "c2", "c3", "c4",
  "c5", "c6", "c7", "c8",
  "p1", "p2", "p3", "p4",
  "p5", "p6", "p7", "p8",
  "t1", "t2", "t3", "t4",
  "t5", "t6", "t7", "t8",
  "t9", "t10", "t11", "t12",
  "t13", "t14", "t15", "t16"
};

const char *enumnames[] = {
  "ORC_VAR_D1", "ORC_VAR_D2", "ORC_VAR_D3", "ORC_VAR_D4",
  "ORC_VAR_S1", "ORC_VAR_S2", "ORC_VAR_S3", "ORC_VAR_S4",
  "ORC_VAR_S5", "ORC_VAR_S6", "ORC_VAR_S7", "ORC_VAR_S8",
  "ORC_VAR_A1", "ORC_VAR_A2", "ORC_VAR_A3", "ORC_VAR_A4",
  "ORC_VAR_C1", "ORC_VAR_C2", "ORC_VAR_C3", "ORC_VAR_C4",
  "ORC_VAR_C5", "ORC_VAR_C6", "ORC_VAR_C7", "ORC_VAR_C8",
  "ORC_VAR_P1", "ORC_VAR_P2", "ORC_VAR_P3", "ORC_VAR_P4",
  "ORC_VAR_P5", "ORC_VAR_P6", "ORC_VAR_P7", "ORC_VAR_P8",
  "ORC_VAR_T1", "ORC_VAR_T2", "ORC_VAR_T3", "ORC_VAR_T4",
  "ORC_VAR_T5", "ORC_VAR_T6", "ORC_VAR_T7", "ORC_VAR_T8",
  "ORC_VAR_T9", "ORC_VAR_T10", "ORC_VAR_T11", "ORC_VAR_T12",
  "ORC_VAR_T13", "ORC_VAR_T14", "ORC_VAR_T15", "ORC_VAR_T16"
};

#if 0
void
output_prototype (OrcProgram *p, FILE *output)
{
  OrcVariable *var;
  int i;
  int need_comma;

  fprintf(output, "%s (", p->name);
  need_comma = FALSE;
  for(i=0;i<4;i++){
    var = &p->vars[ORC_VAR_D1 + i];
    if (var->size) {
      if (need_comma) fprintf(output, ", ");
      if (var->type_name) {
        fprintf(output, "%s * %s", var->type_name,
            varnames[ORC_VAR_D1 + i]);
      } else {
        fprintf(output, "orc_uint%d * %s", var->size*8,
            varnames[ORC_VAR_D1 + i]);
      }
      if (p->is_2d) {
        fprintf(output, ", int %s_stride", varnames[ORC_VAR_D1 + i]);
      }
      need_comma = TRUE;
    }
  }
  for(i=0;i<4;i++){
    var = &p->vars[ORC_VAR_A1 + i];
    if (var->size) {
      if (need_comma) fprintf(output, ", ");
      if (var->type_name) {
        fprintf(output, "%s * %s", var->type_name,
            varnames[ORC_VAR_A1 + i]);
      } else {
        fprintf(output, "orc_uint%d * %s", var->size*8,
            varnames[ORC_VAR_A1 + i]);
      }
      need_comma = TRUE;
    }
  }
  for(i=0;i<8;i++){
    var = &p->vars[ORC_VAR_S1 + i];
    if (var->size) {
      if (need_comma) fprintf(output, ", ");
      if (var->type_name) {
        fprintf(output, "const %s * %s", var->type_name,
            varnames[ORC_VAR_S1 + i]);
      } else {
        fprintf(output, "const orc_uint%d * %s", var->size*8,
            varnames[ORC_VAR_S1 + i]);
      }
      if (p->is_2d) {
        fprintf(output, ", int %s_stride", varnames[ORC_VAR_S1 + i]);
      }
      need_comma = TRUE;
    }
  }
  for(i=0;i<8;i++){
    var = &p->vars[ORC_VAR_P1 + i];
    if (var->size) {
      if (need_comma) fprintf(output, ", ");
      if (var->is_float_param) {
        REQUIRE(0,4,5,1);
      }
      fprintf(output, "%s %s",
          var->is_float_param ? "float" : "int",
          varnames[ORC_VAR_P1 + i]);
      need_comma = TRUE;
    }
  }
  if (p->constant_n == 0) {
    if (need_comma) fprintf(output, ", ");
    fprintf(output, "int n");
    need_comma = TRUE;
  }
  if (p->is_2d && p->constant_m == 0) {
    if (need_comma) fprintf(output, ", ");
    fprintf(output, "int m");
  }
  fprintf(output, ")");
}

void
output_code_header (OrcProgram *p, FILE *output)
{
  fprintf(output, "void ");
  output_prototype (p, output);
  fprintf(output, ";\n");
}
#endif

void
output_code_emulate (OrcProgram *p, FILE *output)
{

  fprintf(output, "void\n");
  if (p->constant_n) {
    fprintf(output, "%s (OrcOpcodeExecutor *ex, int offset)\n", p->name);
  } else {
    fprintf(output, "%s (OrcOpcodeExecutor *ex, int offset, int n)\n", p->name);
  }
  fprintf(output, "{\n");
  {
    OrcCompileResult result;

    result = orc_program_compile_full (p, orc_target_get_by_name("c"),
        ORC_TARGET_C_BARE | ORC_TARGET_C_OPCODE);
    if (ORC_COMPILE_RESULT_IS_SUCCESSFUL(result)) {
      fprintf(output, "%s\n", orc_program_get_asm_code (p));
    } else {
      printf("Failed to compile %s\n", p->name);
      error = TRUE;
    }
  }
  fprintf(output, "}\n");
  fprintf(output, "\n");

}

