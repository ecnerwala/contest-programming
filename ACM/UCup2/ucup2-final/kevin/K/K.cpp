#include <bits/stdc++.h>
using namespace std;

using ll = int64_t;

void solve(){
	int N;
	cin >> N;
	vector<ll> A(N), B(N);
	for(int i = 0; i < N; i++) cin >> A[i];
	for(int i = 0; i < N; i++) cin >> B[i];

	vector<ll> oA = A, oB = B;

	vector<tuple<int, int, ll> > bad {{-1, -1, -1}};

	auto outp = [&]() -> vector<tuple<int, int, ll> > {
		if(A == B) return {};
		if(B == vector<ll>(N, B.front())) return bad;
		if(N == 1) return bad;
		vector<tuple<int, int, ll> > rev_ops;
		int non1 = 0;
		{
			int loc1 = 0;
			while(loc1 < N && B[loc1] != 1) loc1++;
			if(loc1 == N){
				for(int i = 0; i + 1 < N; i++){
					if(B[i] != B[i+1]){
						int x = i;
						int y = i+1;
						if(B[x] > B[y]) swap(x, y);
						rev_ops.push_back({x, y, B[y]});
						B[y] = 1;
						loc1 = y;
						break;
					}
				}
			}
			assert(B[loc1] == 1);
			while(non1 < N && B[non1] == 1) non1++;
			assert(non1 < N);
			for(int i = 0; i < N; i++){
				if(B[i] == 1 || i == non1) continue;
				rev_ops.push_back({loc1, i, B[i]});
				B[i] = 1;
			}
			if(B[non1] % 2 == 1){
				int x = 0;
				if(x == non1) x++;
				rev_ops.push_back({x, non1, B[non1]});
				B[x] = B[non1] + 1;
				B[non1] = 1;
				non1 = x;
			}
			if(B[non1] > 2){
				int x = 0;
				if(x == non1) x++;
				assert(B[x] == 1);
				rev_ops.push_back({x, non1, B[non1] / 2});
				B[non1] = 2;
			}
		}
		assert(B[non1] == 2);

		vector<tuple<int, int, ll> > ops;
		int P = 200000033;
		ll residue = A[non1];

		auto do_op = [&](int a, int b, ll c){
			ops.push_back({a, b, c});
			A[a] %= c;
			A[b] *= c;
		};
		auto do_op_residue = [&](int i){
			residue *= (A[i] - 1);
			residue %= P;
			ops.push_back({i, non1, A[i] - 1});
			A[i] = 1;
		};

		vector<int> twos;
		for(int i = 0; i < N; i++){
			if(i == non1) continue;
			if(A[i] > 2){
				do_op_residue(i);
			} else if(A[i] == 2){
				twos.push_back(i);
			}
		}
		if(N == 2 && twos.size()){
			assert(ops.empty());
			int two = twos.back();
			if(A[non1] == 2){
				do_op(two, non1, 3);
				assert(A[non1] == 6);
			}
			if(A[non1] != 1){
				do_op(non1, two, A[non1]-1);
				assert(A[non1] == 1);
				do_op(two, non1, A[two]-1);
				assert(A[two] == 1);
				do_op(non1, two, 2);
				assert(A[two] == 2);
				assert(A[non1] == 1);
			}
			do_op(non1, two, 4);
			do_op(two, non1, 7);
			do_op(non1, two, 3);
			do_op(two, non1, 2);
			assert(A[two] == 1);
			assert(A[non1] == 2);
		} else {
			if(twos.size() >= 2){
				int six = twos.back();
				twos.pop_back();
				{
					int y = twos.back();
					do_op(y, six, 3);
					assert(A[six] == 6);
				}
				while(twos.size() >= 2){
					int a = twos.back(); twos.pop_back();
					int b = twos.back(); twos.pop_back();
					do_op(six, a, 5);
					assert(A[six] == 1);
					assert(A[a] == 10);
					do_op(a, b, 3);
					assert(A[a] == 1);
					assert(A[b] == 6);
					six = b;
				}
				do_op_residue(six);
				assert(A[six] == 1);
				if(!twos.empty()){
					int r = twos.back();
					twos.pop_back();
					ops.push_back({six, r, 3});
					A[r] *= 3;
					do_op_residue(r);
				}
				assert(twos.empty());
			} else if(twos.size() == 1){
				int two = twos.back();
				int x = 0;
				while(x == non1 || x == two) x++;
				assert(A[x] == 1);
				do_op(x, two, 3);
				assert(A[x] == 1);
				assert(A[two] == 6);
				do_op_residue(two);
			}
			assert(residue != 0);
			int x = 0;
			while(x == non1) x++;
			assert(A[x] == 1);
			ops.push_back({non1, x, P});
			A[non1] = residue;
			A[x] = P;
			do_op(x, non1, 2);
			if(A[non1] > 2){
				do_op(non1, x, A[non1]-1);
				assert(A[non1] == 1);
				do_op(x, non1, 2);
			}
		}
		assert(A == B);

		reverse(rev_ops.begin(), rev_ops.end());
		ops.insert(ops.end(), rev_ops.begin(), rev_ops.end());
		return ops;
	}();
	if(outp == bad){
		cout << "No" << '\n';
	} else {
		cout << "Yes" << '\n';
		cout << outp.size() << '\n';
		for(auto [a, b, c] : outp){
			cout << (a+1) << ' ' << (b+1) << ' ' << c << '\n';
		}

		// vector<ll> A2 = oA;
		// for(auto [a, b, c] : outp){
		// 	cout << flush;
		// 	assert(c >= 1 && c <= int(1e9));
		// 	A2[a] %= c;
		// 	A2[b] *= c;
		// }
		// if(A2 != oB){
		// 	A2 = oA;
		// 	cerr << "doing " << '\n';

		// 	for(auto x : A2) cerr << x << ' ';
		// 	cerr << '\n';
		// 	for(auto [a, b, c] : outp){
		// 		assert(c >= 1 && c <= int(1e9));
		// 		A2[a] %= c;
		// 		A2[b] *= c;
		// 		for(auto x : A2) cerr << x << ' ';
		// 		cerr << '\n';
		// 	}
		// }
		// assert(A2 == oB);
	}
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	while(T--) solve();
}