#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 4e18;

const int MAXN = 4e4;
int N;
ll L[MAXN];

ll costneg[MAXN];
ll costpos[MAXN];

ll go() {
	assert(N);
	sort(L, L + N);
	int nneg = 0;
	for (int i = 0; i < N; i++) {
		assert(L[i]);
		nneg += (L[i] < 0);
	}
	if (nneg == 0) {
		// all positive and all negative are symmetric, so we'll just make them all negative
		for (int i = 0; i < N; i++) {
			L[i] *= -1;
		}
		reverse(L, L + N);
		nneg = N;
	}
	int npos = N - nneg;
	int minneg = nneg - 1;
	assert(minneg >= 0);

	ll res = INF;

	costneg[0] = 0;
	costneg[1] = INF;
	if (nneg >= 2) {
		costneg[2] = L[nneg-1] * L[nneg-2];
		for (int i = 3; i <= nneg; i++) {
			costneg[i] = costneg[i-1] + L[nneg - i] * L[minneg];
			if (i % 2 == 0) {
				ll costpair = 0;
				for (int j = 0; j < i/2; j++) {
					costpair += L[nneg - j - 1] * L[nneg - i + j];
				}
				costneg[i] = min(costneg[i], costpair);
			}
		}
	}

	for (int i = 0; i <= min(npos, nneg); i++) {
		if (nneg - i == 1) continue; // you can't leave just 1 left, might as well leave 2 or 0
		ll v = costneg[nneg - i];
		int r = nneg;
		for (int j = i-1; j >= 0; j--, r++) {
			v += L[r] * L[j];
		}
		for (; r < N; r++) {
			v += L[r] * L[0];
		}
		res = min(res, v);
	}
	return res;
}

int main() {
	int num_cases; cin >> num_cases;
	for (int case_num = 1; case_num <= num_cases; case_num++) {
		cin >> N;
		for (int i = 0; i < N; i++) cin >> L[i];
		ll res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
