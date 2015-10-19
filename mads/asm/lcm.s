# This file will execute MIPS instructions to calculate
# GCD and LCM. Here:
# Mem[0] = GCD($s0, $s1)
# Mem[4] = LCM($s0, $s1)
##  
#------var initialization-----
addi $s0, $zero, 1234
addi $s1, $zero, 8
#-----------------------------

add $t0, $s0, $zero
add $t1, $s1, $zero
GCD:
beq $t1, $zero, DONE
div $t0, $t1
add $t0, $zero, $t1
mfhi $t1
j GCD
DONE:
sw $t0, 0($zero)

LCM:
add $t0, $zero, $s0
add $t1, $zero, $s1
lw $t2, 0($zero)
mult $t0, $t1
mflo $t3
div $t3, $t2
mflo $t0
sw $t0, 4($zero)

