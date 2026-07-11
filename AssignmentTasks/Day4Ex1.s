.data
my_array : .word 10 20 30 40
space : .string " "
newline: .string "\n"

.text 

main:
li a0 -1
li a1  0
call max #max(-1,0)
mv a1 a0
li a0 1
ecall
la a1 newline
li a0 4
ecall

la a0 my_array
li a1 4
call sum_array #test sum_array on the above declared array
mv a1 a0
li a0 1
ecall

li a0 10
ecall #exit program

max:
    bge a0 a1 return_max
    mv a0 a1

    return_max:
    ret

sum_array:
li t0 0 #index
li s0 0 #running sum

    loop:
    bge t0 a1 done_sum #Check index < size
    slli t2 t0 2   #Get address by multiplying by 4
    add t3 a0 t2   #Shift pointer
    lw t4 0(t3)    #Load ith word of array
    add s2 s2 t4   #Add to running sum
    addi t0 t0 1   #Increment index
    j loop         #Continue looping

    done_sum:
    mv a0 s2 #assign to a0 before returning

    return_sum:
    ret
