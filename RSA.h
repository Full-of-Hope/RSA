#include<iostream>
#include <random>
#include <cstdint>
#include <cmath>
using namespace std;
typedef int64_t int1;

int1 e_gcd(int1 a, int1 b, int1& x, int1& y);//����ϵ����xa+yb=gcd(a,b)
bool is_prime(int1 n);//�ж�����
int1 generate_prime(unsigned bitlen);//���������p,q
int1 modinv(int1 a, int1 m);//����Ԫ
int1 modexp(int1 base, int1 e, int1 n);//ģ�ݴη�