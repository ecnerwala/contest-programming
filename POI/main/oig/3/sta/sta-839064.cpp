#include<bits/stdc++.h>
using namespace std;

string S;

int stats[10];
#define SPACES stats[1]
#define NUMS stats[2]
#define WORDS stats[3]
#define SENTS stats[4]
#define PALS stats[5]

bool is_letter(char a) {
	return ('A' <= a && a <= 'Z') || ('a' <= a && a <= 'z');
}

char to_lower(char a) {
	if ('A' <= a && a <= 'Z') return a - 'A' + 'a';
	else return a;
}

int main() {
	ios_base::sync_with_stdio(0);
	int N;
	int A[10];
	cin >> N;
	for (int i = 0; i < N; i++) cin >> A[i];
	getline(cin, S);
	getline(cin, S);
	S += '\n';
	memset(stats, 0, sizeof(stats));
	int w_start = 0;
	bool has_w = 0;
	for (int i = 0; i + 1 < int(S.size()); i++) {
		char cur = S[i];
		char nxt = S[i+1];
		if (cur == ' ') {
			SPACES++;
			w_start = i+1;
		} else if (cur == '.') {
			if (has_w) {
				SENTS ++;
			}
			has_w = false;
			w_start = i+1;
		} else if (is_letter(cur)) {
			if (!is_letter(nxt)) {
				WORDS++;
				bool is_pal = true;
				for (int a = w_start, b = i; a <= b; a++, b--) {
					if (to_lower(S[a]) != to_lower(S[b])) {
						is_pal = false;
					}
				}
				if (is_pal) PALS++;
			}
			has_w = true;
		} else if ('0' <= cur && cur <= '9') {
			if (!('0' <= nxt && nxt <= '9')) {
				NUMS ++;
			}
			w_start = i+1;
		} else {
			// random stuff
			w_start = i+1;
		}
	}
	for (int i = 0; i < N; i++) {
		if (i) cout << ' ';
		cout << stats[A[i]];
	}
	cout << '\n';

	return 0;
}
