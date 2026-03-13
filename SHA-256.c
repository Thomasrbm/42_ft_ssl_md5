p, echo STDIN to STDOUT and append the checksum to STDOUT
• -q, quiet mode
• -r, reverse the format of the output.
• -s, print the sum of the given string








8 registres de 32 bits (a→h), initialisés avec les racines carrées des 8 premiers nombres premiers :
a = 0x6a09e667   e = 0x510e527f
b = 0xbb67ae85   f = 0x9b05688c
c = 0x3c6ef372   g = 0x1f83d9ab
d = 0xa54ff53a   h = 0x5be0cd19



Expansion du message (key schedule)
Chaque bloc de 512 bits → 64 mots de 32 bits (W[0..63]) via :
W[i] = W[i-16] + σ0(W[i-15]) + W[i-7] + σ1(W[i-2])




4. Compression — 64 rounds
À chaque round :
T1 = h + Σ1(e) + Ch(e,f,g) + K[i] + W[i]
T2 = Σ0(a) + Maj(a,b,c)

Ch(e,f,g)  = (e AND f) XOR (NOT e AND g)
Maj(a,b,c) = (a AND b) XOR (a AND c) XOR (b AND c)
Σ0(a)      = ROTR²(a) XOR ROTR¹³(a) XOR ROTR²²(a)
Σ1(e)      = ROTR⁶(e) XOR ROTR¹¹(e) XOR ROTR²⁵(e)



5. Output
Après tous les blocs, les 8 registres sont additionnés aux valeurs initiales et concaténés → 256 bits (64 hex).