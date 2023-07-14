#ifndef __H_ASSERT_H__
#define __H_ASSERT_H__

#define ASSERT_EXIT_CODE 1

//Exp - expression or register
//Op - comparasion (<, >, <=, >=, =0, <>0)
#ifdef DEBUG
#define hassert(Exp, Op) uassert_macro Exp #Op __LINE__ __FILE__
#else
#define hassert(Exp, Op)
#endif
//example call: 
//hassert(gr4, <)
//hassert(gr1-gr0, =0)

.macro hassert_macro Exp, Op, Line, Func

.extern _printf
.extern _exit
.extern _fflush

    push ar5, gr5;
    push ar4, gr4;
    gr4 = \Exp;
    with gr4;
    skip skip_text\@;
assert_msg\@:
    .ascii "Assertion %s(%d,0x%x)%s failed: %s:%d\n\0"
exp_string\@:
    .ascii "\Exp\0"
op_string\@:
    .ascii "\Op\0"
func_string\@:
    .ascii "\Func\0"
skip_text\@:
    // "Assertion failed"
    if \Op delayed skip end_assert_macros_\@;
        ar5 = ar7;
        nul;
    ar7 += 8;

    gr5 = false;
    [ar5++] =  gr5;

    gr5 = \Line;
    [ar5++] =  gr5;

    gr5 = func_string\@;
    [ar5++] =  gr5;

    gr5 = op_string\@;
    [ar5++] =  gr5;

    [ar5++] = gr4;
    [ar5++] = gr4;

    gr5 = exp_string\@;
    [ar5++] =  gr5;

    gr5 = assert_msg\@;
    [ar5++] =  gr5;

    call _printf;


    ar7 -= 8;

    gr5 = 1;
    ar5 = ar7;
    ar7 += 2;
    [ar5++] = gr5;
    [ar5++] = gr5;
    call _fflush;
    ar7 -=2;
    
    gr7 = ASSERT_EXIT_CODE;
    push ar7, gr7;
    goto _exit;


end_assert_macros_\@:    
    pop ar4, gr4;
    pop ar5, gr5;


.endm

#endif //__H_ASSERT_H__
