// https://judge.yosupo.jp/submission/354026

#line 1 "main.cpp"
// @brief Log of Set Power Series
#define PROBLEM "https://judge.yosupo.jp/problem/log_of_set_power_series"
#pragma GCC optimize("O3,unroll-loops")
#include <bits/allocator.h>
//#pragma GCC target("avx2")
#include <iostream>
#line 1 "cp-algorithms-aux/blazingio/blazingio.min.hpp"
// NOLINTBEGIN
// clang-format off
// DO NOT REMOVE THIS MESSAGE. The mess that follows is a minified build of
// https://github.com/purplesyringa/blazingio. Refer to the repository for
// a human-readable version and documentation.
// Options: cbfoiedrhWLMXaIaAn
#define M$(x,...)_mm256_##x##_epi8(__VA_ARGS__)
#define $u(...)__VA_ARGS__
#if __APPLE__
#define $m(A,B)A
#else
#define $m(A,B)B
#endif
#if _WIN32
#define $w(A,B)A
#else
#define $w(A,B)B
#endif
#if __i386__|_M_IX86
#define $H(A,B)A
#else
#define $H(A,B)B
#endif
#if __aarch64__
#define $a(A,B)A
#else
#define $a(A,B)B
#endif
#define $P(x)void F(x K){
#define $T template<$c T
#define $c class
#define $C constexpr
#define $R return
#define $O operator
#define u$ uint64_t
#define $r $R*this;
#include<array>
#include<bitset>
#include<complex>
#include<cstring>
#include $a(<arm_neon.h>,<immintrin.h>)
#include<stdint.h>
#include $w(<windows.h>,<sys/mman.h>)
#include<sys/stat.h>
#include $w(<io.h>,<unistd.h>)
#include $w(<ios>,<sys/resource.h>)
#if _MSC_VER
#define __builtin_add_overflow(a,b,c)_addcarry_u64(0,a,b,c)
#define $s
#else
$H(,u$ _umul128(u$ a,u$ b,u$*D){auto x=(__uint128_t)a*b;*D=u$(x>>64);$R(u$)x;})
#define $s $a(,__attribute__((target("avx2"))))
#endif
#define $z $a(16,32)
#define $t $a(uint8x16_t,__m256i)
#define $I $w(__forceinline,__attribute__((always_inline)))
#define $F M(),
#define E$(x)if(!(x))abort();
$w(LONG WINAPI $x(_EXCEPTION_POINTERS*);,)namespace $f{using namespace std;struct B{enum $c A:char{}c;B&$O=(char x){c=A{x};$r}$O char(){$R(char)c;}};$C u$ C=~0ULL/255;struct D{string&K;};static B E[65568];template<int F>struct G{B*H,*S;void K(off_t C){$w(char*D=(char*)VirtualAlloc(0,(C+8191)&-4096,8192,1);E$(D)E$(VirtualFree(D,0,32768))DWORD A=C&-65536;E$(!A||MapViewOfFileEx(CreateFileMapping(GetStdHandle(-10),0,2,0,A,0),4,0,0,0,D)==D)E$(VirtualAlloc(D+A,65536,12288,4)==D+A)E$(~_lseek(0,A,0))DWORD E=0;ReadFile(GetStdHandle(-10),D+A,65536,&E,0);,int A=getpagesize();char*D=(char*)mmap(0,C+A,3,2,0,0);E$(D!=(void*)-1)E$(mmap(D+((C+A-1)&-A),A,3,$m(4114,50),-1,0)!=(void*)-1))H=(B*)D+C;*H=10;H[1]=48;H[2]=0;S=(B*)D;}void L(){H=S=E;}$I void M(){if(F&&S==H){$w(DWORD A=0;ReadFile(GetStdHandle(-10),S=E,65536,&A,0);,$a($u(register long A asm("x0")=0,D asm("x1")=(long)E,G asm("x2")=65536,C asm($m("x16","x8"))=$m(3,63);asm volatile("svc 0" $m("x80",):"+r"(A),"+r"(D):"r"(C),"r"(G));S=launder(E);),off_t A=$H(3,$m(33554435,0));B*D=E;asm volatile($H("int $128","syscall"):"+a"(A),$H("+c"(D):"b","+S"(D):"D")(0),"d"(65536)$H(,$u(:"rcx","r11")));S=D;))H=S+A;*H=10;if(!A)E[1]=48,E[2]=0;}}$T>$I void N(T&x){while($F(*S&240)==48)x=T(x*10+(*S++-48));}$T>$I decltype((void)~T{1})O(T&x){M();int A=is_signed_v<T>&&*S==45;S+=A;N(x=0);x=A?1+~x:x;}$T>$I decltype((void)T{1.})O(T&x){M();int A=*S==45;S+=A;$F S+=*S==43;u$ n=0;int i=0;for(;i<18&&($F*S&240)==48;i++)n=n*10+*S++-48;int B=20;int C=*S==46;S+=C;for(;i<18&&($F*S&240)==48;i++)n=n*10+*S++-48,B-=C;x=(T)n;while(($F*S&240)==48)x=x*10+*S++-48,B-=C;if(*S==46)S++,C=1;while(($F*S&240)==48)x=x*10+*S++-48,B-=C;int D;if((*S|32)==101)S++,$F S+=*S==43,O(D),B+=D;static $C auto E=[](){array<T,41>E{};T x=1;for(int i=21;i--;)E[40-i]=x,E[i]=1/x,x*=10;$R E;}();while(B>40)x*=(T)1e10,B-=10;while(B<0)x*=(T)1e-10,B+=10;x*=E[B];x=A?-x:x;}$I void O(bool&x){$F x=*S++==49;}$I void O(char&x){$F x=*S++;}$I void O(uint8_t&x){$F x=*S++;}$I void O(int8_t&x){$F x=*S++;}$T>$s void P(string&K,T C){M();B*G=S;C();K.assign((char*)G,S-G);while(F&&S==H&&($F H!=E)){C();K.append(E,S);}}$s void O(string&K){P(K,[&]()$s{B*p=S;$w(ULONG R;,)$t x;$a(uint64x2_t A;while(memcpy(&x,p,16),A=uint64x2_t(x<33),!(A[0]|A[1]))p+=16;S=p+(A[0]?0:8)+$w((_BitScanForward64(&R,A[0]?A[0]:A[1]),R),__builtin_ctzll(A[0]?A[0]:A[1]))/8;,int J;$t C=M$(set1,32);while(memcpy(&x,p,32),!(J=M$(movemask,M$(cmpeq,C,_mm256_max_epu8(C,x)))))p+=32;S=p+$w((_BitScanForward(&R,J),R),__builtin_ctz(J));)});}$s void O(D&A){P(A.K,[&](){S=(B*)memchr(S,10,H-S+1);});if(A.K.size()&&A.K.back()==13)A.K.pop_back();if(A.K.empty()||S<H)S+=*S==10;}$T>$I void O(complex<T>&K){T A,B{};if($F*S==40){S++;O(A);if($F*S++==44)Q(B),S++;}else O(A);K={A,B};}template<size_t N>$s void O(bitset<N>&K){if(N>4095&&!*this)$R;ptrdiff_t i=N;while(i)if($F i%$z||H-S<$z)K[--i]=*S++==49;else{B*p=S;for(int64_t j=0;j<min(i,H-S)/$z;j++){i-=$z;$t x;memcpy(&x,p,$z);$a(auto B=(uint8x16_t)vdupq_n_u64(~2ULL/254)&(48-x);auto C=vzip_u8(vget_high_u8(B),vget_low_u8(B));auto y=vaddvq_u16((uint16x8_t)vcombine_u8(C.val[0],C.val[1]));,u$ a=~0ULL/65025;auto y=$w(_byteswap_ulong,__builtin_bswap32)(M$(movemask,M$(shuffle,_mm256_slli_epi32(x,7),_mm256_set_epi64x(a+C*24,a+C*16,a+C*8,a))));)p+=$z;memcpy((char*)&K+i/8,&y,$z/8);}S=p;}}$T>$I void Q(T&K){if(!is_same_v<T,D>)while($F(uint8_t)*S<33)S++;O(K);}$O bool(){$R!!*this;}bool $O!(){$R S>H;}};struct U{G<0>A;G<1>B;U(){struct stat D;E$(~fstat(0,&D))(D.st_mode>>12)==8?A.K(D.st_size):B.L();}U*tie(nullptr_t){$R this;}void sync_with_stdio(bool){}$T>$I U&$O>>(T&K){A.S?A.Q(K):B.Q(K);$r}$O bool(){$R!!*this;}bool $O!(){$R A.S?!A:!B;}};short A[100];char L[64]{1};struct
V{char*D;B*S;int J;V(){$w(E$(D=(char*)VirtualAlloc(0,536870912,8192,4))E$(VirtualAlloc(D,4096,4096,260))AddVectoredExceptionHandler(1,$x);,size_t C=536870912;$m(,rlimit E;getrlimit(RLIMIT_AS,&E);if(~E.rlim_cur)C=25165824;)D=(char*)mmap(0,C,3,$m(4162,16418),-1,0);E$(D!=(void*)-1))S=(B*)D;for(int i=0;i<100;i++)A[i]=short((48+i/10)|((48+i%10)<<8));for(int i=1;i<64;i++)L[i]=L[i-1]+(0x8922489224892249>>i&1);}~V(){flush($w(!J,));}void flush($w(int F=0,)){$w(J=1;auto E=GetStdHandle(-11);auto C=F?ReOpenFile(E,1073741824,7,2684354560):(void*)-1;DWORD A;E$(C==(void*)-1?WriteFile(E,D,DWORD((char*)S-D),&A,0):(WriteFile(C,D,DWORD(((char*)S-D+4095)&-4096),&A,0)&&~_chsize(1,int((char*)S-D)))),auto G=D;ssize_t A;while((A=write(1,G,(char*)S-G))>0)G+=A;E$(~A))S=(B*)D;}$P(char)*S++=K;}$P(uint8_t)*S++=K;}$P(int8_t)*S++=K;}$P(bool)*S++=48+K;}$T>decltype((void)~T{1})F(T K){using D=make_unsigned_t<T>;D C=K;if(K<0)F('-'),C=1+~C;static $C auto N=[](){array<D,5*sizeof(T)/2>N{};D n=1;for(size_t i=1;i<N.size();i++)n*=10,N[i]=n;$R N;}();$w(ULONG M;,)int G=L[$w(($H(_BitScanReverse(&M,ULONG((int64_t)C>>32))?M+=32:_BitScanReverse(&M,(ULONG)C|1),_BitScanReverse64(&M,C|1)),M),63^__builtin_clzll(C|1))];G-=C<N[G-1];short H[20];if $C(sizeof(T)==2){auto n=33555U*C-C/2;u$ H=A[n>>25];n=(n&33554431)*25;H|=A[n>>23]<<16;H|=u$(48+((n&8388607)*5>>22))<<32;H>>=40-G*8;memcpy(S,&H,8);}else if $C(sizeof(T)==4){auto n=1441151881ULL*C;$H(n>>=25;n++;for(int i=0;i<5;i++){H[i]=A[n>>32];n=(n&~0U)*100;},int K=57;auto J=~0ULL>>7;for(int i=0;i<5;i++){H[i]=A[n>>K];n=(n&J)*25;K-=2;J/=4;})memcpy(S,(B*)H+10-G,16);}else{$H($u(if(C<(1ULL<<32)){$R F((uint32_t)C);}auto J=(u$)1e10;auto x=C/J,y=C%J;int K=100000,b[]{int(x/K),int(x%K),int(y/K),int(y%K)};B H[40];for(int i=0;i<4;i++){int n=int((429497ULL*b[i]>>7)+1);B*p=H+i*5;*p=48+char(n>>25);n=(n&~0U>>7)*25;memcpy(p+1,A+(n>>23),2);memcpy(p+3,A+((n&~0U>>9)*25>>21),2);}),$u(u$ D,E=_umul128(18,C,&D),F;_umul128(0x725dd1d243aba0e8,C,&F);D+=__builtin_add_overflow(E,F+1,&E);for(int i=0;i<10;i++)H[i]=A[D],E=_umul128(100,E,&D);))memcpy(S,(B*)H+20-G,20);}S+=G;}$T>decltype((void)T{1.})F(T K){if(K<0)F('-'),K=-K;auto G=[&](){auto x=u$(K*1e12);$H($u(x-=x>999999999999;uint32_t n[]{uint32_t(x/1000000*429497>>7)+1,uint32_t(x%1000000*429497>>7)+1};int K=25,J=~0U>>7;for(int i=0;i<3;i++){for(int j=0;j<2;j++)memcpy(S+i*2+j*6,A+(n[j]>>K),2),n[j]=(n[j]&J)*25;K-=2;J/=4;}S+=12;),$u(u$ D,E=_umul128(472236648287,x,&D)>>8;E|=D<<56;D>>=8;E++;for(int i=0;i<6;i++)memcpy(S,A+D,2),S+=2,E=_umul128(100,E,&D);))};if(K==0)$R F('0');if(K>=1e16){K*=(T)1e-16;int B=16;while(K>=1)K*=(T).1,B++;F("0.");G();F('e');F(B);}else if(K>=1){auto B=(u$)K;F(B);if((K-=(T)B)>0)F('.'),G();}else F("0."),G();}$P(const char*)$w(size_t A=strlen(K);memcpy((char*)S,K,A);S+=A;,S=(B*)stpcpy((char*)S,K);)}$P(const uint8_t*)F((char*)K);}$P(const int8_t*)F((char*)K);}$P(string_view)memcpy(S,K.data(),K.size());S+=K.size();}$T>$P(complex<T>)*this<<'('<<K.real()<<','<<K.imag()<<')';}template<size_t N>$s $P(const bitset<N>&)auto i=N;while(i%$z)*S++=48+K[--i];B*p=S;while(i){i-=$z;$a(short,int)x;memcpy(&x,(char*)&K+i/8,$z/8);$a(auto A=(uint8x8_t)vdup_n_u16(x);vst1q_u8((uint8_t*)p,48-vtstq_u8(vcombine_u8(vuzp2_u8(A,A),vuzp1_u8(A,A)),(uint8x16_t)vdupq_n_u64(~2ULL/254)));,auto b=_mm256_set1_epi64x(~2ULL/254);_mm256_storeu_si256(($t*)p,M$(sub,M$(set1,48),M$(cmpeq,_mm256_and_si256(M$(shuffle,_mm256_set1_epi32(x),_mm256_set_epi64x(0,C,C*2,C*3)),b),b)));)p+=$z;}S=p;}$T>V&$O<<(const T&K){F(K);$r}V&$O<<(V&(*A)(V&)){$R A(*this);}};struct W{$T>W&$O<<(const T&K){$r}W&$O<<(W&(*A)(W&)){$R A(*this);}};}namespace std{$f::U i$;$f::V o$;$f::W e$;$f::U&getline($f::U&B,string&K){$f::D A{K};$R B>>A;}$f::V&flush($f::V&B){if(!i$.A.S)B.flush();$R B;}$f::V&endl($f::V&B){$R B<<'\n'<<flush;}$f::W&endl($f::W&B){$R B;}$f::W&flush($f::W&B){$R B;}}$w(LONG WINAPI $x(_EXCEPTION_POINTERS*A){auto C=A->ExceptionRecord;auto B=C->ExceptionInformation[1];if(C->ExceptionCode==2147483649&&B-(ULONG_PTR)std::o$.D<0x40000000){E$(VirtualAlloc((char*)B,16777216,4096,4)&&VirtualAlloc((char*)(B+16777216),4096,4096,260))$R-1;}$R 0;},)
#define freopen(...)if(freopen(__VA_ARGS__)==stdin)std::i$=$f::U{}
#define cin i$
#define cout o$
#ifdef ONLINE_JUDGE
#define cerr e$
#define clog e$
#endif
// End of blazingio
// NOLINTEND
// clang-format on
#line 8 "main.cpp"
#define CP_ALGO_CHECKPOINT
#line 1 "cp-algorithms-aux/cp-algo/number_theory/modint.hpp"


#line 1 "cp-algorithms-aux/cp-algo/math/common.hpp"


#include <functional>
#include <cstdint>
#include <cassert>
#include <bit>
namespace cp_algo::math {
#ifdef CP_ALGO_MAXN
    const int maxn = CP_ALGO_MAXN;
#else
    const int maxn = 1 << 19;
#endif
    const int magic = 64; // threshold for sizes to run the naive algo

    auto bpow(auto const& x, auto n, auto const& one, auto op) {
        if (n == 0) {
            return one;
        }
        auto ans = x;
        for(int j = std::bit_width<uint64_t>(n) - 2; ~j; j--) {
            ans = op(ans, ans);
            if((n >> j) & 1) {
                ans = op(ans, x);
            }
        }
        return ans;
    }
    auto bpow(auto x, auto n, auto ans) {
        return bpow(x, n, ans, std::multiplies{});
    }
    template<typename T>
    T bpow(T const& x, auto n) {
        return bpow(x, n, T(1));
    }
    inline constexpr auto inv2(auto x) {
        assert(x % 2);
        std::make_unsigned_t<decltype(x)> y = 1;
        while(y * x != 1) {
            y *= 2 - x * y;
        }
        return y;
    }
}

#line 6 "cp-algorithms-aux/cp-algo/number_theory/modint.hpp"
namespace cp_algo::math {

    template<typename modint, typename _Int>
    struct modint_base {
        using Int = _Int;
        using UInt = std::make_unsigned_t<Int>;
        static constexpr size_t bits = sizeof(Int) * 8;
        using Int2 = std::conditional_t<bits <= 32, int64_t, __int128_t>;
        using UInt2 = std::conditional_t<bits <= 32, uint64_t, __uint128_t>;
        constexpr static Int mod() {
            return modint::mod();
        }
        constexpr static Int remod() {
            return modint::remod();
        }
        constexpr static UInt2 modmod() {
            return UInt2(mod()) * mod();
        }
        constexpr modint_base() = default;
        constexpr modint_base(Int2 rr) {
            to_modint().setr(UInt((rr + modmod()) % mod()));
        }
        constexpr modint inv() const {
            return bpow(to_modint(), mod() - 2);
        }
        modint operator - () const {
            modint neg;
            neg.r = std::min(-r, remod() - r);
            return neg;
        }
        modint& operator /= (const modint &t) {
            return to_modint() *= t.inv();
        }
        modint& operator *= (const modint &t) {
            r = UInt(UInt2(r) * t.r % mod());
            return to_modint();
        }
        modint& operator += (const modint &t) {
            r += t.r; r = std::min(r, r - remod());
            return to_modint();
        }
        modint& operator -= (const modint &t) {
            r -= t.r; r = std::min(r, r + remod());
            return to_modint();
        }
        modint operator + (const modint &t) const {return modint(to_modint()) += t;}
        modint operator - (const modint &t) const {return modint(to_modint()) -= t;}
        modint operator * (const modint &t) const {return modint(to_modint()) *= t;}
        modint operator / (const modint &t) const {return modint(to_modint()) /= t;}
        // Why <=> doesn't work?..
        auto operator == (const modint &t) const {return to_modint().getr() == t.getr();}
        auto operator != (const modint &t) const {return to_modint().getr() != t.getr();}
        auto operator <= (const modint &t) const {return to_modint().getr() <= t.getr();}
        auto operator >= (const modint &t) const {return to_modint().getr() >= t.getr();}
        auto operator < (const modint &t) const {return to_modint().getr() < t.getr();}
        auto operator > (const modint &t) const {return to_modint().getr() > t.getr();}
        Int rem() const {
            UInt R = to_modint().getr();
            return R - (R > (UInt)mod() / 2) * mod();
        }
        constexpr void setr(UInt rr) {
            r = rr;
        }
        constexpr UInt getr() const {
            return r;
        }

        // Only use these if you really know what you're doing!
        static uint64_t modmod8() {return uint64_t(8 * modmod());}
        void add_unsafe(UInt t) {r += t;}
        void pseudonormalize() {r = std::min(r, r - modmod8());}
        modint const& normalize() {
            if(r >= (UInt)mod()) {
                r %= mod();
            }
            return to_modint();
        }
        void setr_direct(UInt rr) {r = rr;}
        UInt getr_direct() const {return r;}
    protected:
        UInt r;
    private:
        constexpr modint& to_modint() {return static_cast<modint&>(*this);}
        constexpr modint const& to_modint() const {return static_cast<modint const&>(*this);}
    };
    template<typename modint>
    concept modint_type = std::is_base_of_v<modint_base<modint, typename modint::Int>, modint>;
    template<modint_type modint>
    decltype(std::cin)& operator >> (decltype(std::cin) &in, modint &x) {
        typename modint::UInt r;
        auto &res = in >> r;
        x.setr(r);
        return res;
    }
    template<modint_type modint>
    decltype(std::cout)& operator << (decltype(std::cout) &out, modint const& x) {
        return out << x.getr();
    }

    template<auto m>
    struct modint: modint_base<modint<m>, decltype(m)> {
        using Base = modint_base<modint<m>, decltype(m)>;
        using Base::Base;
        static constexpr Base::Int mod() {return m;}
        static constexpr Base::UInt remod() {return m;}
        auto getr() const {return Base::r;}
    };

    template<typename Int = int>
    struct dynamic_modint: modint_base<dynamic_modint<Int>, Int> {
        using Base = modint_base<dynamic_modint<Int>, Int>;
        using Base::Base;

        static Base::UInt m_reduce(Base::UInt2 ab) {
            if(mod() % 2 == 0) [[unlikely]] {
                return typename Base::UInt(ab % mod());
            } else {
                typename Base::UInt2 m = typename Base::UInt(ab) * imod();
                return typename Base::UInt((ab + m * mod()) >> Base::bits);
            }
        }
        static Base::UInt m_transform(Base::UInt a) {
            if(mod() % 2 == 0) [[unlikely]] {
                return a;
            } else {
                return m_reduce(a * pw128());
            }
        }
        dynamic_modint& operator *= (const dynamic_modint &t) {
            Base::r = m_reduce(typename Base::UInt2(Base::r) * t.r);
            return *this;
        }
        void setr(Base::UInt rr) {
            Base::r = m_transform(rr);
        }
        Base::UInt getr() const {
            typename Base::UInt res = m_reduce(Base::r);
            return std::min(res, res - mod());
        }
        static Int mod() {return m;}
        static Int remod() {return 2 * m;}
        static Base::UInt imod() {return im;}
        static Base::UInt2 pw128() {return r2;}
        static void switch_mod(Int nm) {
            m = nm;
            im = m % 2 ? inv2(-m) : 0;
            r2 = static_cast<Base::UInt>(static_cast<Base::UInt2>(-1) % m + 1);
        }

        // Wrapper for temp switching
        auto static with_mod(Int tmp, auto callback) {
            struct scoped {
                Int prev = mod();
                ~scoped() {switch_mod(prev);}
            } _;
            switch_mod(tmp);
            return callback();
        }
    private:
        static thread_local Int m;
        static thread_local Base::UInt im, r2;
    };
    template<typename Int>
    Int thread_local dynamic_modint<Int>::m = 1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::im = -1;
    template<typename Int>
    dynamic_modint<Int>::Base::UInt thread_local dynamic_modint<Int>::r2 = 0;
}

#line 1 "cp-algorithms-aux/cp-algo/math/subset_convolution.hpp"


#line 1 "cp-algorithms-aux/cp-algo/util/simd.hpp"


#include <experimental/simd>
#line 5 "cp-algorithms-aux/cp-algo/util/simd.hpp"
#include <cstddef>
#include <memory>

#if defined(__x86_64__) && !defined(CP_ALGO_DISABLE_AVX2)
#define CP_ALGO_SIMD_AVX2_TARGET _Pragma("GCC target(\"avx2\")")
#else
#define CP_ALGO_SIMD_AVX2_TARGET
#endif

#define CP_ALGO_SIMD_PRAGMA_PUSH \
    _Pragma("GCC push_options") \
    CP_ALGO_SIMD_AVX2_TARGET

CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo {
    template<typename T, size_t len>
    using simd [[gnu::vector_size(len * sizeof(T))]] = T;
    using u64x8 = simd<uint64_t, 8>;
    using u32x16 = simd<uint32_t, 16>;
    using i64x4 = simd<int64_t, 4>;
    using u64x4 = simd<uint64_t, 4>;
    using u32x8 = simd<uint32_t, 8>;
    using u16x16 = simd<uint16_t, 16>;
    using i32x4 = simd<int32_t, 4>;
    using u32x4 = simd<uint32_t, 4>;
    using u16x8 = simd<uint16_t, 8>;
    using u16x4 = simd<uint16_t, 4>;
    using i16x4 = simd<int16_t, 4>;
    using u8x32 = simd<uint8_t, 32>;
    using u8x16 = simd<uint8_t, 16>;
    using u8x8 = simd<uint8_t, 8>;
    using u8x4 = simd<uint8_t, 4>;
    using dx4 = simd<double, 4>;

    inline dx4 abs(dx4 a) {
        return dx4{
            std::abs(a[0]),
            std::abs(a[1]),
            std::abs(a[2]),
            std::abs(a[3])
        };
    }

    // https://stackoverflow.com/a/77376595
    // works for ints in (-2^51, 2^51)
    static constexpr dx4 magic = dx4() + (3ULL << 51);
    inline i64x4 lround(dx4 x) {
        return i64x4(x + magic) - i64x4(magic);
    }
    inline dx4 to_double(i64x4 x) {
        return dx4(x + i64x4(magic)) - magic;
    }

    inline dx4 round(dx4 a) {
        return dx4{
            std::nearbyint(a[0]),
            std::nearbyint(a[1]),
            std::nearbyint(a[2]),
            std::nearbyint(a[3])
        };
    }

    inline u64x4 low32(u64x4 x) {
        return x & uint32_t(-1);
    }
    inline auto swap_bytes(auto x) {
        return decltype(x)(__builtin_shufflevector(u32x8(x), u32x8(x), 1, 0, 3, 2, 5, 4, 7, 6));
    }
    inline u64x4 montgomery_reduce(u64x4 x, uint32_t mod, uint32_t imod) {
#ifdef __AVX2__
        auto x_ninv = u64x4(_mm256_mul_epu32(__m256i(x), __m256i() + imod));
        x += u64x4(_mm256_mul_epu32(__m256i(x_ninv), __m256i() + mod));
#else
        auto x_ninv = u64x4(u32x8(low32(x)) * imod);
        x += x_ninv * uint64_t(mod);
#endif
        return swap_bytes(x);
    }

    inline u64x4 montgomery_mul(u64x4 x, u64x4 y, uint32_t mod, uint32_t imod) {
#ifdef __AVX2__
        return montgomery_reduce(u64x4(_mm256_mul_epu32(__m256i(x), __m256i(y))), mod, imod);
#else
        return montgomery_reduce(x * y, mod, imod);
#endif
    }
    inline u32x8 montgomery_mul(u32x8 x, u32x8 y, uint32_t mod, uint32_t imod) {
        return u32x8(montgomery_mul(u64x4(x), u64x4(y), mod, imod)) |
               u32x8(swap_bytes(montgomery_mul(u64x4(swap_bytes(x)), u64x4(swap_bytes(y)), mod, imod)));
    }
    inline dx4 rotate_right(dx4 x) {
        static constexpr u64x4 shuffler = {3, 0, 1, 2};
        return __builtin_shuffle(x, shuffler);
    }

    template<std::size_t Align = 32>
    inline bool is_aligned(const auto* p) noexcept {
        return (reinterpret_cast<std::uintptr_t>(p) % Align) == 0;
    }

    template<class Target>
    inline Target& vector_cast(auto &&p) {
        return *reinterpret_cast<Target*>(std::assume_aligned<alignof(Target)>(&p));
    }
}
#pragma GCC pop_options

#line 1 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"



#include <set>
#include <map>
#include <deque>
#include <stack>
#include <queue>
#include <vector>
#include <string>
#line 13 "cp-algorithms-aux/cp-algo/util/big_alloc.hpp"
#include <generator>
#include <forward_list>

// Single macro to detect POSIX platforms (Linux, Unix, macOS)
#if defined(__linux__) || defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#  define CP_ALGO_USE_MMAP 1
#  include <sys/mman.h>
#else
#  define CP_ALGO_USE_MMAP 0
#endif

namespace cp_algo {
    template <typename T, size_t Align = 32>
    class big_alloc {
        static_assert( Align >= alignof(void*), "Align must be at least pointer-size");
        static_assert(std::popcount(Align) == 1, "Align must be a power of two");
    public:
        using value_type = T;
        template <class U> struct rebind { using other = big_alloc<U, Align>; };
        constexpr bool operator==(const big_alloc&) const = default;
        constexpr bool operator!=(const big_alloc&) const = default;

        big_alloc() noexcept = default;
        template <typename U, std::size_t A>
        big_alloc(const big_alloc<U, A>&) noexcept {}

        [[nodiscard]] T* allocate(std::size_t n) {
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align = std::max<std::size_t>(alignof(T),  Align);
#if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) {
                void* raw = mmap(nullptr, padded,
                                PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
                madvise(raw, padded, MADV_HUGEPAGE);
                madvise(raw, padded, MADV_POPULATE_WRITE);
                return static_cast<T*>(raw);
            }
#endif
            return static_cast<T*>(::operator new(padded, std::align_val_t(align)));
        }

        void deallocate(T* p, std::size_t n) noexcept {
            if (!p) return;
            std::size_t padded = round_up(n * sizeof(T));
            std::size_t align  = std::max<std::size_t>(alignof(T),  Align);
    #if CP_ALGO_USE_MMAP
            if (padded >= MEGABYTE) { munmap(p, padded); return; }
    #endif
            ::operator delete(p, padded, std::align_val_t(align));
        }

    private:
        static constexpr std::size_t MEGABYTE = 1 << 20;
        static constexpr std::size_t round_up(std::size_t x) noexcept {
            return (x + Align - 1) / Align * Align;
        }
    };

    template<typename T> using big_vector = std::vector<T, big_alloc<T>>;
    template<typename T> using big_basic_string = std::basic_string<T, std::char_traits<T>, big_alloc<T>>;
    template<typename T> using big_deque = std::deque<T, big_alloc<T>>;
    template<typename T> using big_stack = std::stack<T, big_deque<T>>;
    template<typename T> using big_queue = std::queue<T, big_deque<T>>;
    template<typename T> using big_priority_queue = std::priority_queue<T, big_vector<T>>;
    template<typename T> using big_forward_list = std::forward_list<T, big_alloc<T>>;
    using big_string = big_basic_string<char>;

    template<typename Key, typename Value, typename Compare = std::less<Key>>
    using big_map = std::map<Key, Value, Compare, big_alloc<std::pair<const Key, Value>>>;
    template<typename T, typename Compare = std::less<T>>
    using big_multiset = std::multiset<T, Compare, big_alloc<T>>;
    template<typename T, typename Compare = std::less<T>>
    using big_set = std::set<T, Compare, big_alloc<T>>;
    template<typename Ref, typename V = void>

    using big_generator = std::generator<Ref, V, big_alloc<std::byte>>;
}

// Deduction guide to make elements_of with big_generator default to big_alloc
namespace std::ranges {
    template<typename Ref, typename V>
    elements_of(cp_algo::big_generator<Ref, V>&&) -> elements_of<cp_algo::big_generator<Ref, V>&&, cp_algo::big_alloc<std::byte>>;
}


#line 1 "cp-algorithms-aux/cp-algo/util/bit.hpp"


#line 8 "cp-algorithms-aux/cp-algo/util/bit.hpp"

#if defined(__x86_64__) && !defined(CP_ALGO_DISABLE_AVX2)
#define CP_ALGO_BIT_OPS_TARGET _Pragma("GCC target(\"avx2,bmi,bmi2,lzcnt,popcnt\")")
#else
#define CP_ALGO_BIT_OPS_TARGET _Pragma("GCC target(\"bmi,bmi2,lzcnt,popcnt\")")
#endif

#define CP_ALGO_BIT_PRAGMA_PUSH \
    _Pragma("GCC push_options") \
    CP_ALGO_BIT_OPS_TARGET

CP_ALGO_BIT_PRAGMA_PUSH
namespace cp_algo {
    template<typename Uint>
    constexpr size_t bit_width = sizeof(Uint) * 8;

    // n < 64
    uint64_t mask(size_t n) {
        return (1ULL << n) - 1;
    }
    size_t order_of_bit(auto x, size_t k) {
        return k ? std::popcount(x << (bit_width<decltype(x)> - k)) : 0;
    }
    inline size_t kth_set_bit(uint64_t x, size_t k) {
        return std::countr_zero(_pdep_u64(1ULL << k, x));
    }
    template<int fl = 0>
    void with_bit_floor(size_t n, auto &&callback) {
        if constexpr (fl >= 63) {
            return;
        } else if (n >> (fl + 1)) {
            with_bit_floor<fl + 1>(n, callback);
        } else {
            callback.template operator()<1ULL << fl>();
        }
    }
    void with_bit_ceil(size_t n, auto &&callback) {
        with_bit_floor(n, [&]<size_t N>() {
            if(N == n) {
                callback.template operator()<N>();
            } else {
                callback.template operator()<N << 1>();
            }
        });
    }

    inline uint32_t read_bits(char const* p) {
        return _mm256_movemask_epi8(__m256i(vector_cast<u8x32 const>(p[0]) + (127 - '0')));
    }
    inline uint64_t read_bits64(char const* p) {
        return read_bits(p) | (uint64_t(read_bits(p + 32)) << 32);
    }

    inline void write_bits(char *p, uint32_t bits) {
        static constexpr u8x32 shuffler = {
            0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 1, 1, 1, 1, 1, 1,
            2, 2, 2, 2, 2, 2, 2, 2,
            3, 3, 3, 3, 3, 3, 3, 3
        };
        auto shuffled = u8x32(_mm256_shuffle_epi8(__m256i() + bits, __m256i(shuffler)));
        static constexpr u8x32 mask = {
            1, 2, 4, 8, 16, 32, 64, 128,
            1, 2, 4, 8, 16, 32, 64, 128,
            1, 2, 4, 8, 16, 32, 64, 128,
            1, 2, 4, 8, 16, 32, 64, 128
        };
        for(int z = 0; z < 32; z++) {
            p[z] = shuffled[z] & mask[z] ? '1' : '0';
        }
    }
    inline void write_bits64(char *p, uint64_t bits) {
        write_bits(p, uint32_t(bits));
        write_bits(p + 32, uint32_t(bits >> 32));
    }
}
#pragma GCC pop_options

#line 1 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"


#line 5 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"
#include <chrono>
#line 8 "cp-algorithms-aux/cp-algo/util/checkpoint.hpp"
namespace cp_algo {
#ifdef CP_ALGO_CHECKPOINT
    big_map<big_string, double> checkpoints;
    double last;
#endif
    template<bool final = false>
    void checkpoint([[maybe_unused]] auto const& _msg) {
#ifdef CP_ALGO_CHECKPOINT
        big_string msg = _msg;
        double now = (double)clock() / CLOCKS_PER_SEC;
        double delta = now - last;
        last = now;
        if(msg.size() && !final) {
            checkpoints[msg] += delta;
        }
        if(final) {
            for(auto const& [key, value] : checkpoints) {
                std::cerr << key << ": " << value * 1000 << " ms\n";
            }
            std::cerr << "Total: " << now * 1000 << " ms\n";
        }
#endif
    }
    template<bool final = false>
    void checkpoint() {
        checkpoint<final>("");
    }
}

#line 8 "cp-algorithms-aux/cp-algo/math/subset_convolution.hpp"
#include <ranges>
#include <algorithm>
#line 12 "cp-algorithms-aux/cp-algo/math/subset_convolution.hpp"
CP_ALGO_SIMD_PRAGMA_PUSH
namespace cp_algo::math {
#ifndef CP_ALGO_SUBSET_CONVOLUTION_MAX_LOGN
#define CP_ALGO_SUBSET_CONVOLUTION_MAX_LOGN 20
#endif
    const size_t max_logn = CP_ALGO_SUBSET_CONVOLUTION_MAX_LOGN;
    
    template<auto N>
    inline void xor_transform(auto &&a) {
        if constexpr (N >> max_logn) {
            throw std::runtime_error("N too large for xor_transform");
        } else if constexpr (N <= 32) {
            for (size_t i = 1; i < N; i *= 2) {
                for (size_t j = 0; j < N; j += 2 * i) {
                    for (size_t k = j; k < j + i; k++) {
                        for (size_t z = 0; z < max_logn; z++) {
                            auto x = a[k][z] + a[k + i][z];
                            auto y = a[k][z] - a[k + i][z];
                            a[k][z] = x;
                            a[k + i][z] = y;
                        }
                    }
                }
            }
        } else {
            auto add = [&](auto &a, auto &b) __attribute__((always_inline)) {
                auto x = a + b, y = a - b;
                a = x, b = y;
            };
            constexpr auto quar = N / 4;

            for (size_t i = 0; i < (size_t)quar; i++) {
                auto x0 = a[i + (size_t)quar * 0];
                auto x1 = a[i + (size_t)quar * 1];
                auto x2 = a[i + (size_t)quar * 2];
                auto x3 = a[i + (size_t)quar * 3];

                #pragma GCC unroll max_logn
                for (size_t z = 0; z < max_logn; z++) {
                    add(x0[z], x2[z]);
                    add(x1[z], x3[z]);
                }
                #pragma GCC unroll max_logn
                for (size_t z = 0; z < max_logn; z++) {
                    add(x0[z], x1[z]);
                    add(x2[z], x3[z]);
                }

                a[i + (size_t)quar * 0] = x0;
                a[i + (size_t)quar * 1] = x1;
                a[i + (size_t)quar * 2] = x2;
                a[i + (size_t)quar * 3] = x3;
            }
            xor_transform<quar>(&a[quar * 0]);
            xor_transform<quar>(&a[quar * 1]);
            xor_transform<quar>(&a[quar * 2]);
            xor_transform<quar>(&a[quar * 3]);
        }
    }
    
    inline void xor_transform(auto &&a, auto n) {
        with_bit_floor(n, [&]<auto NN>() {
            assert(NN == n);
            xor_transform<NN>(a);
        });
    }
    
    inline void xor_transform(auto &&a) {
        xor_transform(a, std::size(a));
    }

    // Generic rank vectors processor with variadic inputs
    // Assumes output[0] = 0, caller is responsible for handling rank 0
    // Returns the output array
    auto on_rank_vectors(auto &&cb, auto const& ...inputs) {
        static_assert(sizeof...(inputs) >= 1, "on_rank_vectors requires at least one input");
        
        // Create tuple of input references once
        auto input_tuple = std::forward_as_tuple(inputs...);
        auto const& first_input = std::get<0>(input_tuple);
        using base = std::decay_t<decltype(first_input[0])>;
        big_vector<base> out(std::size(first_input));
        
        auto N = std::size(first_input);
        constexpr size_t K = 4;
        N = std::max(N, 2 * K);
        const size_t n = std::bit_width(N) - 1;
        const size_t T = std::min<size_t>(n - 3, 3);
        const size_t bottoms = 1 << (n - T - 1);
        const auto M = std::size(first_input);
        
        // Create array buffers for each input
        auto create_buffers = [bottoms]<typename... Args>(const Args&...) {
            return std::make_tuple(
                big_vector<std::array<typename std::decay_t<Args>::value_type, max_logn>>(bottoms)...
            );
        };
        auto buffers = std::apply(create_buffers, input_tuple);
        
        checkpoint("alloc buffers");
        big_vector<uint32_t> counts(2 * bottoms);
        for(size_t i = 1; i < 2 * bottoms; i++) {
            counts[i] = (uint32_t)std::popcount(i);
        }
        checkpoint("prepare");
        
        for(size_t top = 0; top < N / 2; top += bottoms) {
            // Clear all buffers
            std::apply([bottoms](auto&... bufs) {
                (..., memset(bufs.data(), 0, sizeof(bufs[0]) * bottoms));
            }, buffers);
            checkpoint("memset");
            
            // Initialize buffers from inputs
            std::apply([&](auto const&... inps) {
                std::apply([&](auto&... bufs) {
                    auto init_one = [&](auto const& inp, auto& buf) {
                        for(size_t i = 0; i < M; i += 2 * bottoms) {
                            bool parity = __builtin_parity(uint32_t((i >> 1) & top));
                            size_t limit = std::min(M, i + 2 * bottoms) - i;
                            uint32_t count = (uint32_t)std::popcount(i) - 1;
                            for(size_t bottom = (i == 0); bottom < limit; bottom++) {
                                if (parity) {
                                    buf[bottom >> 1][count + counts[bottom]] -= inp[i + bottom];
                                } else {
                                    buf[bottom >> 1][count + counts[bottom]] += inp[i + bottom];
                                }
                            }
                        }
                    };
                    (init_one(inps, bufs), ...);
                }, buffers);
            }, input_tuple);
            
            checkpoint("init");
            std::apply([](auto&... bufs) {
                (..., xor_transform(bufs));
            }, buffers);
            checkpoint("transform");
            
            assert(bottoms % K == 0);
            for(size_t i = 0; i < bottoms; i += K) {
                std::apply([&](auto&... bufs) {
                    auto extract_one = [&](auto& buf) {
                        std::array<u64x4, max_logn> aa;
                        for(size_t j = 0; j < max_logn; j++) {
                            for(size_t z = 0; z < K; z++) {
                                aa[j][z] = buf[i + z][j].getr();
                            }
                        }
                        return aa;
                    };
                    
                    auto aa_tuple = std::make_tuple(extract_one(bufs)...);
                    std::apply(cb, aa_tuple);
                    
                    // Write results back: only first array needs to be written
                    auto& first_buf = std::get<0>(std::forward_as_tuple(bufs...));
                    const auto& first_aa = std::get<0>(aa_tuple);
                    for(size_t j = 0; j < max_logn; j++) {
                        for(size_t z = 0; z < K; z++) {
                            first_buf[i + z][j].setr((uint32_t)first_aa[j][z]);
                        }
                    }
                }, buffers);
            }
            
            checkpoint("dot");
            auto& first_buf = std::get<0>(buffers);
            xor_transform(first_buf);
            checkpoint("transform");
            
            // Gather results from first buffer

            for(size_t i = 0; i < M; i += 2 * bottoms) {
                bool parity = __builtin_parity(uint32_t((i >> 1) & top));
                size_t limit = std::min(M, i + 2 * bottoms) - i;
                uint32_t count = (uint32_t)std::popcount(i) - 1;
                for(size_t bottom = (i == 0); bottom < limit; bottom++) {
                    if (parity) {
                        out[i + bottom] -= first_buf[bottom >> 1][count + counts[bottom]];
                    } else {
                        out[i + bottom] += first_buf[bottom >> 1][count + counts[bottom]];
                    }
                }
            }
            checkpoint("gather");
        }
        const base ni = base(N / 2).inv();
        for(auto& x : out) {x *= ni;}
        return out;
    }

    template<typename base>
    big_vector<base> subset_convolution(std::span<base> f, std::span<base> g) {
        big_vector<base> outpa;
        constexpr size_t lgn = max_logn;
        outpa = on_rank_vectors([](auto &a, auto const& b) {
            std::decay_t<decltype(a)> res = {};
            const auto mod = base::mod();
            const auto imod = math::inv2(-mod);
            const auto r4 = u64x4() + uint64_t(-1) % mod + 1;
            auto add = [&](size_t i) {
                for(size_t j = 0; i + j + 1 < lgn; j++) {
                    res[i + j + 1] += (u64x4)_mm256_mul_epu32(__m256i(a[i]), __m256i(b[j]));
                }
                if constexpr (lgn >= 20) if (i == 15) {
                    for(size_t k = 0; k < lgn; k++) {
                        res[k] -= (res[k] >= base::modmod8()) & base::modmod8();
                    }
                }
            };
            for(size_t i = 0; i < lgn; i++) { add(i); }
            for(size_t k = 0; k < lgn; k++) {
                res[k] = montgomery_reduce(res[k], mod, imod);
                res[k] = montgomery_mul(res[k], r4, mod, imod);
                a[k] = res[k] >= mod ? res[k] - mod : res[k];
            }
        }, f, g);
        
        outpa[0] = f[0] * g[0];
        for(size_t i = 1; i < std::size(f); i++) {
            outpa[i] += f[i] * g[0] + f[0] * g[i];
        }
        checkpoint("fix 0");
        return outpa;
    }

    template<typename base>
    big_vector<base> subset_div(std::span<base> f, std::span<base> g) {
        big_vector<base> outpa;
        constexpr size_t lgn = max_logn;
        auto f0 = f[0].getr(), gi = g[0].inv().getr();
        outpa = on_rank_vectors([f0, gi](auto &a, auto const& b) {
            const auto mod = base::mod();
            const auto imod = math::inv2(-mod);
            const auto gir4 = u64x4() + (uint64_t(-1) % mod + 1) * gi % mod;
            for(size_t k = 0; k < lgn; k++) {
                for(size_t i = 0; i < k; i++) {
                    a[k] -= (u64x4)_mm256_mul_epu32(__m256i(a[i]), __m256i(b[k - 1 - i]));
                }
                a[k] -= (u64x4)_mm256_mul_epu32(__m256i() + f0, __m256i(b[k]));
                a[k] += base::modmod8();
                a[k] = a[k] >= base::modmod8() ? a[k] + base::modmod8() : a[k];
                a[k] = montgomery_reduce(a[k], mod, imod);
                a[k] = montgomery_mul(a[k], gir4, mod, imod);
                a[k] = a[k] >= mod ? a[k] - mod : a[k];
            }
        }, f, g);
        outpa[0] = f0 * gi;
        checkpoint("fix 0");
        return outpa;
    }

    template<typename base>
    big_vector<base> subset_log(std::span<base> g) {
        if (size(g) == 1) {
            assert(g[0] == base(1));
            return big_vector<base>{0};
        }
        size_t N = std::size(g);
        auto out0 = subset_log(std::span(g).first(N / 2));
        auto out1 = subset_div<base>(std::span(g).last(N / 2), std::span(g).first(N / 2));
        out0.insert(end(out0), begin(out1), end(out1));
        cp_algo::checkpoint("extend out");
        return out0;
    }

    template<typename base>
    big_vector<base> subset_exp(std::span<base> g) {
        if (size(g) == 1) {
            assert(g[0] == base(0));
            return big_vector<base>{1};
        }
        size_t N = std::size(g);
        auto out0 = subset_exp(std::span(g).first(N / 2));
        auto out1 = subset_convolution<base>(out0, std::span(g).last(N / 2));
        out0.insert(end(out0), begin(out1), end(out1));
        cp_algo::checkpoint("extend out");
        return out0;
    }

    template<typename base>
    big_vector<big_vector<base>> subset_compose(std::span<base> f, std::span<base> g, size_t n) {
        if (size(g) == 1) {
            size_t M = size(f);
            big_vector res(n, big_vector<base>{0});
            big_vector<base> pw(std::max(n, M));
            pw[0] = 1;
            for (size_t j = 1; j < M; j++) {
                pw[j] = pw[j - 1] * g[0];
            }
            for (size_t i = 0; i < n; i++) {
                for (size_t j = 0; j < M; j++) {
                    res[i][0] += pw[j] * f[j];
                }
                for (size_t j = M; j > i; j--) {
                    pw[j] = pw[j - 1] * base(j);
                }
                pw[i] = 0;
            }
            cp_algo::checkpoint("base case");
            return res;
        }
        size_t N = std::size(g);
        auto deeper = subset_compose(f, std::span(g).first(N / 2), n + 1);
        for(size_t i = 0; i + 1 < size(deeper); i++) {
            auto next = subset_convolution<base>(deeper[i + 1], std::span(g).last(N / 2));
            deeper[i].insert(end(deeper[i]), begin(next), end(next));
        }
        deeper.pop_back();
        cp_algo::checkpoint("combine");
        return deeper;
    }

    template<typename base>
    big_vector<base> subset_compose(std::span<base> f, std::span<base> g) {
        return subset_compose(f, g, 1)[0];
    }

    // Transpose of f -> f * g = h
    template<typename base>
    big_vector<base> subset_conv_transpose(std::span<base> h, std::span<base> g) {
        std::ranges::reverse(h);
        auto res = subset_convolution<base>(h, g);
        std::ranges::reverse(h);
        std::ranges::reverse(res);
        return res;
    }

    template<typename base>
    big_vector<base> subset_power_projection(big_vector<big_vector<base>> &&fg, std::span<base> g, size_t M) {
        if (size(g) == 1) {
            size_t n = size(fg);
            big_vector<base> res(M);
            big_vector<base> pw(std::max(n, M));
            pw[0] = 1;
            for (size_t j = 1; j < M; j++) {
                pw[j] = pw[j - 1] * g[0];
            }
            for (size_t i = 0; i < size(fg); i++) {
                for (size_t j = 0; j < M; j++) {
                    res[j] += pw[j] * fg[i][0];
                }
                for (size_t j = M; j > i; j--) {
                    pw[j] = pw[j - 1] * base(j);
                }
                pw[i] = 0;
            }
            cp_algo::checkpoint("base case");
            return res;
        }
        size_t N = std::size(g);
        fg.emplace_back(N / 2);
        for(auto&& [i, h]: fg | std::views::enumerate | std::views::reverse | std::views::drop(1)) {
            auto prev = subset_conv_transpose<base>(std::span(h).last(N / 2), std::span(g).last(N / 2));
            for (size_t j = 0; j < N / 2; j++) {
                fg[i + 1][j] += prev[j];
            }
            fg[i + 1].resize(N / 2);
        }
        fg[0].resize(N / 2);
        cp_algo::checkpoint("decombine");
        return subset_power_projection(std::move(fg), std::span(g).first(N / 2), M);
    }

    template<typename base>
    big_vector<base> subset_power_projection(std::span<base> g, std::span<base> w, size_t M) {
        return subset_power_projection({{begin(w), end(w)}}, g, M);
    }
}
#pragma GCC pop_options

#line 11 "main.cpp"
#include <bits/stdc++.h>

using namespace std;

const int mod = 998244353;
using base = cp_algo::math::modint<mod>;

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	std::mt19937 mt(48);
	auto solve_min = [&](std::vector<int> A, int K) -> int64_t {
		int N = int(A.size());
		std::vector<std::pair<int64_t, int>> sums(1 << N);
		sums[0] = {0, 0};
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < (1 << i); j++) {
				sums[(1<<i) + j] = {sums[j].first + A[i], (1<<i) + j};
			}
		}
		std::sort(sums.begin(), sums.end());
		int mi = 0;
		int ma = int(sums.size());
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if ([&]() -> bool {
				assert(md >= 1);
				cp_algo::big_vector<base> a(1 << N);
				for (int z = 0; z < md; z++) {
					a[sums[z].second] = base(mt());
				}
				a[0] = base(1);
				auto b = cp_algo::math::subset_exp<base>(a);
				for (auto& v : b) v *= base(K);
				auto c = cp_algo::math::subset_log<base>(b);
				return c.back() != base(0);
			}()) {
				mi = md;
			} else {
				ma = md;
			}
		}
		return sums[mi].first;
	};

	auto solve_max = [&](std::vector<int> A, int K) -> int64_t {
		int N = int(A.size());
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			int fin = A[i]; A.erase(A.begin() + i);
			ans = std::max(ans, fin + solve_min(A, K));
			A.insert(A.begin() + i, fin);
		}
		return ans;
	};

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;
		std::cout << solve_max(A, K) << '\n';
	}

	return 0;
}

// vim: set ts=4 sts=4 sw=4 noet
