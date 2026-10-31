#include <bits/stdc++.h>
#include <vector>

/*
 F - Good Set Query /

Time Limit: 2 sec / Memory Limit: 1024 MB

Score :

525 points
Problem Statement

You are given
Q triples of integers

(a1​,b1​,d1​),(a2​,b2​,d2​),…,(aQ​,bQ​,dQ​).

A subset
S of the set {1,2,…,Q} is defined to be a good set if there exists an integer sequence (X1​,X2​,…,XN​) of length

N that satisfies:

Xai​​−Xbi​​=di​ for all

    i∈S.

Starting with
S as an empty set, perform the following operation for

i=1,2,…,Q in this order:

    If 

S∪{i} is a good set, then replace S with

    S∪{i}.

Print all elements of the final set

S in ascending order.
Constraints

    All input values are integers.

1≤N,Q≤2×105
1≤ai​,bi​≤N

    −109≤di​≤109

Input

The input is given from Standard Input in the following format:

N 

Q

a1​ 

b1​ 

d1​

a2​ 

b2​ 

d2​

⋮

aQ​ 

bQ​ 

dQ​

Output

Print the sequence
(s1​,s2​,…,sk​) of all elements of the final set

S in ascending order, separated by spaces, in the following format:

s1​ 

s2​ 

… 

sk​

Sample Input 1
Copy

3 5
1 2 2
3 2 -3
2 1 -1
3 3 0
1 3 5

Sample Output 1
Copy

1 2 4 5

Starting with
S as an empty set, perform the operation described in the problem statement for

i=1,2,3,4,5 in this order, as follows.

    For 

i=1, the set S∪{i}={1} is a good set, because (X1​,X2​,X3​)=(3,1,4) satisfies the condition in the problem statement, for example, so replace S with
{1}.
For
i=2, the set S∪{i}={1,2} is a good set, because (X1​,X2​,X3​)=(3,1,−2) satisfies the condition in the problem statement, for example, so replace S with
{1,2}.
For
i=3, the set
S∪{i}={1,2,3} is not a good set.
For
i=4, the set S∪{i}={1,2,4} is a good set, because (X1​,X2​,X3​)=(3,1,−2) satisfies the condition in the problem statement, for example, so replace S with
{1,2,4}.
For
i=5, the set S∪{i}={1,2,4,5} is a good set, because (X1​,X2​,X3​)=(3,1,−2) satisfies the condition in the problem statement, for example, so replace S with

    {1,2,4,5}.

Therefore, the final set
S is

{1,2,4,5}.
Sample Input 2
Copy

200000 1
1 1 1

Sample Output 2
Copy

The final set

S is empty.
Sample Input 3
Copy

5 20
4 2 125421359
2 5 -191096267
3 4 -42422908
3 5 -180492387
3 3 174861038
2 3 -82998451
3 4 -134761089
3 1 -57159320
5 2 191096267
2 4 -120557647
4 2 125421359
2 3 142216401
4 5 -96172984
3 5 -108097816
1 5 -50938496
1 2 140157771
5 4 65674908
4 3 35196193
4 4 0
3 4 188711840

Sample Output 3
Copy

1 2 3 6 8 9 11 14 15 16 17 19

Language
Source Code

* at most 512 KiB
* Your source code will be saved as Main.extension.

*/

class UnionFind {
private:
	std::vector<int> parent;
	std::vector<int> rank;

public:
	UnionFind(int size) : parent(size), rank(size, 0) {
		for (int i = 0; i < size; ++i) {
			parent[i] = i;
		}
	}

	int find(int x) {
		if (parent[x] != x) {
			parent[x] = find(parent[x]); // Path compression
		}
		return parent[x];
	}

	void unite(int x, int y) {
		int xRoot = find(x);
		int yRoot = find(y);

		if (xRoot == yRoot) return;

		// Union by rank
		if (rank[xRoot] < rank[yRoot]) {
			parent[xRoot] = yRoot;
		} else if (rank[xRoot] > rank[yRoot]) {
			parent[yRoot] = xRoot;
		} else {
			parent[yRoot] = xRoot;
			rank[xRoot]++;
		}
	}

	bool same(int x, int y) {
		return find(x) == find(y);
	}
};

struct Query {
    int a, b, d;
};

int main() {
	ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N, Q;
	std::cin >> N >> Q;
	std::vector<Query> queries(Q);
	for (int i = 0; i < Q; ++i) {
		cin >> queries[i].a >> queries[i].b >> queries[i].d;
		queries[i].a--; // Make a 0-indexed
		queries[i].b--; // Make b 0-indexed
	}

	// Rest of the code to process the queries will go here

	return 0;
}
