#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	vector<int> A(N); for (int& a : A) cin >> a;
	vector<int> B(M); for (int& b : B) cin >> b;

	vector<int> sortedA = A;
	sort(sortedA.begin(), sortedA.end());

	const int S = N+1;
	struct seg_node {
		int val = 0;
		int lazy = 0;
	};
	vector<seg_node> seg(2*S);

	auto update_node = [&](int a) {
		assert(a < S);
		seg[a].val = min(seg[2*a].val, seg[2*a+1].val);
		seg[a].val += seg[a].lazy;
	};
	auto update_all_changed = [&](int a) {
		assert(a);
		a >>= __builtin_ctz(a);
		a >>= 1;
		for (; a; a >>= 1) {
			update_node(a);
		}
	};

	{
		vector<int> sortedB = B;
		sort(sortedB.begin(), sortedB.end());
		int cur_val = 0;
		for (int k = 1, j = 0; k <= N; k++) {
			cur_val -= sortedA[N-k];
			while (j < M && sortedB[j] < k) {
				j++;
			}
			cur_val += M-j;
			//cerr << k << ' ' << cur_val << '\n';
			seg[S+k].val = cur_val;
		}
	}

	for (int a = S-1; a > 0; a--) {
		update_node(a);
	}

	// inclusive
	auto update_range = [&](int l, int r, int v) -> void {
		if (l > r) return;
		for (int a = S+l, b = S+r+1; a < b; a >>= 1, b >>= 1) {
			if (a & 1) {
				seg[a].val += v;
				seg[a].lazy += v;
				a++;
			}
			if (b & 1) {
				--b;
				seg[b].val += v;
				seg[b].lazy += v;
			}
		}
		update_all_changed(S+l);
		update_all_changed(S+r+1);
	};

	int Q; cin >> Q;
	while (Q--) {
		int op; cin >> op;
		if (op == 1) {
			int i; cin >> i; i--;
			int si = int(upper_bound(sortedA.begin(), sortedA.end(), A[i]) - sortedA.begin()) - 1;
			sortedA[si]++;
			A[i]++;
			update_range(N-si, N, -1);
		} else if (op == 2) {
			int i; cin >> i; i--;
			int si = int(lower_bound(sortedA.begin(), sortedA.end(), A[i]) - sortedA.begin());
			sortedA[si]--;
			A[i]--;
			update_range(N-si, N, +1);
		} else if (op == 3) {
			int j; cin >> j; j--;
			B[j]++;
			update_range(B[j], N, +1);
		} else if (op == 4) {
			int j; cin >> j; j--;
			update_range(B[j], N, -1);
			B[j]--;
		} else assert(false);

		cout << (seg[1].val >= 0 ? 1 : 0) << '\n';
	}

	return 0;
}
