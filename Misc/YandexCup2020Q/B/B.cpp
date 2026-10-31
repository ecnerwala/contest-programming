#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	string S; getline(cin, S);
	int dp_lower = 0;
	int dp_upper = 2;
	for (char c : S) {
		if (c == ' ') {
			dp_lower++, dp_upper++;
		} else if ('a' <= c && c <= 'z') {
			dp_lower++, dp_upper += 2;
		} else if ('A' <= c && c <= 'Z') {
			dp_upper++, dp_lower += 2;
		} else assert(false);
		dp_lower = min(dp_lower, dp_upper + 2);
		dp_upper = min(dp_upper, dp_lower + 2);
	}

	cout << min(dp_lower, dp_upper) << '\n';

	return 0;
}
