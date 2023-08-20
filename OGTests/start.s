	.file	"start.s"
    .text
	.align	2
	.globl	_start
	.type	_start, @function
_start:

    # init stack pointer
    lui sp, %hi(__stack_end)
    add sp, sp, %lo(__stack_end)

    # jump to main
    call main

    # call halt routine.
    lui	a5,0x1
    sb	zero,-2036(a5)

