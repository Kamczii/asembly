.686
.model flat
public _odejmij_jeden
.code
_odejmij_jeden PROC

push ebp
mov ebp, esp
push ebx

mov ebx,  [ebp+8] ; adres adresu do ebx
mov ebx, [ebx] ; adres do ebx
mov eax, [ebx]
dec eax
mov ebx,  [ebp+8] ; adres adresu do ebx
mov ebx, [ebx] ; adres do ebx
mov [ebx], eax

pop ebx
pop ebp
ret

_odejmij_jeden ENDP
END
