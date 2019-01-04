#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = ll(1e9) + 7;

const int S = 260;
void mul(ll *C, ll *A, ll *B, int n) {
	if (n <= 64) {
		for (int i = 0; i < n; i++) {
			for (int k = 0; k < n; k++) {
				for (int j = 0; j < n; j++) {
					C[i*S+j] += A[i*S+k] * B[k*S+j] % MOD;
				}
			}
		}
	} else {
		assert(n % 2 == 0);
		for (int i = 0; i < n; i += n/2) {
			for (int k = 0; k < n; k += n/2) {
				for (int j = 0; j < n; j += n/2) {
					mul(C + i * S + j, A + i * S + k, B + k * S + j, n / 2);
				}
			}
		}
	}
}
struct mat {
	ll v[S][S];
	mat& operator *= (mat& o) {
		ll t[260][260];
		/*
		for (int j = 0; j < S; j++) {
			for (int k = j+1; k < S; k++) {
				swap(o.v[j][k], o.v[k][j]);
			}
		}
		*/
		memset(t, 0, sizeof(t));
		mul(t[0], v[0], o.v[0], 256);
		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				t[i][j] %= MOD;
			}
		}
		/*
		for (int j = 0; j < S; j++) {
			for (int k = j+1; k < S; k++) {
				swap(o.v[j][k], o.v[k][j]);
			}
		}
		*/
		memcpy(v, t, sizeof(t));
		return *this;
	}
	mat& operator += (const mat &o) {
		for (int i = 0; i < S; i++) {
			for (int j = 0; j < S; j++) {
				v[i][j] += o.v[i][j];
				if (v[i][j] >= MOD) v[i][j] -= MOD;
			}
		}
		return *this;
	}
	mat() {
		memset(v, 0, sizeof(v));
	}
	// construct from scalar
	mat(ll val) {
		val %= MOD;
		memset(v, 0, sizeof(v));
		for (int i = 0; i < S; i++) {
			v[i][i] = val;
		}
	}
};

mat pow(mat a, ll b) {
	mat r = 1;
	while (b) {
		if (b & 1) r *= a;
		a *= a;
		b /= 2;
	}
	return r;
}

const int MAXF = 20;
const int MAXB = 200;
const int MAXC = 260;

int F, B;
ll N;
ll P[MAXF];
int C[MAXB];

mat pet; // one petal's effects
mat flower; // one flower's effects

int main() {
	ios_base::sync_with_stdio(0);
	cin >> F >> B >> N;
	for (int i = 0; i < F; i++) cin >> P[i];
	for (int i = 0; i < B; i++) cin >> C[i];

	// initialize pet
	for (int i = 0; i < B; i++) {
		pet.v[0][C[i]-1] ++;
	}
	for (int i = 1; i < MAXC; i++) {
		pet.v[i][i-1] ++;
	}

	for (int i = 0; i < F; i++) {
		flower += pow(pet, P[i]);
	}

	cout << pow(flower, N).v[0][0] << '\n';
	return 0;
}

// add 1 petal:
// 0 -> c_i - 1
// i -> i-1
