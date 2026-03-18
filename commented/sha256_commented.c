

meme flags


char *ft_sha256()
{


}







--------------------------- EXEMPLE ---------------------------

"abc"



ETAPE 1 

'a' = 0x61 = 01100001
'b' = 0x62 = 01100010
'c' = 0x63 = 01100011


0110 0001  0110 0010  0110 0011



ETAPE 2

ajoute juste 1 seul bit 1

0110 0001  0110 0010  0110 0011 1


ajoute 0 jusqu a 448 (64 dernier pour la taille)


24 en 64 bits = 0000 0000 0000 0000 ... 0001 1000
                ←————————— 64 bits —————————————→





ETAPE 3

valeurs de bases.
racine caree de 8 premier nombres premiers


H0 = 6a09e667   (racine carrée de 2)
H1 = bb67ae85   (racine carrée de 3)
H2 = 3c6ef372   (racine carrée de 5)
H3 = a54ff53a   (racine carrée de 7)
H4 = 510e527f   (racine carrée de 11)
H5 = 9b05688c   (racine carrée de 13)
H6 = 1f83d9ab   (racine carrée de 17)
H7 = 5be0cd19   (racine carrée de 19)




ETAPE 4


tableau de constantes k (racines carre des 64 nombres premiers)


Les premières :
K[0]  = 428a2f98
K[1]  = 71374491
K[2]  = b5c0fbcf
K[3]  = e9b5dba5
K[4]  = 3956c25b
K[5]  = 59f111f1
...
K[63] = c67178f2



ETAPE 5

16 mots de 32bits


ETAPE 6

 Étendre W à 64 mots

 W[16] à W[63] avec cette formule :


W[i] = σ1(W[i-2]) + W[i-7] + σ0(W[i-15]) + W[i-16]


σ0 et σ1 =  opérations sur les bits.

ROTR(x, n) = rotation à droite de n bits. Exemple avec 8 bits pour simplifier :
ROTR(10110001, 2) = 01101100
         ↑↑ ces 2 bits partent à gauche
    
SHR(x, n) = décalage à droite de n bits (les bits qui sortent sont perdus, des 0 entrent à gauche) :
SHR(10110001, 2) = 00101100
                   ↑↑ deux zéros ajoutés à gauche




ETAPE 7



a = H0 = 6a09e667
b = H1 = bb67ae85
c = H2 = 3c6ef372
d = H3 = a54ff53a
e = H4 = 510e527f
f = H5 = 9b05688c
g = H6 = 1f83d9ab
h = H7 = 5be0cd19



on fait 64 round sur chaque mots 

T1 = h + Σ1(e) + Ch(e,f,g) + K[0] + W[0]






ÉTAPE 8


H0 = H0_initial + a_final   (mod 2^32)


sur les 8 mots 










