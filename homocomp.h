#include "./src/HEAAN.h"
#include "HEAANfunc.h"

void newcompG(long logq, long logp, long logn, long alpha, long no_f, long no_g, double msg);
void Evalf4(Scheme& scheme, SecretKey secretKey, Ciphertext& res, Ciphertext& cipher);
void Evalg4(Scheme& scheme, SecretKey secretKey, Ciphertext& res, Ciphertext& cipher);
