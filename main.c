#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

// Function to find GCD of two numbers
uint64_t findGCD(uint64_t n1, uint64_t n2) {
	int i;
	uint64_t gcd;

	for(i = 1; i <= n1 && i <= n2; ++i) {
		if(n1 % i == 0 && n2 % i == 0)
			gcd = i;
	}

	return gcd;
}
int64_t modInverse(uint64_t a, uint64_t m);

// Function to find a^b mod n
int powMod(long a, long b, long n) {
	long long x = 1, y = a;

	while (b > 0) {
		if (b % 2 == 1)
			x = (x * y) % n;
		y = (y * y) % n; // Squaring the base
		b /= 2;
	}

	return x % n;
}
//Function to encrypt a message m
uint64_t encrypt (uint64_t e, uint64_t n, uint64_t data) {

    uint64_t cipher;
	cipher = powMod(data, e, n);
}
//Function for naive decryption of cipher text
uint64_t decrypt (uint64_t d, uint64_t n, uint64_t cipher) {

    uint64_t decrypt;
	decrypt = powMod(cipher, d, n);
	return decrypt;
}

//Function for decryption of cipher text using Chinese Remainder Theorem.
uint64_t decrypt_crt (uint64_t d, uint64_t n, uint64_t p, uint64_t q, uint64_t c) {

    uint64_t decrypt;
	uint64_t c_1, c_2, d_1, d_2;
	c_1 = c % p;
	c_2 = c % q;
	d_1 = d % (p-1);
	d_2 = d % (q-1);

	uint64_t vp, vq;
	vp = powMod(c_1, d_1, p);
	vq = powMod(c_2, d_2, q);

	uint64_t xp, xq, vpq;

	xp = q * modInverse(q,p);
	xq = p * modInverse(p,q);

    decrypt = (xp*vp + xq*vq) % n;

    return decrypt;

}
int64_t modInverse(uint64_t a, uint64_t m)
{
    uint64_t m0 = m;
    int64_t y = 0, x = 1;

    if (m == 1)
        return 0;

    while (a > 1) {
        // q is quotient
        int q = a / m;
        int t = m;

        // m is remainder now, process same as
        // Euclid's algo
        m = a % m, a = t;
        t = y;

        // Update y and x
        y = x - q * y;
        x = t;
    }

    // Make x positive
    if (x < 0)
        x += m0;

    return x;
}
int main(int argc, char* argv[]) {
	uint64_t p, q;
	uint64_t n, phin,cipher,message;

    // Choosing p and q as large prime numbers
    p = 66617;
    q = 12281;
	n = p*q;

	uint64_t decr;
    uint64_t count=0,count1=0;
	phin = (p - 1) * (q - 1);

	uint64_t e = 0;
	for (e = 5; e <= 100; e++) {
		if (findGCD(phin, e) == 1)
			break;
	}

	uint64_t d = modInverse (e,phin);
	clock_t start_t, end_t, diff_t;
	clock_t start_t_crt, end_t_crt, diff_t_crt;

	int choice,choice_dec,i;
	printf("\nImplementation of RSA - Public key cryptography\n");
    while(1){
    printf("\nEnter your choice:\t1.Encrypt\t2.Decrypt\t3.Calculate time diff of Naive decryption vs CRT decryption\t4.Exit\n");
    scanf("%d",&choice);
    switch(choice) {
      case 1: {
          printf("\nValue of Public key n: %d\n",n);
          printf("Value of Public key e: %d\n",e);
          printf("Value of Private key p: %d\n",p);
          printf("Value of Private key q : %d\n",q);
          printf("Value of Private key d: %d\n",d);
          printf("Enter some numerical data: ");
          scanf("%d", &message);
          cipher = encrypt(e,n,message);
          printf("\nThe cipher text is: %d\n", cipher);
          break;
        }
      case 2: {
        printf("\nEnter your choice:\t1.Normal Decryption\t2.Decryption using CRT\n");
        scanf("%d",&choice_dec);
        switch(choice_dec) {
            case 1: {
                printf("\nEnter the cipher text: ");
                scanf("%d", &cipher);
                decr = decrypt(d,n,cipher);
                printf("\nThe decrypted message is: %d\n", decr);
                break;
            }
            case 2: {
                printf("\nEnter the cipher text: ");
                scanf("%d", &cipher);
                decr = decrypt_crt(d,n,p,q,cipher);
                printf("\nThe decrypted message using CRT is: %d\n", decr);
                break;
            }
            default: {
                printf("\nInvalid choice,Enter valid choice.\n");
            }
        }
        break;
      }
      case 3: {
          // Incrementing upto n in steps of 10 and finding out number of cases where CRT is better by calculating
          // time difference using clock() function.
          printf("\nValue of Public key n: %d\n",n);
          printf("Value of Public key e: %d\n",e);
          printf("Value of Private key p: %d\n",p);
          printf("Value of Private key q : %d\n",q);
          printf("Value of Private key d: %d\n",d);
          printf("Iterating..... Please wait:)\n");
          for (i=0;i<n;i+=10){
            cipher = encrypt(e,n,i);

            start_t_crt = clock();
            decr = decrypt_crt(d,n,p,q,cipher);
            end_t_crt = clock();
            diff_t_crt = (end_t_crt - start_t_crt);

            start_t = clock();
            decr = decrypt(d,n,cipher);
            end_t = clock();
            diff_t = (end_t - start_t);

            if(diff_t_crt<diff_t){
                count++;
            }
        }
          printf("\nTime improvement for decryption using CRT is for: %d times\n\n",count);

          break;
        }
      case 4: {
          break;
        }
      default: {
          printf("\nInvalid choice,Enter valid choice.\n");
        }
    }
    if(choice==4)
      break;
    }
	return 0;
}
