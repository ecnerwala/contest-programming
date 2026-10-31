#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
const int MAXM = 2.1e5;
int N, M;
int A[MAXN];

vector<int> inds[MAXM];

int ans[MAXM];

const int S = 1 << 18;
struct rangeData {
	bool allCnt = true;
	int leftCnt = 0;
	int rightCnt = 0;
	int innerValue = 0;
	rangeData() { }

	rangeData(bool b) {
		if (b == true) {
			allCnt = true;
			leftCnt = rightCnt = 1;
			innerValue = 0;
		} else {
			allCnt = false;
			leftCnt = rightCnt = 0;
			innerValue = 0;
		}
	}

	rangeData(const rangeData& l, const rangeData& r) {
		allCnt = l.allCnt && r.allCnt;
		leftCnt = l.leftCnt + (l.allCnt ? r.leftCnt : 0);
		rightCnt = r.rightCnt + (r.allCnt ? l.rightCnt : 0);
		innerValue = l.innerValue + r.innerValue + ((!l.allCnt && !r.allCnt) ? (l.rightCnt + r.leftCnt + 1) / 2 : 0);
	}

	int cycCnt() {
		assert(!allCnt);
		return innerValue + (rightCnt + leftCnt + 1) / 2;
	}
};

rangeData seg[2 * S];
int V[MAXN];
void update(int i) {
	int x = V[i], y = V[(i+1) % N];
	seg[S+i] = rangeData(abs(x - y) == 2);
	for (int a = (S+i) / 2; a; a /= 2) {
		seg[a] = rangeData(seg[2*a], seg[2*a+1]);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}
	A[N] = A[0];

	for (int i = 0; i < N; i++) {
		inds[A[i]].push_back(i);
	}

	for (int i = 0; i < N; i++) {
		V[i] = +1;
	}
	for (int i = 0; i < N; i++) {
		update(i);
	}

	for (int v = 1; v <= M; v++) {
		if (inds[v].empty()) {
			ans[v] = -1;
			continue;
		}

		for (int i : inds[v]) {
			assert(V[i] == +1);
			V[i] = 0;
			update((i+N-1) % N);
			update(i);
		}

		ans[v] = (N - int(inds[v].size())) + seg[1].cycCnt();

		for (int i : inds[v]) {
			assert(V[i] == 0);
			V[i] = -1;
			update((i+N-1) % N);
			update(i);
		}
	}

	for (int i = 0; i < N; i++) {
		assert(V[i] == -1);
	}

	for (int v = 1; v <= M; v++) {
		cout << ans[v] << " \n"[v==M];
	}


	return 0;
}
