%include "rw32-2018.inc"

section .data

    const3 dq 3.0
    ;const4 dq 4.0
    const4 dw 4
    sphereRadius dq 3.0
    strSphereVolume db "Sphere volume = ",0

section .text
CMAIN:
    push ebp
    mov ebp, esp

; Zde inicializujeme jednotku FPU
    finit
    
    
    
    ; *** ZDE DOPLNTE VAS KOD ***
    
    push dword [sphereRadius+4]
    push dword [sphereRadius]
    
    FLD qword [sphereRadius]
    SUB ESP, 8
    FSTp qword [ESP]
    
    call SphereVolume
    ADD ESP, 8
    
    mov esi, strSphereVolume
    call WriteString
    call WriteDouble
    call WriteNewLine
    call WriteNewLine
    fstp st0    ;pop
        
    mov esp, ebp
    pop ebp
    ret
    

; Vypocet objemu koule dle: 
;    V = (4/3)*pi*r^3
;
; Funkce prijima jediny argument typu *DOUBLE* a vraci hodnotu objemu
; v registru *ST0* FPU (take *DOUBLE*).
; double SphareVolume(double radius);       
SphereVolume:
    push ebp
    mov ebp, esp
    
    ; [EBP+8]   radius
    ; [EBP+4]   EIP (ret)
    ; [EBP]     old EBP
    
; *** ZDE DOPLNTE VAS KOD ***
    ;FLD qword [const4] ;ST0 = 4
    ;FILD word [const4] ;ST0 = 4
    
    FLD1 ;ST0 = 1.0
    FLD1 ;ST0 = 1.0 ST1 = 1.0
    FADD ;ST0 = 2
    FADD ST0 ;ST0 = 4.0
    
    FLD qword [const3] ;st0 = 3-0, st1 = 4.0
    FDIV ;ST0 = 4/3
    FLDPI ;ST0 = pi, ST1 = 4/3
    FMUL ;ST0 = 4/3*pi
    FLD qword [EBP+8] ;ST0 = r, ST1 = 4/3*pi
    FLD st0 ;st0 = r, stI1 = r, ST2 = 4/3*pi
    FMUL ST0 ;ST0 = R*R, ST1 = R, ST2 = 4/3*pi
    FMUL ; ST0 = R*R*R, ST1 = 4/3*pi
    FMUL ; ST0 = 4/3*pi*R*R*R
    
    ;FSTP ST0 => POP
    mov esp, ebp
    pop ebp
    ret