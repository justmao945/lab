# Provide math operations 
# SRL64:  Shift right logical
# SRA64:  Shift right arithmetic
# SLL64:  Shift left logical
# ADD64:  Add
# NEG64:  Negative
# SUB64:  Subtract
# ABS32:  Absolute value
# ABS64:  Absolute value
# MUL32:  Multiply
# MULU32: Multiply unsigned
# DIV32:  Divide
# DIVU32: Divide unsigned
# PRINT: Print int, char, string
#--------------------------

#---Program init
addiu  $sp,  $zero,  0xffff    # set stack pointer
j MAIN                        # goto main section

#--------------- SRL64 BEGIN ----------
# $v0:$v1 = $a0:$a1 >> $a2
SRL64:
addi  $sp,  $sp,  -20       # store $t0 $a[i]
sw    $t0,  0($sp)          # [$sp+0] = $t0
sw    $a0,  4($sp)          # [$sp+4] = $a0
sw    $a1,  8($sp)          # [$sp+8] = $a1
sw    $a2,  12($sp)         # [$sp+12] = $a2
sw    $ra,  16($sp)         # [$sp+16] = $ra

bne   $a2,  $zero,  SRL64_S   # off != 0 ? jmp
add   $v0,  $zero,  $a0       # $v0 = $a0
add   $v1,  $zero,  $a1       # $v1 = $a1
j     SRL64_END               # return
SRL64_S:
srlv  $a1,  $a1,    $a2       # *al = (*al >> off)
sltiu $t0,  $a2,    32        # $t0 = off < 32 ? 1 : 0
beq   $t0,  $zero,  SRL64_G   # off >= 32 ? jmp
sub   $t0,  $zero,  $a2       # $t0 = -off
addi  $t0,  $t0,    32        # $t0 = 32 - off
sllv  $t0,  $a0,    $t0       # $t0 = *ah << (32-off)
j     SRL64_AL_OR             # al shift done to or
SRL64_G:
addi  $t0,  $a2,  -32       # $t0 = off - 32
srlv  $t0,  $a0,  $t0       # $t0 = *ah >> (off-32)
SRL64_AL_OR:
or    $a1,  $a1,  $t0       # *al = (*al >> off)| $t0
srlv  $a0,  $a0,  $a2       # *ah = *ah >>= off
add   $v0,  $a0,  $zero     # $v0 = $a0
add   $v1,  $a1,  $zero     # $v1 = $a1

SRL64_END:
lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a2,  12($sp)         # restore $a2 = [$sp+12]
lw    $a1,  8($sp)          # restore $a1 = [$sp+8]
lw    $a0,  4($sp)          # restore $a0 = [$sp+4]
lw    $t0,  0($sp)          # restore $t0 = [$sp+0]
addi  $sp,  $sp,  20        # restroe stack
jr    $ra                   # return
#------------ SRL64 END ------------


#--------------- SRA64 BEGIN ----------
# $v0:$v1 = $a0:$a1 >>> $a2
SRA64:
addi  $sp,  $sp,  -20       # store $t0 $a[i]
sw    $t0,  0($sp)          # [$sp+0] = $t0
sw    $a0,  4($sp)          # [$sp+4] = $a0
sw    $a1,  8($sp)          # [$sp+8] = $a1
sw    $a2,  12($sp)         # [$sp+12] = $a2
sw    $ra,  16($sp)         # [$sp+16] = $ra

bne   $a2,  $zero,  SRA64_S   # off != 0 ? jmp
add   $v0,  $zero,  $a0       # $v0 = $a0
add   $v1,  $zero,  $a1       # $v1 = $a1
j     SRA64_END               # return
SRA64_S:
srlv  $a1,  $a1,    $a2       # *al = (*al >> off)
sltiu $t0,  $a2,    32        # $t0 = off < 32 ? 1 : 0
beq   $t0,  $zero,  SRA64_G   # off >= 32 ? jmp
sub   $t0,  $zero,  $a2       # $t0 = -off
addi  $t0,  $t0,    32        # $t0 = 32 - off
sllv  $t0,  $a0,    $t0       # $t0 = *ah << (32-off)
j     SRA64_AL_OR             # al shift done to or
SRA64_G:
addi  $t0,  $a2,  -32       # $t0 = off - 32
srav  $t0,  $a0,  $t0       # $t0 = (i32_t)*ah >> (off-32)
SRA64_AL_OR:
or    $a1,  $a1,  $t0       # *al = (*al >> off)| $t0
srav  $a0,  $a0,  $a2       # *ah = (i32_t)*ah >>= off
add   $v0,  $a0,  $zero     # $v0 = $a0
add   $v1,  $a1,  $zero     # $v1 = $a1

SRA64_END:
lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a2,  12($sp)         # restore $a2 = [$sp+12]
lw    $a1,  8($sp)          # restore $a1 = [$sp+8]
lw    $a0,  4($sp)          # restore $a0 = [$sp+4]
lw    $t0,  0($sp)          # restore $t0 = [$sp+0]
addi  $sp,  $sp,  20        # restroe stack
jr    $ra                   # return
#------------ SRA64 END ------------


#--------------- SLL64 BEGIN ----------
# $v0:$v1 = $a0:$a1 << $a2
SLL64:
addi  $sp,  $sp,  -20       # store $t0 $a[i]
sw    $t0,  0($sp)          # [$sp+0] = $t0
sw    $a0,  4($sp)          # [$sp+4] = $a0
sw    $a1,  8($sp)          # [$sp+8] = $a1
sw    $a2,  12($sp)         # [$sp+12] = $a2
sw    $ra,  16($sp)         # [$sp+16] = $ra

bne   $a2,  $zero,  SLL64_S   # off != 0 ? jmp
add   $v0,  $zero,  $a0       # $v0 = $a0
add   $v1,  $zero,  $a1       # $v1 = $a1
j     SLL64_END               # return
SLL64_S:
sllv  $a0,  $a0,    $a2       # *ah = (*ah << off)
sltiu $t0,  $a2,    32        # $t0 = off < 32 ? 1 : 0
beq   $t0,  $zero,  SLL64_G   # off >= 32 ? jmp
sub   $t0,  $zero,  $a2       # $t0 = -off
addi  $t0,  $t0,    32        # $t0 = 32 - off
srlv  $t0,  $a1,    $t0       # $t0 = *al >> (32-off)
j     SLL64_AL_OR             # al shift done to or
SLL64_G:
addi  $t0,  $a2,  -32       # $t0 = off - 32
sllv  $t0,  $a1,  $t0       # $t0 = *al << (off-32)
SLL64_AL_OR:
or    $a0,  $a0,  $t0       # *ah = (*ah << off)| $t0
sllv  $a1,  $a1,  $a2       # *al = *al <<= off
add   $v0,  $a0,  $zero     # $v0 = $a0
add   $v1,  $a1,  $zero     # $v1 = $a1

SLL64_END:
lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a2,  12($sp)         # restore $a2 = [$sp+12]
lw    $a1,  8($sp)          # restore $a1 = [$sp+8]
lw    $a0,  4($sp)          # restore $a0 = [$sp+4]
lw    $t0,  0($sp)          # restore $t0 = [$sp+0]
addi  $sp,  $sp,  20        # restroe stack
jr    $ra                   # return
#------------ SLL64 END ------------




#--------------- ADD64 BEGIN ----------
# $v0:$v1 = $a0:$a1 + $a2:$a3
ADD64:
addi  $sp,  $sp,  -24       # store $t0 $a[i]
sw    $t0,  0($sp)          # [$sp+0] = $t0
sw    $a0,  4($sp)          # [$sp+4] = $a0
sw    $a1,  8($sp)          # [$sp+8] = $a1
sw    $a2,  12($sp)         # [$sp+12] = $a2
sw    $a3,  16($sp)         # [$sp+16] = $a3
sw    $ra,  20($sp)         # [$sp+20] = $ra

add   $t0,  $zero,  $a1             # $t0 = t = *al
addu  $a1,  $a1,    $a3             # $a1 = *al + *bl
ADD64_IF:
sltu  $t0,  $a1,    $t0             # $t0 = *al < t ? 1 : 0
bne   $t0,  $zero,  ADD64_IF_CARRY  # *al < t ? jmp
sltu  $t0,  $a1,    $a3             # $t0 = *al < bl ? 1 : 0
bne   $t0,  $zero,  ADD64_IF_CARRY  # *al < t ? jmp
j     ADD64_IF_NO_CARRY
ADD64_IF_CARRY:
addiu $t0,  $zero,  1       # $t0 = ca = 1
j     ADD64_IF_END
ADD64_IF_NO_CARRY:
add   $t0,  $zero,  $zero   # $t0 = ca = 0
ADD64_IF_END:
addu  $a0,  $a0,    $a2     # ah += bh
addu  $a0,  $a0,    $t0     # ah += ca

lw    $ra,  20($sp)         # restore $ra = [$sp+20]
lw    $a3,  16($sp)         # restore $a3 = [$sp+16]
lw    $a2,  12($sp)         # restore $a2 = [$sp+12]
lw    $a1,  8($sp)          # restore $a1 = [$sp+8]
lw    $a0,  4($sp)          # restore $a0 = [$sp+4]
lw    $t0,  0($sp)          # restore $t0 = [$sp+0]
addi  $sp,  $sp,  24        # restroe stack
jr    $ra                   # return
#------------ ADD64 END ------------


#--------------- NEG64 BEGIN ----------
# $v0:$v1 = -$a0:$a1
NEG64:
addi  $sp,  $sp,  -20       # store $t0 $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0
sw    $a1,  4($sp)          # [$sp+4] = $a1
sw    $a2,  8($sp)          # [$sp+8] = $a2
sw    $a3,  12($sp)         # [$sp+12] = $a3
sw    $ra,  16($sp)         # [$sp+16] = $ra

addi  $a2,  $zero,  0xffff      # $a2 = 0x0000ffff
lui   $a2,  0xffff              # $a2 = 0xffffffff
xor   $a0,  $a0,    $a2         # $a0 = ~*ah
xor   $a1,  $a1,    $a2         # $a1 = ~*al
add   $a2,  $zero,  $zero       # $a2 = 0
addi  $a3,  $zero,  1           # $a3 = 1
jal   ADD64                     # call ADD64

lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a3,  12($sp)         # restore $a3 = [$sp+12]
lw    $a2,  8($sp)          # restore $a2 = [$sp+8]
lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,  20        # restroe stack
jr    $ra                   # return
#------------ NEG64 END ------------


#--------------- SUB64 BEGIN ----------
# $v0:$v1 = $a0:$a1 - $a2:$a3
SUB64:
addi  $sp,  $sp,  -20       # store $t0 $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0
sw    $a1,  4($sp)          # [$sp+4] = $a1
sw    $a2,  8($sp)          # [$sp+8] = $a2
sw    $a3,  12($sp)         # [$sp+12] = $a3
sw    $ra,  16($sp)         # [$sp+16] = $ra

add   $a0,  $a2,  $zero     # $a0 = bh
add   $a1,  $a3,  $zero     # $a1 = bl
jal   NEG64                 # call NEG64
lw    $a0,  0($sp)          # restore $a0 = *ah
lw    $a1,  4($sp)          # restore $a1 = *al
add   $a2,  $zero,  $v0     # $a2 \
add   $a3,  $zero,  $v1     # $a3 _\--> -(bh bl)
jal   ADD64

lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a3,  12($sp)         # restore $a3 = [$sp+12]
lw    $a2,  8($sp)          # restore $a2 = [$sp+8]
lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,  20        # restroe stack
jr    $ra                   # return
#------------ SUB64 END ------------

#--------------- ABS32 BEGIN ----------
# $v0 = | $a0 |
ABS32:
addi  $sp,  $sp,  -4        # store $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0

ABS32_IF:
slti  $at,  $a0,    0               # $at = $a0 < 0 ? 1 : 0
beq   $at,  $zero,  ABS32_IF_ELSE   # $a0 >= 0 ? jmp
sub   $v0,  $zero,  $a0             # $v0 = -$a0
j     ABS32_IF_END
ABS32_IF_ELSE:
addi  $v0,  $a0,    0               # $v0 = $a0
ABS32_IF_END:

lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,  4         # restroe stack
jr    $ra                   # return
#------------ ABS32 END ------------


#--------------- ABS64 BEGIN ----------
# $v0:$v1 = | $a0:$a1 |
ABS64:
addi  $sp,  $sp,    -8      # store $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0
sw    $a1,  4($sp)          # [$sp+4] = $a1

ABS64_IF:
slti  $at,  $a0,    0               # $at = $a0 < 0 ? 1 : 0
beq   $at,  $zero,  ABS64_IF_END    # $a0 >= 0 ? jmp
jal   NEG64                         # call NEG64
addi  $a0,  $v0,    0               # $a0 = $v0
addi  $a1,  $v1,    0               # $a1 = $v1
ABS64_IF_END:
addi  $v0,  $a0,    0               # $v0 = $a0
addi  $v1,  $a1,    0               # $v1 = $a1

lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,    8       # restroe stack
jr    $ra                   # return
#------------ ABS64 END ------------


#--------------- MUL32 BEGIN ----------
# $v0:$v1 = $a0 * $a1
MUL32:
addi  $sp,  $sp,  -32       # store $t0 $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0
sw    $a1,  4($sp)          # [$sp+4] = $a1
sw    $a2,  8($sp)          # [$sp+8] = $a2
sw    $a3,  12($sp)         # [$sp+12] = $a3
sw    $ra,  16($sp)         # [$sp+16] = $ra
sw    $t0,  20($sp)         # [$sp+20] = $t0
sw    $t1,  24($sp)         # [$sp+24] = $t1
sw    $t2,  28($sp)         # [$sp+28] = $t2

add   $a2,  $zero,  $zero   # $a2 = ch = 0
add   $a3,  $zero,  $a0     # $a3 = cl = *ah
add   $t0,  $zero,  $zero   # $t0 = i = 0
add   $t1,  $zero,  $zero   # $t1 = bit = 0
MUL32_FOR:
sltiu $at,  $t0,    32               # $at = i < 32 ? 1 : 0
beq   $at,  $zero,  MUL32_FOR_END    # i >= 32 ? jmp
MUL32_FOR_SWITCH:
ori   $at,  $a3,    1                         # $at = cl | 1
sll   $at,  $at,    1                         # $at = (cl|1)<<1
or    $at,  $at,    $t1                       # $at = ((cl|1)<<1)|bit
addi  $t2,  $at,    -1                        # $t2 = $at - 1
beq   $t2,  $zero,  MUL32_FOR_SWITCH_CASE_1   # case 1 ? jmp
addi  $t2,  $at,    -2                        # $t2 = $at - 2
beq   $t2,  $zero,  MUL32_FOR_SWITCH_CASE_2   # case 2 ? jmp
j     MUL32_FOR_SWITCH_END
MUL32_FOR_SWITCH_CASE_1:
addu  $a2,  $a2,    $a1     # ch += *al
j     MUL32_FOR_SWITCH_END  # break
MUL32_FOR_SWITCH_CASE_2:
subu  $a2,  $a2,    $a1     # ch -= *al
j     MUL32_FOR_SWITCH_END  # break
MUL32_FOR_SWITCH_END:
andi  $t1,  $a3,    1       # bit = cl & 1
add   $a0,  $a2,    $zero   # $a0 = ch
add   $a1,  $a3,    $zero   # $a1 = cl
addi  $a2,  $zero,  1       # $a2 = 1
jal   SRA64                 # call SRA64
add   $a2,  $v0,    $zero   # ch = $v0
add   $a3,  $v1,    $zero   # cl = $v1
lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
j     MUL32_FOR             # for loop
MUL32_FOR_END:
add   $v0,  $a2,    $zero   # $v0 = ch
add   $v1,  $a3,    $zero   # $v1 = cl

lw    $t2,  28($sp)         # restore $t2 = [$sp+28]
lw    $t1,  24($sp)         # restore $t1 = [$sp+24]
lw    $t0,  20($sp)         # restore $t0 = [$sp+20]
lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a3,  12($sp)         # restore $a3 = [$sp+12]
lw    $a2,  8($sp)          # restore $a2 = [$sp+8]
lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,  32        # restroe stack
jr    $ra                   # return
#------------ MUL32 END ------------


#--------------- MULU32 BEGIN ----------
# $v0:$v1 = $a0 * $a1
MULU32:
addi  $sp,  $sp,  -36       # store $t0 $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0
sw    $a1,  4($sp)          # [$sp+4] = $a1
sw    $a2,  8($sp)          # [$sp+8] = $a2
sw    $a3,  12($sp)         # [$sp+12] = $a3
sw    $ra,  16($sp)         # [$sp+16] = $ra
sw    $t0,  20($sp)         # [$sp+20] = $t0
sw    $t1,  24($sp)         # [$sp+24] = $t1
sw    $t2,  28($sp)         # [$sp+28] = $t2
sw    $t3,  32($sp)         # [$sp+32] = $t3

addi  $t2,  $zero,  0       # $t2 = ch = 0
addi  $t3,  $zero,  0       # $t3 = cl = 0
addi  $t0,  $zero,  1       # $t0 = i = 1
addi  $t1,  $zero,  0       # $t1 = j = 0
MULU32_FOR:
beq   $t0,  $zero,  MULU32_FOR_END    # i == 0 ? jmp
and   $at,  $a1,    $t0               # $at = *al & i
MULU32_FOR_IF:
beq   $at,  $zero,  MULU32_FOR_IF_END # *al & i == 0 ? jmp
addi  $a0,  $zero,  0                 # $a0 = 0
lw    $a1,  0($sp)                    # $a1 = *ah
add   $a2,  $t1,    $zero             # off = $a2 = j
jal   SLL64                           # call SLL64
addi  $a2,  $v0,    0                 # bh
addi  $a3,  $v1,    0                 # bl
addi  $a0,  $t2,    0                 # ch
addi  $a1,  $t3,    0                 # cl
jal   ADD64                           # call ADD64
addi  $t2,  $v0,    0                 # ch = $v0
addi  $t3,  $v1,    0                 # cl = $v1
MULU32_FOR_IF_END:
sll   $t0,  $t0,    1                 # i <<= 1
addi  $t1,  $t1,    1                 # j++
j     MULU32_FOR                      # for loop
MULU32_FOR_END:
addi  $v0,  $t2,    0                 #\
addi  $v1,  $t3,    0                 #/ --> return val

lw    $t3,  32($sp)         # restore $t3 = [$sp+32]
lw    $t2,  28($sp)         # restore $t2 = [$sp+28]
lw    $t1,  24($sp)         # restore $t1 = [$sp+24]
lw    $t0,  20($sp)         # restore $t0 = [$sp+20]
lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a3,  12($sp)         # restore $a3 = [$sp+12]
lw    $a2,  8($sp)          # restore $a2 = [$sp+8]
lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,  36        # restroe stack
jr    $ra                   # return
#------------ MULU32 END ------------



#--------------- DIV32 BEGIN ----------
# $v0 = $a0:$a1 % $a2
# $v1 = $a0:$a1 / $a2
DIV32:
addi  $sp,  $sp,  -40       # store $t0 $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0
sw    $a1,  4($sp)          # [$sp+4] = $a1
sw    $a2,  8($sp)          # [$sp+8] = $a2
sw    $a3,  12($sp)         # [$sp+12] = $a3
sw    $ra,  16($sp)         # [$sp+16] = $ra
sw    $t0,  20($sp)         # [$sp+20] = $t0
sw    $t1,  24($sp)         # [$sp+24] = $t1
sw    $s0,  28($sp)         # [$sp+28] = $s0
sw    $s1,  32($sp)         # [$sp+32] = $s1
sw    $s2,  36($sp)         # [$sp+36] = $s2

jal   ABS64                 # call ABS64
addi  $s0,  $v0,  0         # $s0 \
addi  $s1,  $v1,  0         # $s1 / = |*ah : *al|
lw    $a0,  8($sp)          # $a0 = bh
jal   ABS32                 # call ABS32
addi  $s2,  $v0,  0         # s2 = |bh|
addi  $a0,  $s0,  0         # $a0 \ = |*ah : *al|
addi  $a1,  $s1,  0         # $a1 /
addi  $a2,  $s2,  0         # $a2 = |bh|
jal   DIVU32                # call DIVU32
addi  $t0,  $v1,  0         # $t0 = q = *al
DIV32_IF:
lw    $a0,  0($sp)                  # $a0 = *ah
lw    $a2,  8($sp)                  # $a2 = bh
xor   $at,  $a0,    $a2             # $at = *ah ^ bh
lui   $a0,  0x8000                  # $a0 = 0x80000000
and   $at,  $at,    $a0             # $at = (*ah ^ bh)&0x80000000
beq   $at,  $zero,  DIV32_IF_END    # $at == 0 ? jmp
sub   $t0,  $zero,  $t0             # $t0 = -$t0 == -*al
DIV32_IF_END:
addi  $a0,  $t0,    0       # $a0 = q
lw    $a1,  8($sp)          # $a1 = bh
jal   MUL32                 # call MUL32
addi  $a2,  $v0,    0       # $a2 = $v0
addi  $a3,  $v1,    0       # $a3 = $v1
lw    $a0,  0($sp)          # restore $a0 = *ah
lw    $a1,  4($sp)          # retrore $a1 = *al
jal   SUB64                 # call SUB64
addi  $v0,  $v1,    0       # *ah = $v1  = r
addi  $v1,  $t0,    0       # *al = q

lw    $s2,  36($sp)         # restore $s2 = [$sp+36]
lw    $s1,  32($sp)         # restore $s1 = [$sp+32]
lw    $s0,  28($sp)         # restore $s0 = [$sp+28]
lw    $t1,  24($sp)         # restore $t1 = [$sp+24]
lw    $t0,  20($sp)         # restore $t0 = [$sp+20]
lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a3,  12($sp)         # restore $a3 = [$sp+12]
lw    $a2,  8($sp)          # restore $a2 = [$sp+8]
lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,  40        # restroe stack
jr    $ra                   # return
#------------ DIV32 END ------------


#--------------- DIVU32 BEGIN ----------
# $v0 = $a0:$a1 % $a2
# $v1 = $a0:$a1 / $a2
DIVU32:
addi  $sp,  $sp,  -44       # store $t0 $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0
sw    $a1,  4($sp)          # [$sp+4] = $a1
sw    $a2,  8($sp)          # [$sp+8] = $a2
sw    $a3,  12($sp)         # [$sp+12] = $a3
sw    $ra,  16($sp)         # [$sp+16] = $ra
sw    $t0,  20($sp)         # [$sp+20] = $t0
sw    $t1,  24($sp)         # [$sp+24] = $t1
sw    $t2,  28($sp)         # [$sp+28] = $t2
sw    $s0,  32($sp)         # [$sp+32] = $s0
sw    $s1,  36($sp)         # [$sp+36] = $s1
sw    $s2,  40($sp)         # [$sp+40] = $s2

addi  $t0,  $zero,  0       # $t0 = t = 0
addi  $t1,  $zero,  0       # $t1 = q = 0
lui   $t2,  0x8000          # $t2 = i = 0x80000000
addi  $s0,  $a0,    0       # $s0 = *ah
addi  $s1,  $a1,    0       # $s1 = *al
addi  $s2,  $a2,    0       # $s2 = bh
DIVU32_FOR:
beq   $t2,  $zero,  DIVU32_FOR_END   # i == 0 ? jmp
sll   $t0,  $t0,    1                # t <<= 1
srl   $at,  $a0,    31               # $at = *ah >> 31
or    $t0,  $t0,    $at              # t |= *ah >> 31
addi  $a0,  $s0,    0                # $a0 = *ah
addi  $a1,  $s1,    0                # $a1 = *al
addi  $a2,  $zero,  1                # $a2 = 1
jal   SLL64                          # call SLL64
addi  $s0,  $v0,    0                # *ah = $v0
addi  $s1,  $v1,    0                # *al = $v1
addi  $a0,  $t0,    0                # $a0 = t
addi  $a1,  $s0,    0                # $a1 = *ah
addi  $a2,  $zero,  0                # $a2 = 0
addi  $a3,  $s2,    0                # $a3 = bh
jal   SUB64                          # call SUB64
addi  $t0,  $v0,    0                # t = $v0
addi  $s0,  $v1,    0                # *ah = $v1
DIVU32_FOR_IF:
lui   $at,  0x8000                    # $at = 0x80000000
and   $at,  $at,    $t0               # $at = 0x80000000 & t
beq   $at,  $zero,  DIVU32_FOR_ELSE   # 0x80000000 & t == 0 ? jmp
addi  $a0,  $t0,    0                 # $a0 = t
addi  $a1,  $s0,    0                 # $a1 = *ah
addi  $a2,  $zero,  0                 # $a2 = 0
addi  $a3,  $s2,    0                 # $a3 = bh
jal   ADD64                           # call ADD64
addi  $t0,  $v0,    0                 # t = $v0
addi  $s0,  $v1,    0                 # *ah = $v1
j     DIVU32_FOR_IF_END
DIVU32_FOR_ELSE:
or    $t1,  $t1,    $t2     # q |= i
DIVU32_FOR_IF_END:
srl   $t2,  $t2,    1       # i >>= 1
j     DIVU32_FOR
DIVU32_FOR_END:
addi  $v0,  $s0,    0       # *ah = r
addi  $v1,  $t1,    0       # *al = q

lw    $s2,  40($sp)         # restore $s2 = [$sp+40]
lw    $s1,  36($sp)         # restore $s1 = [$sp+36]
lw    $s0,  32($sp)         # restore $s0 = [$sp+32]
lw    $t2,  28($sp)         # restore $t2 = [$sp+28]
lw    $t1,  24($sp)         # restore $t1 = [$sp+24]
lw    $t0,  20($sp)         # restore $t0 = [$sp+20]
lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a3,  12($sp)         # restore $a3 = [$sp+12]
lw    $a2,  8($sp)          # restore $a2 = [$sp+8]
lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,  44        # restroe stack
jr    $ra                   # return
#------------ DIVU32 END ------------



#--------------- PRINT BEGIN ----------
# $a0: Function code
#       1:  int 32
#       2:  unsigned int 32
#       4:  hex 32
#       8:  string
# $a1: Value to print
# $a2: Address to print (DRAM mapped in MEM)
PRINT:
addi  $sp,  $sp,  -44       # store $t0 $a[i]
sw    $a0,  0($sp)          # [$sp+0] = $a0
sw    $a1,  4($sp)          # [$sp+4] = $a1
sw    $a2,  8($sp)          # [$sp+8] = $a2
sw    $a3,  12($sp)         # [$sp+12] = $a3
sw    $ra,  16($sp)         # [$sp+16] = $ra
sw    $t0,  20($sp)         # [$sp+20] = $t0
sw    $t1,  24($sp)         # [$sp+24] = $t1
sw    $t2,  28($sp)         # [$sp+28] = $t2
sw    $s0,  32($sp)         # [$sp+32] = $s0
sw    $s1,  36($sp)         # [$sp+36] = $s1
sw    $s2,  40($sp)         # [$sp+40] = $s2

PRINT_SWITCH:
addi  $at,  $zero,  1                           # $at = 1
beq   $a0,  $at,    PRINT_SWITCH_CASE_1         # ah == 1 ? jmp
addi  $at,  $zero,  2                           # $at = 2
beq   $a0,  $at,    PRINT_SWITCH_CASE_2         # ah == 2 ? jmp
addi  $at,  $zero,  4                           # $at = 4
beq   $a0,  $at,    PRINT_SWITCH_CASE_4         # ah == 4 ? jmp
addi  $at,  $zero,  8                           # $at = 8
beq   $a0,  $at,    PRINT_SWITCH_CASE_8         # ah == 8 ? jmp

PRINT_SWITCH_DEFAULT:
j     PRINT_SWITCH_END

PRINT_SWITCH_CASE_1:                            # int32
addi  $t0,  $a2,    0                           # $t0 = base = bh
PRINT_SWITCH_CASE_1_IF:
slt   $at,  $a1,    $zero                       # $at = al < 0 ? 1 : 0
beq   $at,  $zero,  PRINT_SWITCH_CASE_1_IF_END  # al >= 0 ? jmp
addi  $t1,  $zero,  45                          # $t1 = '-'
sw    $t1,  0($t0)                              # store to dram
addiu $t0,  $zero,  4                           # $t0 += 4
PRINT_SWITCH_CASE_1_IF_END:
addi  $a0,  $a1,    0                           # $a0 = al
jal   ABS32                                     # call ABS32
addi  $a0,  $zero,  2                           # $a0 = 2, $a2 = bh
addiu $a1,  $v0,    0                           # $a1 = |bh|
jal   PRINT                                     # call PRINT
j     PRINT_SWITCH_END

PRINT_SWITCH_CASE_2:                      # u32
lui   $t0,  0x3b9a                        # $t0 = t = 0x3b9a0000
ori   $t0,  $t0,    0xca00                # $t0 = t = 0x3b9aca00 = 1000000000
addi  $t1,  $zero,  0                     # $t1 = q = 0
addi  $t2,  $zero,  1                     # $t2 = zero = 1
addi  $s0,  $zero,  0                     # $s0 = off = 0
PRINT_SWITCH_CASE_2_IF:
bne   $zero,  $a1,  PRINT_SWITCH_CASE_2_IF_END    # al != 0 ? jmp
addi  $at,  $zero,  48                            # $at = '0'
sb    $at,  0($a2)                                # [bh] = '0'
j     PRINT_SWITCH_END                            # return
PRINT_SWITCH_CASE_2_IF_END:
add   $at,  $zero,  $zero                         # nop
PRINT_SWITCH_CASE_2_WHILE:
beq   $t0,  $zero,  PRINT_SWITCH_CASE_2_WHILE_END     # t == 0 ? break
addi  $a0,  $zero,  0                                 # $a0 = 0, $a1 = al
addi  $a2,  $t0,    0                                 # $a2 = t
jal   DIVU32                                          # call DIVU32
addi  $t1,  $v1,    0                                 # q = al / t
addi  $a1,  $v0,    0                                 # al = al % t
PRINT_SWITCH_CASE_2_WHILE_IF_Q:
beq   $t1,  $zero,  PRINT_SWITCH_CASE_2_WHILE_IF_Q_END      # q == 0 ? jmp
addi  $t2,  $zero,  0                                       # zero = 0
PRINT_SWITCH_CASE_2_WHILE_IF_Q_END:
add   $at,  $zero,  $zero                                   # nop
PRINT_SWITCH_CASE_2_WHILE_IF_ZERO:
bne   $t2,  $zero,  PRINT_SWITCH_CASE_2_WHILE_IF_ZERO_END   # zero != 0 ? jmp
lw    $s1,  8($sp)                                          # $s1 = bh
addu  $s1,  $s1,    $s0                                     # $s1 = bh = + off
addi  $t1,  $t1,    48                                      # q += '0'
sb    $t1,  0($s1)                                          # [bh+off] = q + '0'
addiu $s0,  $s0,    1                                       # off++
PRINT_SWITCH_CASE_2_WHILE_IF_ZERO_END:
addi  $a0,  $zero,  0                                       # $a0 = 0
addi  $a1,  $t0,    0                                       # $a1 = t
addi  $a2,  $zero,  10                                      # $a2 = 10
jal   DIVU32                                                # call DIVU32, t / 10
addi  $t0,  $v1,    0                                       # t = t / 10
j     PRINT_SWITCH_CASE_2_WHILE
PRINT_SWITCH_CASE_2_WHILE_END:
j     PRINT_SWITCH_END

PRINT_SWITCH_CASE_4:                      # hex32
lui   $t0,  0xF000                        # $t0 = t = 0xF0000000
addi  $t1,  $zero,  0                     # $t1 = off = 0
PRINT_SWITCH_CASE_4_WHILE:
slti  $at,  $t1,    8                               # $at = off < 8 ? 1 : 0
beq   $at,  $zero,  PRINT_SWITCH_CASE_4_WHILE_END   # off >= 8 ? break
sub   $at,  $zero,  $t1                             # $at  = -off
addi  $at,  $at,    7                               # $at  = 7 - off
sll   $at,  $at,    2                               # $at = (7-off) << 2
and   $t2,  $t0,    $a1                             # $t2 = t & al
srlv  $t2,  $t2,    $at                             # $t2 = q = (t&al)>>((7-off)<<2)
PRINT_SWITCH_CASE_4_WHILE_IF:
slt   $at,  $t2,    $zero                                 # $at = q < 0 ? 1 : 0
bne   $at,  $zero,  PRINT_SWITCH_CASE_4_WHILE_IF_ELSE_IF  # q < 0 ? jmp
slti  $at,  $t2,    0xa                                   # $at = q < 0xa ? 1 : 0
beq   $at,  $zero,  PRINT_SWITCH_CASE_4_WHILE_IF_ELSE_IF  # q >= 0xa ? jmp
addi  $t2,  $t2,    48                                    # q += '0'
j     PRINT_SWITCH_CASE_4_WHILE_IF_END
PRINT_SWITCH_CASE_4_WHILE_IF_ELSE_IF:
slti  $at,  $t2,    0x10                                  # $at = q<0x10 ? 1 : 0
beq   $at,  $zero,  PRINT_SWITCH_CASE_4_WHILE_IF_END      # q >= 0x10 ? jmp
addi  $t2,  $t2,    55                                    # q += 'A' - 10
j     PRINT_SWITCH_CASE_4_WHILE_IF_END
PRINT_SWITCH_CASE_4_WHILE_IF_ELSE:
addi  $t2,  $zero,  0                                     # q = 0
PRINT_SWITCH_CASE_4_WHILE_IF_END:
addu  $s0,  $a2,    $t1                           # $s0 = (bh + off)
sb    $t2,  0($s0)                                # [bh+off] = q
addiu $t1,  $t1,    1                             # off++
srl   $t0,  $t0,    4                             # t>>=4
j     PRINT_SWITCH_CASE_4_WHILE
PRINT_SWITCH_CASE_4_WHILE_END:
j     PRINT_SWITCH_END

PRINT_SWITCH_CASE_8:                            # string
addi  $t0,  $a1,  0                             # $t0 = al
addi  $t1,  $a2,  0                             # $t1 = bh
PRINT_SWITCH_CASE_8_WHILE:
lb    $at,  0($t0)                              # $at = [$t0]
sb    $at,  0($t1)                              # [$t1] =  $at
beq   $at,  $zero,  PRINT_SWITCH_END            # if ($at == 0) break
addiu $t0,  $zero,  1                           # $t0++
addiu $t1,  $zero,  1                           # $t1++
j     PRINT_SWITCH_CASE_8_WHILE                 # while loop

PRINT_SWITCH_END:

lw    $s2,  40($sp)         # restore $s2 = [$sp+40]
lw    $s1,  36($sp)         # restore $s1 = [$sp+36]
lw    $s0,  32($sp)         # restore $s0 = [$sp+32]
lw    $t2,  28($sp)         # restore $t2 = [$sp+28]
lw    $t1,  24($sp)         # restore $t1 = [$sp+24]
lw    $t0,  20($sp)         # restore $t0 = [$sp+20]
lw    $ra,  16($sp)         # restore $ra = [$sp+16]
lw    $a3,  12($sp)         # restore $a3 = [$sp+12]
lw    $a2,  8($sp)          # restore $a2 = [$sp+8]
lw    $a1,  4($sp)          # restore $a1 = [$sp+4]
lw    $a0,  0($sp)          # restore $a0 = [$sp+0]
addi  $sp,  $sp,  44        # restroe stack
jr    $ra                   # return
#------------ PRINT END ------------


#------------- MAIN -----------------
MAIN:
addi  $a0, $zero, 1
addi  $a1, $zero, 2
addi  $a2, $zero, 1
addi  $a3, $zero, 1
jal   SRL64

