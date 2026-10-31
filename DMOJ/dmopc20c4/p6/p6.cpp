#include <bits/stdc++.h>

namespace kactl {
using namespace std;

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

typedef bitset<2000> bs;

int solveLinear(vector<bs>& A, vi& b, bs& x, int m) {
	int n = sz(A), rank = 0, br;
	assert(m <= sz(x));
	vi col(m); iota(all(col), 0);
	rep(i,0,n) {
		for (br=i; br<n; ++br) if (A[br].any()) break;
		if (br == n) {
			rep(j,i,n) if(b[j]) return -1;
			break;
		}
		int bc = (int)A[br]._Find_next(i-1);
		swap(A[i], A[br]);
		swap(b[i], b[br]);
		swap(col[i], col[bc]);
		rep(j,0,n) if (A[j][i] != A[j][bc]) {
			A[j].flip(i); A[j].flip(bc);
		}
		rep(j,i+1,n) if (A[j][i]) {
			b[j] ^= b[i];
			A[j] ^= A[i];
		}
		rank++;
	}

	x = bs();
	for (int i = rank; i--;) {
		if (!b[i]) continue;
		x[col[i]] = 1;
		rep(j,0,i) b[j] ^= A[j][i];
	}
	return rank; // (multiple solutions if rank < m)
}

#undef rep
#undef all
#undef sz
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<kactl::bs> mat(N);
	vector<int> b(N);

	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;

		mat[u].flip(u);
		mat[u].flip(v);
		mat[v].flip(u);
		mat[v].flip(v);

		b[u] ^= 1;
		b[v] ^= 1;
	}

	kactl::bs x;
	kactl::solveLinear(mat, b, x, N);
	cout << N << '\n';
	for (int i = 0; i < N; i++) {
		cout << x[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
