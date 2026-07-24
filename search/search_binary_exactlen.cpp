#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

struct __attribute__((packed)) Rec { uint64_t prod; uint32_t n; uint8_t pop; };
static inline uint32_t revbits(uint32_t x, int len){uint32_t r=0;for(int i=0;i<len;++i){r=(r<<1)|(x&1u);x>>=1;}return r;}
static inline bool same_sig(uint32_t a,uint32_t b,int L){for(int d=1;d<L;++d)if(__builtin_popcount(a&(a>>d))!=__builtin_popcount(b&(b>>d)))return false;return true;}
static std::string bits(uint32_t x,int L){std::string s;for(int i=L-1;i>=0;--i)s.push_back((x>>i&1)?'1':'0');return s;}
static void print_rec(const Rec&r,int L){std::cout<<"n="<<r.n<<" pop="<<(int)r.pop<<" bits="<<bits(r.n,L)<<" rev="<<revbits(r.n,L)<<" counts=";for(int d=1;d<L;++d)std::cout<<__builtin_popcount(r.n&(r.n>>d))<<(d+1<L?',':'\n');}
int main(int argc,char**argv){int L=argc>1?atoi(argv[1]):29;uint64_t total=1ull<<(L-2);std::cerr<<"L="<<L<<" records="<<total<<" sizeof="<<sizeof(Rec)<<"\n";std::vector<Rec>a;a.reserve(total);uint32_t lo=(1u<<(L-1))|1u,hi=1u<<L;for(uint32_t n=lo;n<hi;n+=2){uint32_t r=revbits(n,L);a.push_back({(uint64_t)n*r,n,(uint8_t)__builtin_popcount(n)});}std::cerr<<"generated\n";std::sort(a.begin(),a.end(),[](const Rec&x,const Rec&y){if(x.prod!=y.prod)return x.prod<y.prod;if(x.pop!=y.pop)return x.pop<y.pop;return x.n<y.n;});std::cerr<<"sorted\n";size_t i=0;uint64_t groups=0,cands=0;while(i<a.size()){size_t pend=i+1;while(pend<a.size()&&a[pend].prod==a[i].prod)++pend;if(pend-i>=3){groups++;size_t s=i;while(s<pend){size_t e=s+1;while(e<pend&&a[e].pop==a[s].pop)++e;if(e-s>=3){cands++;for(size_t u=s;u<e;++u)for(size_t v=u+1;v<e;++v){if(a[v].n==revbits(a[u].n,L))continue;if(!same_sig(a[u].n,a[v].n,L)){std::cout<<"COUNTEREXAMPLE L="<<L<<" product="<<a[u].prod<<"\n";for(size_t t=s;t<e;++t)print_rec(a[t],L);return 0;}}}s=e;}}i=pend;}std::cerr<<"none; groups="<<groups<<" candidates="<<cands<<"\n";}
