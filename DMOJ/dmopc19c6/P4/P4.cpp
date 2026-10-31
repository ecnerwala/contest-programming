#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;

using ordered_set = __gnu_pbds::tree<
int,
__gnu_pbds::null_type,
less<int>,
__gnu_pbds::rb_tree_tag,
__gnu_pbds::tree_order_statistics_node_update>;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, Q; cin >> N >> Q;
	vector<int> A(N, Q);
	vector<ordered_set> people(2*Q+1);
	for (int i = 0; i < N; i++) people[A[i]].insert(i);

	for (int q = 0; q < Q; q++) {
		int t; cin >> t;
		if (t == 1) {
			int x; cin >> x; x--;
			people[A[x]].erase(x);
			A[x]++;
			people[A[x]].insert(x);
		} else if (t == 2) {
			int x; cin >> x; x--;
			people[A[x]].erase(x);
			A[x]--;
			people[A[x]].insert(x);
		} else if (t == 3) {
			int l, r, c; cin >> l >> r >> c; l--, r--, c += Q;
			auto& s = people[c];
			cout << s.order_of_key(r+1) - s.order_of_key(l) << '\n';
		} else assert(false);
	}

	return 0;
}
