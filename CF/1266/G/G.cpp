#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	return 0;
}

// No repeats => done
//
// Only repeats of size N => permutation, and non-repeated has at least one increasing seq
// Repeats of size N -> exactly 1 non-match
//   aaab  c  aaac
//  must have b < c, and then b<->c are all > c
// Repeats of size N -> >=2 non-matches
//   aaab  c d aaacd    OR aaab  c  aaacb
//  uniquely determines the whole starting one, so count by that
//
//
// First repeat of size < N => it's either the
