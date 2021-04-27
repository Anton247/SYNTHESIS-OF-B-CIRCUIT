#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
#include <set>
using namespace std;

struct FD//�������������� ����������� 
{
	vector<string> left;
	vector<string> right;
	FD(const vector<string>& _l, const vector<string>& _r) : left(_l), right(_r) {};
};

void ReadFile(string& name, vector<string>& X, vector<FD>& F);
void SX(const vector<string>& X, const vector<FD>& F, vector<string>& X_plus);
bool PRF(const FD& X_struct, const vector<FD>& F);
void NPOK(const vector<FD>& F, vector<FD>& G);
void LRED(const vector<FD>& F, vector<FD>& Fl);
void PRED(const vector<FD>& Fl, vector<FD>& G);
void LRED_for_key(const vector<FD>& F, vector<FD>& Fl);
void PRED_for_key(const vector<FD>& Fl, vector<FD>& G);
bool Sweep_Board(string& R, const vector<FD>& F, const vector<string>& Scheme);

int main()
{
	setlocale(0, "");
	HANDLE consoleHandle = 0;
	consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	string file("Input.txt");//���� ��� ���������� ������
	vector<string> X; X;//��������� ��������� (������ ������ � �����!)
	vector<FD> F;//��������� �������������� ������������ (F=������ �������� ������������ ��� 0)
	vector<FD> G;//������������ ��������
	ReadFile(file, X, F);
	cout << "�������� ������\n";
	cout << "R=";
	for (auto x : X)
		cout << x;
	cout << "\n";
	cout << "F={";
	for (int i = 0; i < F.size(); i++)
	{
		if (F[i].left[0] == "" && F[i].right[0] == "")
			cout << "0" << "}\n";
		else
		{
			if (F[i].left[0] == "")
				cout << "0";
			else
				for (int j = 0; j < F[i].left.size(); j++)
					cout << F[i].left[j];
			cout << "->";
			if (F[i].right[0] == "")
				cout << "0";
			else
				for (int j = 0; j < F[i].left.size(); j++)
					cout << F[i].left[j];
			if (i != F.size() - 1)
				cout << ", ";
			else
				cout << "}\n";
		}
	}
	
	NPOK(F, G); //���������� ������������� ��������
	cout << "\n1-� ������������\n������������ ��������\n";
	if (G.size() == 0)
		cout << "0\n";
	for (int i = 0; i < G.size(); i++)
	{
		if (G[i].left[0] == "")
			cout << "0";
		else {
			for (int j = 0; j < G[i].left.size(); j++)
				cout << G[i].left[j];
		}
		cout << "->";
		if (G[i].right[0] == "")
			cout << "0";
		else {
			for (int j = 0; j < G[i].right.size(); j++)
				cout << G[i].right[j];
		}
		cout << "\n";
	}

	vector<FD> Fl;//����� �������������
	vector<FD> Fr;//������ �������������
	cout << endl;
	LRED(G, Fl);

	cout << "\n2-� ������������\n";
	cout << "����� �������������\n";
	if (Fl.size() == 0)
		cout << 0 << endl;
	for (int i = 0; i < Fl.size(); i++)
	{
		if (Fl[i].left[0] == "")
			cout << "0";
		else
			for (int j = 0; j < Fl[i].left.size(); j++)
				cout << Fl[i].left[j];
		cout << "->";
		if (Fl[i].right[0] == "")
			cout << "0";
		else
			for (int j = 0; j < Fl[i].left.size(); j++)
				cout << Fl[i].right[j];
	}

	PRED(Fl, Fr);
	cout << "\n������ �������������";
	cout << endl;
	if (Fr.size() == 0)
		cout << 0 << endl;
	for (int i = 0; i < Fr.size(); i++)
	{
		if (Fr[i].left[0] == "")
			cout << "0";
		else
			for (int j = 0; j < Fr[i].left.size(); j++)
				cout << Fr[i].left[j];
		cout << "->";
		if (Fr[i].right[0] == "")
			cout << "0";
		else
			for (int j = 0; j < Fr[i].right.size(); j++)
				cout << Fr[i].right[j];
	}
	
	/*
	vector<FD> Key_p = Fr;//"��������" ��������
	vector<FD> Key_l;//���� �����
	Key_p.push_back(FD("", ""));
	Key_p[Key_p.size() - 1].left = X;
	Key_p[Key_p.size() - 1].right = "&";//����� �������� ��� � ��������

	LRED_for_key(Key_p, Key_l);
	cout << "\n����(�����): " << Key_l[Key_l.size() - 1].left << endl;

	vector<FD> Key_r;//���� ������
	Key_p.clear();
	Key_p = Fr;
	Key_p.push_back(FD("", ""));
	Key_p[Key_p.size() - 1].right = "&";
	for (int i = 0; i < X.size(); i++)//"�������������" ��������� ���������, ����� �������� ���� ������
	{
		Key_p[Key_p.size() - 1].left.push_back(X[X.size() - i - 1]);
	}
	LRED_for_key(Key_p, Key_r);
	sort(Key_r[Key_r.size() - 1].left.begin(), Key_r[Key_r.size() - 1].left.end());
	if (Key_r[Key_r.size() - 1].left != Key_l[Key_l.size() - 1].left)
		cout << "����(������): " << Key_r[Key_r.size() - 1].left << endl << endl;
	else
		cout << "���� ������ ����� �� ��� � �����\n\n";

	//������ ����� �����
	cout << "4-� ������������\n������ B-�����\n";
	if (F[0].left == "" && F[0].right == "")
	{
		cout << "F=0\nro={";
		SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//��������� �����
		cout << X;
		SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
		cout << "}\n";
		system("pause");
		return 4;
	}
	string check;
	for (int i = 0; i < Fr.size(); i++)
	{
		check.clear();
		check += Fr[i].left;
		check += Fl[i].right;
		sort(check.begin(), check.end());
		auto last = unique(check.begin(), check.end());
		check.erase(last, check.end());
		sort(X.begin(), X.end());
		if (check == X)
		{
			cout << "ro={";
			SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//��������� �����
			cout << X;
			SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
			cout << "}\n";
			system("pause");
			return 1;
		}
	}

	//����r X->Y; X->P ��� �������� X->YP
	for (int i = 0; Fr.size() != 0 && i < (Fr.size() - 1); i++)
	{
		for (int j = i + 1; j < Fr.size(); j++)
			if (Fr[i].left == Fr[j].left)
			{
				Fr[i].right += Fr[j].right;
				sort(Fr[i].right.begin(), Fr[i].right.end());
				auto last = unique(Fr[i].right.begin(), Fr[i].right.end());
				Fr[i].right.erase(last, Fr[i].right.end());
				Fr.erase(Fr.begin() + j);
				j--;
			}
	}

	vector<string> Ro(Fr.size(), "");
	for (int i = 0; i < Fr.size(); i++)
	{
		Ro[i] = Fr[i].left;
		Ro[i] += Fr[i].right;
		sort(Ro[i].begin(), Ro[i].end());
	}

	//����� "���������" ����
	//������������� ��� ���������
	/*
	for (int i = 0; i < Ro.size() - 1; i++)
	{
	for (int j = i + 1; j<Ro.size(); j++)
	{
	sort(Ro[i].begin(),Ro[i].end());
	sort(Ro[j].begin(), Ro[j].end());
	if (includes(Ro[i].begin(), Ro[i].end(), Ro[j].begin(), Ro[j].end()))
	{
	Ro.erase(Ro.begin() + j);
	j--;
	}
	}
	}
	*/

	/*
	if (Sweep_Board(X, Fr, Ro))
	{
		cout << "���� ��������� �� �����\n";
		cout << "ro= {";
		for (int i = 0; i < Ro.size(); i++)
		{
			cout << "R" << i << "(";
			for (int j = 0; j < Ro[i].size(); j++)
			{
				string S;
				S.push_back(Ro[i][j]);
				sort(Fr[i].left.begin(), Fr[i].left.end());
				if (includes(Fr[i].left.begin(), Fr[i].left.end(), S.begin(), S.end()))
					SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//��������� �����
				cout << Ro[i][j];
				SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
			}
			if (i != Ro.size() - 1)
				cout << "); ";
			else
				cout << ")";
		}
		cout << "}\n";
	}
	else
	{
		cout << "���� ��������� �����\n";
		cout << "ro= {";
		int i;
		for (i = 0; i < Ro.size(); i++)
		{
			cout << "R" << i << "(";
			for (int j = 0; j < Ro[i].size(); j++)
			{
				string S;
				S.push_back(Ro[i][j]);
				sort(Fr[i].left.begin(), Fr[i].left.end());
				if (includes(Fr[i].left.begin(), Fr[i].left.end(), S.begin(), S.end()))
					SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//��������� �����
				cout << Ro[i][j];
				SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
			}
			cout << "); ";
		}
		cout << "R" << i << "(";
		SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//��������� �����
		cout << Key_l[Key_l.size() - 1].left;
		SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
		cout << ")";
		cout << "}\n";
	}
	cout << "\n\n";
	*/
	system("pause");
	return 0;
}

void ReadFile(string& name, vector<string>& X, vector<FD>& F) {
	set<string> tX;
	tX.insert("");
	ifstream In(name);
	if (!In) {
		cout << "���� �� ������!";
		exit(-1);
	}
	//������ ��������
	char ch;
	string buff;
	getline(In, buff, '\n');
	for (int i = 0; i < buff.length(); i++) {
		if (buff[i] == ' ') {
			buff.erase(i, 1);
			i--;
		}
	}
	//�������� � buff
	//��������� buff �� ��������� ��������
	string A;
	int len = buff.length();
	int j = 0;
	while (j < len) {
		A += buff[j];
		while (j < len - 1)
			if (buff[j + 1] >= '0' && buff[j + 1] <= '9') {
				j++;
				A += buff[j];
			}
			else {
				break;
			}

		tX.insert(A);
		A.clear();
		j++;
	}
	X.assign(tX.begin(), tX.end());
	int currentF = 0;
	while (!In.eof()) {

		buff.clear();
		getline(In, buff, '-');
		for (int i = 0; i < buff.length(); i++) {
			if (buff[i] == ' ') {
				buff.erase(i, 1);
				i--;
			}
		}
		//��������� buff �� ��������� ��������
		string A;
		set<string> FLeft;
		int len = buff.length();
		int j = 0;
		while (j < len) {
			A += buff[j];
			while (j < len - 1)
				if (buff[j + 1] >= '0' && buff[j + 1] <= '9') {
					j++;
					A += buff[j];
				}
				else {
					break;
				}
			if (A == "0")
				A.clear();
			FLeft.insert(A);
			FLeft.insert("");
			A.clear();
			j++;
		}
		In.get(ch);
		buff.clear();
		getline(In, buff, '\n');
		for (int i = 0; i < buff.length(); i++) {
			if (buff[i] == ' ') {
				buff.erase(i, 1);
				i--;
			}
		}
		//��������� buff �� ��������� ��������
		A.clear();
		set<string> FRight;
		len = buff.length();
		j = 0;
		while (j < len) {
			A += buff[j];
			while (j < len - 1)
				if (buff[j + 1] >= '0' && buff[j + 1] <= '9') {
					j++;
					A += buff[j];
				}
				else {
					break;
				}
			if (A == "0")
				A.clear();
			FRight.insert(A);
			FRight.insert("");
			A.clear();
			j++;
		}
		vector<string> left;
		vector<string> right;
		left.assign(FLeft.begin(), FLeft.end());
		right.assign(FRight.begin(), FRight.end());
		F.push_back(FD(left, right));
	}
}

void SX(const vector<string>& X, const vector<FD>& F, vector<string>& X_plus) {
	vector<string> OLD;
	vector<string> NEW = X;
	vector<string> Q;
	vector<string> A;
	vector<string> B;
	vector<string> C;
	while (NEW != OLD)
	{
		Q.clear();
		OLD = NEW;
		for (int i = 0; i < F.size(); i++)
		{
			A = NEW;
			B = F[i].left;
			C = F[i].right;
			sort(A.begin(), A.end());
			sort(B.begin(), B.end());
			sort(C.begin(), C.end());
			if (includes(A.begin(), A.end(), B.begin(), B.end()))//������ ��������
			{
				Q.clear();
				set_union(A.begin(), A.end(), C.begin(), C.end(), back_inserter(Q));//����������� ��������
				NEW = Q;
			}
		}
	}
	X_plus = NEW;
	X_plus.push_back("");
	set<string> XP(X_plus.begin(), X_plus.end());
	X_plus.clear();
	X_plus.assign(XP.begin(), XP.end());
}

bool PRF(const FD& X_struct, const vector<FD>& F)
{
	vector<string> X_plus;
	vector<string> X = X_struct.left;
	SX(X, F, X_plus);
	if (includes(X_plus.begin(), X_plus.end(), X_struct.right.begin(), X_struct.right.end()))//������ ��������
		return true;
	else
		return false;
}

void NPOK(const vector<FD>& F, vector<FD>& G)
{
	G = F;
	vector<FD> G_minus;
	for (int i = 0; i < F.size(); i++)
	{
		G_minus = G;
		int k = 0;
		while (true)
		{
			if (k < G_minus.size())
			{
				if (F[i].left == G_minus[k].left && F[i].right == G_minus[k].right)
					G_minus.erase(G_minus.begin() + k);
				else
					k++;
			}
			else
				break;
		}
		if (PRF(F[i], G_minus))
			G = G_minus;
	}
}


void LRED(const vector<FD>& F, vector<FD>& Fl)
{
	Fl = F;
	for (int i = 0; i < F.size(); i++)
	{
		for (int j = 0; j < F[i].left.size(); j++)
		{
			string::size_type n;
			string S = "";
			S = F[i].left;
			string A;
			A = F[i].left[j];
			n = S.find(A);
			S.erase(S.begin() + n);
			FD X(S, A); //(X\A)->A
			if (PRF(X, Fl))
			{
				n = Fl[i].left.find(A);
				Fl[i].left.erase(Fl[i].left.begin() + n);
			}
		}
	}
}

void LRED_for_key(const vector<FD>& F, vector<FD>& Fl)
{
	Fl = F;
	for (int i = 0; i < F.size(); i++)
	{
		for (int j = 0; j < Fl[i].left.size(); j++)
		{
			string::size_type n;
			string S = "";
			S = Fl[i].left;
			string A;
			A = Fl[i].left[j];
			n = S.find(A);
			S.erase(S.begin() + n);
			FD X(S, A); //(X\A)->A
			if (PRF(X, Fl))
			{
				n = Fl[i].left.find(A);
				Fl[i].left.erase(Fl[i].left.begin() + n);
				j--;
			}
		}
	}
}

void PRED(const vector<FD>& Fl, vector<FD>& G)
{
	G = Fl;
	vector<FD> G_minus = G;
	for (int i = 0; i < G.size(); i++)
	{
		for (int j = 0; j < G[i].right.size(); j++)
		{
			string::size_type n;
			string S = "";
			S = Fl[i].right;
			string A;
			A = Fl[i].right[j];
			n = S.find(A);
			S.erase(S.begin() + n);

			G_minus = G;
			int k = 0;
			while (true)
			{
				if (k < G_minus.size())
				{
					if (G[i].left == G_minus[k].left && G[i].right == G_minus[k].right)
						G_minus.erase(G_minus.begin() + k);
					else
						k++;
				}
				else
					break;
			}

			FD X(Fl[i].left, A); //(X->A)
			FD Xn(Fl[i].left, S); //X->Y\A
			k = 0;
			for (; k < G_minus.size(); k++)
				if (G_minus[k].left == Xn.left && G_minus[k].right == Xn.right)
					break;
			if (k == G_minus.size())
				G_minus.push_back(Xn);

			if (PRF(X, G_minus))
			{
				n = G[i].right.find(A);
				if (n == string::npos)
					continue;
				G[i].right.erase(G[i].right.begin() + n);
				j--;//����� ����� �������� �� �����������
			}
		}
	}
	for (int i = 0; i < G.size(); i++)
		if (G[i].right == "")
		{
			G.erase(G.begin() + i);
			i--;
		}
}

void PRED_for_key(const vector<FD>& Fl, vector<FD>& G)
{
	G = Fl;
	vector<FD> G_minus = G;
	for (int i = 0; i < G.size(); i++)
	{
		for (int j = 0; j < G[i].right.size(); j++)
		{
			string::size_type n;
			string S = "";
			S = G[i].right;
			string A;
			A = G[i].right[j];
			n = S.find(A);
			S.erase(S.begin() + n);

			G_minus = G;
			int k = 0;
			while (true)
			{
				if (k < G_minus.size())
				{
					if (G[i].left == G_minus[k].left && G[i].right == G_minus[k].right)
						G_minus.erase(G_minus.begin() + k);
					else
						k++;
				}
				else
					break;
			}

			FD X(G[i].left, A); //(X->A)
			FD Xn(G[i].left, S); //X->Y\A
			k = 0;
			for (; k < G_minus.size(); k++)
				if (G_minus[k].left == Xn.left && G_minus[k].right == Xn.right)
					break;
			if (k == G_minus.size())
				G_minus.push_back(Xn);

			if (PRF(X, G_minus))
			{
				n = G[i].right.find(A);
				G[i].right.erase(G[i].right.begin() + n);
				j--;//����� ����� �������� �� �����������
			}
		}
	}
	for (int i = 0; i < G.size(); i++)
		if (G[i].right == "")
		{
			G.erase(G.begin() + i);
			i--;
		}
}

bool Sweep_Board(string& R, const vector<FD>& F, const vector<string>& Scheme)
{
	//R-��������; F - �����.����������� 
	vector<vector<int>> board;
	board.resize(Scheme.size());
	for (int i = 0; i < Scheme.size(); i++)
	{
		board[i].resize(R.size());
	}
	int N = 1;//a_j
	int m_N = -1;//b_j
	for (int i = 0; i < Scheme.size(); i++)
		for (int j = 0; j < R.size(); j++)
		{
			string Rj;
			Rj += R[j];
			string Schem = Scheme[i];
			sort(Schem.begin(), Schem.end());
			if (includes(Schem.begin(), Schem.end(), Rj.begin(), Rj.end()))
				board[i][j] = j + 1;
			else
				board[i][j] = m_N--;
		}
	while (true)
	{
		vector<vector<int>> board_old = board;
		for (int i = 0; i < F.size(); i++)//���������� ��
		{
			for (int k = 0; k < board.size() - 1; k++)//"������� ������"
				for (int n = k + 1; n < board.size(); n++)//"������" ������
				{
					int j, q;
					for (j = 0; j < F[i].left.size(); j++)//���������� �������� ��
					{
						for (q = 0; q < board.size(); q++)//���� ������ ������� �� X
							if (F[i].left[j] == R[q])//Xi ����� �������
								if (board[k][q] == board[n][q])//������ �����
									continue;
								else
									break;
						if (q != board.size())
							break;
					}
					if (j != F[i].left.size())
						continue;

					for (j = 0; j < F[i].right.size(); j++)
					{
						for (q = 0; q < R.size(); q++)//���� ������ ������� �� X
							if (F[i].right[j] == R[q])//Xi ����� �������
								if (board[k][q] > 0)
									board[n][q] = board[k][q];
								else if (board[n][q] > 0)
									board[k][q] = board[n][q];
								else
									board[n][q] = board[k][q];
					}
				}
		}
		if (board_old == board)
			break;
	}

	for (int i = 0; i < Scheme.size(); i++)
	{
		int j;
		for (j = 0; j < R.size(); j++)
		{
			if (board[i][j] < 0)
				break;
		}
		if (j == R.size())
			return true;
	}
	return false;
}