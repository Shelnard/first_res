#include <stdio.h>
#include "rsa.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct public_key_class pub[1];
    struct private_key_class priv[1];
    rsa_genKeys(pub, priv);

    printf("Private Key:\n Modulus: %lld\n Exponent: %lld\n", priv->modulus, priv->exponent);
    printf("Public Key:\n Modulus: %lld\n Exponent: %lld\n", pub->modulus, pub->exponent);

    //char message[] = "123abc";
    unsigned char message[8] = {0xFF, 0x52, 0x05, 0x03, 0x01, 0x24, 0x3E, 0xFF};
    int i;

    printf("Original:\n");
    for(i=0; i < 8; i++){
        printf("%02X\n", message[i]);
    }

    unsigned long long*encrypted = rsa_encrypt(message, sizeof(message), pub);
    if (!encrypted){
        fprintf(stderr, "Error in encryption!\n");
        return 1;
    }
    printf("Encrypted:\n");
    for(i=0; i < 8; i++){
        printf("%X\n", encrypted[i]);
    }

    unsigned char *decrypted = rsa_decrypt(encrypted, 8*8, priv);
    if (!decrypted){
        fprintf(stderr, "Error in decryption!\n");
        return 1;
    }
    printf("Decrypted:\n");
    for(i=0; i < 8; i++){
        printf("%02X\n", decrypted[i]);
    }

    printf("\n");
    free(encrypted);
    free(decrypted);
    return 0;
}
