%include "rw32-2018.inc"

section .data

    ; a*x*x + b*x + c = 0
    
    ; a = 2.0, b= 5.0, c= 3.125, x1 = x2 = -1.25
    strJeden    db "Rovnice ma jeden koren",10,0
    ; a = 2.0, b= 9.0, c= 4.0, x1 = -0.5, x2 = -4.0
    strDva      db "Rovnice ma dva koreny",10,0
    ; a = 4.0, b = 4.0, c=9.0  x1 = -0.500000+1.414214i x2 = -0.500000-1.414214i
    strZadny    db "Rovnice nema realne koreny",10,0
    
    
    a dq 4.0
    b dq 4.0
    c dq 9.0
    
section .bss
    ; realne koreny
    x1 resq 1 
    x2 resq 1 
    ; imaginarni slozka korene (D<0)
    y1 resq 1 
    y2 resq 1

section .text
CMAIN:
    push ebp
    mov ebp, esp

    ; Zde inicializujeme jednotku FPU
    finit
    
    ; QuadraticRoot(a,b,c,&x1,&x2);
    ; *** ZDE DOPLNTE VAS KOD (Bonus cast 1/3) ***
    ; Pro bonusovy bod upravte volani dle 'cdecl' (pres stack)
    ; QuadraticRoot(a,b,c,&x1,&x2,&y1,&y2);
    
    push dword [a+4]
    push dword [a]
    push dword [b+4]
    push dword [b]
    push dword [c+4]
    push dword [c]
    push x1
    push x2
    
    call QuadraticRoot
    
    ;ADD ESB, 32
   
    
    call printReal    
    
    mov esp, ebp
    pop ebp
    ret
    
    
QuadraticRoot:
    push ebp
    mov ebp, esp
    push ESI
    
    
; Vypocet korenu kvadraticke rovnice: 
; result = QuadraticRoot(
;   double a,
;   double b,
;   double c,
;   double* x1,
;   double* x2) {
;       D = b*b - 4*a*c;
;       if (D = 0) {
;           printf("Rovnice ma jeden koren\n");
;           x1 = -b/(2*a);
;           x2 = x1;
;       } else if (D > 0) {
;           printf("Rovnice ma dva koreny\n");
;           x1 = (-b+sqrt(D))/(2*a);
;           x2 = (-b-sqrt(D))/(2*a);;
;       } else {
;           printf("Rovnice ma imaginarni koreny\n");
;           /** Doplnte pro bonusovy bod (cast 2/3) **/
;       }
;}   
;
;
; Vypoctete determinant (D = b*b - 4*a*c)

    ; [ESP+40] = a/1 ;1. polovina a
    ; [ESP+36] = a/2 ;2. polovina a
    ; [ESP+32] = b/1 ;1. polovina b
    ; [ESP+28] = b/2 ;2. polovina b
    ; [ESP+24] = c/1 ;1. polovina c
    ; [ESP+20] = c/2 ;2. polovina c
    ; [ESP+16] = *x1
    ; [ESP+12] = *x2
    ; [ESP+8] = ret
    ; [ESP+4] = old EBP
    ; [ESP] = ESI
    
; *** ZDE DOPLNTE VAS KOD ***
    FLD qword [ESP+32] ; ST0 = b
    FMUL ST0 ; ST0 = b*b
    FLD qword [ESP+40] ; ST0 = a, ST1 = b*b
    FLD qword [ESP+24] ; ST0= c, ST1 = a, ST2 = b*b
    FMUL ; ST0 = a*c, ST1 = b*b
    FLD1 ; ST0 = 1, ST1 = a*c, ST2 = b*b
    FLD1 ; ST0 = 1, ST1 = 1, ST2 = a*c, ST3 = b*b
    FADD ; ST0 = 2, ST1 = a*c, ST2 = b*b
    FMUL ST0 ; ST0 = 4, ST1 = a*c, ST2 = b*b
    FMUL ; ST0 = 4*a*c, ST1 b*b
    FSUB ; ST0 = b*b-4*a*c 
   
    
; Porovnejte determinant (ST0) aby vypsal spravnou zpravu
; (podivejte se na wiki k FPU, jak vetvit)

    FTST ;porovnáni s nulou
    FSTSW AX ;nahrání Flagu do AX
    SAHF ;neco :D
                        ; -> mapovani priznaku FPU do FLAGS registru:
                        ;   C3 = ZF
                        ;   C2 = PF
                        ;   C0 = CF
    
    JZ .rovno
    JC .mensi
    ;JNC .vetsi      ;nebo rovno
    
    
    
.vetsi:    
    mov ESI, strDva
    call print
    
    ;   x1 = (-b+sqrt(D))/(2*a);
    ;   x2 = (-b-sqrt(D))/(2*a);
    
    ; *** ZDE DOPLNTE VAS KOD ***
       
    
    jmp .end

.rovno:    
    mov ESI, strJeden
    call print
    ;   x1 = -b/(2*a);
    ;   x2 = x1;
    
    ; *** ZDE DOPLNTE VAS KOD ***

    jmp .end
    
.mensi:
    mov ESI, strZadny
    call print
    ; Doplnte pro bonusovy bod (cast 2/3)
    ; Naleznete komplexni koreny
    
    ; *** ZDE DOPLNTE VAS KOD ***

        
    jmp .end   

 .end:
    pop ESI   
    mov esp, ebp
    pop ebp

    ret

print:
    call WriteString
    call WriteNewLine    
    ret    
      
printReal:

    ; Write x1
    fld qword [x1]
    call WriteDouble
    call WriteNewLine
    fstp st0
    
    ; Write x2
    fld qword [x2]
    call WriteDouble
    call WriteNewLine
    fstp st0
    
    ret
    
    
    
printComplex:

    ; Doplnte pro bonusovy bod (cast 3/3)
    ; Vypiste komplexni koreny ve formatu: 
    ; x1-y1i
    ; x2+y2i
    
    
    ; *** ZDE DOPLNTE VAS KOD ***

    
    ret

    
    