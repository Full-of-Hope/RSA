#include<iostream>
#include <random>
#include <cstdint>
#include <cmath>
using namespace std;
typedef int64_t int1;

int1 e_gcd(int1 a, int1 b, int1& x, int1& y);//计算系数，xa+yb=gcd(a,b)
bool is_prime(int1 n);//判断素数
int1 generate_prime(unsigned bitlen);//产生随机数p,q
int1 modinv(int1 a, int1 m);//求逆元
int1 modexp(int1 base, int1 e, int1 n);//模幂次方