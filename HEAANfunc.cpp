#include "HEAANfunc.h"
// power of 2
long pow2(long n)
{
	long prod = 1;
	for(int i=0; i<n; i++) prod *= 2;
	
	return prod;
}
// add a ciphertext multiple times
void MultipleAdd(Scheme& scheme, Ciphertext& res, Ciphertext& cipher, long long n)
{
	long long k, abs_n;
	long long *binary;
	Ciphertext temp;
	if(n>=0) abs_n = n;
	else abs_n = -n;

	for(k=1; k<100; k++) {
		if(abs_n < pow2(k)) break;
	}

	binary = new long long[k];
	for(long i=0; i<k; i++) {
		binary[i] = (abs_n / pow2(i)) % 2;
	}

	scheme.add(temp, cipher, cipher);
	if(binary[k-2] == 1) scheme.addAndEqual(temp, cipher);

	for(long i=k-3; i>=0; i--) {
		scheme.addAndEqual(temp, temp);
		if(binary[i] == 1) scheme.addAndEqual(temp, cipher);
	}

	if(n<0) scheme.negateAndEqual(temp);

	res.copyParams(temp);
	res.copy(temp);
}
// evaluate p = qTi + r
void eval_division(Scheme& scheme, SecretKey& secretKey, long logp, Ciphertext& p, Ciphertext& q, Ciphertext& Ti, Ciphertext& r)
{
	Ciphertext temp1, temp2;
	
	scheme.mult(temp1, q, Ti);
	scheme.reScaleByAndEqual(temp1, logp);
	scheme.modDownBy(temp2, r, logp);	
	scheme.add(p, temp1, temp2);		// p
}
// print the message of the ciphertext
void print_cipher(Scheme& scheme, SecretKey secretKey, Ciphertext& cipher) {
	long logn = 2;
	long n = (1<<logn);

	complex<double>* dvec;
	dvec = scheme.decrypt(secretKey, cipher);
	StringUtils::showVec(dvec,n);
	cout << endl;
}
// print the number of failures
long ShowFailure(complex<double>* vals, long precision, long n) {
	long failure = 0;
	double bound = pow(2.0,static_cast<double>(-precision));
	for (int i = 0; i < n; ++i) if(abs(1.0 - vals[i].real()) > bound) failure++;

	cout << "failure : " << failure << endl;
	return failure;
}

