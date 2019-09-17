#include <stdio.h>
#include "rsa.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct public_key_class pub[1];
    struct private_key_class priv[1];
    rsa_genKeys(pub, priv);

    printf("Private Key:\n Modulus: %lld\n Exponent: %lld\n", (long long)priv->modulus, (long long) priv->exponent);
    printf("Public Key:\n Modulus: %lld\n Exponent: %lld\n", (long long)pub->modulus, (long long) pub->exponent);

    //char message[] = "123abc";
    unsigned char message[8] = {0x48, 0x52, 0x05, 0x03, 0x01, 0x24, 0x3E, 0x00};
    int i;

    printf("Original:\n");
    for(i=0; i < 8; i++){
        printf("%02X\n", (long long)message[i]);
    }

    long long *encrypted = rsa_encrypt(message, sizeof(message), pub);
    if (!encrypted){
        fprintf(stderr, "Error in encryption!\n");
        return 1;
    }
    printf("Encrypted:\n");
    for(i=0; i < 8; i++){
        printf("%X\n", (long long)encrypted[i]);
    }

    char *decrypted = rsa_decrypt(encrypted, 8*8, priv);
    if (!decrypted){
        fprintf(stderr, "Error in decryption!\n");
        return 1;
    }
    printf("Decrypted:\n");
    for(i=0; i < 8; i++){
        printf("%02X\n", (long long)decrypted[i]);
    }

    printf("\n");
    free(encrypted);
    free(decrypted);
    return 0;
}

//int main(int argc, char **argv)
//{
//    struct public_key_class pub[1];
//    struct private_key_class priv[1];
//    rsa_genKeys(pub, priv);
//
//    printf("Private Key:\n Modulus: %lld\n Exponent: %lld\n", (long long)priv->modulus, (long long) priv->exponent);
//    printf("Public Key:\n Modulus: %lld\n Exponent: %lld\n", (long long)pub->modulus, (long long) pub->exponent);
//
//    //char message[] = "123abc";
//    //unsigned char message[8] = {0x48, 0x52, 0x05, 0x03, 0x01, 0x24, 0x00, 0x00};
//    //long long message = 0x4852050301243500;
//    long long message = 0xf1;
//    int i;
//
//    printf("long long:\n");
//    printf("%lld\n", (long long)sizeof(char));
//
//    printf("Original:\n");
//    printf("%X\n", (long long)message);
//
//    long long encrypted;
//    encrypted = rsa_modExp(message, pub->exponent, pub->modulus);
//    printf("Encrypted:\n");
//    printf("%X\n", encrypted);
//
//    long long decrypted;
//    decrypted = rsa_modExp(encrypted, priv->exponent, priv->modulus);
//    printf("Decrypted:\n");
//    printf("%X\n", decrypted);
//
//
//    printf("\n");
//
//    return 0;
//}

