#include<bits/stdc++.h>
using namespace std;

int N;
string S;

int many() {
	int res = 0;
	for (int i = 0; true; i++) {
		if (S[i] == '#' && S[i+1] == '#' && S[i+2] != '*') {
			break;
		} else if (S[i] == '#' && S[i+1] == '#' && S[i+2] == '*') {
			i += 3; // skip all of these
			i--; // counteract the i++
		} else if (S[i] == '*') {
			res++;
		}
	}
	return res;
}

int go() {
	assert(S[0] == '.');
	S += "###"; // might as well terminate it
	N = int(S.size());
	int res = 0;
	bool jammerwaiting = false;
	bool hasjammer = false;
	for (int i = 0; true; i++) {
		if (S[i] == '#' && S[i+1] == '#' && S[i+2] != '*') {
			// we're screwed
			break;
		}
		if (S[i] == '#' && S[i+1] == '#' && S[i+2] == '*') {
			if (hasjammer) {
				// then we just pass through
				i += 3; // skip the 3 #s and continue
				i--; continue; // counteract the i++
			} else {
				// we're screwed
				break;
			}
			assert(false);
		}

		if (S[i] == '*') {
			if (hasjammer || jammerwaiting) {
				return many();
			} else {
				res = 1;
				hasjammer = true;
			}
		} else if (S[i] == '#') {
			assert(S[i+1] != '#');
			if (hasjammer) {
				jammerwaiting = true;
				hasjammer = false;
			} else if (S[i+1] == '*') {
				// we can go through
				jammerwaiting = false;
				// we just pick up the next jammer
			} else {
				// we're stuck
				break;
			}
		} else if (S[i] == '.') {
			// not much
		} else { assert(false); }
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num++) {
		cin >> S;
		int res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}


	return 0;
}
