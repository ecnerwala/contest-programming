#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class Brainstuck {

	static const int MAXN = 200;
	int N;
	ll M;

	ll choose[MAXN][MAXN];
	ll pow2[MAXN];

	ll zpm[MAXN]; // zero pm strings
	ll dpm[MAXN]; // direction pm, for a given sign

	ll bal[MAXN]; // number of total balanced strings;

	ll blk[MAXN]; // number of valid strings of the form a[b]
	ll zblk[MAXN]; // number of valid strings of the form blk zpm
	ll dblk[MAXN]; // number of valid strings of the form blk pm, not zblk
	// dblk + zblk is answer

	int go() {
		for(int i = 0; i <= N; i++) {
			choose[i][0] = choose[i][i] = 1;
			for(int j = 1; j < i; j++) {
				choose[i][j] = choose[i - 1][j - 1] + choose[i - 1][j];
				if(choose[i][j] >= M) choose[i][j] -= M;
			}
		}
		pow2[0] = 1;
		for(int i = 0; i <= N; i++) pow2[i + 1] = pow2[i] * 2 % M;
		for(int i = 0; i <= N; i++) {
			if(i % 2 == 0) zpm[i] = choose[i][i / 2];
			for(int j = 0; j * 2 < i; j++) {
				dpm[i] += choose[i][j];
				if(dpm[i] >= M) dpm[i] -= M;
			}
		}

		bal[0] = 1;
		bal[1] = 2;
		for(int i = 2; i <= N; i++) {
			bal[i] = bal[i - 1] * 2 % M;
			for(int j = 0; j <= i - 2; j++) {
				bal[i] += bal[j] * bal[i - 2 - j];
				bal[i] %= M;
			}
		}

		blk[0] = 1;
		zblk[0] = 1;
		dblk[0] = 0;
		blk[1] = 0;
		zblk[1] = 0;
		dblk[1] = 2;
		for(int i = 2; i <= N; i++) {
			blk[i] = 0;
			for(int j = 0; j <= i - 2; j++) {
				blk[i] += zblk[j] * bal[i - 2 - j];
				blk[i] %= M;
			}
			for(int j = 0; j <= i - 2; j++) {
				blk[i] += dblk[j] * dpm[i - 2 - j];
				blk[i] %= M;
			}
			// b = blk(w) zpm, a = dblk(x) blk(w) is valid
			// = number of zblk[i - 2] - # zblk zblk
			blk[i] += zblk[i - 2];
			blk[i] %= M;
			for(int j = 0; j <= i - 2; j++) {
				blk[i] -= zblk[j] * zblk[i - 2 - j];
				blk[i] %= M;
			}
			if(blk[i] < 0) blk[i] += M;

			zblk[i] = 0;
			for(int j = 0; j <= i; j++) {
				zblk[i] += blk[j] * zpm[i - j];
				zblk[i] %= M;
			}
			dblk[i] = 0;
			for(int j = 0; j <= i; j++) {
				dblk[i] += blk[j] * dpm[i - j] * 2;
				dblk[i] %= M;
			}
		}
		return (dblk[N] + zblk[N]) % M;
	}

public:

	int countPrograms(int n, int m) {
		N = n, M = m;
		return go() % M;
	}
};

// [] block: only moves inside if it's non-zero, stays inside until it's 0
// pm := +++------
// zpm := pm with even plus and minus
// blk := a [b]
// if a is zero (ie (blk) zpm), blk is valid
// if b is pm, blk is valid if sign(eval(a)) = -sign(dir(b))
// if b is b = c [d] pm, valid if ac [d] is valid and pm = 0
//
// for each length:
// number of strings which evaluate to 0
// number of strings which are valid

// vim:ft=cpp
