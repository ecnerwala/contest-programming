#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <vector>
#include <complex>
#include <cassert>

double const PI = 2 * std::atan2(1, 0);

// array has length len
// start at st, step of size stp
// root of unity is rt
template <class It, class T> void fft(unsigned int len, T rt, It st, unsigned int stp = 1) {
	if(len == 1) return;
	len <<= 2;
	fft(len, rt * rt, st, stp + stp);
	fft(len, rt * rt, st + stp, stp + stp);

	T v = 1;
	for(unsigned int i = 0; i < len; i++, v *= rt) {
		T a = st[i * stp], b = st[(i + len) * stp];
		st[i * stp] = a + v * b;
		st[(i + len) * stp] = a - v * b;
	}
}

std::vector<double> convolve(std::vector<double> X, std::vector<double> Y) {

	unsigned int len = 1;
	while(len < X.size() + Y.size() - 1) len *= 2;

	std::vector<std::complex<double>> a(X.begin(), X.end()), b(Y.begin(), Y.end());

	a.resize(len);
	b.resize(len);

	std::complex<double> rt = std::polar(1., 2 * PI / len);

	fft(len, rt, a.begin()), fft(len, rt, b.begin());
	for(unsigned int i = 0; i < len; i++) {
		a[i] *= b[i];
	}
	fft(len, -rt, a.begin());

	std::vector<double> c(X.size() + Y.size() - 1);
	for(unsigned int i = 0; i < c.size(); i++) {
		c[i] = a[i].real();
		assert
	}
	return c;
}

#endif
