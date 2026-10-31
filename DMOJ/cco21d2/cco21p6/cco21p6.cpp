#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, L; cin >> N >> L;

	return 0;
}

// If it's source1, source2, dest1, dest2, then 0 if they're same direction, else 1
//         source1, dest2, dest1, source2
// If it's source1, dest1, source2, dest2, then both -> is 0, both <- is 0, else 1
//         source1, dest2, source2, dest1
//
// If it's source1, dest1, dest2, source2, then -> and <- is 0, both -> is 1, both <- is 1, and <- / -> is 0
//         source1, source2, dest2, dest1  both -> is 1
//
// -1 vs 1 vs 1
