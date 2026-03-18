#include "md5.h"
#include <stdio.h>
#include <string.h>

// padding : msg + 0x80 + zeros jusqu'a 448 bits + taille en bits (little-endian)
static unsigned char	*md5_pad(const char *msg, int len, int *pad_len)
{
	unsigned char	*padded;
	uint64_t		len_in_bits;
	int			padded_len;

	padded_len = len + 1;
	while (padded_len % 64 != 56)
		padded_len++;
	padded_len += 8;
	padded = calloc(padded_len, 1);
	memcpy(padded, msg, len);
	padded[len] = 0x80;
	len_in_bits = (uint64_t)len * 8;
	memcpy(padded + padded_len - 8, &len_in_bits, 8);
	*pad_len = padded_len;
	return (padded);
}

// un round : addition + rotation + decalage des registres
// a + LETTRE + T[i]
// temp = rotation de tmp sur S[i]  + b

// a = d
// d  = c
// c = b
// b  = tmp
static void	md5_round(uint32_t *curr_temp_hash, uint32_t f, int i)
{
	uint32_t	tmp;

	tmp = curr_temp_hash[0] + f + g_t[i];
	tmp = ROTL(tmp, g_s[i]) + curr_temp_hash[1];
	curr_temp_hash[0] = curr_temp_hash[3];
	curr_temp_hash[3] = curr_temp_hash[2];
	curr_temp_hash[2] = curr_temp_hash[1];
	curr_temp_hash[1] = tmp;
}

// les 4 groupes de 16 rounds
// Chaque LETTRE shuffle les hash courrant (dans leur etat actuel)  = un HASH de 32bit a partir des hash courrant 
// + block de i pour que le hash dependent du message qu on hash

// on prend les block dans ordre decide par le createur. 
static void	md5_rounds(uint32_t *curr_temp_hash, uint32_t *block)
{
	int	i;

	i = -1;
	for (i = 0; i < 16; i++)
		md5_round(curr_temp_hash, F(curr_temp_hash[1], curr_temp_hash[2], curr_temp_hash[3]) + block[i], i); // block dans l ordre 
	for (; i < 32; i++)
		md5_round(curr_temp_hash, G(curr_temp_hash[1], curr_temp_hash[2], curr_temp_hash[3]) + block[(5 * i + 1) % 16], i); //  ordre : 1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12
	for (; i < 48; i++)
		md5_round(curr_temp_hash, H(curr_temp_hash[1], curr_temp_hash[2], curr_temp_hash[3]) + block[(3 * i + 5) % 16], i); // ordre : 5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2
	for (; i < 64; i++)
		md5_round(curr_temp_hash, I(curr_temp_hash[1], curr_temp_hash[2], curr_temp_hash[3]) + block[(7 * i) % 16], i); // ordre : 0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9
}

// convertit 64 octets en 16 mots de 32 bits little-endian
static void md5_64byte_to_16_4bytes_blocks(uint32_t *block,
    const unsigned char *chunk_padded)
{
    memcpy(block, chunk_padded, 64); // ecrit sur tout le tableau mais chaque case = bloc bien divided
}

// fonction de compression : decode + rounds + accumule
static void	md5_handle_64bytes(uint32_t *hash, const unsigned char *chunk_padded)
{
	uint32_t	block[16];  // chaque case fait 4 octets,  16 * 4 = 64 byte = 512 bits
	uint32_t	curr_temp_hash[4];

	md5_64byte_to_16_4bytes_blocks(block, chunk_padded); 
	curr_temp_hash[0] = hash[0];
	curr_temp_hash[1] = hash[1];
	curr_temp_hash[2] = hash[2];
	curr_temp_hash[3] = hash[3];
	md5_rounds(curr_temp_hash, block);
	hash[0] += curr_temp_hash[0];
	hash[1] += curr_temp_hash[1];
	hash[2] += curr_temp_hash[2];
	hash[3] += curr_temp_hash[3];
}

// convertit le hash binaire en string hexa

// donc notre hash est binaire pour le debut ? puis on prend le binaire bytes[i] = 8bits  
//  on le convertie avec %x  mais faut faire 02 avant pour print soit 2 char convertit soit 0 et 1 char convertit 
static char	*md5_bits_to_hex(uint32_t *hash)
{
	unsigned char	*bytes;
	char			*result;
	int				i;
 
	result = malloc(33);
	bytes = (unsigned char *)hash;
	for (i = 0; i < 16; i++)
		sprintf(result + i * 2, "%02x", bytes[i]);
	return (result);
}

char	*ft_md5(const char *input)
{
	unsigned char	*padded;
	int				padded_len;
	int				byte;
	uint32_t		hash[4] = {MD5_A0, MD5_B0, MD5_C0, MD5_D0};
 
	padded = md5_pad(input, strlen(input), &padded_len);
	if (!padded)
		return (NULL);
	for (byte = 0; byte < padded_len; byte += 64)
		md5_handle_64bytes(hash, padded + byte);
	free(padded);
	return (md5_bits_to_hex(hash));
}
