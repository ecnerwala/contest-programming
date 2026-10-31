#include <bits/stdc++.h>
#include <cassert>

// ecnerwala/cp-book
// clang-format off
// @formatter:off
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wunknown-warning-option"
#pragma GCC diagnostic ignored "-Wmisleading-indentation"
#pragma GCC diagnostic ignored "-Wmultistatement-macros"
// src/fft/common.hpp
namespace ecnerwala{
template<class T>int sz(T&&arg){using std::size;return int(size(std::forward<T>(arg)));}
inline int nextPow2(int s){return 1<<(s>1?32-__builtin_clz(s-1):0);}
namespace fft{
using std::swap;
using std::vector;
using std::min;
using std::max;
#ifndef ECNERWALA_FFT_POOL_STORAGE
#define ECNERWALA_FFT_POOL_STORAGE
#endif
template<typename T>struct buffer_pool{
static inline ECNERWALA_FFT_POOL_STORAGE std::vector<std::vector<T>>free_list;
struct handle{
std::vector<T>v;
explicit handle(int n){
if(!free_list.empty()){
v=std::move(free_list.back());
free_list.pop_back();
}
v.assign(n,T());
}
handle(const handle&)=delete;
handle&operator=(const handle&)=delete;
handle(handle&&o)noexcept:v(std::move(o.v)){}
~handle(){if(v.capacity())free_list.push_back(std::move(v));}
T&operator[](int i){return v[i];}
operator std::span<T>(){return std::span<T>(v);}
std::span<T>span(){return std::span<T>(v);}
};
static handle get(int n){return handle(n);}
};
}
}
// src/fft/engine.hpp
namespace ecnerwala::fft{
struct assign_op{template<typename T>void operator()(T&d,T v)const{d=v;}};
struct add_op{template<typename T>void operator()(T&d,T v)const{d+=v;}};
struct sub_op{template<typename T>void operator()(T&d,T v)const{d-=v;}};
struct add_twice_op{template<typename T>void operator()(T&d,T v)const{d+=v+v;}};
template<typename E>
concept engine=requires(
std::span<const typename E::value_type>in,
std::span<typename E::value_type>out,
typename E::transformed&t,
const typename E::transformed&ct,
typename E::product&p,
const typename E::product&cp,
int n
){
typename E::value_type;
{E::transform(in,n)}->std::same_as<typename E::transformed>;
{ct.size()}->std::same_as<int>;
E::extend_to(t,n,in);
{E::downsample(ct,n,false)}->std::same_as<typename E::transformed>;
{E::downsample(cp,n,false)}->std::same_as<typename E::product>;
{E::negate_arg(ct,n)}->std::same_as<typename E::transformed>;
{E::mul(ct,ct,n)}->std::same_as<typename E::product>;
{E::sq(ct,n)}->std::same_as<typename E::product>;
{E::mul2(ct,ct,ct,ct,n)}->std::same_as<typename E::template product_t<2*E::unit_scale>>;
E::finish(std::move(p),out);
E::finish(std::move(p),out,add_op{});
E::finish(E::add(std::move(p),std::move(p)),out);
{E::add(E::transform(in,n),ct)}->std::same_as<typename E::template transformed_t<2*E::unit_scale>>;
{E::add(std::move(p),std::move(p))}->std::same_as<typename E::template product_t<2*E::unit_scale>>;
requires std::same_as<std::remove_cvref_t<decltype(E::commutative)>,bool>;
requires std::same_as<std::remove_cvref_t<decltype(E::unit_scale)>,int>;
};
template<typename A,typename B>
concept same_engine=std::same_as<typename A::engine_t,typename B::engine_t>;
template<engine E>using transformed=typename E::transformed;
}
// src/fft/multiply.hpp
namespace ecnerwala::fft{
template<engine E,typename Op=assign_op>
void multiply_circular(std::span<const typename E::value_type>a,std::span<const typename E::value_type>b,
std::span<typename E::value_type>out,int n,Op op={}){
assert(!(n&(n-1)));
auto ta=E::transform(a,n);
auto tb=E::transform(b,n);
E::finish(E::mul(ta,tb,n),out,op);
}
template<engine E,typename Op=assign_op>
void square_circular(std::span<const typename E::value_type>a,std::span<typename E::value_type>out,int n,Op op={}){
assert(!(n&(n-1)));
auto ta=E::transform(a,n);
E::finish(E::sq(ta,n),out,op);
}
namespace detail{
struct conv_size{int n;bool cut;};
inline conv_size conv_size_for(int s){
int n=nextPow2(s);
bool cut=(n==2*(s-1));
return{cut?n/2:n,cut};
}
template<typename T,typename Op>
void emit_linear(std::span<T>buf,int n,int s,bool cut,T c0,std::span<T>out,Op op){
T cn{};
if(cut){
cn=buf[0]-c0;
buf[0]=c0;
}
int lim=min(sz(out),min(s,n));
for(int i=0;i<lim;i++)op(out[i],buf[i]);
if(cut&&sz(out)>=s)op(out[s-1],cn);
}
template<typename T,typename Op>
struct cut_op{
Op op;
T*out0;
T c0;
T&cn;
void operator()(T&x,T v)const{
if(&x==out0){cn=v-c0;v=c0;}
op(x,v);
}
};
template<engine E,typename P,typename Op=assign_op>
void finish_linear(
P&&p,int n,int s,bool cut,
typename E::value_type c0,std::span<typename E::value_type>out,Op op={}
){
using T=typename E::value_type;
if(sz(out)==0)return;
int lim=min(sz(out),min(s,n));
if(!cut){
E::finish(std::move(p),out.subspan(0,lim),op);
}else{
T cn{};
E::finish(std::move(p),out.subspan(0,lim),cut_op<T,Op>{op,&out[0],c0,cn});
if(sz(out)>=s)op(out[s-1],cn);
}
}
}
template<engine E,typename Op=assign_op>
void multiply(std::span<const typename E::value_type>a,std::span<const typename E::value_type>b,
std::span<typename E::value_type>out,Op op={}){
using T=typename E::value_type;
if(sz(a)==0||sz(b)==0)return;
int s=sz(a)+sz(b)-1;
auto[n,cut]=detail::conv_size_for(s);
T c0=a[0]*b[0];
auto buf=buffer_pool<T>::get(n);
multiply_circular<E>(a,b,buf.span(),n);
detail::emit_linear<T>(buf.span(),n,s,cut,c0,out,op);
}
template<engine E,typename Op=assign_op>
void multiply(std::span<const typename E::value_type>a,transformed<E>&ta,
std::span<const typename E::value_type>b,transformed<E>&tb,
std::span<typename E::value_type>out,Op op={}){
using T=typename E::value_type;
if(sz(a)==0||sz(b)==0)return;
int s=sz(a)+sz(b)-1;
auto[n,cut]=detail::conv_size_for(s);
T c0=a[0]*b[0];
E::extend_to(ta,n,a);
E::extend_to(tb,n,b);
detail::finish_linear<E>(E::mul(ta,tb,n),n,s,cut,c0,out,op);
}
template<engine E,typename Op=assign_op>
void multiply_add2(std::span<const typename E::value_type>a1,transformed<E>&ta1,
std::span<const typename E::value_type>b1,transformed<E>&tb1,
std::span<const typename E::value_type>a2,transformed<E>&ta2,
std::span<const typename E::value_type>b2,transformed<E>&tb2,
std::span<typename E::value_type>out,Op op={}){
using T=typename E::value_type;
assert(sz(a1)>0&&sz(b1)>0&&sz(a2)>0&&sz(b2)>0);
int s=sz(a1)+sz(b1)-1;
assert(sz(a2)+sz(b2)-1==s);
auto[n,cut]=detail::conv_size_for(s);
T c0=a1[0]*b1[0]+a2[0]*b2[0];
E::extend_to(ta1,n,a1);E::extend_to(tb1,n,b1);
E::extend_to(ta2,n,a2);E::extend_to(tb2,n,b2);
detail::finish_linear<E>(E::mul2(ta1,tb1,ta2,tb2,n),n,s,cut,c0,out,op);
}
template<engine E>
void multiply_add2_cached(
std::span<const typename E::value_type>a1,transformed<E>&ta1,
std::span<const typename E::value_type>b1,transformed<E>&tb1,
std::span<const typename E::value_type>a2,transformed<E>&ta2,
std::span<const typename E::value_type>b2,transformed<E>&tb2,
std::vector<typename E::value_type>&coeffs,transformed<E>&t){
using T=typename E::value_type;
assert(sz(a1)>0&&sz(b1)>0&&sz(a2)>0&&sz(b2)>0);
int s=sz(a1)+sz(b1)-1;
assert(sz(a2)+sz(b2)-1==s);
coeffs.assign(size_t(s),T{});
t=transformed<E>{};
if constexpr(std::same_as<typename E::product,transformed<E>>){
auto[n,cut]=detail::conv_size_for(s);
T c0=a1[0]*b1[0]+a2[0]*b2[0];
E::extend_to(ta1,n,a1);E::extend_to(tb1,n,b1);
E::extend_to(ta2,n,a2);E::extend_to(tb2,n,b2);
auto p=E::mul2(ta1,tb1,ta2,tb2,n);
auto tp=p;
detail::finish_linear<E>(std::move(p),n,s,cut,c0,std::span<T>(coeffs));
t=std::move(tp);
}else{
multiply_add2<E>(a1,ta1,b1,tb1,a2,ta2,b2,tb2,std::span<T>(coeffs));
}
}
template<engine E>
void multiply_cached(std::span<const typename E::value_type>a,transformed<E>&ta,
std::span<const typename E::value_type>b,transformed<E>&tb,
std::vector<typename E::value_type>&coeffs,transformed<E>&t){
using T=typename E::value_type;
coeffs.assign(size_t(sz(a)&&sz(b)?sz(a)+sz(b)-1:0),T{});
t=transformed<E>{};
if(coeffs.empty())return;
int s=sz(coeffs);
if constexpr(std::same_as<typename E::product,transformed<E>>){
auto[n,cut]=detail::conv_size_for(s);
T c0=a[0]*b[0];
E::extend_to(ta,n,a);
E::extend_to(tb,n,b);
auto p=E::mul(ta,tb,n);
auto tp=p;
detail::finish_linear<E>(std::move(p),n,s,cut,c0,std::span<T>(coeffs));
t=std::move(tp);
}else{
multiply<E>(a,ta,b,tb,std::span<T>(coeffs));
}
}
template<engine E,typename Op=assign_op>
void square(std::span<const typename E::value_type>a,std::span<typename E::value_type>out,Op op={}){
using T=typename E::value_type;
if(sz(a)==0)return;
int s=2*sz(a)-1;
auto[n,cut]=detail::conv_size_for(s);
T c0=a[0]*a[0];
auto buf=buffer_pool<T>::get(n);
square_circular<E>(a,buf.span(),n);
detail::emit_linear<T>(buf.span(),n,s,cut,c0,out,op);
}
template<engine E,typename Op=assign_op>
void square(std::span<const typename E::value_type>a,transformed<E>&ta,
std::span<typename E::value_type>out,Op op={}){
using T=typename E::value_type;
if(sz(a)==0)return;
int s=2*sz(a)-1;
auto[n,cut]=detail::conv_size_for(s);
T c0=a[0]*a[0];
E::extend_to(ta,n,a);
detail::finish_linear<E>(E::sq(ta,n),n,s,cut,c0,out,op);
}
template<engine E>
void square_cached(std::span<const typename E::value_type>a,transformed<E>&ta,
std::vector<typename E::value_type>&coeffs,transformed<E>&t){
using T=typename E::value_type;
coeffs.assign(size_t(sz(a)?2*sz(a)-1:0),T{});
t=transformed<E>{};
if(coeffs.empty())return;
int s=sz(coeffs);
if constexpr(std::same_as<typename E::product,transformed<E>>){
auto[n,cut]=detail::conv_size_for(s);
T c0=a[0]*a[0];
E::extend_to(ta,n,a);
auto p=E::sq(ta,n);
auto tp=p;
detail::finish_linear<E>(std::move(p),n,s,cut,c0,std::span<T>(coeffs));
t=std::move(tp);
}else{
square<E>(a,ta,std::span<T>(coeffs));
}
}
template<engine E>vector<typename E::value_type>multiply(
const vector<typename E::value_type>&a,const vector<typename E::value_type>&b){
using T=typename E::value_type;
if(sz(a)==0||sz(b)==0)return{};
vector<T>r(sz(a)+sz(b)-1);
multiply<E>(std::span<const T>(a),std::span<const T>(b),std::span<T>(r));
return r;
}
template<engine E>vector<typename E::value_type>square(const vector<typename E::value_type>&a){
using T=typename E::value_type;
if(sz(a)==0)return{};
vector<T>r(2*sz(a)-1);
square<E>(std::span<const T>(a),std::span<T>(r));
return r;
}
namespace detail{
template<typename T,typename Op>
void emit_middle(std::span<T>buf,bool cut,int la,int lb,T c0,T ctop,std::span<T>out,Op op){
int m=la-lb+1;
T cn{};
if(cut){
cn=buf[0]-c0;
buf[lb-1]-=ctop;
}
int lim=min(sz(out),cut?m-1:m);
for(int t=0;t<lim;t++)op(out[t],buf[lb-1+t]);
if(cut&&sz(out)>=m)op(out[m-1],cn);
}
}
template<engine E,typename Op=assign_op>
void middle_product(std::span<const typename E::value_type>a,std::span<const typename E::value_type>b,
std::span<typename E::value_type>out,Op op={}){
using T=typename E::value_type;
if(sz(a)==0||sz(b)==0)return;
assert(sz(a)>=sz(b));
if(sz(a)==sz(b)){
T r{};
for(int i=0;i<sz(a);i++){
r+=a[i]*b[sz(b)-1-i];
}
if(sz(out)>0)op(out[0],r);
return;
}
auto[n,cut]=detail::conv_size_for(sz(a));
auto buf=buffer_pool<T>::get(n);
multiply_circular<E>(a,b,buf.span(),n);
detail::emit_middle<T>(buf.span(),cut,sz(a),sz(b),
a[0]*b[0],a[sz(a)-1]*b[sz(b)-1],out,op);
}
template<engine E>vector<typename E::value_type>middle_product(
std::span<const typename E::value_type>a,std::span<const typename E::value_type>b){
using T=typename E::value_type;
if(sz(a)==0||sz(b)==0)return{};
assert(sz(a)>=sz(b));
vector<T>r(size_t(sz(a)-sz(b)+1));
middle_product<E>(a,b,std::span<T>(r));
return r;
}
template<engine E,typename Op=assign_op>
void middle_product(std::span<const typename E::value_type>a,transformed<E>&ta,
std::span<const typename E::value_type>b,transformed<E>&tb,
std::span<typename E::value_type>out,Op op={}){
using T=typename E::value_type;
if(sz(a)==0||sz(b)==0)return;
assert(sz(a)>=sz(b));
if(sz(a)==sz(b)){
T r{};
for(int i=0;i<sz(a);i++){
r+=a[i]*b[sz(b)-1-i];
}
if(sz(out)>0)op(out[0],r);
return;
}
auto[n,cut]=detail::conv_size_for(sz(a));
E::extend_to(ta,n,a);
E::extend_to(tb,n,b);
auto buf=buffer_pool<T>::get(n);
E::finish(E::mul(ta,tb,n),buf.span());
detail::emit_middle<T>(buf.span(),cut,sz(a),sz(b),
a[0]*b[0],a[sz(a)-1]*b[sz(b)-1],out,op);
}
template<engine E>
vector<typename E::value_type>middle_product(std::span<const typename E::value_type>a,transformed<E>&ta,
std::span<const typename E::value_type>b,transformed<E>&tb){
using T=typename E::value_type;
if(sz(a)==0||sz(b)==0)return{};
assert(sz(a)>=sz(b));
vector<T>r(size_t(sz(a)-sz(b)+1));
middle_product<E>(a,ta,b,tb,std::span<T>(r));
return r;
}
}
// src/fft/series_core.hpp
namespace ecnerwala::series{
template<fft::engine E,bool exact_>
struct span{
using T=typename E::value_type;
using engine_t=E;
static constexpr bool exact_v=exact_;
span()=default;
explicit span(std::span<const T>s_):s(s_){}
template<bool oe>requires(oe!=exact_)
explicit(oe<exact_)span(span<E,oe>o):s(o.coeffs()){}
int len()const{return sz(s);}
const T&operator[](int i)const{return s[size_t(i)];}
auto begin()const{return s.begin();}
auto end()const{return s.end();}
std::span<const T>coeffs()const{return s;}
span first(int n)const{
assert(n<=len());
return span(s.first(size_t(n)));
}
private:
std::span<const T>s;
};
template<fft::engine E,bool exact_>
struct vec:public std::vector<typename E::value_type>{
using T=typename E::value_type;
using engine_t=E;
static constexpr bool exact_v=exact_;
using std::vector<T>::vector;
operator span<E,exact_>()const{
return span<E,exact_>(std::span<const T>(*this));
}
template<bool oe>requires(oe!=exact_)
explicit(oe<exact_)vec(const vec<E,oe>&p):std::vector<T>(p){}
template<bool oe>requires(oe!=exact_)
explicit(oe<exact_)vec(vec<E,oe>&&p):std::vector<T>(std::move(p)){}
explicit vec(std::vector<T>v):std::vector<T>(std::move(v)){}
explicit vec(span<E,exact_>s):std::vector<T>(s.begin(),s.end()){}
explicit vec(span<E,!exact_>s):std::vector<T>(s.begin(),s.end()){}
span<E,exact_>first(int n)const{return span<E,exact_>(*this).first(n);}
int len()const{
return int(this->size());
}
int degree()const requires(exact_){
return len()-1;
}
void extend(int sz){
assert(sz>=len());
this->resize(sz);
}
void shrink(int sz){
assert(sz<=len());
this->resize(sz);
}
void shift_trunc(int n=1)requires(!exact_){
assert(n>=0&&n<=len());
std::rotate(this->begin(),this->end()-n,this->end());
std::fill(this->begin(),this->begin()+n,T(0));
}
void unshift_trunc(int n=1)requires(!exact_){
assert(n>=0&&n<=len());
std::fill(this->begin(),this->begin()+n,T(0));
std::rotate(this->begin(),this->begin()+n,this->end());
}
template<bool oe>requires(exact_<=oe)
vec&operator+=(const vec<E,oe>&o){
if constexpr(exact_){if(o.len()>len())this->resize(o.len());}
else if constexpr(!oe){if(o.len()<len())this->resize(o.len());}
for(int i=0;i<std::min(len(),o.len());i++){
(*this)[i]+=o[i];
}
return*this;
}
template<bool oe>requires(exact_<=oe)
vec&operator-=(const vec<E,oe>&o){
if constexpr(exact_){if(o.len()>len())this->resize(o.len());}
else if constexpr(!oe){if(o.len()<len())this->resize(o.len());}
for(int i=0;i<std::min(len(),o.len());i++){
(*this)[i]-=o[i];
}
return*this;
}
vec&operator*=(const T&n){
for(auto&v:*this)v*=n;
return*this;
}
friend vec operator*(const vec&a,const T&n){
vec r(a.size());
for(int i=0;i<a.len();i++){
r[i]=a[i]*n;
}
return r;
}
friend vec operator*(const T&n,const vec&a){
vec r(a.size());
for(int i=0;i<a.len();i++){
r[i]=n*a[i];
}
return r;
}
vec&operator*=(const vec&o){
return*this=(*this)*o;
}
};
template<fft::engine E>using exact=vec<E,true>;
template<fft::engine E>using trunc=vec<E,false>;
template<typename S>
concept like=fft::engine<typename S::engine_t>&&requires(const S&s,int i){
{S::exact_v}->std::convertible_to<bool>;
{s.len()}->std::same_as<int>;
{s[i]}->std::convertible_to<const typename S::engine_t::value_type&>;
{std::span<const typename S::engine_t::value_type>(s)};
requires std::convertible_to<const S&,span<typename S::engine_t,S::exact_v>>;
{s.first(i)}->std::convertible_to<span<typename S::engine_t,S::exact_v>>;
};
template<typename S>
concept exact_like=like<S>&&S::exact_v;
template<typename S>
concept trunc_like=like<S>&&!S::exact_v;
template<typename S>
concept has_cache=like<S>&&requires(const S&s){
{s.cache()}->std::same_as<fft::transformed<typename S::engine_t>&>;
};
template<fft::engine E,bool exact_>
struct maybe_cached;
template<fft::engine E,bool exact_>
struct cached_span{
using engine_t=E;
static constexpr bool exact_v=exact_;
span<E,exact_>s;
std::reference_wrapper<fft::transformed<E>>f;
cached_span(span<E,exact_>s_,fft::transformed<E>&f_):s(s_),f(f_){}
template<bool oe>requires(oe!=exact_)
explicit(oe<exact_)cached_span(cached_span<E,oe>o):s(span<E,exact_>(o.s)),f(o.f){}
int len()const{return s.len();}
const typename E::value_type&operator[](int i)const{return s[i];}
operator std::span<const typename E::value_type>()const{return s.coeffs();}
operator span<E,exact_>()const{return s;}
maybe_cached<E,exact_>first(int n)const;
fft::transformed<E>&cache()const{return f;}
};
template<typename S>
concept has_cache_opt=like<S>&&requires(const S&s){
{s.cache_opt()}->std::same_as<std::optional<std::reference_wrapper<fft::transformed<typename S::engine_t>>>>;
};
namespace detail{
template<like S>
std::optional<std::reference_wrapper<fft::transformed<typename S::engine_t>>>cache_of(const S&s){
if constexpr(has_cache<S>)return s.cache();
else if constexpr(has_cache_opt<S>)return s.cache_opt();
else return std::nullopt;
}
}
template<fft::engine E,bool exact_>
struct maybe_cached{
using T=typename E::value_type;
using engine_t=E;
static constexpr bool exact_v=exact_;
span<E,exact_>s;
std::optional<std::reference_wrapper<fft::transformed<E>>>f;
explicit maybe_cached(span<E,exact_>s_):s(s_){}
maybe_cached(span<E,exact_>s_,fft::transformed<E>&f_):s(s_),f(f_){}
maybe_cached(cached_span<E,exact_>c):s(c.s),f(c.f){}
template<like S>requires std::same_as<typename S::engine_t,E>&&(S::exact_v==exact_)
maybe_cached(const S&o):s(o),f(detail::cache_of(o)){}
int len()const{return s.len();}
const T&operator[](int i)const{return s[i];}
operator std::span<const T>()const{return s.coeffs();}
operator span<E,exact_>()const{return s;}
maybe_cached first(int n)const{
return n==len()?*this:maybe_cached(s.first(n));
}
std::optional<std::reference_wrapper<fft::transformed<E>>>cache_opt()const{return f;}
};
template<fft::engine E,bool exact_>
maybe_cached<E,exact_>cached_span<E,exact_>::first(int n)const{
return maybe_cached<E,exact_>(*this).first(n);
}
template<fft::engine E>
struct resized{
using T=typename E::value_type;
using engine_t=E;
static constexpr bool exact_v=false;
trunc<E>s;
std::optional<std::reference_wrapper<fft::transformed<E>>>f;
int len()const{return s.len();}
const T&operator[](int i)const{return s[size_t(i)];}
operator std::span<const T>()const{return std::span<const T>(s);}
operator span<E,false>()const{return s;}
maybe_cached<E,false>first(int n)const{
span<E,false>v=s;
if(n==len()&&f)return{v,f->get()};
return maybe_cached<E,false>(v.first(n));
}
std::optional<std::reference_wrapper<fft::transformed<E>>>cache_opt()const{return f;}
};
template<like S>
resized<typename S::engine_t>with_len(const S&s,int n){
using E=typename S::engine_t;
using T=typename E::value_type;
auto p=s.first(std::min(n,s.len()));
resized<E>r;
r.s.assign(size_t(n),T{});
std::span<const T>pc(p);
std::copy(pc.begin(),pc.end(),r.s.begin());
r.f=detail::cache_of(p);
return r;
}
template<typename S>
concept has_prefix_cache=like<S>&&!S::exact_v&&requires(const S&s,int n){
{s.prefix_cache(n)}->std::same_as<fft::transformed<typename S::engine_t>&>;
};
template<fft::engine E,bool exact_>
struct cached{
using T=typename E::value_type;
using engine_t=E;
static constexpr bool exact_v=exact_;
cached()=default;
cached(vec<E,exact_>&&s_):s(std::move(s_)){}
explicit cached(const vec<E,exact_>&s_):s(s_){}
operator vec<E,exact_>()&&{return std::move(s);}
int len()const{return s.len();}
const vec<E,exact_>&uncached()const{return s;}
const T&operator[](int i)const{return s[size_t(i)];}
auto begin()const{return s.cbegin();}
auto end()const{return s.cend();}
operator span<E,exact_>()const{return s;}
maybe_cached<E,exact_>first(int n)const{
return n==len()?maybe_cached<E,exact_>(s,f):maybe_cached<E,exact_>(s.first(n));
}
fft::transformed<E>&cache()const{return f;}
template<like S>
friend bool operator==(const cached&a,const S&b){
span<E,S::exact_v>bs=b;
return a.len()==bs.len()&&std::equal(a.s.begin(),a.s.end(),bs.begin());
}
private:
vec<E,exact_>s;
mutable fft::transformed<E>f;
};
template<fft::engine E>using cached_exact=cached<E,true>;
template<fft::engine E>using cached_trunc=cached<E,false>;
namespace detail{
template<like S>
cached_span<typename S::engine_t,S::exact_v>as_cached_span(const S&s,fft::transformed<typename S::engine_t>&tmp){
auto co=cache_of(s);
return{s,co?co->get():tmp};
}
}
template<trunc_like S>
trunc<typename S::engine_t>ps_inv(const S&a){
using E=typename S::engine_t;
using T=typename E::value_type;
int N=a.len();
trunc<E>r(size_t(N),T{});
if(N==0)return r;
int s=nextPow2(N);
std::vector<T>b(size_t(s),T{});
b[0]=inv(a[0]);
for(int n=1;n<N;n*=2){
int m=2*n;
auto ta=E::transform(a.first(std::min(N,m)),m);
auto tb=E::transform(std::span<const T>(b).first(n),m);
auto e=fft::buffer_pool<T>::get(m);
E::finish(E::mul(ta,tb,m),e.span());
for(int i=0;i<n;i++)e[i]=T{};
auto te=E::transform(std::span<const T>(e.span()),m);
auto c=fft::buffer_pool<T>::get(m);
E::finish(E::mul(tb,te,m),c.span());
for(int i=n;i<std::min(m,N);i++)b[i]=-c[i];
}
std::copy(b.begin(),b.begin()+N,r.begin());
return r;
}
template<like A>
auto square(const A&a){
using E=typename A::engine_t;
using T=typename E::value_type;
fft::transformed<E>ta_;
auto av=detail::as_cached_span(a,ta_);
if constexpr(A::exact_v){
std::vector<T>coeffs;
fft::transformed<E>f;
fft::square_cached<E>(av,av.cache(),coeffs,f);
cached<E,true>w(exact<E>(std::move(coeffs)));
w.cache()=std::move(f);
return w;
}else{
trunc<E>r(size_t(a.len()),T{});
fft::square<E>(av,av.cache(),std::span<T>(r));
return r;
}
}
template<like A,like B,like C,like D>
requires fft::same_engine<A,B>&&fft::same_engine<A,C>&&fft::same_engine<A,D>
&&A::exact_v&&B::exact_v&&C::exact_v&&D::exact_v
cached<typename A::engine_t,true>multiply_add2(
const A&a,const B&b,const C&c,const D&d){
using E=typename A::engine_t;
using T=typename E::value_type;
fft::transformed<E>ta_,tb_,tc_,td_;
auto av=detail::as_cached_span(a,ta_),bv=detail::as_cached_span(b,tb_);
auto cv=detail::as_cached_span(c,tc_),dv=detail::as_cached_span(d,td_);
std::vector<T>coeffs;
fft::transformed<E>f;
fft::multiply_add2_cached<E>(
av,av.cache(),
bv,bv.cache(),
cv,cv.cache(),
dv,dv.cache(),
coeffs,f
);
cached<E,true>w(exact<E>(std::move(coeffs)));
w.cache()=std::move(f);
return w;
}
template<like A,exact_like B>requires fft::same_engine<A,B>
vec<typename A::engine_t,A::exact_v>middle_product(const A&a,const B&b){
using E=typename A::engine_t;
fft::transformed<E>ta_,tb_;
auto av=detail::as_cached_span(a,ta_);
auto bv=detail::as_cached_span(b,tb_);
return vec<E,A::exact_v>(fft::middle_product<E>(
av,av.cache(),
bv,bv.cache()
));
}
namespace detail{
template<bool ea,bool eb>int product_prec(int la,int lb){
if constexpr(ea&&eb)return la>0&&lb>0?la+lb-1:0;
else return ea?lb:eb?la:std::min(la,lb);
}
template<like S>
auto product_operand(const S&s,int prec,fft::transformed<typename S::engine_t>&tmp){
using E=typename S::engine_t;
if constexpr(has_prefix_cache<S>){
return s.first(std::min(s.len(),nextPow2(prec)));
}else{
span<E,S::exact_v>v=s;
int used=std::min(v.len(),prec);
if(auto co=cache_of(s)){
if(s.len()<=prec||fft::detail::conv_size_for(s.len()+prec-1).n
==fft::detail::conv_size_for(2*prec-1).n){
return cached_span<E,S::exact_v>{v,co->get()};
}
}
return cached_span<E,S::exact_v>{v.first(used),tmp};
}
}
}
template<like A,like B>requires fft::same_engine<A,B>
vec<typename A::engine_t,A::exact_v&&B::exact_v>operator+(const A&a,const B&b){
using T=typename A::engine_t::value_type;
int n=(A::exact_v&&B::exact_v)?std::max(a.len(),b.len())
:A::exact_v?b.len():B::exact_v?a.len():std::min(a.len(),b.len());
vec<typename A::engine_t,A::exact_v&&B::exact_v>r(size_t(n),T(0));
for(int i=0;i<n;i++){
r[i]=(i<a.len()?a[i]:T(0))+(i<b.len()?b[i]:T(0));
}
return r;
}
template<like A,like B>requires fft::same_engine<A,B>
vec<typename A::engine_t,A::exact_v&&B::exact_v>operator-(const A&a,const B&b){
using T=typename A::engine_t::value_type;
int n=(A::exact_v&&B::exact_v)?std::max(a.len(),b.len())
:A::exact_v?b.len():B::exact_v?a.len():std::min(a.len(),b.len());
vec<typename A::engine_t,A::exact_v&&B::exact_v>r(size_t(n),T(0));
for(int i=0;i<n;i++){
r[i]=(i<a.len()?a[i]:T(0))-(i<b.len()?b[i]:T(0));
}
return r;
}
template<like A,like B>requires fft::same_engine<A,B>
auto operator*(const A&a,const B&b){
using E=typename A::engine_t;
using T=typename E::value_type;
constexpr bool ea=A::exact_v,eb=B::exact_v;
int prec=detail::product_prec<ea,eb>(a.len(),b.len());
if(prec==0||a.len()==0||b.len()==0){
if constexpr(ea&&eb)return cached<E,true>{};
else return trunc<E>(size_t(prec),T(0));
}
fft::transformed<E>ta_,tb_;
auto va=detail::product_operand(a,prec,ta_);
auto vb=detail::product_operand(b,prec,tb_);
if constexpr(ea&&eb){
std::vector<T>coeffs;
fft::transformed<E>f;
auto ca=detail::as_cached_span(va,ta_),cb=detail::as_cached_span(vb,tb_);
fft::multiply_cached<E>(
ca,ca.cache(),
cb,cb.cache(),
coeffs,f
);
cached<E,true>w(exact<E>(std::move(coeffs)));
w.cache()=std::move(f);
return w;
}else{
trunc<E>r(size_t(prec),T(0));
auto ca=detail::as_cached_span(va,ta_),cb=detail::as_cached_span(vb,tb_);
fft::multiply<E>(
ca,ca.cache(),
cb,cb.cache(),
std::span<T>(r)
);
return r;
}
}
template<fft::engine E>
struct prefix_cached{
using T=typename E::value_type;
using engine_t=E;
static constexpr bool exact_v=false;
prefix_cached()=default;
prefix_cached(trunc<E>&&s_):s(std::move(s_)){}
explicit prefix_cached(const trunc<E>&s_):s(s_){}
operator trunc<E>()&&{return std::move(s);}
int len()const{return s.len();}
const trunc<E>&uncached()const{return s;}
const T&operator[](int i)const{return s[size_t(i)];}
auto begin()const{return s.cbegin();}
auto end()const{return s.cend();}
operator span<E,false>()const{return s;}
void append(std::span<const T>tail){
s.insert(s.end(),tail.begin(),tail.end());
}
maybe_cached<E,false>first(int k)const{
assert(k<=len());
span<E,false>v=s.first(k);
int n=nextPow2(k);
if(std::min(n,len())==k)return{v,prefix_cache(n)};
return maybe_cached<E,false>(v);
}
fft::transformed<E>&cache()const{return prefix_cache(nextPow2(len()));}
fft::transformed<E>&prefix_cache(int n)const{
assert(n>0&&!(n&(n-1)));
int k=__builtin_ctz(unsigned(n));
if(k>=sz(caches))caches.resize(size_t(k)+1);
auto&c=caches[k];
int e=std::min(n,len());
if(c.len!=e){
c.t=E::transform(s.first(e),2*n);
c.len=e;
}
return c.t;
}
private:
trunc<E>s;
struct entry{fft::transformed<E>t;int len=0;};
mutable std::vector<entry>caches;
};
}
// src/fft/poly.hpp
namespace ecnerwala::poly{
template<fft::engine E>struct vec{
using T=typename E::value_type;
using engine_t=E;
series::exact<E>c;
vec()=default;
explicit vec(int len):c(size_t(len),T{}){}
vec(std::initializer_list<T>coeffs):c(std::rbegin(coeffs),std::rend(coeffs)){}
explicit vec(std::span<const T>coeffs):c(coeffs.rbegin(),coeffs.rend()){}
const series::exact<E>&rev_series()const{return c;}
static vec from_rev_series(series::exact<E>s){
vec r;
r.c=std::move(s);
return r;
}
series::trunc<E>unrev_series(int n)const{
series::trunc<E>r(size_t(n),T{});
std::copy(begin(),begin()+std::min(n,len()),r.begin());
return r;
}
auto begin(){return c.rbegin();}
auto end(){return c.rend();}
auto begin()const{return c.rbegin();}
auto end()const{return c.rend();}
int len()const{return c.len();}
int degree()const{return len()-1;}
T&operator[](int i){return c[len()-1-i];}
const T&operator[](int i)const{return c[len()-1-i];}
T leading()const{return c.front();}
void shift(int k=1){
if(len()>0)c.insert(c.end(),size_t(k),T(0));
}
void resize(int n){
if(n>=len())c.insert(c.begin(),size_t(n-len()),T(0));
else c.erase(c.begin(),c.begin()+(len()-n));
}
T operator()(const T&x)const{
T r{};
for(const T&v:c)r=r*x+v;
return r;
}
vec&operator+=(const vec&o){
if(o.len()>len())resize(o.len());
for(int i=0;i<o.len();i++)(*this)[i]+=o[i];
return*this;
}
friend vec operator+(vec a,const vec&b){a+=b;return a;}
vec&operator-=(const vec&o){
if(o.len()>len())resize(o.len());
for(int i=0;i<o.len();i++)(*this)[i]-=o[i];
return*this;
}
friend vec operator-(vec a,const vec&b){a-=b;return a;}
friend bool operator==(const vec&a,const vec&b){return a.c==b.c;}
vec&operator*=(const T&n){for(T&v:c)v*=n;return*this;}
friend vec operator*(vec a,const T&n){a*=n;return a;}
friend vec operator*(const T&n,vec a){a*=n;return a;}
vec&operator*=(const vec&o){return*this=(*this)*o;}
};
template<typename P>
concept like=requires(const P&p){
typename P::engine_t;
{p.len()}->std::same_as<int>;
p.rev_series();
requires series::like<std::remove_cvref_t<decltype(p.rev_series())>>;
requires std::remove_cvref_t<decltype(p.rev_series())>::exact_v;
};
template<fft::engine E>
struct cached{
using T=typename E::value_type;
using engine_t=E;
cached()=default;
cached(vec<E>&&p):c(std::move(p.c)){}
explicit cached(const vec<E>&p):c(p.c){}
operator vec<E>()&&{return vec<E>::from_rev_series(std::move(c));}
const series::cached_exact<E>&rev_series()const{return c;}
static cached from_rev_series(series::cached_exact<E>s){
cached r;
r.c=std::move(s);
return r;
}
int len()const{return c.len();}
int degree()const{return len()-1;}
const T&operator[](int i)const{return c[len()-1-i];}
T leading()const{return c[0];}
T operator()(const T&x)const{
T r{};
for(const T&v:c)r=r*x+v;
return r;
}
private:
series::cached_exact<E>c;
};
template<like A,like B>requires fft::same_engine<A,B>
cached<typename A::engine_t>operator*(const A&a,const B&b){
return cached<typename A::engine_t>::from_rev_series(a.rev_series()*b.rev_series());
}
template<like A>
cached<typename A::engine_t>square(const A&a){
return cached<typename A::engine_t>::from_rev_series(square(a.rev_series()));
}
template<like A,like B,like C,like D>
requires fft::same_engine<A,B>&&fft::same_engine<A,C>&&fft::same_engine<A,D>
cached<typename A::engine_t>multiply_add2(
const A&a,const B&b,const C&c,const D&d){
return cached<typename A::engine_t>::from_rev_series(
multiply_add2(a.rev_series(),b.rev_series(),c.rev_series(),d.rev_series()));
}
template<like A,like B>requires fft::same_engine<A,B>
bool operator==(const A&a,const B&b){
return a.rev_series()==b.rev_series();
}
template<fft::engine E>
struct form{
using T=typename E::value_type;
series::cached_exact<E>c;
form()=default;
explicit form(int len):c(series::exact<E>(size_t(len),T{})){}
const series::cached_exact<E>&rev_series()const{return c;}
static form from_rev_series(series::cached_exact<E>s){
form r;
r.c=std::move(s);
return r;
}
static form from_poly(const vec<E>&p){return from_rev_series(series::cached_exact<E>(p.rev_series()));}
int len()const{return c.len();}
form for_length(int n)const{
auto r=series::exact<E>(c);
if(n>=len())r.insert(r.begin(),size_t(n-len()),T(0));
else r.erase(r.begin(),r.begin()+(len()-n));
return from_rev_series(std::move(r));
}
static form polynomial_evaluation(T z,int len){
series::exact<E>k(size_t(len),T{});
T p=T(1);
for(int i=0;i<len;i++){k[i]=p;p*=z;}
return from_rev_series(std::move(k));
}
template<like P>
T operator()(const P&p)const{
assert(p.len()<=len());
T r{};
for(int i=0;i<p.len();i++)r+=c[i]*p[i];
return r;
}
template<like P>
form composed_with(const P&q)const{
assert(q.len()>0&&q.len()<=len());
return from_rev_series(middle_product(c,q.rev_series()));
}
template<series::like S>requires std::same_as<typename S::engine_t,E>
form composed_with(const S&s)const{
if constexpr(!S::exact_v)assert(s.len()>=len());
series::vec<E,S::exact_v>r=c*s;
r.resize(size_t(len()));
return from_rev_series(series::exact<E>(std::move(r)));
}
};
template<fft::engine E>
struct subproduct_tree{
using T=typename E::value_type;
int N;
std::vector<cached<E>>nodes;
explicit subproduct_tree(std::span<const T>pts):N(sz(pts)),nodes(size_t(2)*N){
assert(N>0);
for(int i=0;i<N;i++){
nodes[N+i]=vec<E>{-pts[i],T(1)};
}
for(int i=N-1;i>0;i--){
nodes[i]=nodes[2*i]*nodes[2*i+1];
}
}
int size(int i)const{return nodes[i].len()-1;}
const cached<E>&prod(int i)const{return nodes[i];}
std::vector<T>pushdown(form<E>f)const{
assert(f.len()==N);
std::vector<form<E>>down(size_t(2)*N);
down[1]=std::move(f);
for(int i=1;i<N;i++){
down[2*i+0]=down[i].composed_with(nodes[2*i+1]);
down[2*i+1]=down[i].composed_with(nodes[2*i+0]);
down[i]=form<E>{};
}
std::vector<T>out(size_t(N),T{});
for(int i=0;i<N;i++)out[i]=down[N+i].rev_series()[0];
return out;
}
cached<E>combine_up(std::span<const T>leaf_vals)const{
assert(sz(leaf_vals)==N);
std::vector<cached<E>>up(size_t(2)*N);
for(int i=0;i<N;i++){
up[N+i]=vec<E>{leaf_vals[i]};
}
for(int i=N-1;i>0;i--){
up[i]=multiply_add2(up[2*i+0],nodes[2*i+1],up[2*i+1],nodes[2*i+0]);
up[2*i+0]=cached<E>{};
up[2*i+1]=cached<E>{};
}
return std::move(up[1]);
}
};
template<fft::engine E>
std::vector<typename E::value_type>multipoint(
const vec<E>&p,
std::span<const typename E::value_type>pts
){
if(pts.empty())return{};
int N=sz(pts);
subproduct_tree<E>tree{pts};
auto q=series::trunc<E>(tree.prod(1).rev_series());
q.resize(p.len());
form<E>f=form<E>::from_poly(p).composed_with(ps_inv(q));
return tree.pushdown(f.for_length(N));
}
template<fft::engine E>
vec<E>interpolate(
std::span<const typename E::value_type>pts,
std::span<const typename E::value_type>vals
){
using T=typename E::value_type;
assert(sz(pts)==sz(vals));
if(pts.empty())return{};
int N=sz(pts);
using ps=series::trunc<E>;
subproduct_tree<E>tree{pts};
auto root=ps(tree.prod(1).rev_series());
root.shrink(N);
ps deriv_root=root;
for(int i=0;i<N;i++){
deriv_root[i]*=T(N-i);
}
std::vector<T>denoms=tree.pushdown(
form<E>::from_rev_series(series::exact<E>(ps_inv(root)*deriv_root))
);
std::vector<T>leaf_vals(size_t(N),T{});
for(int i=0;i<N;i++)leaf_vals[i]=vals[i]/denoms[i];
return tree.combine_up(std::span<const T>(leaf_vals));
}
}
// src/fft/series.hpp
namespace ecnerwala::series{
template<like S>
vec<typename S::engine_t,S::exact_v>stretch(const S&a_,int n){
using E=typename S::engine_t;
span<E,S::exact_v>a=a_;
vec<E,S::exact_v>r(size_t(a.len()));
for(int i=0;i*n<a.len();i++){
r[i*n]=a[i];
}
return r;
}
template<like S>
vec<typename S::engine_t,S::exact_v>deriv_shift(const S&a_){
using E=typename S::engine_t;
span<E,S::exact_v>a=a_;
vec<E,S::exact_v>r(a.begin(),a.end());
for(int i=0;i<r.len();i++){
r[i]*=i;
}
return r;
}
template<like S>
vec<typename S::engine_t,S::exact_v>integ_shift(const S&a_){
using E=typename S::engine_t;
using T=typename E::value_type;
span<E,S::exact_v>a=a_;
assert(a[0]==0);
vec<E,S::exact_v>r(a.begin(),a.end());
T f=1;
for(int i=1;i<r.len();i++){
r[i]*=f;
f*=i;
}
f=inv(f);
for(int i=r.len()-1;i>0;i--){
r[i]*=f;
f*=i;
}
return r;
}
template<like S>
vec<typename S::engine_t,S::exact_v>integ_shift_offset(const S&a_,int offset){
using E=typename S::engine_t;
using T=typename E::value_type;
span<E,S::exact_v>a=a_;
vec<E,S::exact_v>r(a.begin(),a.end());
T f=1;
for(int i=0;i<r.len();i++){
r[i]*=f;
f*=i+offset;
}
assert(f!=0);
f=inv(f);
for(int i=r.len()-1;i>=0;i--){
r[i]*=f;
f*=i+offset;
}
return r;
}
template<trunc_like S>
trunc<typename S::engine_t>deriv_shift_log(const S&a){
return deriv_shift(a)*ps_inv(a);
}
template<trunc_like S>
trunc<typename S::engine_t>ps_log(const S&a){
assert(a[0]==1);
return integ_shift(deriv_shift_log(a));
}
template<trunc_like S>
trunc<typename S::engine_t>ps_exp(const S&a_){
using E=typename S::engine_t;
using T=typename E::value_type;
span<E,false>a=a_;
assert(a.len()>=1);
assert(a[0]==0);
trunc<E>r(1,T(1));r.reserve(size_t(a.len()));
trunc<E>invR(1,T(1));invR.reserve(size_t(a.len()));
while(r.len()<a.len()){
int o_sz=r.len();
int n_sz=std::min(o_sz*2,a.len());
trunc<E>t=deriv_shift(trunc<E>(a.begin(),a.begin()+o_sz));
fft::multiply_circular<E>(std::span<const T>(t),std::span<const T>(r).first(o_sz),std::span<T>(t),o_sz);
t=deriv_shift(r)-t;
t*=invR;
t.resize(size_t(n_sz-o_sz));
trunc<E>v(a.begin()+o_sz,a.begin()+n_sz);
v-=integ_shift_offset(t,o_sz);
v*=r;
r.resize(size_t(n_sz));
std::copy(v.begin(),v.end(),r.begin()+o_sz);
if(r.len()<a.len()){
assert(r.len()==2*invR.len());
int n=invR.len();
int nn=r.len();
trunc<E>tmp(size_t(4)*n);
fft::square<E>(std::span<const T>(invR).first(n),std::span<T>(tmp));
fft::multiply<E>(std::span<const T>(tmp).first(nn),std::span<const T>(r).first(nn),std::span<T>(tmp));
invR.resize(size_t(nn));
for(int i=n;i<nn;i++)invR[i]=-tmp[i];
}
}
return r;
}
template<trunc_like S>
trunc<typename S::engine_t>ps_pow_monic(const S&a_,typename S::engine_t::value_type k){
using E=typename S::engine_t;
span<E,false>a=a_;
if(a.len()==0)return{};
assert(a[0]==1);
trunc<E>l=ps_log(a_);
l*=k;
return ps_exp(l);
}
template<trunc_like S>
trunc<typename S::engine_t>ps_pow(const S&a_,int64_t k){
using E=typename S::engine_t;
using T=typename E::value_type;
span<E,false>a=a_;
assert(k>=0);
if(k==0){
trunc<E>r(size_t(a.len()),T(0));
if(r.len()>0)r[0]=T(1);
return r;
}
int st=0;
while(st<a.len()&&a[st]==0)st++;
if(st>0&&k>(a.len()-1)/st){
return trunc<E>(size_t(a.len()),T(0));
}
trunc<E>r(a.begin()+st,a.end()-(st*(k-1)));
T leading_coeff=r[0];
r*=inv(leading_coeff);
r=ps_pow_monic(r,T(k));
r*=power(leading_coeff,k);
r.insert(r.begin(),size_t(st*k),T(0));
assert(r.len()==a.len());
return r;
}
template<trunc_like S>
trunc<typename S::engine_t>to_newton_sums(const S&a,int deg){
auto r=deriv_shift_log(a);
r[0]=deg;
for(int i=1;i<r.len();i++)r[i]=-r[i];
return r;
}
template<trunc_like S>
trunc<typename S::engine_t>from_newton_sums(const S&s_,int deg){
using E=typename S::engine_t;
span<E,false>s=s_;
assert(s[0]==deg);
trunc<E>r(s.begin(),s.end());
r[0]=0;
for(int i=1;i<r.len();i++)r[i]=-r[i];
return ps_exp(integ_shift(std::move(r)));
}
template<trunc_like S>
trunc<typename S::engine_t>euler_transform(const S&a){
using E=typename S::engine_t;
trunc<E>r=deriv_shift(a);
std::vector<bool>is_prime(size_t(r.len()),true);
for(int p=2;p<r.len();p++){
if(!is_prime[p])continue;
for(int i=1;i*p<r.len();i++){
r[i*p]+=r[i];
is_prime[i*p]=false;
}
}
return ps_exp(integ_shift(r));
}
template<trunc_like S>
trunc<typename S::engine_t>inverse_euler_transform(const S&a){
using E=typename S::engine_t;
trunc<E>r=deriv_shift(ps_log(a));
std::vector<bool>is_prime(size_t(r.len()),true);
for(int p=2;p<r.len();p++){
if(!is_prime[p])continue;
for(int i=(r.len()-1)/p;i>=1;i--){
r[i*p]-=r[i];
is_prime[i*p]=false;
}
}
return integ_shift(r);
}
template<fft::engine E>struct packed_bivariate{
using T=typename E::value_type;
int L,l;
std::vector<T>c;
packed_bivariate(int L_,std::span<const T>g):L(L_),l(0),c(size_t(4)<<L){
c[0]=T(1);
for(int i=0;i<sz(g);i++)c[(2<<L)+i]=-g[i];
}
fft::transformed<E>advance(){
int B=4<<L;
auto tq=E::transform(std::span<const T>(c),B);
auto tn=E::negate_arg(tq,B);
E::finish(E::mul(tq,tn,B),std::span<T>(c));
l++;
for(int i=1;i<(2<<L);i++)c[i]=c[2*i];
for(int i=0;i<(2<<(L-l));i++){
c[(2<<L)+i]=c[i];
c[i]=T(0);
}
c[2<<L]-=T(1);
c[0]=T(1);
std::fill(c.begin()+(2<<L)+(1<<(L-l)),c.end(),T(0));
for(int i=0;i<(2<<L);i+=2<<(L-l)){
for(int j=0;j<(1<<(L-l));j++){
c[i+(1<<(L-l))+j]=T(0);
}
}
return tn;
}
};
template<trunc_like SF,trunc_like SG>requires fft::same_engine<SF,SG>
trunc<typename SF::engine_t>ps_compose(const SF&f_,const SG&g_){
using E=typename SF::engine_t;
using T=typename E::value_type;
span<E,false>f=f_;
span<E,false>g=g_;
if(g.len()==0)return{};
int m=f.len();
int n=g.len();
int L=__builtin_ctz(unsigned(nextPow2(n)));
int B=4<<L;
packed_bivariate<E>Q(L,g.coeffs());
std::vector<fft::transformed<E>>tneg;
tneg.reserve(L);
for(int l=1;l<=L;l++)tneg.push_back(Q.advance());
trunc<E>P;
{
P=trunc<E>(f.begin(),f.end());
std::reverse(P.begin(),P.end());
trunc<E>QL((1<<L)+1);
for(int i=0;i<=(1<<L);i++){
QL[i]=Q.c[2*i];
}
QL.resize(size_t(m),T(0));
P*=ps_inv(QL);
std::reverse(P.begin(),P.end());
P.resize(size_t(1)<<L,T(0));
std::reverse(P.begin(),P.end());
P.resize(size_t(B),T(0));
for(int i=(1<<L)-1;i>0;i--){
P[2*i]=P[i];
P[i]=T(0);
}
}
for(int l=L-1;l>=0;l--){
for(int i=(2<<L)-1;i>0;i--){
T v=P[i];
P[2*i]=((2*i)&(1<<(L-l)))?T(0):v;
P[i]=T(0);
}
auto tp=E::transform(std::span<const T>(P),B);
E::finish(E::mul(tneg[l],tp,B),std::span<T>(P));
for(int i=0;i<(2<<L);i++){
P[i]=P[(2<<L)+i];
P[(2<<L)+i]=T(0);
}
}
return trunc<E>(P.begin(),P.begin()+n);
}
template<exact_like P,exact_like Q>requires fft::same_engine<P,Q>
P::engine_t::value_type kth_term_of_rational_function(
const P&p,
const Q&q,
uint64_t k
){
using E=P::engine_t;
using T=E::value_type;
assert(q.len()>0&&q[0]!=T(0));
if(p.len()==0)return T(0);
int d=std::max(p.len()+1,q.len());
assert(d>=2);
int n=nextPow2((d-1)+d-1);
fft::transformed<E>tq,tp;
if(auto cq=detail::cache_of(q)){E::extend_to(cq->get(),n,q);tq=cq->get();}
if(auto cp=detail::cache_of(p)){E::extend_to(cp->get(),n,p);tp=cp->get();}
std::vector<T>p_buf(d-1,T(0));
std::ranges::copy(std::span<const T>(p),p_buf.begin());
std::vector<T>q_buf(d,T(0));
std::ranges::copy(std::span<const T>(q),q_buf.begin());
while(k>0){
E::extend_to(tq,n,q_buf);
auto tnq=E::negate_arg(tq,n);
E::extend_to(tp,n,p_buf);
auto ntp=E::downsample(E::mul(tp,tnq,n),n/2,bool(k&1));
assert(ntp.size()==n/2);
if constexpr(std::same_as<typename E::product,typename E::transformed>){
tp=ntp;
}else{
tp={};
}
E::finish(std::move(ntp),std::span(p_buf));
k>>=1;
if(!k){
q_buf[0]*=q_buf[0];
break;
}
auto ntq=E::downsample(E::mul(tq,tnq,n),n/2,false);
assert(ntq.size()==n/2);
if constexpr(std::same_as<typename E::product,typename E::transformed>){
tq=ntq;
}else{
tq={};
}
if(n/2==d-1){
T v0=q_buf[0]*q_buf[0];
E::finish(std::move(ntq),std::span(q_buf).first(d-1));
q_buf[d-1]=std::exchange(q_buf[0],v0)-v0;
}else{
E::finish(std::move(ntq),std::span(q_buf));
}
}
return p_buf[0]*inv(q_buf[0]);
}
template<trunc_like S,exact_like Q>requires fft::same_engine<S,Q>
S::engine_t::value_type kth_term_of_linear_recurrence(
const S&s,
const Q&q,
uint64_t k
){
using E=S::engine_t;
using T=E::value_type;
assert(q.len()>0&&q[0]!=T(0));
assert(s.len()>=q.len()-1);
fft::transformed<E>tq;
auto q_cached=detail::as_cached_span(q,tq);
span<E,false>sv=s;
auto p=exact<E>(sv.first(q.len()-1)*q_cached);
return kth_term_of_rational_function(p,q_cached,k);
}
}
// src/modnum.hpp
template<typename T>T mod_inv_in_range(T a,T m){
T x=a,y=m;
T vx=1,vy=0;
bool swap=false;
while(x){
T k=y/x;
y%=x;
vy+=k*vx;
std::swap(x,y);
std::swap(vx,vy);
swap^=1;
}
assert(y==1);
return swap?vy:m-vy;
}
template<typename T>struct extended_gcd_result{
T gcd;
T coeff_a,coeff_b;
};
template<typename T>extended_gcd_result<T>extended_gcd(T a,T b){
T x=a,y=b;
T ax=1,ay=0;
T bx=0,by=1;
while(x){
T k=y/x;
y%=x;
ay-=k*ax;
by-=k*bx;
std::swap(x,y);
std::swap(ax,ay);
std::swap(bx,by);
}
return{y,ay,by};
}
template<typename T>T mod_inv(T a,T m){
a%=m;
a=a<0?a+m:a;
return mod_inv_in_range(a,m);
}
template<typename Self>
struct num_ops{
Self operator+()const{return static_cast<const Self&>(*this);}
Self operator-()const{return static_cast<const Self&>(*this).neg();}
friend Self operator++(Self&a,int){Self r=a;++a;return r;}
friend Self operator--(Self&a,int){Self r=a;--a;return r;}
friend Self operator+(const Self&a,const Self&b){return Self(a)+=b;}
friend Self operator-(const Self&a,const Self&b){return Self(a)-=b;}
friend Self operator*(const Self&a,const Self&b){return Self(a)*=b;}
friend Self operator/(const Self&a,const Self&b){return Self(a)/=b;}
friend bool operator!=(const Self&a,const Self&b){return!(a==b);}
friend Self neg(const Self&a){return a.neg();}
friend Self inv(const Self&a){return a.inv();}
};
template<typename Self,typename V>
struct mod_ops:num_ops<Self>{
static_assert(std::unsigned_integral<V>);
V v;
struct is_reduced_tag{};
mod_ops():v(0){}
mod_ops(V v_,is_reduced_tag):v(v_){assert(v<Self::MOD);}
template<std::integral I>mod_ops(I x):v(Self::reduce(x)){}
static Self from_reduced(V v){return Self(v,is_reduced_tag{});}
static V reduce(std::signed_integral auto x){
using U=std::make_unsigned_t<decltype(x)>;
return x<0?V(Self::MOD-1-Self::reduce(U(~x))):Self::reduce(U(x));
}
explicit operator V()const{return v;}
std::make_signed_t<V>balanced()const{
return std::make_signed_t<V>(Self::MOD-v>v?v:v-Self::MOD);
}
friend bool operator==(const Self&a,const Self&b){return a.v==b.v;}
friend std::ostream&operator<<(std::ostream&out,const Self&n){return out<<n.v;}
friend std::istream&operator>>(std::istream&in,Self&n){int64_t v_;in>>v_;n=Self(v_);return in;}
Self&operator++(){
++v;
if(v==Self::MOD)v=0;
return self();
}
Self&operator--(){
if(v==0)v=Self::MOD;
--v;
return self();
}
Self&operator+=(const Self&o){v=Self::sub_mod_raw(v,Self::MOD-o.v);return self();}
Self&operator-=(const Self&o){v=Self::sub_mod_raw(v,o.v);return self();}
Self&operator/=(const Self&o){return self()*=o.inv();}
static V sub_mod_raw(V a,V b){return a<b?a-b+Self::MOD:a-b;}
Self neg()const{return from_reduced(v?Self::MOD-v:0);}
Self inv()const{return from_reduced(mod_inv_in_range(v,Self::MOD));}
private:
Self&self(){return static_cast<Self&>(*this);}
};
template<auto MOD_>struct modnum:mod_ops<modnum<MOD_>,std::make_unsigned_t<decltype(MOD_)>>{
using Self=modnum;
static_assert(MOD_>0,"MOD must be positive");
using V=std::make_unsigned_t<decltype(MOD_)>;
static constexpr V MOD=V(MOD_);
using base=mod_ops<modnum,V>;
using base::base;
using base::v;
using base::reduce;
static V reduce(std::unsigned_integral auto x){return V(x%MOD);}
explicit operator std::make_signed_t<V>()const
requires(MOD<=V(std::numeric_limits<std::make_signed_t<V>>::max()))
{
return std::make_signed_t<V>(v);
}
Self&operator*=(const Self&o){
if constexpr(sizeof(V)<=4)v=V(uint64_t(v)*o.v%MOD);
else v=V(__uint128_t(v)*o.v%MOD);
return*this;
}
};
struct mod_goldilocks:mod_ops<mod_goldilocks,uint64_t>{
using Self=mod_goldilocks;
static constexpr uint64_t MOD=0xffffffff00000001ull;
static constexpr uint64_t EPS=-MOD;
static constexpr uint64_t PRIMITIVE_ROOT=2717;
using base=mod_ops<mod_goldilocks,uint64_t>;
using base::base;
using base::reduce;
mod_goldilocks()=default;
mod_goldilocks(__int128_t a):base(a<0?uint64_t(MOD-1-__uint128_t(~a)%MOD):uint64_t(__uint128_t(a)%MOD),is_reduced_tag{}){}
mod_goldilocks(__uint128_t a):base(uint64_t(a%MOD),is_reduced_tag{}){}
static uint64_t reduce(std::unsigned_integral auto x){
static_assert(sizeof(x)<=8);
uint64_t a=x;
return a>=MOD?a-MOD:a;
}
static uint64_t sub_mod_raw(uint64_t a,uint64_t b){
#if defined(__x86_64__)
uint64_t res_wrapped=a;
uint64_t adjustment=b;
asm(
"sub %[y], %[x]\n\t"
"sbb %k[y], %k[y]\n\t"
:[x]"+r"(res_wrapped),
[y]"+r"(adjustment)
:
:"cc"
);
#else
uint64_t res_wrapped=a-b;
uint64_t adjustment=(res_wrapped>a)?EPS:0;
#endif
return res_wrapped-adjustment;
}
static uint64_t reduce_u160_raw(uint64_t lo,uint32_t mi,uint64_t hi){
return sub_mod_raw(sub_mod_raw(lo,hi),MOD-(uint64_t(mi)<<32)+mi);
}
static uint64_t reduce_u128_raw(__uint128_t v){
uint64_t hi=uint64_t(v>>64);
uint64_t lo=uint64_t(v);
uint32_t hi_hi=uint32_t(hi>>32);
uint32_t hi_lo=uint32_t(hi);
return reduce_u160_raw(lo,hi_lo,hi_hi);
}
Self&operator*=(Self o){
v=reduce_u128_raw(__uint128_t(v)*__uint128_t(o.v));
return*this;
}
};
template<typename T>T power(T a,long long b){
assert(b>=0);
T r=1;while(b){if(b&1)r*=a;b>>=1;a*=a;}return r;
}
template<typename U,typename V>struct pairnum:num_ops<pairnum<U,V>>{
using Self=pairnum;
U u;
V v;
pairnum():u(0),v(0){}
pairnum(long long val):u(val),v(val){}
pairnum(const U&u_,const V&v_):u(u_),v(v_){}
friend std::ostream&operator<<(std::ostream&out,const Self&n){return out<<'('<<n.u<<','<<' '<<n.v<<')';}
friend std::istream&operator>>(std::istream&in,Self&n){long long val;in>>val;n=Self(val);return in;}
friend bool operator==(const Self&a,const Self&b){return a.u==b.u&&a.v==b.v;}
Self inv()const{
return Self(u.inv(),v.inv());
}
Self neg()const{
return Self(u.neg(),v.neg());
}
Self&operator++(){
++u,++v;
return*this;
}
Self&operator--(){
--u,--v;
return*this;
}
Self&operator+=(const Self&o){
u+=o.u;
v+=o.v;
return*this;
}
Self&operator-=(const Self&o){
u-=o.u;
v-=o.v;
return*this;
}
Self&operator*=(const Self&o){
u*=o.u;
v*=o.v;
return*this;
}
Self&operator/=(const Self&o){
u/=o.u;
v/=o.v;
return*this;
}
};
template<typename tag>struct dynamic_modnum:mod_ops<dynamic_modnum<tag>,uint32_t>{
using Self=dynamic_modnum;
private:
inline static uint32_t MOD_=0;
inline static uint64_t BARRETT_M=0;
public:
static constexpr uint32_t const&MOD=MOD_;
using base=mod_ops<dynamic_modnum,uint32_t>;
using base::base;
using base::v;
using base::reduce;
static void set_mod(int mod){
assert(mod>0);
MOD_=uint32_t(mod);
BARRETT_M=(uint64_t(-1)/MOD);
}
static uint32_t barrett_reduce_partial(uint64_t a){
return uint32_t(a-uint64_t((__uint128_t(BARRETT_M)*a)>>64)*MOD);
}
static uint32_t barrett_reduce(uint64_t a){
int32_t res=int32_t(barrett_reduce_partial(a)-MOD);
return uint32_t((res<0)?res+int32_t(MOD):res);
}
struct mod_reader{
friend std::istream&operator>>(std::istream&i,mod_reader){
int mod;i>>mod;
Self::set_mod(mod);
return i;
}
};
static mod_reader MOD_READER(){
return mod_reader();
}
static uint32_t reduce(std::unsigned_integral auto x){
static_assert(sizeof(x)<=8);
return barrett_reduce(x);
}
explicit operator int()const{return int(v);}
Self&operator*=(const Self&o){
v=barrett_reduce(uint64_t(v)*o.v);
return*this;
}
};
template<typename T>struct mod_constraint{
T v,mod;
friend mod_constraint operator&(mod_constraint a,mod_constraint b){
if(a.mod<b.mod)std::swap(a,b);
if(b.mod==1)return a;
extended_gcd_result<T>egcd=extended_gcd<T>(a.mod,b.mod);
assert(a.v%egcd.gcd==b.v%egcd.gcd);
T extra=b.v-a.v%b.mod;
extra/=egcd.gcd;
extra*=egcd.coeff_a;
extra%=b.mod/egcd.gcd;
extra+=(extra<0)?b.mod/egcd.gcd:0;
return mod_constraint{
a.v+extra*a.mod,
a.mod*(b.mod/egcd.gcd)
};
}
};
// src/fft/core.hpp
namespace ecnerwala::fft{
template<typename dbl>struct cplx{
dbl x,y;
cplx(dbl x_=0,dbl y_=0):x(x_),y(y_){}
friend cplx operator+(cplx a,cplx b){return cplx(a.x+b.x,a.y+b.y);}
friend cplx operator-(cplx a,cplx b){return cplx(a.x-b.x,a.y-b.y);}
friend cplx operator*(cplx a,cplx b){return cplx(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x);}
friend cplx conj(cplx a){return cplx(a.x,-a.y);}
friend cplx inv(cplx a){dbl n=(a.x*a.x+a.y*a.y);return cplx(a.x/n,-a.y/n);}
};
template<typename num>struct getRoot{
static num f(int k)=delete;
};
template<typename dbl>struct getRoot<cplx<dbl>>{
static cplx<dbl>f(int k){
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
dbl a=2*M_PI/k;
return cplx<dbl>(cos(a),sin(a));
}
};
template<int MOD>struct primitive_root{
static const int value;
};
template<>struct primitive_root<998244353>{
static const int value=3;
};
template<>struct primitive_root<(15<<27)+1>{
static const int value=31;
};
template<>struct primitive_root<(127<<24)+1>{
static const int value=3;
};
template<>struct primitive_root<(7<<26)+1>{
static const int value=3;
};
template<>struct primitive_root<(5<<25)+1>{
static const int value=3;
};
template<int MOD>struct getRoot<modnum<MOD>>{
static modnum<MOD>f(int k){
assert((MOD-1)%k==0);
return power(modnum<MOD>(primitive_root<MOD>::value),(MOD-1)/k);
}
};
template<>struct getRoot<mod_goldilocks>{
static mod_goldilocks f(int k){
assert((mod_goldilocks::MOD-1)%k==0);
return power(mod_goldilocks(mod_goldilocks::PRIMITIVE_ROOT),(mod_goldilocks::MOD-1)/k);
}
};
template<typename num>struct fft_core{
static inline vector<int>rev;
static inline vector<num>rt,inv_rt;
static void init(int n){
if(n<=sz(rt))return;
rev.resize(n);
for(int i=0;i<n;i++){
rev[i]=(rev[i>>1]|((i&1)*n))>>1;
}
rt.reserve(n);inv_rt.reserve(n);
while(sz(rt)<2&&sz(rt)<n){rt.push_back(num(1));inv_rt.push_back(num(1));}
for(int k=sz(rt);k<n;k*=2){
rt.resize(2*k);inv_rt.resize(2*k);
num z=getRoot<num>::f(2*k);
num iz=inv(z);
for(int i=k/2;i<k;i++){
rt[2*i]=rt[i],rt[2*i+1]=rt[i]*z;
inv_rt[2*i]=inv_rt[i],inv_rt[2*i+1]=inv_rt[i]*iz;
}
}
}
static int brev(int i,int n){
int s=__builtin_ctz(unsigned(sz(rev)/n));
return rev[i]>>s;
}
static int conj_index(int j){
return j==0?0:j^((1<<(31-__builtin_clz(unsigned(j))))-1);
}
static void forward(std::span<num>a){
int n=sz(a);
if(n<=1)return;
init(n);
for(int k=n/2;k>=1;k/=2){
for(int i=0;i<n;i+=2*k){
for(int j=0;j<k;j++){
num u=a[i+j],v=a[i+j+k];
a[i+j]=u+v;
a[i+j+k]=(u-v)*rt[j+k];
}
}
}
}
static void inverse(std::span<num>a){
int n=sz(a);
if(n<=1)return;
init(n);
for(int k=1;k<n;k*=2){
for(int i=0;i<n;i+=2*k){
for(int j=0;j<k;j++){
num t=inv_rt[j+k]*a[i+j+k];
a[i+j+k]=a[i+j]-t;
a[i+j]=a[i+j]+t;
}
}
}
}
static void extend(std::span<num>t,std::span<const num>coeffs){
int n=sz(t)/2;
assert(sz(coeffs)<=2*n);
init(sz(t));
auto b=t.subspan(n,n);
int lo=min(sz(coeffs),n);
for(int i=0;i<lo;i++){
b[i]=coeffs[i]*rt[n+i];
}
std::fill(b.begin()+lo,b.end(),num(0));
for(int i=n;i<sz(coeffs);i++){
b[i-n]=b[i-n]-coeffs[i]*rt[i];
}
forward(b);
}
static void even_half(std::span<const num>t,std::span<num>out){
int n=sz(out);
assert(sz(t)>=2*n);
num half=inv(num(2));
for(int j=0;j<n;j++)out[j]=(t[2*j]+t[2*j+1])*half;
}
static void odd_half(std::span<const num>t,std::span<num>out){
int n=sz(out);
assert(sz(t)>=2*n);
init(2*n);
num half=inv(num(2));
for(int j=0;j<n;j++){
out[j]=(t[2*j]-t[2*j+1])*half*inv_rt[n+brev(j,n)];
}
}
};
}
// src/fft/engines/ntt.hpp
namespace ecnerwala::fft::engines{
template<typename num>struct ntt{
using value_type=num;
static constexpr bool commutative=true;
using core=fft_core<num>;
struct transformed{
vector<num>v;
int size()const{return sz(v);}
};
using product=transformed;
static constexpr int unit_scale=0;
template<int A=0>using transformed_t=transformed;
template<int K=0>using product_t=product;
static transformed transform(std::span<const num>a,int n){
assert(sz(a)<=2*n);
transformed r;
r.v.assign(n,num(0));
int lo=min(sz(a),n);
std::copy(a.begin(),a.begin()+lo,r.v.begin());
for(int i=n;i<sz(a);i++)r.v[i-n]+=a[i];
core::forward(std::span<num>(r.v));
return r;
}
static void extend_to(transformed&t,int m,std::span<const num>coeffs){
assert(!(m&(m-1))&&sz(coeffs)<=2*m);
if(t.size()>=m)return;
if(t.size()==0){t=transform(coeffs,m);return;}
while(t.size()<m){
int s=t.size();
t.v.resize(2*s);
core::extend(std::span<num>(t.v),coeffs.first(size_t(min(sz(coeffs),2*s))));
}
}
static transformed downsample(const transformed&t,int n,bool odd){
transformed r;r.v.resize(n);
if(odd)core::odd_half(std::span<const num>(t.v),std::span<num>(r.v));
else core::even_half(std::span<const num>(t.v),std::span<num>(r.v));
return r;
}
static transformed negate_arg(const transformed&t,int n){
assert(n>=2&&t.size()>=n);
transformed r;r.v.resize(n);
for(int j=0;j<n;j++)r.v[j]=t.v[j^1];
return r;
}
static product mul(const transformed&a,const transformed&b,int n){
assert(a.size()>=n&&b.size()>=n);
product p;p.v.resize(n);
for(int i=0;i<n;i++)p.v[i]=a.v[i]*b.v[i];
return p;
}
static product sq(const transformed&a,int n){return mul(a,a,n);}
static product mul2(
const transformed&a1,const transformed&b1,
const transformed&a2,const transformed&b2,
int n
){
assert(a1.size()>=n&&b1.size()>=n&&a2.size()>=n&&b2.size()>=n);
product p;p.v.resize(n);
for(int i=0;i<n;i++)p.v[i]=a1.v[i]*b1.v[i]+a2.v[i]*b2.v[i];
return p;
}
static product add(product&&a,const product&b){
assert(a.size()==b.size());
for(int i=0;i<a.size();i++)a.v[i]+=b.v[i];
return std::move(a);
}
template<typename Op=assign_op>static void finish(product&&p,std::span<num>out,Op op={}){
int n=p.size();
assert(sz(out)<=n);
core::inverse(std::span<num>(p.v));
num d=inv(num(n));
for(int i=0;i<sz(out);i++)op(out[i],p.v[i]*d);
}
};
}
#pragma GCC diagnostic pop
// clang-format on
// @formatter:on

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		using num = modnum<998244353>;
		std::vector<num> A(N); for (auto& a : A) std::cin >> a;

		std::vector<num> fact(N+1);
		fact[0] = 1;
		for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(N+1);
		ifact[N] = inv(fact[N]);
		for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);
		// we have 2 guys which are all cw or all ccw

		// the rest are picking some guys to be sinks, some guys to be sources, and everyone else to be middle
		// the weight of k sinks/k sources is you arrange them, then mix the rest however
		// how do we casework over product of k / product of k??
		// allow overlaps, and then cut them down

		using E = ecnerwala::fft::engines::ntt<num>;
		// just reverse them for convenience
		for (auto& a : A) a = -a;
		auto vieta = ecnerwala::poly::subproduct_tree<E>(A).prod(1).rev_series();

		// v2[k] = sum_a <= k ways[a] * fact[N-2*a] / fact[k-a] / fact[N-a-k]
		// (we want sum of ways[a] * fact[N]/fact[2*a]/fact[N-2*a] * fact[a] * fact[a] * 2)
		// (src: Devin)
		// We can actually compute ways[a] now:
		// We can write this relation as
		// sum_k v2[k] s^k = sum_a ways[a] * s^a * (1+s)^(N-2a)
		// Then, computing the GF of ways[a] is just an inverse change of variables.
		// Let y = s / (1+s)^2. Then,
		//   sum ways[a] * y^a = (1+s)^{-N} * sum__k v2[k] s^k
		//
		// To write s in terms of y:
		//   y = s / (1+s)^2 => (1+s) = 1 + (1+s)^2 * y => 1+s = catalan(y)
		// Thus, we just want poly_compose on catalan

		using ps = ecnerwala::series::trunc<E>;
		ps v2(N/2+1);
		for (int k = 0; k <= N/2; k++) v2[k] = vieta[k] * vieta[N-k];
		ps catalan(N/2+1);
		for (int i = 0; i <= N/2; i++) { catalan[i] = fact[2*i] * ifact[i] * ifact[i+1]; }
		ps catalan1 = catalan; catalan1[0] = 0;
		ps ways = ps_compose(v2, catalan1) * ps_pow_monic(catalan, -N);
		num ans = 0;
		for (int k = 0; k <= N/2; k++) { std::cerr << ways[k] << ' '; } std::cerr << '\n';
		for (int k = 0; k <= N/2; k++) {
			ans += ways[k] * fact[N] * ifact[2*k] * fact[k] * fact[k] * 2;
		}
		std::cout << ans / num(N) << '\n';
	}

	return 0;
}
