; wczytywanie i wyświetlanie tekstu wielkimi literami
; (inne znaki się nie zmieniają)
.686
.model flat
extern _ExitProcess@4 : PROC
extern __write : PROC ; (dwa znaki podkreślenia)
extern __read : PROC ; (dwa znaki podkreślenia)
public _main
.data
tekst_pocz db 10, 'Proszę napisać jakiś tekst '
db 'i nacisnac Enter', 10
koniec_t db ?
magazyn db 80 dup (?)
nowa_linia db 10
liczba_znakow dd ?
polskie_male db 165,134,169,136,228,162,152,171,190
polskie_duze db 164,143,168,157,227,224,151,141,189
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
ptl: mov dl, magazyn[ebx] ; pobranie kolejnego znaku
 cmp dl, 'a'
 jb dalej ; skok, gdy znak nie wymaga zamiany
 cmp dl, 'z'
 ja polski ; skok, gdy znak nie wymaga zamiany
 sub dl, 20H ; zamiana na wielkie litery
 polski:
	push ecx ; zapamiętanie rejestru ecx
	push ebx ; zapamiętanie rejestru edx
	mov ecx, (OFFSET polskie_duze)-(OFFSET polskie_male) ; liczba polskich znaków
	mov ebx, 0 ; indeks początkowy
	petl:
		cmp dl, polskie_male[ebx] ; porównanie z polską małą literą
		jne incr ; jeśli to nie ta, sprawdza następną
		mov dl,  polskie_duze[ebx] ; przypisuje odpowiadającą dużą literę
		mov ecx,1 ; kończy iterację, ponieważ już znaleźliśmy literę
		incr:
			inc ebx ; inkrementacja indeksu
		loop petl ; sterowanie pętlą
	pop ebx
	pop ecx
; odesłanie znaku do pamięci
 mov magazyn[ebx], dl
dalej: inc ebx ; inkrementacja indeksu
 loop ptl ; sterowanie pętlą
; wyświetlenie przekształconego tekstu
 push liczba_znakow
 push OFFSET magazyn
 push 1
 call __write ; wyświetlenie przekształconego tekstu
 add esp, 12 ; usuniecie parametrów ze stosu
 push 0
 call _ExitProcess@4 ; zakończenie programu
_main ENDP
END
