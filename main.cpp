#include<iostream>
#include<bitset>
#include<graphics.h>
#include<string>
#include<algorithm>
#include <sstream>
#include <iomanip>
#include"RSA.h"
using namespace std;

void drawMain();//������
void drawset();//���ý���
void drawEncrypt();//���ܽ���
void drawDecrypt();//���ܽ���
void drawlogin();//��¼
void drawcheck();//���
void handleMouse(int x, int y);//��괦��
void handle();//��������
void handleEncryption();//�������
void handleDecryption();//�������
//����ö��״̬
enum State { MAIN, SET, ENCRYPT, DECRYPT, LOGIN, PRIVATE_KEY_CHECK };//�����桢���ܡ����ܡ���¼�����
State c_state = MAIN;
//����״̬ö��
enum ActiveInput { TEXT_INPUT };
ActiveInput activeInput = TEXT_INPUT;
//ȫ�ֱ���
string inputtext;//�����ı�
string resulttext;//����ı�
string encryptedResult; // �������һ�μ��ܵĽ��
pair<int1, int1> public_key;  // ��Կ�ԣ�e,n)
int1 private_key;             // ˽Կ
const string LOGIN_PASSWORD = "123";
//��ť
const int BTN_WIDTH = 100;
const int BTN_HEIGHT = 40;
RECT mainEncryptBtn = { 220, 250, 220 + BTN_WIDTH, 250 + BTN_HEIGHT };
RECT mainDecryptBtn = { 220, 350, 220 + BTN_WIDTH, 350 + BTN_HEIGHT };
RECT mainset = { 220,150,220 + BTN_WIDTH,150 + BTN_HEIGHT };
//����
RECT P_KeyInput = { 50, 130, 590, 160 };
RECT Result = { 50, 210, 590, 240 };
//����
RECT e_Input = { 50, 130, 590, 160 };
RECT e_Result = { 50, 210, 590, 240 };
//����
RECT d_Input = { 50, 130, 590, 160 };
RECT d_Result = { 50, 210, 590, 240 };
//����
RECT backBtn = { 50, 300, 150, 340 };
RECT actionBtn = { 450, 300, 550, 340 };

int main() {
	cout << "����" << endl;
	int1 x = 0, y = 0;
	int1 a = e_gcd(100, 3, x, y);
	cout << "100 mod 3=" << x << endl;
	int1 b = modinv(10, 3);
	cout << "10�� mod 3 =" << b << endl;
	int1 c = modexp(2, 10, 1000);
	cout << "2^10 mod 1000=" << c << endl;
	initgraph(640, 480);
	ExMessage message;//�����Ϣ

	while (true) {
		if (peekmessage(&message)) {//�¼�
			if (message.message == WM_LBUTTONDOWN) {//���
				handleMouse(message.x, message.y);
			}
			else if (message.message == WM_CHAR) {//����
				if (message.ch == 8) {//�˸��
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
		//������ͼ
		BeginBatchDraw();
		cleardevice();
		switch (c_state) {//״̬
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

//������
void drawMain() {
	cleardevice();
	setbkmode(TRANSPARENT);
	settextcolor(WHITE);
	//����
	outtextxy(224, 70, _T("RSA�ӽ���ϵͳ"));
	//����
	rectangle(mainset.left, mainset.top, mainset.right, mainset.bottom);
	outtextxy(mainset.left + 34, mainset.top + 12, _T("����"));
	//����
	rectangle(mainEncryptBtn.left, mainEncryptBtn.top, mainEncryptBtn.right, mainEncryptBtn.bottom);
	outtextxy(mainEncryptBtn.left + 34, mainEncryptBtn.top + 12, _T("����"));
	//����
	rectangle(mainDecryptBtn.left, mainDecryptBtn.top, mainDecryptBtn.right, mainDecryptBtn.bottom);
	outtextxy(mainDecryptBtn.left + 34, mainDecryptBtn.top + 12, _T("����"));
}
//���ý���
void drawset() {
	cleardevice();
	settextcolor(WHITE);
	//��Կ
	outtextxy(P_KeyInput.left, P_KeyInput.top - 20, _T("��Կ:"));
	rectangle(P_KeyInput.left, P_KeyInput.top, P_KeyInput.right, P_KeyInput.bottom);
	outtextxy(P_KeyInput.left + 10, P_KeyInput.top + 10, inputtext.c_str());
	//���
	outtextxy(Result.left, Result.top - 20, _T("���:"));
	rectangle(Result.left, Result.top, Result.right, Result.bottom);
	outtextxy(Result.left + 10, Result.top + 10, resulttext.c_str());
	//����
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("����"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("ִ��"));
}
//���ܽ���
void drawEncrypt() {
	cleardevice();
	settextcolor(WHITE);
	//����
	outtextxy(e_Input.left, e_Input.top - 20, _T("����:"));
	rectangle(e_Input.left, e_Input.top, e_Input.right, e_Input.bottom);
	outtextxy(e_Input.left + 10, e_Input.top + 10, inputtext.c_str());
	//����
	outtextxy(e_Result.left, e_Result.top - 20, _T("���ܽ��:"));
	rectangle(e_Result.left, e_Result.top, e_Result.right, e_Result.bottom);
	outtextxy(e_Result.left + 10, e_Result.top + 10, resulttext.c_str());
	//����
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("����"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("����"));
}
//���ܽ���
void drawDecrypt() {
	cleardevice();
	settextcolor(WHITE);
	//����
	outtextxy(d_Input.left, d_Input.top - 20, _T("����:"));
	rectangle(d_Input.left, d_Input.top, d_Input.right, d_Input.bottom);
	outtextxy(d_Input.left + 10, d_Input.top + 10, inputtext.c_str());

	//����
	outtextxy(d_Result.left, d_Result.top - 20, _T("���ܽ��:"));
	rectangle(d_Result.left, d_Result.top, d_Result.right, d_Result.bottom);
	outtextxy(d_Result.left + 10, d_Result.top + 10, resulttext.c_str());
	//��ť
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("����"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("����"));
}
//��������¼�
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
			inputtext = encryptedResult; // �Զ������ܽ�������������
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
			c_state = LOGIN;  // ��������ǰ�ȵ���¼����
			inputtext.clear();
			resulttext.clear();
		}
		if (PtInRect(&mainDecryptBtn, { x, y })) {
			c_state = PRIVATE_KEY_CHECK;  // �������ǰ�ȵ�˽Կ��֤
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
				resulttext = "�������";
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
					inputtext = encryptedResult;  // �Զ������ܽ��
				}
				else {
					resulttext = "˽Կ��֤ʧ�ܣ�";
				}
			}
			catch (...) {
				resulttext = "��Ч��˽Կ��ʽ��";
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
			handle();//����
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
			handleEncryption();//����
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
			handleDecryption();//����

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
	outtextxy(50, 100, _T("���������Ա����:"));
	rectangle(50, 130, 590, 160);
	outtextxy(60, 140, inputtext.c_str());

	// ���ư�ť
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("����"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("ȷ��"));

	// ��ʾ������Ϣ
	outtextxy(50, 200, resulttext.c_str());
}
void drawcheck() {
	cleardevice();
	settextcolor(WHITE);
	outtextxy(50, 100, _T("������˽Կ���������֤:"));
	rectangle(50, 130, 590, 160);
	outtextxy(60, 140, inputtext.c_str());

	// ���ư�ť
	rectangle(backBtn.left, backBtn.top, backBtn.right, backBtn.bottom);
	outtextxy(backBtn.left + 34, backBtn.top + 12, _T("����"));
	rectangle(actionBtn.left, actionBtn.top, actionBtn.right, actionBtn.bottom);
	outtextxy(actionBtn.left + 34, actionBtn.top + 12, _T("ȷ��"));

	// ��ʾ������Ϣ
	outtextxy(50, 200, resulttext.c_str());
}
//���ܴ���
void handleDecryption() {
	try {
		if (inputtext.empty()) throw std::invalid_argument("����Ϊ��");
		vector<int1> ciphertext;
		istringstream iss(inputtext);
		string token;
		while (iss >> token) {
			try {
				ciphertext.push_back(stoull(token));
			}
			catch (...) {
				throw invalid_argument("��Ч�����ĸ�ʽ");
			}
		}
		string plaintext;
		for (auto ct : ciphertext) {
			int1 pt = modexp(ct, private_key, public_key.second);
			if (pt > 255) throw runtime_error("����ֵ����ASCII��Χ");
			plaintext += static_cast<unsigned char>(pt);
		}
		resulttext = plaintext;
	}
	catch (const exception& e) {
		resulttext = "����: " + string(e.what());
	}
}
// ���ܴ���
void handleEncryption() {
	if (inputtext.empty()) {
		resulttext = "����������!";
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
// handle����
void handle() {
	if (inputtext.empty()) {
		resulttext = "�����빫Կe!";
		return;
	}
	// ת���û������e
	int1 e;
	try {
		e = stoull(inputtext);
	}
	catch (...) {
		resulttext = "��Կ������������!";
		return;
	}
	// ��������
	int1 p = generate_prime(12);
	int1 q = generate_prime(12);
	// ���˻��Ƿ����
	if (p > UINT64_MAX / q) {
		resulttext = "�������������!";
		return;
	}
	int1 n = p * q;
	int1 phi_n = (p - 1) * (q - 1);

	// ��֤e����Ч��
	if (e <= 1 || e >= phi_n) {
		resulttext = "e��������1 < e < phi(n)";
		return;
	}
	// ���e��phi_n�Ƿ���
	int1 x, y;
	int1 g = e_gcd(e, phi_n, x, y);
	if (g != 1) {
		resulttext = "e������O(n)����";
		return;
	}
	// ����˽Կ
	int1 d;
	try {
		d = modinv(e, phi_n);
	}
	catch (const runtime_error&) {
		resulttext = "��Ч�Ĺ�Կ���޷�����˽Կ";
		return;
	}
	// ������Կ����ʾ���
	public_key = make_pair(e, n);
	private_key = d;
	resulttext = "���ɳɹ�! ����:(" + to_string(p) + "��" + to_string(q) + "),��Կ(e, n):(" +
		to_string(e) + "," + to_string(n) + ")��˽Կd:" + to_string(d);
}