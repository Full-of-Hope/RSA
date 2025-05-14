#include"RSA.h"
//判断是否为素数
bool is_prime(int1 n) {
    if (n < 2)return false;
    else
        if (n % 2 == 0)return (n == 2);//只有2是素数
    for (int1 i = 3; i * i <= n; i += 2) {
        if (n % i == 0)return false;
    }
    return true;
}
//产生随机数素数p,q
int1 generate_prime(unsigned bitlen) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int1> dis(
        (int1(1) << (bitlen - 1)) + 1,
        (int1(1) << bitlen) - 1
    );//生成范围[2^(bitlen-1)+1,2^bitlen-1]
    while (true) {
        int1 candidate = dis(gen);
        if (candidate % 2 == 0) ++candidate;//确保是奇数
        if (is_prime(candidate)) return candidate;
    }
}
//xa+yb=gcd(a,b)
// 扩展欧几里得算法函数，返回gcd(a, b)并计算系数x和y
int1 e_gcd(int1 a, int1 b, int1& x, int1& y) {
    // 当b为0时，gcd是a，此时a*x + b*y = a*1 + 0 = a
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    else {
        int1 d = e_gcd(b, a % b, x, y);// 递归调用
        int1 temp = x;
        x = y;
        y = temp - (a / b) * y;
        return d;
    }
}
int1 modinv(int1 a, int1 m) {//a是公钥，m=pq，求逆元
    int1 x, y;
    int1 g = e_gcd(a, m, x, y);
    if (g != 1) throw runtime_error("No inverse");
    return (x % m + m) % m;//私钥，确保是正数
}
int1 modexp(int1 base, int1 e, int1 n) {//幂运算
    if (n == 0) {
        throw invalid_argument("Modulus must be > 0");
    }
    if (n == 1) {
        return 0;
    }
    base = (base % n + n) % n;
    int1 result = 1;
    int1 base64 = base;  // 来避免溢出
    while (e > 0) {
        // 如果现在位数是1，结果是基数
        if (e & 1) {
            result = (result * base64) % n;
            result = (result % n + n) % n; // 确保result非负
        }
        base64 = (base64 * base64) % n;
        base64 = (base64 % n + n) % n; // 确保base64非负
        e >>= 1;
    }
    return result;
}