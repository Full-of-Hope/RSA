#include"RSA.h"
//�ж��Ƿ�Ϊ����
bool is_prime(int1 n) {
    if (n < 2)return false;
    else
        if (n % 2 == 0)return (n == 2);//ֻ��2������
    for (int1 i = 3; i * i <= n; i += 2) {
        if (n % i == 0)return false;
    }
    return true;
}
//�������������p,q
int1 generate_prime(unsigned bitlen) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int1> dis(
        (int1(1) << (bitlen - 1)) + 1,
        (int1(1) << bitlen) - 1
    );//���ɷ�Χ[2^(bitlen-1)+1,2^bitlen-1]
    while (true) {
        int1 candidate = dis(gen);
        if (candidate % 2 == 0) ++candidate;//ȷ��������
        if (is_prime(candidate)) return candidate;
    }
}
//xa+yb=gcd(a,b)
// ��չŷ������㷨����������gcd(a, b)������ϵ��x��y
int1 e_gcd(int1 a, int1 b, int1& x, int1& y) {
    // ��bΪ0ʱ��gcd��a����ʱa*x + b*y = a*1 + 0 = a
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    else {
        int1 d = e_gcd(b, a % b, x, y);// �ݹ����
        int1 temp = x;
        x = y;
        y = temp - (a / b) * y;
        return d;
    }
}
int1 modinv(int1 a, int1 m) {//a�ǹ�Կ��m=pq������Ԫ
    int1 x, y;
    int1 g = e_gcd(a, m, x, y);
    if (g != 1) throw runtime_error("No inverse");
    return (x % m + m) % m;//˽Կ��ȷ��������
}
int1 modexp(int1 base, int1 e, int1 n) {//������
    if (n == 0) {
        throw invalid_argument("Modulus must be > 0");
    }
    if (n == 1) {
        return 0;
    }
    base = (base % n + n) % n;
    int1 result = 1;
    int1 base64 = base;  // ���������
    while (e > 0) {
        // �������λ����1������ǻ���
        if (e & 1) {
            result = (result * base64) % n;
            result = (result % n + n) % n; // ȷ��result�Ǹ�
        }
        base64 = (base64 * base64) % n;
        base64 = (base64 % n + n) % n; // ȷ��base64�Ǹ�
        e >>= 1;
    }
    return result;
}