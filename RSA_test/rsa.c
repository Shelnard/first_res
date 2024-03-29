#include <stdio.h>
//#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "rsa.h"

unsigned char buffer[1024];
const int MAX_DIGITS = 50;
int i,j = 0;

//struct public_key_class{
//    unsigned long long modulus;
//    unsigned long long exponent;
//};
//
//struct private_key_class{
//    unsigned long long modulus;
//    unsigned long long exponent;
//};


// This should totally be in the math library.
long long gcd(long long a, long long b)
{
    long long c;
    while ( a != 0 ) {
        c = a; a = b%a;  b = c;
    }
    return b;
}


long long ExtEuclid(long long a, long long b)
{
    long long x = 0, y = 1, u = 1, v = 0, gcd = b, m, n, q, r;
    while (a!=0) {
        q = gcd/a; r = gcd % a;
        m = x-u*q; n = y-v*q;
        gcd = a; a = r; x = u; y = v; u = m; v = n;
    }
    return y;
}

unsigned long long rsa_modExp(unsigned long long b, unsigned long long e, unsigned long long m)
{
    if (b < 0 || e < 0 || m <= 0){
        exit(1);
    }
    b = b % m;
    if(e == 0) return 1;
    if(e == 1) return b;
    if( e % 2 == 0){
        return ( rsa_modExp(b * b % m, e/2, m) % m );
    }
    if( e % 2 == 1){
        return ( b * rsa_modExp(b, (e-1), m) % m );
    }

}

void rsa_genKeys(struct public_key_class *pub, struct private_key_class *priv)
{
    unsigned long long p = 76213;
    unsigned long long q = 19553;

    unsigned long long e = powl(2, 8) + 1;
    unsigned long long d = 0;
    unsigned long long max = 0;
    unsigned long long phi_max = 0;

    max = p*q;
    phi_max = (p-1)*(q-1);

    // Next, we need to choose a,b, so that a*max+b*e = gcd(max,e). We actually only need b
    // here, and in keeping with the usual notation of RSA we'll call it d. We'd also like
    // to make sure we get a representation of d as positive, hence the while loop.
    d = ExtEuclid(phi_max,e);
    while(d < 0){
        d = d+phi_max;
    }

    printf("primes are %lld and %lld\n",(unsigned long long)p, (unsigned long long )q);
    // We now store the public / private keys in the appropriate structs
    pub->modulus = max;
    pub->exponent = e;

    priv->modulus = max;
    priv->exponent = d;

}

// Calling this function will generate a public and private key and store them in the pointers
// it is given. 
void rsa_gen_keys(struct public_key_class *pub, struct private_key_class *priv, unsigned char *PRIME_SOURCE_FILE)
{
    FILE *primes_list;
    if(!(primes_list = fopen(PRIME_SOURCE_FILE, "r"))){
        fprintf(stderr, "Problem reading %s\n", PRIME_SOURCE_FILE);
        exit(1);
    }

    // count number of primes in the list
    unsigned long long prime_count = 0;
    do{
        int bytes_read = fread(buffer,1,sizeof(buffer)-1, primes_list);
        buffer[bytes_read] = '\0';
        for (i=0 ; buffer[i]; i++){
            if (buffer[i] == '\n'){
                prime_count++;
            }
        }
    }
    while(feof(primes_list) == 0);


    // choose random primes from the list, store them as p,q

    unsigned long long p = 76213;
    unsigned long long q = 19553;

    unsigned long long e = powl(2, 8) + 1;
    unsigned long long d = 0;
    //unsigned char prime_buffer[MAX_DIGITS];
    unsigned long long max = 0;
    unsigned long long phi_max = 0;

    //    srand(time(NULL));
    //
    //    do{
    //        // a and b are the positions of p and q in the list
    //        int a =  (double)rand() * (prime_count+1) / (0x7FFF+1.0);
    //        int b =  (double)rand() * (prime_count+1) / (0x7FFF+1.0);
    //
    //        // here we find the prime at position a, store it as p
    //        rewind(primes_list);
    //        for(i=0; i < a + 1; i++){
    //            //  for(j=0; j < MAX_DIGITS; j++){
    //            //	prime_buffer[j] = 0;
    //            //  }
    //            fgets(prime_buffer,sizeof(prime_buffer)-1, primes_list);
    //        }
    //        p = atol(prime_buffer);
    //
    //        // here we find the prime at position b, store it as q
    //        rewind(primes_list);
    //        for(i=0; i < b + 1; i++){
    //            for(j=0; j < MAX_DIGITS; j++){
    //                prime_buffer[j] = 0;
    //            }
    //            fgets(prime_buffer,sizeof(prime_buffer)-1, primes_list);
    //        }
    //        q = atol(prime_buffer);
    //
    //        max = p*q;
    //        phi_max = (p-1)*(q-1);
    //    }
    //    while(!(p && q) || (p == q) || (gcd(phi_max, e) != 1));
    max = p*q;
    phi_max = (p-1)*(q-1);

    // Next, we need to choose a,b, so that a*max+b*e = gcd(max,e). We actually only need b
    // here, and in keeping with the usual notation of RSA we'll call it d. We'd also like
    // to make sure we get a representation of d as positive, hence the while loop.
    d = ExtEuclid(phi_max,e);
    while(d < 0){
        d = d+phi_max;
    }

    printf("primes are %lld and %lld\n",(unsigned long long)p, (unsigned long long )q);
    // We now store the public / private keys in the appropriate structs
    pub->modulus = max;
    pub->exponent = e;

    priv->modulus = max;
    priv->exponent = d;
}


unsigned long long *rsa_encrypt(unsigned char *message, unsigned long message_size,
        struct public_key_class *pub)
{
    unsigned long long *encrypted = malloc(sizeof(unsigned long long)*message_size);
    if(encrypted == NULL){
        fprintf(stderr,
                "Error: Heap allocation failed.\n");
        return NULL;
    }
    unsigned long long i = 0;
    for(i=0; i < message_size; i++){
        encrypted[i] = rsa_modExp(message[i], pub->exponent, pub->modulus);
    }
    return encrypted;
}


unsigned char *rsa_decrypt(unsigned long long *message,
        unsigned long message_size,
        struct private_key_class *priv)
{
    if(message_size % sizeof(unsigned long long) != 0){
        fprintf(stderr,
                "Error: message_size is not divisible by %d, so cannot be output of rsa_encrypt\n", (int)sizeof(unsigned long long));
        return NULL;
    }
    // We allocate space to do the decryption (temp) and space for the output as a unsigned char array
    // (decrypted)
    unsigned char *decrypted = malloc(message_size/sizeof(unsigned long long));
    unsigned char *temp = malloc(message_size);
    if((decrypted == NULL) || (temp == NULL)){
        fprintf(stderr,
                "Error: Heap allocation failed.\n");
        return NULL;
    }
    // Now we go through each 8-byte chunk and decrypt it.
    unsigned long long i = 0;
    for(i=0; i < message_size/8; i++){
        temp[i] = rsa_modExp(message[i], priv->exponent, priv->modulus);
    }
    // The result should be a number in the unsigned char range, which gives back the original byte.
    // We put that into decrypted, then return.
    for(i=0; i < message_size/8; i++){
        decrypted[i] = temp[i];
    }
    free(temp);
    return decrypted;
}
