#include "assembler.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Input file missing\n");
        exit(1);
    }

    int buf_size = 4096;
    char* buf = (char*)malloc(buf_size);
    assert(buf);

    FILE *f = fopen(argv[1], "r");
    FILE* outf = fopen("./assembler.out", "w");
    assert(f);
    assert(outf);

    Assembler* a = new Assembler(outf);

    while (!feof(f)) {
        char* ret = fgets(buf, buf_size, f);
        // skip past empty lines
        if (!ret)
            continue;
        // remove terminating newline
        buf[strlen(buf) - 1] = 0;

        // skip past lines with just newline
        if (strlen(buf) == 0)
            continue;

        a->assemble(buf);

        // add separator newline
        fprintf(outf, "\n");
    }
    
    delete a;
    free(buf);
    fclose(f);
    fclose(outf);
    return 0;
}
