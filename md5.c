// -p, echo STDIN to STDOUT and append the checksum to STDOUT
// • -q, quiet mode
// • -r, reverse the format of the output.
// • -s, print the sum of the given string
// Emulate the behavior of the md5sum executable.



//  For your first bonus your program must be able to parse commands from STDIN the
// same way that OpenSSL does.


// sans rien (ni file ni flag)  lit tout depuis stdin 
// comme heredoc => concatenne tout jusqu au EOF. 

// hash et -


// -p (inpu)=          (ecrase r)
// -q quiet, rien ...  (ecrase R)
// -r hash file
// -s 


// md5 -r /tmp/file
// md4-help

//  md5sum --quiet Readme                                                                                                                                     ✔ 
// md5sum: the --quiet option is meaningful only when verifying checksums
// Try 'md5sum --help' for more information.


// => flags pas testables => faire docker pour old version


// bonus = ne pas prendre depuis argv mais depuis stdin
// faire ./ft_ssl 

// ft_ssl > md5 -q fichier


//  echo "Pity the living." | ./ft_ssl md5 -q -r
//  prendre depuis STDIN 

#include <stdint.h>
#include <stddef.h>

static size_t   padding(unsigned char *hash, char *input, size_t len)
{
    size_t      padded_len;
    uint64_t    bit_len;

    ft_memcpy(hash, input, len);
    hash[len] = 0x80;             // bit 1 obligatoire apres le message (spec MD5)
    padded_len = len + 1;
    while (padded_len % 64 != 56) // si nombre etait plus grand, garde les 8 pour la len.
        hash[padded_len++] = 0x00;
    bit_len = (uint64_t)len * 8;  // unint pas double car stock pas les bits pareil
    ft_memcpy(hash + padded_len, &bit_len, 8);
    padded_len += 8;
    return (padded_len);
}

static void     split_blocks_m(unsigned char *hash, uint32_t *m)
{
    int i;

    i = 0;
    while (i < 16)
    {
        m[i] = (uint32_t)hash[i * 4]
            | (uint32_t)hash[i * 4 + 1] << 8
            | (uint32_t)hash[i * 4 + 2] << 16
            | (uint32_t)hash[i * 4 + 3] << 24;
        i++;
    }
}

static uint32_t rotate_left(uint32_t x, uint32_t n)
{
    return ((x << n) | (x >> (32 - n)));
}

static void     md5_rounds(uint32_t *m, uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d)
{	
    uint32_t    T[64];
    uint32_t    S[64] = {
		7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
        5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
        4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
        6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
    };
	
    for (int i = 0; i < 64; i++)
    {
		T[i] = (uint32_t)(fabs(sin(i + 1)) * 4294967296.0); // 2^32 = 4294967296
        i++;
    }
	
	uint32_t    f;
    uint32_t    k;
	uint32_t    temp;
    for  (int i = 0; i < 64; i++)
    {
        if (i < 16)
        {
            f = (*b & *c) | (~*b & *d);  // F
            k = i;
        }
        else if (i < 32)
        {
            f = (*b & *d) | (*c & ~*d);  // G
            k = (5 * i + 1) % 16;
        }
        else if (i < 48)
        {
            f = *b ^ *c ^ *d;            // H
            k = (3 * i + 5) % 16;
        }
        else
        {
            f = *c ^ (*b | ~*d);         // I
            k = (7 * i) % 16;
        }
        temp = *a + f + m[k] + T[i];
        temp = rotate_left(temp, S[i]);
        temp = temp + *b;
        *a = *d;
        *d = *c;
        *c = *b;
        *b = temp;
    }
}

static char     *md5_final_concat(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
    unsigned char   digest[16];
    char            *result;
    int             i;
    char            *hex = "0123456789abcdef";

    // colle a b c d en little-endian dans digest
    ft_memcpy(digest,      &a, 4);
    ft_memcpy(digest + 4,  &b, 4);
    ft_memcpy(digest + 8,  &c, 4);
    ft_memcpy(digest + 12, &d, 4);
    // convertit les 16 octets en 32 caracteres hex
    result = malloc(33);
    i = 0;
    while (i < 16)
    {
        result[i * 2]     = hex[digest[i] >> 4];  // octet fort
        result[i * 2 + 1] = hex[digest[i] & 0xF]; // octet faible
        i++;
    }
    result[32] = '\0';
    return (result);
}

char            *ft_md5(char *input)
{
    unsigned char   hash[512]; // unsigned : sinon les char lisent mal (-127 128)
    size_t          len;
    size_t          padded_len;
    uint32_t        m[16];

    len = ft_strlen(input);
    padded_len = padding(hash, input, len);
    split_blocks_m(hash, m);  // 

	uint32_t        a;
    uint32_t        b;
    uint32_t        c;
    uint32_t        d;

    a = 0x67452301;
    b = 0xEFCDAB89;
    c = 0x98BADCFE;
    d = 0x10325476;

    md5_rounds(m, &a, &b, &c, &d);

	a += 0x67452301;
    b += 0xEFCDAB89;
    c += 0x98BADCFE;
    d += 0x10325476;

    return (md5_final_concat(a, b, c, d));
}



// ===================== EXEMPLE ============================

// "abc"


// ETAPE 1



// 0x61 0x62 0x63  = longeur 24 bits

// 61 62 63


// on ajoute un octet de delimiteur
// pour marquer fin du truc

// 0000 0001  vaut 0x80  pck on inverse et met le 1 en tete 1000 0000



// on remplit de 0 jusqu a 56 


// 61 62 63 80 00 00 00 00 00 00 00 00 00 00 00 00
// 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// 00 00 00 00 00 00 00 00
// 					   ^ on est à l'octet 56




// les 8 dernier octets sont pour la longueur

// abc = 3 octets = 24 bits. 24 en hexa = 0x18.




// Exemple normal (big-endian) : 00 00 00 00 00 00 00 18
// Little-endian :               18 00 00 00 00 00 00 00
// 61 62 63 80 00 00 00 00 00 00 00 00 00 00 00 00
// 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
// 00 00 00 00 00 00 00 00 18 00 00 00 00 00 00 00



// on fait en little endian ( octet le plus gros en premier ) car fait en 1991




// ETAPE 2


// DIVISER les 64 otet par 16

// 16 bloc de 4 octets / bytes


// 512 bit en 16 x 32bits

// M[0] à M[15]. (des mots / words)

// 61 62 63 80 | 00 00 00 00 | 00 00 00 00 | 00 00 00 00

// 00 00 00 00 | 00 00 00 00 | 00 00 00 00 | 00 00 00 00

// 00 00 00 00 | 00 00 00 00 | 00 00 00 00 | 00 00 00 00

// 00 00 00 00 | 00 00 00 00 | 18 00 00 00 | 00 00 00 00


// ETAPE 3


// 5 valeurs de base toujours les memes 


// A = 0x67452301
// B = 0xEFCDAB89
// C = 0x98BADCFE
// D = 0x10325476



// ETAPE 4


// Table T et S 



// T[i] = floor( abs( sin(i+1) ) × 2^32 )

// sur 64 


// T[0]  = 0xD76AA478
// T[1]  = 0xE8C7B756
// T[2]  = 0x242070DB
// T[3]  = 0xC1BDCEEE
// ...
// T[63] = 0xEB86D391





// S

// liste figée de 64 nombres

// int S[64] = {
// 	 7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
// 	 5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
// 	 4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
// 	 6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
// };








// ETAPE 5 



// on determine 4 lettres a partir de ABCD

// FGHI

// operation binaires 


// Round 1 : F(B,C,D) = (B AND C) OR (NOT B AND D)   1 a 16
// Round 2 : G(B,C,D) = (B AND D) OR (C AND NOT D)
// Round 3 : H(B,C,D) = B XOR C XOR D
// Round 4 : I(B,C,D) = C XOR (B OR NOT D)         48 a 64



// chaque round = 



// 1. calculer F(B,C,D)  ← la fonction du round en cours
// 2. temp = A + F + M[k] + T[i]   ← tout additionner
// 3. temp = rotation_gauche(temp, S[i])   ← faire tourner les bits
// 4. B = B + temp   ← mettre à jour B
// 5. A, B, C, D = D, A, B, C   ← décaler les rôles




// ETAPE 6 

// A_final = A + 0x67452301
// B_final = B + 0xEFCDAB89
// C_final = C + 0x98BADCFE
// D_final = D + 0x10325476


// ETAPE 7


// On écrit chaque registre en little-endian

// A = 0x90015098
// 	  ^^ ^^ ^^ ^^
// 	  98 50 01 90   ← lu de droite à gauche



// EXEMPLE :

// A = 0x90015098  →  98 50 01 90
// B = 0x3CD24FB0  →  B0 4F D2 3C
// C = 0xD6963F7D  →  7D 3F 96 D6
// D = 0x28E17F72  →  72 7F E1 28

// 98 50 01 90 B0 4F D2 3C 7D 3F 96 D6 72 7F E1 28



// 900150983cd24fb0d6963f7d28e17f72



// ========= SOURCES ========== 