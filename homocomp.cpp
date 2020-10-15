#include "homocomp.h"

// execute newcompG
void newcompG(long logq, long logp, long logn, long alpha, long no_f, long no_g, double msg) {

	// Parameters setting
	srand(time(NULL));
	SetNumThreads(8);
	TimeUtils timeutils;
	Ring ring;
	SecretKey secretKey(ring);
	Scheme scheme(secretKey, ring);
	Ciphertext cipher_res, ctxt_x, ctxt_1;
	Ciphertext temp;


	// generate a message
	long n = (1 << logn);
	complex<double>* m = new complex<double>[n];
	complex<double>* dvec;	
	for (long i=0; i<n; ++i) m[i] = msg;

	
	// encrypt and evaluation
	scheme.encrypt(ctxt_x, m, n, logp, logq);
	cout << "ctxt_x " << endl;
	print_cipher(scheme,secretKey, ctxt_x);
	timeutils.start("Homomorphic Comparison ");
		

	for (int i = 0; i < no_g; ++i) {
		cout << "No: " << i << endl;
		Evalg4(scheme, secretKey, ctxt_x, ctxt_x);
			
		cout << "ctxt_x " << endl;
		print_cipher(scheme,secretKey, ctxt_x);
	}
	for (int i = 0; i < no_f; ++i) {
		cout << "No: " << i << endl;
		Evalf4(scheme, secretKey, ctxt_x, ctxt_x);
		
		cout << "ctxt_x " << endl;
		print_cipher(scheme,secretKey, ctxt_x);
	}

	// drcryption and print
	timeutils.stop("Homomorphic Comparison");
	dvec = scheme.decrypt(secretKey, ctxt_x);
	StringUtils::showVec(dvec, 4);
	cout << endl;
	cout << "*********************************************" << endl;
	cout << "                result                       " << endl;
	cout << "*********************************************" << endl;
	ShowFailure(dvec, alpha-1, n);
	cout << "Slot number : " << n << endl << endl;
}
// evaluate f
void Evalf4(Scheme& scheme, SecretKey secretKey, Ciphertext& res, Ciphertext& cipher) {
	
	// Variables
		long logq, logp, n;
		logq=cipher.logq, logp=cipher.logp, n=cipher.n;
		Ciphertext X0, X1, X2, X3, qprime, rprime, r, p, q;
		Ciphertext temp1, temp2, ctxt_1;
		complex<double>* m_one = new complex<double>[n];
		for(long i=0; i<n; i++) m_one[i] = 1.0;
		scheme.encrypt(ctxt_1, m_one, cipher.n, cipher.logp, cipher.logq);


		// depth 0: X0, X1
		X0.copyParams(cipher);
		X1.copyParams(cipher);
		X0.copy(ctxt_1);
		X1.copy(cipher);


		// depth 1: X2
		scheme.square(temp1, X1);
		scheme.reScaleBy(X2, temp1, logp);	// X2[1]
		scheme.modDownByAndEqual(X0, logp);	// X0[1]
		scheme.modDownByAndEqual(X1, logp);	// X1[1]


		// depth 2: X3
		scheme.mult(temp1, X1, X2);
		scheme.reScaleBy(X3, temp1, logp);	// X3[2]
		scheme.modDownByAndEqual(X0, logp); 	// X0[2]
		scheme.modDownByAndEqual(X1, logp); 	// X1[2]
		scheme.modDownByAndEqual(X2, logp); 	// X2[2]

	
		// qprime, rprime, r
		MultipleAdd(scheme, temp1, X3, 35);
		MultipleAdd(scheme, temp2, X1, -180);
		scheme.add(qprime, temp1, temp2);
		MultipleAdd(scheme, temp1, X2, 378);
		MultipleAdd(scheme, temp2, X0, -420);
		scheme.add(rprime, temp1, temp2);
		MultipleAdd(scheme, r, X1, 315);	


		// depth 3: q
		eval_division(scheme, secretKey, logp, q, qprime, X3, rprime);
		scheme.modDownByAndEqual(X3, logp);
		scheme.modDownByAndEqual(r, logp);


		// depth 4: p
		eval_division(scheme, secretKey, logp, p, q, X3, r);
		scheme.divByPo2AndEqual(p, 7);

		res.copyParams(p);
		res.copy(p);

		return;

}
// evaluate g
void Evalg4(Scheme& scheme, SecretKey secretKey, Ciphertext& res, Ciphertext& cipher) {
	
	// Variables
		long logq, logp, n;
		logq=cipher.logq, logp=cipher.logp, n=cipher.n;
		Ciphertext X0, X1, X2, X3, qprime, rprime, r, q, p;
		Ciphertext temp1, temp2, ctxt_1;
		complex<double>* m_one = new complex<double>[n];
		for(long i=0; i<n; i++) m_one[i] = 1.0;
		scheme.encrypt(ctxt_1, m_one, cipher.n, cipher.logp, cipher.logq);


		// depth 0: X0, X1
		X0.copyParams(cipher);
		X1.copyParams(cipher);
		X0.copy(ctxt_1);
		X1.copy(cipher);


		// depth 1: X2
		scheme.square(temp1, X1);
		scheme.reScaleBy(X2, temp1, logp);	// X2[1]
		scheme.modDownByAndEqual(X0, logp);	// X0[1]
		scheme.modDownByAndEqual(X1, logp);	// X1[1]


		// depth 2: X3
		scheme.mult(temp1, X1, X2);
		scheme.reScaleBy(X3, temp1, logp);	// X3[2]
		scheme.modDownByAndEqual(X0, logp); 	// X0[2]
		scheme.modDownByAndEqual(X1, logp); 	// X1[2]
		scheme.modDownByAndEqual(X2, logp); 	// X2[2]


		// qprime, rprime, r
		MultipleAdd(scheme, temp1, X3, 46623);
		MultipleAdd(scheme, temp2, X1, -113492);
		scheme.add(qprime, temp1, temp2);
		scheme.divByPo2AndEqual(qprime, 10);	// qprime[2]


		MultipleAdd(scheme, temp1, X2, 97015);
		MultipleAdd(scheme, temp2, X0, -34974);
		scheme.add(rprime, temp1, temp2);
		scheme.divByPo2AndEqual(rprime, 10);	// rprime[2]

		MultipleAdd(scheme, temp1, X1, 5850);
		scheme.divByPo2(r, temp1, 10);	// r[2]

		scheme.modDownByAndEqual(X3, 10);



		// depth 3: q
		eval_division(scheme, secretKey, logp, q, qprime, X3, rprime);
		scheme.modDownByAndEqual(X3, logp);
		scheme.modDownByAndEqual(r, logp);


		// depth 4: p
		eval_division(scheme, secretKey, logp, p, q, X3, r);

		res.copyParams(p);
		res.copy(p);

		// for debugging
		cout << "X0" << endl;
		print_cipher(scheme,secretKey, X0);
		cout << "X1" << endl;
		print_cipher(scheme,secretKey, X1);
		cout << "X2" << endl;
		print_cipher(scheme,secretKey, X2);
		cout << "X3" << endl;
		print_cipher(scheme,secretKey, X3);
		cout << "qprime" << endl;
		print_cipher(scheme,secretKey, qprime);
		cout << "rprime" << endl;
		print_cipher(scheme,secretKey, rprime);
		cout << "r" << endl;
		print_cipher(scheme,secretKey, r);
		cout << "q" << endl;
		print_cipher(scheme,secretKey, q);
		cout << "p" << endl;
		print_cipher(scheme,secretKey, p);

		return;

}
