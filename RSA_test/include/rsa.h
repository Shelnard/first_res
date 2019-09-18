#ifndef __RSA_H__
#define __RSA_H__

#include <stdint.h>

// This is the header file for the library librsaencrypt.a

// Change this line to the file you'd like to use as a source of primes.
// The format of the file should be one prime per line.
//unsigned char *PRIME_SOURCE_FILE = "primes.txt";


struct public_key_class{
    unsigned long long modulus;
    unsigned long long exponent;
};

struct private_key_class{
    unsigned long long modulus;
    unsigned long long exponent;
};

// This function generates public and private keys, then stores them in the structures you
// provide pointers to. The 3rd argument should be the text PRIME_SOURCE_FILE to have it use
// the location specified above in this header.
void rsa_gen_keys(struct public_key_class *pub, struct private_key_class *priv, unsigned char *PRIME_SOURCE_FILE);
void rsa_genKeys(struct public_key_class *pub, struct private_key_class *priv);

// This function will encrypt the data pointed to by message. It returns a pointer to a heap
// array containing the encrypted data, or NULL upon failure. This pointer should be freed when 
// you are finished. The encrypted data will be 8 times as large as the original data.
unsigned long long*rsa_encrypt(unsigned char *message, const unsigned long message_size, struct public_key_class *pub);

// This function will decrypt the data pointed to by message. It returns a pointer to a heap
// array containing the decrypted data, or NULL upon failure. This pointer should be freed when 
// you are finished. The variable message_size is the size in bytes of the encrypted message. 
// The decrypted data will be 1/8th the size of the encrypted data.
unsigned char *rsa_decrypt(unsigned long long*message, unsigned long message_size, struct private_key_class *pub);

#endif
