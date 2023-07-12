.text
.globl boo
boo:
    pushq %rbp			
    movq %rsp, %rbp		
    # todo abrir espaco no ra

    movl $1, %ecx 	# i = 1 
    # usando o rax como registrador auxiliar para guardar o vetor de double
    movq %xmm0, %rax 	# guardando 1o param como conteudo apontado pelo rax
    

for:
    cmp %edi, %ecx	# n e o 1o param inteiro
    jge final		# se i >= n, sai do for
    movslq %ecx, %rdx	
    imulq $8, %rdx	# usando rdx para guardar o indice do vetor que vamos acessar
    addq %rdx, %rax
    
    cvtsi2sd %ecx, %xmm1	# guardando o valor double de i para poder somar depois	
    addq %rdx, %rax		# andando com o ponteiro rax
    addsd -8(%rax), %xmm1	# i = v[i-1] + i
    movsd %xmm1, (%rax)		# atribuicao v[i] = v[i-1]
    incl %ecx			# i++
    jmp for			# volta p loop

final:
    movq %rax, %xmm0	# guardando o valor float do conteudo do ponteiro no registrador de retorno
    cvtsd2ss %xmm0, %xmm0
    leave
    ret