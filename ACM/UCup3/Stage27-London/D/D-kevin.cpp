#include <bits/stdc++.h>
using namespace std;

template<class T> struct Point3D {
	typedef Point3D P;
	typedef const P& R;
	T x, y, z;
	explicit Point3D(T x_=0, T y_=0, T z_=0) : x(x_), y(y_), z(z_) {}
	bool operator<(R p) const {
		return tie(x, y, z) < tie(p.x, p.y, p.z); }
	bool operator==(R p) const {
		return tie(x, y, z) == tie(p.x, p.y, p.z); }
	P operator+(R p) const { return P(x+p.x, y+p.y, z+p.z); }
	P operator-(R p) const { return P(x-p.x, y-p.y, z-p.z); }
	P operator*(T d) const { return P(x*d, y*d, z*d); }
	P operator/(T d) const { return P(x/d, y/d, z/d); }
	T dot(R p) const { return x*p.x + y*p.y + z*p.z; }
	P cross(R p) const {
		return P(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
	}
	T dist2() const { return x*x + y*y + z*z; }
	double dist() const { return sqrt((double)dist2()); }
	//Azimuthal angle (longitude) to x-axis in interval [-pi, pi]
	double phi() const { return atan2(y, x); } 
	//Zenith angle (latitude) to the z-axis in interval [0, pi]
	double theta() const { return atan2(sqrt(x*x+y*y),z); }
	P unit() const { return *this/(T)dist(); } //makes dist()=1
	//returns unit vector normal to *this and p
	P normal(P p) const { return cross(p).unit(); }
	//returns point rotated 'angle' radians ccw around axis
	P rotate(double angle, P axis) const {
		double s = sin(angle), c = cos(angle); P u = axis.unit();
		return u*dot(u)*(1-c) + (*this)*c - cross(u)*s;
	}
};

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int TEST = 1;
	cout << fixed << setprecision(20);
	int N;
	if(TEST){
		N = 10;
	} else {
		cin >> N;
	}
	using ld = long double;
	using P3 = Point3D<ld>;
	mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

	uniform_real_distribution<double> unif(0, 1);
	auto g = [&](){
		ld x = unif(rng);
		return x;
	};

	vector<vector<ld> > A(N, vector<ld>(N, 0));
	if(TEST){
		vector<P3> pts(N);
		for(int i = 0; i < N; i++){
			pts[i] = P3(g(), g(), g());
		}
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				if(i == j){
					A[i][j] = 0;
				} else {
					A[i][j] = A[j][i] = (pts[i] - pts[j]).dist() + unif(rng);
				}
			}
		}
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				cout << A[i][j] << ' ';
			}
			cout << '\n';
		}
	} else {
		for(int i = 0; i < N; i++){
			for(int j = 0; j < N; j++){
				cin >> A[i][j];
			}
		}
	}

	vector<P3> pts(N);
	for(int i = 0; i < N; i++){
		pts[i] = P3(g(), g(), g());
	}

	ld THRESH = 0.0999;
	while(true){
		bool done = false;
		for(int it = 0; it < 5000; it++){
			// for(int i = 0; i < N; i++){
			// 	cout << pts[i].x << ' ' << pts[i].y << ' ' << pts[i].z << ' ';
			// }
			// cout << '\n';
			ld mult = 0.1 * unif(rng);
			bool works = true;
			vector<P3> force(N);
			ld tot = 0;
			for(int i = 0; i < N; i++){
				for(int j = 0; j < i; j++){
					ld d = (pts[i] - pts[j]).dist();
					ld goal = A[i][j];
					if(abs(d - goal) > 0.1){
						works = false;
						tot += abs(d - goal) - 0.1;
					}
					ld bad = abs(d - goal) - 0.09;
					if(bad > 0){
						ld mult2 = mult;
						{
							works = false;
							if(d > goal){
								force[i] = force[i] + (pts[i] - pts[j]).unit() * bad  * mult2;
								force[j] = force[j] + (pts[j] - pts[i]).unit() * bad * mult2;
							} else {
								force[i] = force[i] - (pts[i] - pts[j]).unit() * bad * mult2;
								force[j] = force[j] - (pts[j] - pts[i]).unit() * bad * mult2;
							}
						}
					}
				}
			}
			for(int i = 0; i < N; i++){
				cerr << pts[i].x << ',' << pts[i].y << ',' << pts[i].z << ' ';
			}
			cerr << '\n';
			cerr << "cost " << tot << '\n';
			if(works) {
				done = true;
				break;
			}
			for(int i = 0; i < N; i++){
				pts[i] = pts[i] - force[i];
			}
		}
		if(done){
			P3 st = pts[0];
			for(int i = 0; i < N; i++){
				st.x = min(st.x, pts[i].x);
				st.y = min(st.y, pts[i].y);
				st.z = min(st.z, pts[i].z);
			}
			for(int i = 0; i < N; i++){
				pts[i] = pts[i] - st;
			}
			for(int i = 0; i < N; i++){
				cout << pts[i].x << ' ' << pts[i].y << ' ' << pts[i].z << '\n';
			}
			break;
		}
		break;
	}
}
