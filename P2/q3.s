.text
.globl foo
foo:
    pushq %rbp				
    movq %rsp, %rbp	
    # todo abrir espaco no ra

    movl $0, %ecx # i = 0
while:
    cmp %edx, %ecx		# compara i com n 		 
    jge fora_while		# se i >= n sai do loop		
    movl %ecx, (%rsi) 		# coloca o valor de i em cad na struct
    movq %rdi, 8(%rsi)		# coloca o valor de v em val na struct
    addq $32, %rsi		# anda com o ponteiro da struct p struct prox
    addq $8, %rdi		# anda com ponteiro do vetor de double
    incl %ecx			# i++
    jmp while			# volta p loop
fora_while:
    leave
    ret