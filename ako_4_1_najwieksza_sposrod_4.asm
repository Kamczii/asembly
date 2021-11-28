.686
.model flat
public _szukaj_max
.code
_szukaj_max PROC
push ebp ; zapisanie zawartości EBP na stosie
mov ebp, esp ; kopiowanie zawartości ESP do EBP

mov eax, [ebp+8] ; liczba x
cmp eax, [ebp+12] ; porownanie liczb x i y
jge x_wieksza ; skok, gdy x >= y

; przypadek x < y
mov eax, [ebp+12] ; liczba y
cmp eax, [ebp+16] ; porownanie liczb y i z
jge y_wieksza ; skok, gdy y >= z

z_wieksze:
; przypadek y < z i x < z
mov eax, [ebp+16] ; liczba z
cmp eax, [ebp+20] ; porownanie liczb z i d
jge wpisz_z
mov eax, [ebp+20] ; liczba d
jmp zakoncz

wpisz_z: mov eax, [ebp+16] ; liczba z
zakoncz:
pop ebp
ret

x_wieksza: ; nie bierzemy już y pod uwage
cmp eax, [ebp+16] ; porownanie x i z
jl z_wieksze ; skok, gdy x < z
; przypadek x > z
mov eax, [ebp+8] ; liczba x
cmp eax, [ebp+20] ; porownanie x i d
jl d_wieksza ; skok, gdy x < d
jmp zakoncz

y_wieksza: ; nie bierzemy już x i z pod uwage
mov eax, [ebp+12] ; liczba y
cmp eax, [ebp+20] ; porownanie y i d
jl d_wieksza ; skok, jeśli y < d
jmp zakoncz

d_wieksza:
mov eax, [ebp+20]
jmp zakoncz

_szukaj_max ENDP
END
