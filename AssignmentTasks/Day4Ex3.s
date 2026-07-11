.text

main:
    li a0, 10
    call funcA

    mv a1, a0
    li a0, 1
    ecall

    li a0, 10
    ecall


funcA:
    addi sp, sp, -16
    sw ra, 12(sp)
    sw s0, 8(sp)

    mv s0, a0
    call funcB
    add a0, a0, s0 #funcA does a0 + funcB(a0)

    lw s0, 8(sp)
    lw ra, 12(sp)
    addi sp, sp, 16
    ret


funcB:
    slli a0, a0, 1 #funcB does 2 * a0
    ret