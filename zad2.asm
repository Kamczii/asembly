; wczytywanie i wyœwietlanie tekstu wielkimi literami 
; (inne znaki siê nie zmieniaj¹) 
 
.686 
.model flat 
extern  _ExitProcess@4 : PROC 
extern  __write : PROC ; (dwa znaki podkreœlenia) 
extern  __read  : PROC ; (dwa znaki podkreœlenia) 
public  _main 
 
.data 
tekst_pocz db 10, 'Proszê napisaæ jakiœ tekst ' 
db 'i nacisnac Enter', 10 
koniec_t   db ? 
magazyn  db 80 dup (?) 
wyjscie  db 80 dup (?) 
nowa_linia  db 10 
liczba_znakow  dd ? 
 
 
 
.code 
_main   PROC 
 
; wyœwietlenie tekstu informacyjnego 
 
; liczba znaków tekstu 
           mov     ecx,(OFFSET koniec_t) - (OFFSET tekst_pocz) 
           push    ecx 
 
           push    OFFSET tekst_pocz  ; adres tekstu 
           push    1 ; nr urz¹dzenia (tu: ekran - nr 1) 
           call    __write  ; wyœwietlenie tekstu pocz¹tkowego 
 
           add     esp, 12 ; usuniecie parametrów ze stosu 
 
; czytanie wiersza z klawiatury 
           push    80 ; maksymalna liczba znaków 
           push    OFFSET magazyn 
           push    0  ; nr urz¹dzenia (tu: klawiatura - nr 0) 
           call    __read ; czytanie znaków z klawiatury 
           add     esp, 12 ; usuniecie parametrów ze stosu 
; kody ASCII napisanego tekstu zosta³y wprowadzone 
; do obszaru 'magazyn' 
 
 
; funkcja read wpisuje do rejestru EAX liczbê 
; wprowadzonych znaków 
           mov     liczba_znakow, eax 
; rejestr ECX pe³ni rolê licznika obiegów pêtli 
           mov     ecx, eax 
           mov     ebx, 0  ; indeks pocz¹tkowy 
           mov     edi, 0
ptl:       mov     dl, magazyn[ebx] ; pobranie kolejnego znaku 
           cmp     dl, '\' 
           je porownaj
           jb      wpisz   ; skok, gdy znak nie wymaga zamiany 
           cmp     dl, 'z' 
           ja      wpisz   ; skok, gdy znak nie wymaga zamiany 
        
; odes³anie znaku do pamiêci 
           wpisz: mov     wyjscie[edi], dl 
           jmp dalej
           porownaj:
            mov dl, magazyn[ebx+1]
            cmp dl,'t'
            jne wpisz_slash
            inc ebx
            mov wyjscie[edi],' '
            inc edi
            mov wyjscie[edi],' '
            inc edi
            mov wyjscie[edi],' '
            inc edi
            mov wyjscie[edi],' '
            add liczba_znakow,4
            jmp dalej
            wpisz_slash:
                mov wyjscie[edi],'\'
dalej:     inc     ebx     ; inkrementacja indeksu 
           inc     edi
           loop    ptl     ; sterowanie pêtl¹ 
 wyswietl:
; wyœwietlenie przekszta³conego tekstu 
           push    liczba_znakow 
           push    OFFSET wyjscie 
           push    1 
           call    __write  ; wyœwietlenie przekszta³conego 
           add     esp, 12  ; usuniecie parametrów ze stosu 
 
           push    0 
           call    _ExitProcess@4      ; zakoñczenie programu 
_main ENDP 
END
