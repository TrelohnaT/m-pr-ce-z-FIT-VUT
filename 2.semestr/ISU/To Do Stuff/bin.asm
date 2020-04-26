%include "rw32-2018.inc"


section .data
	string1 db "Hello FIT",0
	string2 db "Hello World",0
        login   db "xlogin00",0
      
 
 ;Block Started by Symbol 
section .bss
        buffer resb 255
  
section .text
 
MyWriteStrings:
;Buffer
        MOV ESI,buffer
	call WriteString 
	call WriteNewLine
;Prvni string
	MOV ESI,string1
	call WriteString
	call WriteNewLine
;Druhy string
	MOV ESI,string2
	call WriteString
	call WriteNewLine

;Volny radek
	call WriteNewLine

	ret
 
   
CMAIN:
        ;stack frame
        push ebp
        mov ebp, esp
        
        ; Vynulovani bufferu
        mov [buffer], byte 'a' 
        mov [buffer+1], byte 0 
 
        ; Vypise retezce
        call MyWriteStrings 

        ; Opakovani:
        ; Doplnte parametry funkce, implementaci a vypiste vysledek
        call MyMin
      
            
        ; UKOL 1: Zjistete delku kretsiho z obou retezcu (string1, string2)
        ; Doplnte argumenty, implementaci, vypiste vysledek
        push dword string1
        
        call MyStrLen
        add esp, 4
        
        push EAX
        
        ;
        push dword string2
        call MyStrLen
        add esp, 4
        
        push EAX
        ;
        PUSH DWORD 10
        PUSH DWORD 4
        
        call MyMin
          
        add ESP, 8
        
        call WriteString
        call WriteNewLine  
        ; UKOL 2: Zjistete delku prefixu shodnych pro retezce (string1, string2)
        ; Doplnte argumenty, implementaci, vypiste vysledek
        push dword string1
        push dword string2
        call MyPrefixLen  
        add esp, 8
 
        ; UKOL 3: Zkopirujte jeden string do bufferu, a nasledne druhy. 
        ;
        push dword buffer
        push dword string2
        call MyCopyStr ; string2->buffer 
        add esp, 8
        ;
        call MyWriteStrings
        ;
        push dword buffer
        push dword string1
        call MyCopyStr ; string1->buffer 
        add esp, 8
        ;
        call MyWriteStrings
        
        
        ; Bonusovy Ukol:
        ;
        call MySomething
        call MyWriteStrings
        
        xor eax,eax ; navratova hodnota
        mov esp, ebp
        pop ebp
ret
 
 
; Misto pro vase funkce

; Opakovani z minula
;
; Funkce vezme dva argumenty ze stacku (32 bit, Neznamenkove cele cisla)
; Vrati mensi z nich
; int MyMin(int a(4), int b(10));        
MyMin:
    ; Doplnit
    PUSH EBP
    mov EBP, ESP
    
    mov EAX, [EBP+8]
    
    cmp EAX, [EBP+12]
    jna konec
    
    mov EAX, [EBP+12]
      
konec:
    mov ESP, EBP
    pop EBP
    ret                

; UKOL 1

; Funkce prochazi retezec dokud nenalezne ukoncovaci znak a vrati delku retezce
; int MyStrLen(char* string);
MyStrLen:
    ; Doplnit
    push ebp
    mov EBP, ESP
    ; ebp+8 -> prvni argument char* string
    ; ebp+4 -> ret
    ; ebp -> old ebp
    
    push EDI
    
    cld
    
    mov EDI, [ebp+8]
    mov ECX, -1
    mov AL, 0
    repne scasb
    
    mov EAX, EDI
    sub EAX, [EBP+8]
    
    pop EDI
    mov ESP, EBP
    pop ebp
    ret
 
; UKOL 2
; Funkce pracuje s tremi argumenty. Prochazi dva retezce dokud jsou shodne, nebo dokud neprecte kratsi z nich. 
; int MyPrefixLen(char* str1, char* str2, int len);
MyPrefixLen:
    ; doplnit
    push ebp
    mov EBP, ESP
    ; ebp+8 -> prvni argument char* string
    ; ebp+4 -> ret
    ; ebp -> old ebp
    
    push EDI
    
    cld
    
    mov EDI, [ebp+8]
    mov ECX, -1
    mov ESI, [ebp+4]
    
    repe cmpsb
    
    
    mov EAX, EDI
    sub EAX, [EBP+8]
    
    pop EDI
    mov ESP, EBP
    pop ebp
    ret

  
; UKOL 3 
; Kopiruje z src do dest. Predpoklada se ze src se vzdy vleze do dest.
; char* MyCopyStr(char* src, char* dst)
MyCopyStr:
    ; doplnit obalku (EBP, ESP)
    ; len - MyStrLen (src)
    ; MOVSB src -> dst
    push ebp
    mov EBP, ESP
    
    push dword [EBP+8]
    call MyStrLen
    ;len -> EAX
    add esp, 8  
 
    mov ECX, EAX
    cld
    mov ESI, [EBP+8]
    mov EDI, [EBP+12]
    rep movsb
 
 
    mov ESP, EBP
    pop ebp
    ret    
            
; ukol navic
; buffer, string1, string2
; string1, string2, prefix(string1, string2) login
MySomething:
    ; doplnit
    ret    
 