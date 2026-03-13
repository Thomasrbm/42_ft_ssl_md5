-p, echo STDIN to STDOUT and append the checksum to STDOUT
• -q, quiet mode
• -r, reverse the format of the output.
• -s, print the sum of the given string
Emulate the behavior of the md5sum executable.



 For your first bonus your program must be able to parse commands from STDIN the
same way that OpenSSL does.


padding pour que taille = 

448 mod 512 + 64 = longueur multiple 512





4 registres de 32 bits (A, B, C, D) initialisés avec des constantes magiques :
A = 0x67452301
B = 0xEFCDAB89
C = 0x98BADCFE
D = 0x10325476





 4 rounds de 16 opérations chacun (64 ops total)





Round 1 — F(B,C,D) = (B AND C) OR (NOT B AND D)
Round 2 — G(B,C,D) = (B AND D) OR (C AND NOT D)
Round 3 — H(B,C,D) = B XOR C XOR D
Round 4 — I(B,C,D) = C XOR (B OR NOT D)



À chaque opération : addition modulaire 2³², rotation bit, XOR, constante pré-calculée.





Après tous les blocs, A+B+C+D concaténés → digest de 128 bits (32 hex).