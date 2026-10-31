#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	return 0;
}

// What are the conditions?
// Let S = (sum_{i, j} v_{ij}) / (2N-1)
// S must be an integer
// Let r_i be (sum_j v_{ij} - S) / (N-1), and let c_j be symmetric. We can check that sum r_i == S identically.
// Then set e_{ij} = r_i + c_j - v_{i,j}
// We require that r_i, c_j are integer
// We also need that r_i = sum_{j} e_{ij} = -(sum_j v_{ij}) + N * r_i + sum {c_j} = -(N-1) * r_i - S + (N * r_i) + S == r_i, which is always satisfied.
//
// So we just need the total sum of a multiple of (2N-1), and the rows/column sums are each equal to S mod (N-1)
