#include "src/HEAAN.h"

long pow2(long n);
void MultipleAdd(Scheme& scheme, Ciphertext& res, Ciphertext& cipher, long long n);
void eval_division(Scheme& scheme, SecretKey& secretKey, long logp, Ciphertext& p, Ciphertext& q, Ciphertext& Ti, Ciphertext& r);
void print_cipher(Scheme& scheme, SecretKey secretKey, Ciphertext& cipher);
long ShowFailure(complex<double>* vals, long precision, long n);
