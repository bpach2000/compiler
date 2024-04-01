


#enter instruction main
.align 2
.text
_main:

# Set up the stack fram
    la $sp, -8($sp)     # allocate space for old $fp and $ra
    sw $fp, 4($sp)      # save old $fp
    sw $ra, 0($sp)      # save return address
    la $fp, 0($sp)      # set up frame pointer
    la $sp, -20($sp)    # allocate stack frame: n = space for locals/temps, in bytes

    # ASSG _tmp0 = 12345
    li $t0, 12345
    sw $t0, 0($fp)

    # ASSG x = _tmp0
    lw $t0, 0($fp)
    sw $t0, 0($fp)

    # PARAM x
    lw $t0, 0($fp)
    la $sp, -4($sp)
    sw $t0, 0($sp)

    # Call instruction println
    jal _println
    la $sp, 4($sp)

    # ASSG _tmp2 = 23456
    li $t0, 23456
    sw $t0, 0($fp)

    # ASSG y = _tmp2
    lw $t0, 0($fp)
    sw $t0, 0($fp)

    # PARAM y
    lw $t0, 0($fp)
    la $sp, -4($sp)
    sw $t0, 0($sp)

    # Call instruction println
    jal _println
    la $sp, 4($sp)

    # ASSG _tmp4 = 34567
    li $t0, 34567
    sw $t0, 0($fp)

    # ASSG z = _tmp4
    lw $t0, 0($fp)
    sw $t0, 0($fp)

    # PARAM z
    lw $t0, 0($fp)
    la $sp, -4($sp)
    sw $t0, 0($sp)

    # Call instruction println
    jal _println
    la $sp, 4($sp)

    # RETURN_VOID
    la $sp, 0($fp)  # deallocate locals
    lw $ra, 0($sp)  # restore return address
    lw $fp, 4($sp)  # restore frame pointer
    la $sp, 8($sp)  # restore stack pointer
    jr $ra          # return

.align 2
.data
_nl: .asciiz "\n"
.align 2
.text

# println: print out an integer followed by a newline
_println:
    li $v0, 1
    lw $a0, 0($sp)
    syscall
    li $v0, 4
    la $a0, _nl
    syscall
    jr $ra

.align 2
.text
# main: calls the user-defined main function
main:
    j _main 
