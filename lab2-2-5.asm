; wczytywanie i wyświetlanie tekstu wielkimi literami
; (inne znaki się nie zmieniają)
.686
.model flat
extern _ExitProcess@4 : PROC
extern __write : PROC ; (dwa znaki podkreślenia)
extern __read : PROC ; (dwa znaki podkreślenia)
extern _MessageBoxA@16 : PROC
extern _MessageBoxW@16 : PROC

public _main
.data
tekst_pocz db 10, 'Proszę napisać jakiś tekst '
db 'i nacisnac Enter', 10
koniec_t db ?
magazyn db 80 dup (?)
magazyn16 dw 80 dup (?)
nowa_linia db 10
liczba_znakow dd ?
polskie_male db 165,134,169,136,228,162,152,171,190
polskie_duze db 165,198,202,163,209,211,140,143,175
polskie_utf dw 0104H,0106H,0118H,0141H,0143H,00D3H,015AH,0179H,017BH
.code
_main PROC
; wyświetlenie tekstu informacyjnego
; liczba znaków tekstu
 mov ecx,(OFFSET koniec_t) - (OFFSET tekst_pocz)
 push ecx
 push OFFSET tekst_pocz ; adres tekstu
 push 1 ; nr urządzenia (tu: ekran - nr 1)
 call __write ; wyświetlenie tekstu początkowego
 add esp, 12 ; usuniecie parametrów ze stosu
; czytanie wiersza z klawiatury
 push 80 ; maksymalna liczba znaków
 push OFFSET magazyn
 push 0 ; nr urządzenia (tu: klawiatura - nr 0)
 call __read ; czytanie znaków z klawiatury
 add esp, 12 ; usuniecie parametrów ze stosu
; kody ASCII napisanego tekstu zostały wprowadzone
; do obszaru 'magazyn'
; funkcja read wpisuje do rejestru EAX liczbę
; wprowadzonych znaków
 mov liczba_znakow, eax
; rejestr ECX pełni rolę licznika obiegów pętli
 mov ecx, eax
 mov ebx, 0 ; indeks początkowy
ptl: mov edx, 0
	mov dl, magazyn[ebx] ; pobranie kolejnego znaku
 cmp dl, 'a'
 jb dalej ; skok, gdy znak nie wymaga zamiany
 cmp dl, 'z'
 ja polski ; skok, gdy znak nie wymaga zamiany
 sub dl, 20H ; zamiana na wielkie litery
 jmp magazynuj
 polski:
	push ecx ; zapamiętanie rejestru ecx
	push ebx ; zapamiętanie rejestru edx
	mov ecx, (OFFSET polskie_duze)-(OFFSET polskie_male) ; liczba polskich znaków
	mov ebx, 0 ; indeks początkowy
	petl:
		cmp dl, polskie_male[ebx] ; porównanie z polską małą literą
		jne incr ; jeśli to nie ta, sprawdza następną
		mov dx,  polskie_utf[2*ebx] ; przypisuje odpowiadającą dużą literę
		mov ecx,1 ; kończy iterację, ponieważ już znaleźliśmy literę
		incr:
			inc ebx ; inkrementacja indeksu
		loop petl ; sterowanie pętlą
	pop ebx
	pop ecx
; odesłanie znaku do pamięci
magazynuj:
 mov magazyn[ebx], dl
 mov magazyn16[2*ebx], dx
dalej: inc ebx ; inkrementacja indeksu
 loop ptl ; sterowanie pętlą
; wyświetlenie przekształconego tekstu

 push 0
 push OFFSET magazyn
 push OFFSET magazyn
 push 0
 call _MessageBoxA@16 ; wyświetlenie przekształconego tekstu
 add esp, 16 ; usuniecie parametrów ze stosu

 push 0
 push OFFSET magazyn16
 push OFFSET magazyn16
 push 0
 call _MessageBoxW@16 ; wyświetlenie przekształconego tekstu
 add esp, 16 ;
 push 0
 call _ExitProcess@4 ; zakończenie programu
_main ENDP
END
