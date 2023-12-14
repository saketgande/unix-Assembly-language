#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"

static int lbl;

int ex(nodeType *p) {
    int lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
    case typeCon:
        printf("\t# typecon\n");
        printf("\tpushq\t$%d\n", p->con.value);
        break;
    case typeId:
        printf("\t# typeid\n");
        printf("\tpushq\t%c(%%rip)\n", p->id.i + 'a');
        break;
    case typeOpr:
        switch (p->opr.oper) {
        case WHILE:
            printf("\t# while\n");
            lbl1 = lbl++;
            lbl2 = lbl++;
            printf("L%03d:\n", lbl1);
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            printf("\tcmpq\t$0, %%rax\n");
            printf("\tje\tL%03d\n", lbl2);
            ex(p->opr.op[1]);
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case IF:
            printf("# if\n");
            lbl1 = lbl++;
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            if (p->opr.nops > 2) {
                lbl2 = lbl++;
                printf("\ttestq\t%%rax, %%rax\n");
                printf("\tjz\tL%03d\n", lbl1);
                ex(p->opr.op[1]);
                printf("\tjmp\tL%03d\n", lbl2);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            } else {
                printf("\ttestq\t%%rax, %%rax\n");
                printf("\tjz\tL%03d\n", lbl1);
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;
        case PRINT:
	    ex(p->opr.op[0]);
	    printf("\tpopq\t%%rsi\t\t\t# Load value to print into rsi\n");
	    printf("\tmovq\t$0, %%rax\t\t# Indicate no floating-point args\n");
	    printf("\tsubq\t$8, %%rsp\t\t# Adjust stack for printf call\n");
	    printf("\tleaq\t.LC0(%%rip), %%rdi\t# Load format string address\n");
	    printf("\tcall\tprintf\t\t\t# Call printf\n");
	    printf("\taddq\t$8, %%rsp\t\t# Restore stack pointer\n");
	    break;

        case '=':
            printf("# EQUAL\n");
            ex(p->opr.op[1]);
            printf("\tpopq\t%%rax\n");
            printf("\tmovq\t%%rax, %c(%%rip)\n", p->opr.op[0]->id.i + 'a');
            break;
        case UMINUS:
            printf("# uMINUS \n");
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            printf("\tnegq\t%%rax\n");
            printf("\tpushq\t%%rax\n");
            break;
        case FACT:
            printf("# FACT \n");  
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            printf("\t# FACT implementation\n");
            printf("\tmovq\t$1, %%rbx\n");
            printf("L%03d:\n", lbl1 = lbl++);
            printf("\tcmpq\t$1, %%rax\n");
            printf("\tjle\tL%03d\n", lbl2 = lbl++);
            printf("\timulq\t%%rax, %%rbx\n");
            printf("\tdecq\t%%rax\n");
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            printf("\tpushq\t%%rbx\n");
            break;
        case LNTWO:
            printf("# lntwo \n");  
            ex(p->opr.op[0]);
            printf("\tpopq\t%%rax\n");
            printf("\t# LNTWO implementation\n");
            printf("\tmovq\t$1, %%rbx\n");
            printf("L%03d:\n", lbl1 = lbl++);
            printf("\tcmpq\t$2, %%rax\n");
            printf("\tjl\tL%03d\n", lbl2 = lbl++);
            printf("\tshrq\t$1, %%rax\n");
            printf("\taddq\t%%rbx, %%rbx\n");
            printf("\tjmp\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            printf("\tpushq\t%%rbx\n");
            break;
        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);
            switch(p->opr.oper) {
            case '+':   
                printf("# ADD \n");  
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\taddq\t%%rbx, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case '-':   
                printf("\t # minus \n");  
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tsubq\t%%rbx, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case '*':   
                printf("\t # multi \n");  
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\timulq\t%%rbx, %%rax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case '/':   
                printf("\t# divide \n");
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\txorq\t%%rdx, %%rdx\n");  // Zero out rdx
                printf("\tidivq\t%%rbx\n");
                printf("\tpushq\t%%rax\n");
                break;
            case '<':   
                printf("\t # lt \n");
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetl\t%%al\n");
                printf("\tmovzbl\t%%al, %%eax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case '>':   
                printf("\t # gt \n");
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetg\t%%al\n");
                printf("\tmovzbl\t%%al, %%eax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case GE:   
                printf("\t # GE \n"); 
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetge\t%%al\n");
                printf("\tmovzbl\t%%al, %%eax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case LE:    
                printf("\t # LE \n");
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetle\t%%al\n");
                printf("\tmovzbl\t%%al, %%eax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case NE:    
                printf("\t # NE \n");
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsetne\t%%al\n");
                printf("\tmovzbl\t%%al, %%eax\n");
                printf("\tpushq\t%%rax\n");
                break;
            case EQ:    
                printf("\t # EQ \n");
                printf("\tpopq\t%%rbx\n");
                printf("\tpopq\t%%rax\n");
                printf("\tcmpq\t%%rbx, %%rax\n");
                printf("\tsete\t%%al\n");
                printf("\tmovzbl\t%%al, %%eax\n");
                printf("\tpushq\t%%rax\n");
                break;
            }
        }
    }
    return 0;
}

