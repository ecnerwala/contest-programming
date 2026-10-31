#include <bits/stdc++.h>

using dist_t = std::array<double, 256>;
dist_t add(dist_t a, dist_t b) {
	dist_t r; r.fill(0.);
	for (int i = 0; i <= 255; i++) {
		for (int j = 0; j <= 255; j++) {
			r[(i+j)&255] += a[i] * b[j];
		}
	}
	return r;
}
dist_t sub(dist_t a, dist_t b) {
	dist_t r; r.fill(0.);
	for (int i = 0; i <= 255; i++) {
		for (int j = 0; j <= 255; j++) {
			r[(i-j)&255] += a[i] * b[j];
		}
	}
	return r;
}
dist_t mul(dist_t a, dist_t b) {
	dist_t r; r.fill(0.);
	for (int i = 0; i <= 255; i++) {
		for (int j = 0; j <= 255; j++) {
			r[(i*j)&255] += a[i] * b[j];
		}
	}
	return r;
}
dist_t div(dist_t a, dist_t b) {
	dist_t r; r.fill(0.);
	for (int i = 0; i <= 255; i++) {
		for (int j = 1; j <= 255; j++) {
			r[i/j] += a[i] * b[j];
		}
	}
	return r;
}
dist_t max(dist_t a, dist_t b) {
	dist_t r; r.fill(0.);
	for (int i = 0; i <= 255; i++) {
		for (int j = 1; j <= 255; j++) {
			r[std::max(i, j)] += a[i] * b[j];
		}
	}
	return r;
}
dist_t min(dist_t a, dist_t b) {
	dist_t r; r.fill(0.);
	for (int i = 0; i <= 255; i++) {
		for (int j = 1; j <= 255; j++) {
			r[std::min(i, j)] += a[i] * b[j];
		}
	}
	return r;
}
dist_t uni() {
	dist_t r; r.fill(1. / 256);
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cerr << div(max(max(uni(), uni()), max(uni(), uni())), max(uni(), uni()))[1] << '\n';

	return 0;
}
