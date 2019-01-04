#include<bits/stdc++.h>
using namespace std;

using pii = pair<int, int>;

const int MAXN = 3e5;
int N;
pair<pii, int> A[MAXN];
int res[MAXN];

const int MAXM = 3e5;
int M;
pair<pii, int> B[MAXN];

set<pii> wants;

void insert(int v, int ind) {
	wants.emplace(v, ind);
}
void query(int v, int ind) {
	auto it = wants.upper_bound(pii(v+1, -1)); // first thing that's at least v+1
	if (it == wants.begin()) {
		// we don't get anything
		return;
	} else {
		it--;
		assert(it->first <= v);
		res[it->second] = ind;
		wants.erase(it);
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		A[i].first = pii(x, y);
		A[i].second = i;
	}
	sort(A, A + N);

	cin >> M;
	for (int i = 0; i < M; i++) {
		int x, y; cin >> x >> y;
		B[i].first = pii(x, y);
		B[i].second = i;
	}
	sort(B, B + M);

	memset(res, -1, sizeof(res));
	int j = 0;
	for (int i = 0; i < N; i++) {
		while (j < M && B[j].first.first < A[i].first.first) {
			query(B[j].first.second, B[j].second);
			j++;
		}
		insert(A[i].first.second, A[i].second);
	}
	while (j < M) {
		query(B[j].first.second, B[j].second);
		j++;
	}

	if (!wants.empty()) {
		cout << -1 << '\n';
		return 0;
	}

	for (int i = 0; i < N; i++) {
		cout << res[i] + 1 << " \n"[i == N+1];
	}
	return 0;
}
