#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TEMP 4096

class Assembler {
    // state of register
    enum RegState {
        EMPTY,
        LOADED
    };

  public:
    Assembler(FILE *f) :
        outf(f), reg(EMPTY), offset(0), last_temp_used(1) {
    }

    ~Assembler() {
    }

    void assemble(char* buf) {
        // initialize state
        len = strlen(buf);
        init();

        char c;
        while (next_char(buf, c)) {
            if (is_operand(c)) {
                switch (regstate) {
                    case EMPTY:
                        // If the state of the register is empty then load
                        // operand
                        fprintf(outf, "L %c\n", c);
                        reg = c;
                        regstate = LOADED;
                        break;
                    case LOADED:
                        char next;
                        // Loop until we find the first operator. This loop
                        // handles the case where multple operands appear in
                        // sequence. When we find the first operator, we can
                        // apply the operator to the last loaded operand. This
                        // handles the case where operators can be applied
                        // directly to the input operand.
                        do {
                            assert(next_char(buf, next));
                            if (is_operand(next)) {
                                fprintf(outf, "ST $%d\n", last_temp_used);
                                temp[last_temp_used++] = reg;
                                fprintf(outf, "L %c\n", c);
                                reg = c;
                                c = next;
                            } else {
                                // it's an operator...
                                switch (next) {
                                    case '+':
                                        fprintf(outf, "A %c\n", c);
                                        break;
                                    case '-':
                                        fprintf(outf, "S %c\n", c);
                                        break;
                                    case '*':
                                        fprintf(outf, "M %c\n", c);
                                        break;
                                    case '/':
                                        fprintf(outf, "D %c\n", c);
                                        break;
                                }
                                break;
                            }
                        } while (true);
                        break;
                    default:
                        assert(false && "What state?");
                };
            } else {
                // if it's an operator, then this is the case where the
                // operator must be applied to the operands in the register and
                // the latest temporary location.
                assert(regstate == LOADED && "Need operand to add");
                switch (c) {
                    case '+':
                        assert(last_temp_used > 0);                        
                        fprintf(outf, "A $%d\n", last_temp_used - 1);
                        --last_temp_used;
                        break;
                    case '-':
                        assert(last_temp_used > 0);                        
                        fprintf(outf, "N\n");
                        fprintf(outf, "A $%d\n", last_temp_used - 1);
                        --last_temp_used;
                        break;
                    case '*':
                        assert(last_temp_used > 0);                        
                        fprintf(outf, "M $%d\n", last_temp_used - 1);
                        --last_temp_used;
                        break;
                    case '/':
                        assert(last_temp_used > 0);                        
                        // store contents of register
                        fprintf(outf, "ST $%d\n", last_temp_used);
                        temp[last_temp_used++] = reg;
                        // load previous value
                        fprintf(outf, "L $%d\n", last_temp_used - 2);
                        fprintf(outf, "D $%d\n", last_temp_used - 1);
                        // remove both from temp space
                        --last_temp_used;
                        --last_temp_used;
                        break;
                }
            }
        }
    }

  private:

    // initialize assembler state
    void init() {
        regstate = EMPTY;
        offset = 0;
        last_temp_used = 1;

    }

    // check if operand
    bool is_operand(char c) {
        if (c >= 'A' && c <= 'Z')
            return true;
        return false;
    }

    // check if operator
    bool is_operator(char c) {
        if (c == '+' || c == '-' || c == '*' || c == '/')
            return true;
        return false;
    }

    // return next character. Checks for illegal characters and loops over
    // spaces
    bool next_char(char* buf, char& c) {
        if (offset == len)
            return false;
        // skip past spaces
        for (; buf[offset] == ' '; ++offset);
        if (is_operand(buf[offset]) || is_operator(buf[offset])) {
            c = buf[offset];
            ++offset;
            return true;
        } else
            assert(false && "Illegal character");
    }


  private:
    char temp[MAX_TEMP];
    int last_temp_used;
    RegState regstate;
    char reg;
    FILE* outf;

    int offset;
    int len;
};
