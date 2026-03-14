-p, echo STDIN to STDOUT and append the checksum to STDOUT
• -q, quiet mode
• -r, reverse the format of the output.
• -s, print the sum of the given string
Emulate the behavior of the md5sum executable.



 For your first bonus your program must be able to parse commands from STDIN the
same way that OpenSSL does.


sans rien (ni file ni flag)  lit tout depuis stdin 
comme heredoc => concatenne tout jusqu au EOF. 

hash et -


-p (inpu)=          (ecrase r)
-q quiet, rien ...  (ecrase R)
-r hash file
-s 


md5 -r /tmp/file
md4-help

 md5sum --quiet Readme                                                                                                                                     ✔ 
md5sum: the --quiet option is meaningful only when verifying checksums
Try 'md5sum --help' for more information.


=> flags pas testables => faire docker pour old version


bonus = ne pas prendre depuis argv mais depuis stdin
faire ./ft_ssl 

ft_ssl > md5 -q fichier


 echo "Pity the living." | ./ft_ssl md5 -q -r
 prendre depuis STDIN 

char *ft_md5(char *input)
{


}

===================== EXEMPLE ============================

"abc"


ETAPE 1



0x61 0x62 0x63  = longeur 24 bits

61 62 63


on ajoute un octet de delimiteur
pour marquer fin du truc

0000 0001  vaut 0x80  pck on inverse et met le 1 en tete 1000 0000



on remplit de 0 jusqu a 56 


61 62 63 80 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
                       ^ on est à l'octet 56




les 8 dernier octets sont pour la longueur

abc = 3 octets = 24 bits. 24 en hexa = 0x18.




Exemple normal (big-endian) : 00 00 00 00 00 00 00 18
Little-endian :               18 00 00 00 00 00 00 00
61 62 63 80 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 18 00 00 00 00 00 00 00



on fait en little endian ( octet le plus gros en premier ) car fait en 1991




ETAPE 2


DIVISER les 64 otet par 16

16 bloc de 4 octets / bytes


512 bit en 16 x 32bits

M[0] à M[15]. (des mots / words)

61 62 63 80 | 00 00 00 00 | 00 00 00 00 | 00 00 00 00

00 00 00 00 | 00 00 00 00 | 00 00 00 00 | 00 00 00 00

00 00 00 00 | 00 00 00 00 | 00 00 00 00 | 00 00 00 00

00 00 00 00 | 00 00 00 00 | 18 00 00 00 | 00 00 00 00


ETAPE 3


5 valeurs de base toujours les memes 


A = 0x67452301
B = 0xEFCDAB89
C = 0x98BADCFE
D = 0x10325476



ETAPE 4


Table T et S 



T[i] = floor( abs( sin(i+1) ) × 2^32 )

sur 64 


T[0]  = 0xD76AA478
T[1]  = 0xE8C7B756
T[2]  = 0x242070DB
T[3]  = 0xC1BDCEEE
...
T[63] = 0xEB86D391





S

liste figée de 64 nombres

int S[64] = {
     7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
     5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
     4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
     6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};








ETAPE 5 



on determine 4 lettres a partir de ABCD

FGHI

operation binaires 


Round 1 : F(B,C,D) = (B AND C) OR (NOT B AND D)   1 a 16
Round 2 : G(B,C,D) = (B AND D) OR (C AND NOT D)
Round 3 : H(B,C,D) = B XOR C XOR D
Round 4 : I(B,C,D) = C XOR (B OR NOT D)         48 a 64



chaque round = 



1. calculer F(B,C,D)  ← la fonction du round en cours
2. temp = A + F + M[k] + T[i]   ← tout additionner
3. temp = rotation_gauche(temp, S[i])   ← faire tourner les bits
4. B = B + temp   ← mettre à jour B
5. A, B, C, D = D, A, B, C   ← décaler les rôles




ETAPE 6 

A_final = A + 0x67452301
B_final = B + 0xEFCDAB89
C_final = C + 0x98BADCFE
D_final = D + 0x10325476


ETAPE 7


On écrit chaque registre en little-endian

A = 0x90015098
      ^^ ^^ ^^ ^^
      98 50 01 90   ← lu de droite à gauche



EXEMPLE :

A = 0x90015098  →  98 50 01 90
B = 0x3CD24FB0  →  B0 4F D2 3C
C = 0xD6963F7D  →  7D 3F 96 D6
D = 0x28E17F72  →  72 7F E1 28

98 50 01 90 B0 4F D2 3C 7D 3F 96 D6 72 7F E1 28



900150983cd24fb0d6963f7d28e17f72



========= SOURCES ========== 