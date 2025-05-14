#include<iostream>
#include<bitset>
#include<graphics.h>
#include<string>
#include<algorithm>
#include <sstream>
#include <iomanip>
#include"RSA.h"
using namespace std;

void drawMain();//主界面
void drawset();//设置界面
void drawEncrypt();//加密界面
void drawDecrypt();//解密界面
void drawlogin();//登录
void drawcheck();//检查
void handleMouse(int x, int y);//鼠标处理
void handle();//处理设置
void handleEncryption();//处理加密
void handleDecryption();//处理解密
//界面枚举状态
enum State { MAIN, SET, ENCRYPT, DECRYPT, LOGIN, PRIVATE_KEY_CHECK };//主界面、加密、解密、登录，检查
State c_state = MAIN;
//输入状态枚举
enum ActiveInput { TEXT_INPUT };
ActiveInput activeInput = TEXT_INPUT;
//全局变量
string inputtext;//输入文本
string resulttext;//输出文本
string encryptedResult; // 保存最近一次加密的结果
pair<int1, int1> public_key;  // 公钥对（e,n)
int1 private_key;             // 私钥
const string LOGIN_PASSWORD = "123";
//按钮
const int BTN_WIDTH = 100;
const int BTN_HEIGHT = 40;
RECT mainEncryptBtn = { 220, 250, 220 + BTN_WIDTH, 250 + BTN_HEIGHT };
RECT mainDecryptBtn = { 220, 350, 220 + BTN_WIDTH, 350 + BTN_HEIGHT };
RECT mainset = { 220,150,220 + BTN_WIDTH,150 + BTN_HEIGHT };
//设置
RECT P_KeyInput = { 50, 130, 590, 160 };
RECT Result = { 50, 210, 590, 240 };
//加密
RECT e_Input = { 50, 130, 590, 160 };
RECT e_Result = { 50, 210, 590, 240 };
//解密
RECT d_Input = { 50, 130, 590, 160 };
RECT d_Result = { 50, 210, 590, 240 };
//操作
RECT backBtn = { 50, 300, 150, 340 };
RECT actionBtn = { 450, 300, 550, 340 };

int main() {
	cout << "测试" << endl;
	int1 x = 0, y = 0;
	int1 a = e_gcd(100, 3, x, y);
	cout << "100 mod 3=" << x << endl;
	int1 b = modinv(10, 3);
	cout << "10逆 mod 3 =" << b << endl;
	int1 c = modexp(2, 10, 1000);
	cout << "2^10 mod 1000=" << c << endl;
	initgraph(640, 480);
	ExMessage message;//鼠标信息

	while (true) {
		if (peekmessage(&message)) {//事件
			if (message.message == WM_LBUTTONDOWN) {//鼠标
				handleMouse(message.x, message.y);
			}
			else if (message.message == WM_CHAR) {//键盘
				if (message.ch == 8) {//退格键
					if (c_state == ENCRYPT) {
						if (activeInput == TEXT_INPUT && !inputtext.empty())
							inputtext.pop_back();
					}
					else if (c_state == DECRYPT) {
						if (activeInput == TEXT_INPUT && !inputtext.empty())
							inputtext.pop_back();
					}
					else if (c_state == SET) {
						if (activeInput == TEXT_INPUT && !inputtext.empty())
							inputtext.pop_back();
					}
					else if (c_state == LOGIN) {
						if (activeInput == TEXT_INPUT && !inputtext.empty())
							inputtext.pop_back();
					}
					else if (c_state == PRIVATE_KEY_CHECK) {
						if (activeInput == TEXT_INPUT && !inputtext.empty())
							inputtext.pop_back();
					}
				}
				else if (message.ch >= 32 && message.ch <= 126) {
					if (c_state == ENCRYPT) {
						if (activeInput == TEXT_INPUT)
							inputtext += char(message.ch);
					}
					if (c_state == SET) {
						if (activeInput == TEXT_INPUT)
							inputtext += char(message.ch);
					}
					if (c_state == LOGIN) {
						if (activeInput == TEXT_INPUT)
							inputtext += char(message.ch);
					}
					if (c_state == PRIVATE_KEY_CHECK) {
						if (isdigit(message.ch))
							inputtext += char(message.ch);
					}
					if (c_state == DECRYPT) {
						if (activeInput == TEXT_INPUT) {
							char c = toupper(message.ch);
							if ((c >= '0' && c <= '9') || c == ' ')
								inputtext += c;
						}
					}
				}
			}
		}
		//批量绘图
		BeginBatchDraw();
		cleardevice();
		switch (c_state) {//状态
		case MAIN:
			drawMain();
			break;
		case LOGIN:
			drawlogin();
			break;
		case PRIVATE_KEY_CHECK:
			drawcheck();
			break;
		case SET:
			drawset();
			break;
		case ENCRYPT:
			drawEncrypt();
			break;
		case DECRYPT:
			drawDecrypt();
			break;
		}
		EndBatchDraw();
		Sleep(10);
	}
	closegraph();
	return 0;
}

//主界面
void drawMain() {
	cleardevice();
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	//标题
	outtextxy(224, 70, _T("RSA加解密系统"));
	//设置
	rectangle(mainset.left, mainset.top, mainset.right, mainset.bottom);
	outtextxy(mainset.left + 34, mainset.top + 12, _T("设置"));
	//加密
	rectangle(mainEncryptBtn.left, mainEncryptBtn.top, mainEncryptBtn.right, mainEncryptBtn.bottom);
	outtextxy(mainEncryptBtn.left + 34, mainEncryptBtn.top + 12, _T("加密"));
	//解密
	rectangle(mainDecryptBtn.left, mainDecryptBtn.top, mainDecryptBtn.right, mainDecryptBtn.bottom);
	outtextxy(mainDecryptBtn.left + 34, mainDecryptBtn.top + 12, _T("解密"));
}
//设置界面
void drawset() {
	cleardevice();
	settextcolor(WHITE);
	//公钥
	outtextxy(P_KeyInput.left, P_KeyInput.top - 20, _T("公钥:"));
	rectangle(P_KeyInput.left, P_KeyInput.top, P_KeyInput.right, P_KeyInput.bottom);
	outtextxy(P_KeyInput.left + 10, P_KeyInput.top + 10, inputtext.c_str());
	//结果
	outtextxy(Result.left, Result.top - 20, _T("结果:"));
	rectangle(Result.left, Result.top, Result.right, Result.bottom);
	outtextxy(Result.left + 10, Result.top + 10, resulttext.c_str());
	//操作
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("返回"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("执行"));
}
//加密界面
void drawEncrypt() {
	cleardevice();
	settextcolor(WHITE);
	//明文
	outtextxy(e_Input.left, e_Input.top - 20, _T("明文:"));
	rectangle(e_Input.left, e_Input.top, e_Input.right, e_Input.bottom);
	outtextxy(e_Input.left + 10, e_Input.top + 10, inputtext.c_str());
	//密文
	outtextxy(e_Result.left, e_Result.top - 20, _T("加密结果:"));
	rectangle(e_Result.left, e_Result.top, e_Result.right, e_Result.bottom);
	outtextxy(e_Result.left + 10, e_Result.top + 10, resulttext.c_str());
	//操作
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("返回"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("加密"));
}
//解密界面
void drawDecrypt() {
	cleardevice();
	settextcolor(WHITE);
	//密文
	outtextxy(d_Input.left, d_Input.top - 20, _T("密文:"));
	rectangle(d_Input.left, d_Input.top, d_Input.right, d_Input.bottom);
	outtextxy(d_Input.left + 10, d_Input.top + 10, inputtext.c_str());

	//明文
	outtextxy(d_Result.left, d_Result.top - 20, _T("解密结果:"));
	rectangle(d_Result.left, d_Result.top, d_Result.right, d_Result.bottom);
	outtextxy(d_Result.left + 10, d_Result.top + 10, resulttext.c_str());
	//按钮
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("返回"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("解密"));
}
//处理鼠标事件
void handleMouse(int x, int y) {
	switch (c_state) {
	case MAIN:
		if (x > mainEncryptBtn.left && x < mainEncryptBtn.right &&
			y > mainEncryptBtn.top && y < mainEncryptBtn.bottom) {
			c_state = ENCRYPT;
			inputtext.clear();
			resulttext.clear();
			activeInput = TEXT_INPUT;
		}
		else if (x > mainDecryptBtn.left && x < mainDecryptBtn.right &&
			y > mainDecryptBtn.top && y < mainDecryptBtn.bottom) {
			c_state = DECRYPT;
			inputtext = encryptedResult; // 自动填充加密结果到解密输入框
			resulttext.clear();
			activeInput = TEXT_INPUT;
		}
		else if (x > mainset.left && x < mainset.right &&
			y > mainset.top && y < mainset.bottom) {
			c_state = SET;
			inputtext.clear();
			resulttext.clear();
			activeInput = TEXT_INPUT;
		}
		if (PtInRect(&mainset, { x, y })) {
			c_state = LOGIN;  // 进入设置前先到登录界面
			inputtext.clear();
			resulttext.clear();
		}
		if (PtInRect(&mainDecryptBtn, { x, y })) {
			c_state = PRIVATE_KEY_CHECK;  // 进入解密前先到私钥验证
			inputtext.clear();
			resulttext.clear();
		}
		break;
	case LOGIN:
		if (PtInRect(&backBtn, { x, y })) {
			c_state = MAIN;
			inputtext.clear();

		}
		else if (PtInRect(&actionBtn, { x, y })) {
			if (inputtext == LOGIN_PASSWORD) {
				c_state = SET;
				inputtext.clear();
				resulttext.clear();
			}
			else {
				resulttext = "密码错误！";
			}
		}
		break;

	case PRIVATE_KEY_CHECK:
		if (PtInRect(&backBtn, { x, y })) {
			c_state = MAIN;
			inputtext.clear();
		}
		else if (PtInRect(&actionBtn, { x, y })) {
			try {
				int1 input_key = stoull(inputtext);
				if (input_key == private_key) {
					c_state = DECRYPT;
					inputtext = encryptedResult;  // 自动填充加密结果
				}
				else {
					resulttext = "私钥验证失败！";
				}
			}
			catch (...) {
				resulttext = "无效的私钥格式！";
			}
		}
		break;
	case SET:
		if (x > backBtn.left && x < backBtn.right &&
			y > backBtn.top && y < backBtn.bottom) {
			c_state = MAIN;
			inputtext.clear();
			resulttext.clear();
		}
		else if (x > actionBtn.left && x < actionBtn.right &&
			y > actionBtn.top && y < actionBtn.bottom) {
			handle();//处理
		}
		else if (x >= P_KeyInput.left && x <= P_KeyInput.right &&
			y >= P_KeyInput.top && y <= P_KeyInput.bottom) {
			activeInput = TEXT_INPUT;
		}
		break;
	case ENCRYPT:
		if (x > backBtn.left && x < backBtn.right &&
			y > backBtn.top && y < backBtn.bottom) {
			c_state = MAIN;
			inputtext.clear();
			resulttext.clear();
		}
		else if (x > actionBtn.left && x < actionBtn.right &&
			y > actionBtn.top && y < actionBtn.bottom) {
			handleEncryption();//加密
		}
		else if (x >= e_Input.left && x <= e_Input.right &&
			y >= e_Input.top && y <= e_Input.bottom) {
			activeInput = TEXT_INPUT;
		}
		break;

	case DECRYPT:
		if (x > backBtn.left && x < backBtn.right &&
			y > backBtn.top && y < backBtn.bottom) {
			c_state = MAIN;
			inputtext.clear();
			resulttext.clear();
		}
		else if (x > actionBtn.left && x < actionBtn.right &&
			y > actionBtn.top && y < actionBtn.bottom) {
			handleDecryption();//解密

		}
		else if (x >= d_Input.left && x <= d_Input.right &&
			y >= d_Input.top && y <= d_Input.bottom) {
			activeInput = TEXT_INPUT;
		}
		break;
	}
}
void drawlogin() {
	cleardevice();
	settextcolor(WHITE);
	outtextxy(50, 100, _T("请输入管理员密码:"));
	rectangle(50, 130, 590, 160);
	outtextxy(60, 140, inputtext.c_str());

	// 绘制按钮
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("返回"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("确认"));

	// 显示错误信息
	outtextxy(50, 200, resulttext.c_str());
}
void drawcheck() {
	cleardevice();
	settextcolor(WHITE);
	outtextxy(50, 100, _T("请输入私钥进行身份验证:"));
	rectangle(50, 130, 590, 160);
	outtextxy(60, 140, inputtext.c_str());

	// 绘制按钮
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("返回"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("确认"));

	// 显示错误信息
	outtextxy(50, 200, resulttext.c_str());
}
//解密处理
void handleDecryption() {
	try {
		if (inputtext.empty()) throw std::invalid_argument("输入为空");
		vector<int1> ciphertext;
		istringstream iss(inputtext);
		string token;
		while (iss >> token) {
			try {
				ciphertext.push_back(stoull(token));
			}
			catch (...) {
				throw invalid_argument("无效的密文格式");
			}
		}
		string plaintext;
		for (auto ct : ciphertext) {
			int1 pt = modexp(ct, private_key, public_key.second);
			if (pt > 255) throw runtime_error("解密值超出ASCII范围");
			plaintext += static_cast<unsigned char>(pt);
		}
		resulttext = plaintext;
	}
	catch (const exception& e) {
		resulttext = "错误: " + string(e.what());
	}
}
// 加密处理
void handleEncryption() {
	if (inputtext.empty()) {
		resulttext = "请输入明文!";
		return;
	}
	string plaintext = inputtext;
	string ciphertext = "";
	for (size_t i = 0; i < plaintext.size(); ++i) {
		int1 pt = static_cast<unsigned char>(plaintext[i]);
		int1 ct = modexp(pt, public_key.first, public_key.second);
		ciphertext += to_string(static_cast<uint64_t>(ct));
		if (i != plaintext.size() - 1) ciphertext += " ";
	}
	resulttext = ciphertext;
	encryptedResult = ciphertext;
}
// handle函数
void handle() {
	if (inputtext.empty()) {
		resulttext = "请输入公钥e!";
		return;
	}
	// 转换用户输入的e
	int1 e;
	try {
		e = stoull(inputtext);
	}
	catch (...) {
		resulttext = "公钥必须是正整数!";
		return;
	}
	// 生成素数
	int1 p = generate_prime(12);
	int1 q = generate_prime(12);
	// 检查乘积是否溢出
	if (p > UINT64_MAX / q) {
		resulttext = "素数过大导致溢出!";
		return;
	}
	int1 n = p * q;
	int1 phi_n = (p - 1) * (q - 1);

	// 验证e的有效性
	if (e <= 1 || e >= phi_n) {
		resulttext = "e必须满足1 < e < phi(n)";
		return;
	}
	// 检查e与phi_n是否互质
	int1 x, y;
	int1 g = e_gcd(e, phi_n, x, y);
	if (g != 1) {
		resulttext = "e必须与O(n)互质";
		return;
	}
	// 计算私钥
	int1 d;
	try {
		d = modinv(e, phi_n);
	}
	catch (const runtime_error&) {
		resulttext = "无效的公钥，无法生成私钥";
		return;
	}
	// 保存密钥并显示结果
	public_key = make_pair(e, n);
	private_key = d;
	resulttext = "生成成功! 素数:(" + to_string(p) + "，" + to_string(q) + "),公钥(e, n):(" +
		to_string(e) + "," + to_string(n) + ")，私钥d:" + to_string(d);
}