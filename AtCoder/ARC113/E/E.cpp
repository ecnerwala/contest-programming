#include <bits/stdc++.h>

std::string solve(std::string S) {
	using namespace std;
	assert(!S.empty());
	int N = int(S.size());
	array<int, 2> cnt{0,0};
	for (char c : S) cnt[c - 'a']++;

	// If it's monochromatic, we're done
	if (cnt[0] == 0 || cnt[1] == 0) return S;

	int cur_run = 0;
	int tot_len = 2;
	int num_run = 0;
	int last_run = -1;
	for (char c : S + "b") {
		if (c == 'a') {
			cur_run++;
		} else if (cur_run > 0) {
			if (cur_run >= 2) {
				num_run++;
				tot_len += cur_run - 2;
			}
			last_run = cur_run;
			cur_run = 0;
		}
	}

	if (S.back() == 'a') {
		int a_cnt;
		if (!num_run) {
			a_cnt = cnt[0] % 2;
		} else {
			if (last_run == 1) tot_len--;
			if (tot_len % 2 != cnt[0] % 2) tot_len--;
			a_cnt = tot_len;
		}
		return string(cnt[1], 'b') + string(a_cnt, 'a');
	}

	assert(S.back() == 'b');

	// Question 1: what's the max number of b's we can put at the start?
	// If a's is even, then it's all of them
	// If a's is odd, then it's all before the last a, or all but 2 if there's at least 1 before the last a
	if (cnt[0] % 2 == 0) {
		// We can put all the b's at the front by deleting a's, but how many a's can we leave at the end?
		// We need to move this guy to the front
		return string(cnt[1], 'b');
	} else {
		int last_a = N-1;
		int late_b = 0;
		while (S[last_a] != 'a') late_b++, last_a--;

		// If all the a's come before all the b's, then we just have to delete a's
		if (late_b == cnt[1]) {
			return "a" + string(cnt[1], 'b');
		}
		assert(late_b > 0);
		if (late_b == 1 || late_b == 2) {
			// we're not allowed to do any b ops;
			return string(cnt[1] - late_b, 'b') + "a" + string(late_b, 'b');
		}
		assert(late_b >= 3);
		int a_cnt;
		if (!num_run) {
			a_cnt = cnt[0] % 2;
		} else {
			if (num_run == 1 && N >= 2 && S[0] == 'a' && S[1] == 'a') tot_len--;
			if (tot_len % 2 != cnt[0] % 2) tot_len--;
			a_cnt = tot_len;
		}
		return string(cnt[1] - 2, 'b') + string(a_cnt, 'a');
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		cout << solve(std::move(S)) << '\n';
	}

	return 0;
}
