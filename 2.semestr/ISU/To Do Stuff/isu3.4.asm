%include "rw32-2018.inc"

section .data
    width db 16     ; Sirka mapy
    height db 16    ; Vyska mapy
    pos_x db 1      ; Aktualni pozice v ose X
    pos_y db 1      ; Aktualni pozice v ose Y
    status db 0     ; 0- hraj, 1- vyhra, -1 ->prohra
    
    winMessage db "Victory!",0
        
    errorMessage db "Invalid character. Only L,R,D,U allowed.",0
    
    ;X -> Pozice Hrace
    ;G -> Goal (Cil)
    ;. -> Misto ktere hrac uz navstivil
    ;
    ; Data mapy
 map    db "################"
        db "#X             #"
        db "#              #"
        db "#              #"
        db "#              #"
        db "#              #"
        db "#       G      #"
        db "#              #"
        db "#              #"
        db "#              #"
        db "#              #"
        db "#              #"
        db "#              #"
        db "#              #"
        db "#              #"
        db "################"
        
  _map  db "################"
        db "#X #      #G   #"
        db "#  #       ### #"
        db "#  #      #    #"
        db "#         # ####"
        db "######### # #  #"
        db "#  #      #  # #"
        db "#  #      ## ###"
        db "#  #       #  ##"
        db "#           #  #"
        db "#  ########### #"
        db "#              #"
        db "#          #####"
        db "#              #"
        db "#           ## #"
        db "################"        
        
        
section .bss
    dirString: resb 256 ; Misto pro ulozeni vstupniho retezce s kodem pohybu

section .text
_main:
    push ebp
    mov ebp, esp
    
    push ebx
    push esi
    push edi
    
    
    
    ; UKOL 1: Doplnte funkci kresleni mapy
    ; Vykresleni mapy na zacatku
    call drawMap
    
.getMoves:    
    
    ; Rozcvicka: Nactete retezec se vstupnim kodem pro pohyb maximalni delce 256 
    ; a ulozte ho na adresu "dirString"
    
    mov EDI, dirString
    mov EBX, 257
    call ReadString
    
    
    mov esi, dirString
    ;call WriteString
    
.mainLoop:

    ; UKOL 2: Doplnte ridici smycku .mainLoop
    ; while(dirString[ESI] != 0) {
    ;   ;;UKOL3
    ;   ; Pred vypracovanim ukolu 3 vypise dirString[ESI]
    ;}
    mov al, [ESI]
    
    cmp al, 0
    
    je .exitMainLoop
    
    call WriteChar
    call WriteNewLine
    
    ;pokud je nula, (konec retezce), smycka konci
    cmp al, 0
    je .exitMainLoop
    
    ; UKOL 3: Doplnte logiku zpracovani vstupniho retezce.
    ; Znaky retezce znamenaji smer pohybu:
    ; L - vlevo, R - vpravo, U - nahoru, D - dolu
    ; Pro vykonani pohybu volejte funkci "move" s jedinym parametrem parametrem v EAX
    ;
    ; switch(al) {
    ;     case 'L': 
    ;         move(-1);
    ;         break;
    ;     case 'R': 
    ;         move(1);
    ;         break;
    ;     case 'U':
    ;         move(-[width]);
    ;         break;
    ;     case 'D': 
    ;         move([width]);
    ;         break;
    ;     default:
    ;         goto .exitMainLoop;
    ; }
    ;
    ; *** ZDE DOPLNTE VAS KOD ***

    mov AL, [ESI]
    
    cmp AL, 'L'
    je .caseL
    
    cmp AL,'R'
    je .caseR
    
    cmp AL, 'D'
    je .caseD
    
    cmp AL, 'U'
    je .caseU
    
    jmp .default
    
.caseL:
    mov EAX, -1
    call move
    jmp .sw_end
    
.caseR:
    mov EAX, +1
    call move
    jmp .sw_end
    
.caseU:
    xor EAX, EAX
    mov AL, [width]
    neg EAX
    call move
    jmp .sw_end

.caseD:
    xor EAX, EAX
    mov AL, [width]
    neg EAX
    call move
    jmp .sw_end

.default:
    jmp .error
    
.sw_end:
    
    ;Docasne pro smycku
    ;mov eax, 'a'
    ;call WriteChar
    ;call WriteNewLine
    
    ; Zabranime nekonecne smycce v neupravenem zadani
    ;jmp .end        

    inc ESI
    jmp .mainLoop
.exitMainLoop:
    
    ; Vykresleni mapy po provedeni vsech kroku
    call drawMap

    cmp byte [status],0
    je .getMoves
    jl .lost
    jg .win
.win:
    mov esi, winMessage
    jmp .end       
.error:
    mov esi, errorMessage
    jmp .end
.lost:    
                
.end:
    call drawMap
    call WriteString
    
    pop EDI
    pop ESI
    POP EBX
    
    mov esp, ebp
    pop ebp
    ret

; Funkce pro vykresleni mapy o velikosti width x height
; Neprijima zadne argumenty -- pristupujte ke globalnim promnennym
drawMap:
    push ebp
    mov ebp, esp
    call WriteNewLine  
    
    ; UKOL 1: Doplnte vykresleni 2D mapy v poli "map" pomoci funkci "WriteChar" 
    ; a "WriteNewLine".
    ;
    ; char *p = map;
    mov ESI, map
    ; for(int y = 0; y < *height; ++y) {
    
    
    
    XOR ECX, ECX
    
    mov CL, [height]
    
    .l1: ;For pro Y osu
    
    PUSH ECX
    
    XOR ECX, ECX
    
    mov CL, [width]
    
    
        .l2: ;For pro X osu
        
    mov AL, [ESI]
    call WriteChar
    
    INC ESI
        
        
        
        loop .l2
    
    call WriteNewLine
    pop ECX
    
    loop .l1
    ;     for(int x = 0; x < *width; ++y) {
    ;         WriteChar(*p);
    ;         p++;
    ;     }
    ;     
    ;     WriteNewLine();
    ; }
    ;
    ; *** ZDE DOPLNTE VAS KOD ***




    
    mov esp, ebp
    pop ebp
    ret

; Funkce pro provedeni pohybu z aktualni pozice ve smeru LEFT, RIGHT, UP, DOWN
; Argument urcujici smer pohybu je predan v registru EAX
move:
    push ebp
    mov ebp, esp
    push esi
    ; Zaloha argumentu funkce
    push eax

    ; Vypocet adresy z aktualni pozice
    call To1D
    
    ;Stejne jako
    ; add eax, map
    ; mov esi, eax
    lea esi, [map + eax]
    mov edi, eax
    ; Nacteni argumentu
    pop eax

    ; EDI nova pozice v souradnicich mapy
    add edi, eax
    
    ; Pamet soucasne pozice je ESI, pristi pozice je ESI+EAX        
    ; Cesta je naznacena znaky '.', jeji konec, soucasna pozice hrace, pak znakem 'X'
    
    ; UKOL N: Zkontrolujte znak v mape na pristi pozici.
    ; switch([pristi pozice]) {
    ;   case '.':
    ;   case ' ';
    ;       SetChar(soucasna pozice,'.');       
    ;       SetChar(pristi pozice,'X');
    ;       Set2D(pristi pozice);
    ;       break;
    ;   case '#':   ;Prekazka/zed
    ;       break;  ;Nedelej nic
    ;   case 'G';
    ;       SetChar(soucasna pozice,'.');
    ;       SetChar(pristi pozice,'X');
    ;       Set2D(pristi pozice);
    ;       break;
    ; }
    ; *** ZDE DOPLNTE VAS KOD ***

    push ESI
    
    add ESI, EAX ;Dalsi pozice hrace
    
    cmp ESI, '#'
    je .wall
    
    cmp ESI, ' '
    je .space
    
    ;cmp ESI, '.'
    ;je .track
    
    ;cmp ESI, 'G'
   ; je .target
    
    ;jmp .def
    
 .wall:
    jmp .end
    
.space:
    pop ESI
    
    mov [esi], byte '.'
    
    mov [esi+eax], byte 'X'
    
    push edi
    
    call Set2D
    
    jmp .end
    
    


.end:
    

    pop esi
    mov esp, ebp
    pop ebp
    ret

; Funkce vezme dve hodnoty (PoziceX,PoziceY) a vrati ukazatel do 1D pole (mapy)
; int Get1D() {
;   return pos_y * width + pos_x;
; }
;
To1D:
    push ebp
    mov ebp, esp
    
    ; Menim jen EDX,EAX a ECX, nemusim zalohovat registry
          
    ; Vypocet adresy z aktualni pozice
    ; Nacti pos_y
    xor ecx, ecx
    mov cl, [pos_y]
    ; Bude vzdy kladne, nemusim cbw->cwde/movsx
    xor eax, eax
    mov al, [width]
    ;edx:eax = pos_y * width
    mul ecx  
    ; EDX muzu ignorovat, male hodnoty 
    ; pricti pos_x
    xor edx,edx
    mov dl, [pos_x]  
    add eax, edx
    
    mov esp, ebp
    pop ebp
    ret



; void Set2D(int index1D) {
;   "this->"pos_y = index1D/"this->"width;
;   "this->"pos_x = index1D%"this->"width;
;}
; !!! SAMO MAZE ARGUMENTY ZE STACKU 
Set2D:
    push ebp
    mov ebp, esp
    
    ; Menim jen EDX,EAX a ECX, nemusim zalohovat registry
    
    ; Vypocet pozice z aktualni adresy
    ; Nacti poz1D
    mov eax, [ebp+8]

    ; Bude vzdy kladne, nemusim cbw
    xor cx, cx
    xor dx,dx
    mov cl, [width]
    ;ax = poz1D(eax) / width(cx)
    ;dx = poz1D(eax) % width(cx)
    div cx  
    
    mov [pos_y], al
    mov [pos_x], dl
    
    xor eax, eax
    mov al, [pos_y]
    mov al, [pos_x]
    
    mov esp, ebp
    pop ebp
    
    ; Vyuziju toho ze RET umi mazat vstupni argument. Je to proti standardu  "cdecl (C declaration)"
    ; Ale assembler to umi. 
    ret 4 ; add esp,4 po skoku