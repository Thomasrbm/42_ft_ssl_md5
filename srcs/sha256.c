#include "sha256.h"
#include <stdio.h>
#include <string.h>

// padding : msg + 0x80 + zeros jusqu'a 448 bits + taille en bits (big-endian)
// seule diff avec md5 : la taille est stockee en big-endian
static unsigned char	*sha256_pad(const char *msg, int len, int *pad_len)
{
	unsigned char	*padded;
	int			padded_len;

	padded_len = len + 1;
	while (padded_len % 64 != 56)
		padded_len++;
	padded_len += 8;
	padded = calloc(padded_len, 1);
	memcpy(padded, msg, len);
	padded[len] = 0x80;
	uint64_t len_in_bits = (uint64_t)len * 8; // ex len de 1000 : 0x1F40  = 40 1F 00 00 00 00 00 00  en little endian
	for (int i = 7; i >= 0; i--)
    	padded[padded_len - 8 + (7 - i)] = len_in_bits >> (i * 8);  // big endiant donc doit bit shift, repart de 8 avant la fin, avance de 1 (7-7, 7-6 etc)    =  shit 40 de 56 (7 octets), puis 1F 48 (6 octets etc)
	*pad_len = padded_len;
	return (padded);
}



// convertit 64 octets en 16 mots de 32 bits big-endian
// diff avec md5 : on peut pas juste memcpy, faut sblockap les octets
static void	sha256_64byte_to_16first_4bytes_blocks(uint32_t *block, const unsigned char *chunk)
{
	for (int i = 0; i < 16; i++)																																// reel nombre		// stock																		 // lu  a lenvers			
		block[i] = (chunk[i * 4] << 24) | (chunk[i * 4 + 1] << 16) | (chunk[i * 4 + 2] << 8) | chunk[i * 4 + 3];   // peut pas memcpy car copier dans l ordre ex:  1 027 489 600 = 3D 3E 3F 40. int little endian le lit a l envers => stock puis interpretre comme 1 077 886 525  en  40 3F 3E 3D  car lit a l envers
} 	// avanc de  4 donc *4 
								// nombre envoye																			// vaudrait 1 077 886 525																mais est lu comme le vrai chiffre = 1 027 423 040
// avec bitwise :  1 027 423 040 =  3D 3E 3F 40 => fabriquer dans le registre envoyer d un coup dans block de i. => stock comme 40 3F 3E 3D little endian vas l inverser et le stocker a l enver.  => interprete puis stock en 40 3F 3E 3D




// forme les block suivant a partir d un calcul qui depend des block d avant deja forme
// et ainsi de suite.
static void sha256_64byte_to_48other_4bytes_blocks(uint32_t *block)
{
	for (int i = 16; i < 64; i++)
		block[i] = SIG1(block[i - 2]) + block[i - 7] + SIG0(block[i - 15]) + block[i - 16];
}





// un round : 2 valeurs temporaires + decalage des 8 registres
// t1 = h + SIGMA1(e) + Ch(e,f,g) + K[i] + block[i]
// t2 = SIGMA0(a) + Maj(a,b,c)
// puis shift : h=g g=f f=e e=d+t1 d=c c=b b=a a=t1+t2
static void	sha256_round(uint32_t *curr_temp_hash, uint32_t *block, int i)
{
	uint32_t	t1;
	uint32_t	t2;

	t1 = curr_temp_hash[7] + SIGMA1(curr_temp_hash[4]) + CH(curr_temp_hash[4], curr_temp_hash[5], curr_temp_hash[6]) + g_k[i] + block[i];
	t2 = SIGMA0(curr_temp_hash[0]) + MAJ(curr_temp_hash[0], curr_temp_hash[1], curr_temp_hash[2]);
	curr_temp_hash[7] = curr_temp_hash[6];
	curr_temp_hash[6] = curr_temp_hash[5];
	curr_temp_hash[5] = curr_temp_hash[4];
	curr_temp_hash[4] = curr_temp_hash[3] + t1;
	curr_temp_hash[3] = curr_temp_hash[2];
	curr_temp_hash[2] = curr_temp_hash[1];
	curr_temp_hash[1] = curr_temp_hash[0];
	curr_temp_hash[0] = t1 + t2;
}

// 64 rounds avec le meme schema (pas 4 groupes differents comme md5)
static void	sha256_rounds(uint32_t *curr_temp_hash, uint32_t *block)
{
	int	i;

	for (i = 0; i < 64; i++)
		sha256_round(curr_temp_hash, block, i);
}

// fonction de compression : decode big-endian + expand + rounds + accumule
static void	sha256_handle_64bytes(uint32_t *hash, const unsigned char *chunk)
{
	uint32_t	block[64]; // 64 * 4 bytes = 256 bytes = 2048 bits
	uint32_t	curr_temp_hash[8];
	int			i;

	sha256_64byte_to_16first_4bytes_blocks(block, chunk);
	sha256_64byte_to_48other_4bytes_blocks(block);
	for (i = 0; i < 8; i++)
		curr_temp_hash[i] = hash[i];
	sha256_rounds(curr_temp_hash, block);
	for (i = 0; i < 8; i++)
		hash[i] += curr_temp_hash[i];
}

// convertit le hash en string hexa
// diff avec md5 : big-endian donc %08x sur chaque mot direct
// md5 devait lire byte par byte car little-endian
static char	*sha256_bits_to_hex(uint32_t *hash)
{
	char	*result;
	int		i;

	result = malloc(65);
	if (!result)
		return (NULL);
	for (i = 0; i < 8; i++)
		sprintf(result + i * 8, "%08x", hash[i]); // print pas bit par bit car BIG endian. (comme pb au dessus)
	result[64] = '\0';
	return (result);
}

char	*ft_sha256(const char *input)
{
	unsigned char	*padded;
	int				padded_len;
	int				byte;
	uint32_t		hash[8] = {SHA256_H0, SHA256_H1 ,SHA256_H2 ,SHA256_H3 ,SHA256_H4 ,SHA256_H5 ,SHA256_H6 ,SHA256_H7};
	
	padded = sha256_pad(input, strlen(input), &padded_len);
	if (!padded)
		return (NULL);
	for (byte = 0; byte < padded_len; byte += 64)
		sha256_handle_64bytes(hash, padded + byte);
	free(padded);
	return (sha256_bits_to_hex(hash));
}