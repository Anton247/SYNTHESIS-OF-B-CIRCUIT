#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;

struct FD//Functional dependence
{
	string left;
	string right;
	FD(const string& _l, const string& _r) : left(_l), right(_r) {};
};

void ReadFile(string& name, string& X, vector<FD>& F);
void SX(const string& X, const vector<FD>& F, string& X_plus);
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
	string file("Input.txt");//файл для считывания данных
	string X;//множество атрибутов
	vector<FD> F;//множество функциональных зависимостей (F=пустое множесто обозначается как 0->0)
	vector<FD> G;//неизбыточное покрытие
	ReadFile(file, X, F);
	cout << "Введённые данные\n";
	cout << "R=" << X << endl;
	cout << "F={";
	for (int i = 0; i < F.size(); i++)
	{
		if (F[i].left == "" && F[i].right == "")
			cout << "0" << "}\n";
		else
		{
			if (F[i].left == "")
				cout << "0";
			else
				cout << F[i].left;
			cout << "->";
			if (F[i].right == "")
				cout << "0";
			else
				cout << F[i].right;
			if (i != F.size() - 1)
				cout << ", ";
			else
				cout << "}\n";
		}
	}
	NPOK(F, G);
	cout << "\n1-я лабораторная\nНеизбыточное покрытие\n";
	if (G.size() == 0)
		cout << "0\n";
	for (int i = 0; i < G.size(); i++)
	{
		if (G[i].left == "")
			cout << "0";
		else
			cout << G[i].left;
		cout << "->";
		if (G[i].right == "")
			cout << "0";
		else
			cout << G[i].right << endl;
	}
	vector<FD> Fl;//левое редуцирование
	vector<FD> Fr;//правое редуцирование
	cout << endl;
	LRED(G, Fl);

	cout << "\n2-я лабораторная\n";
	cout << "Левое редуцирование\n";
	if (Fl.size() == 0)
		cout << 0 << endl;
	for (int i = 0; i < Fl.size(); i++)
	{
		if (Fl[i].left == "")
			cout << "0";
		else
			cout << Fl[i].left;
		cout << "->";
		if (Fl[i].right == "")
			cout << "0";
		else
			cout << Fl[i].right << endl;
	}

	PRED(Fl, Fr);
	cout << "\nПравое редуцирование";
	cout << endl;
	if (Fr.size() == 0)
		cout << 0 << endl;
	for (int i = 0; i < Fr.size(); i++)
	{
		if (Fr[i].left == "")
			cout << "0";
		else
			cout << Fr[i].left;
		cout << "->";
		if (Fr[i].right == "")
			cout << "0";
		else
			cout << Fr[i].right << endl;
	}

	vector<FD> Key_p = Fr;//"ключевые" атрибуты
	vector<FD> Key_l;//ключ левый
	Key_p.push_back(FD("", ""));
	Key_p[Key_p.size() - 1].left = X;
	Key_p[Key_p.size() - 1].right = "&";//Сивол которого нет в алфавите (на лекциях N)

	LRED_for_key(Key_p, Key_l);
	cout << "\nКлюч(слева): " << Key_l[Key_l.size() - 1].left << endl;

	vector<FD> Key_r;//ключ правый
	Key_p.clear();
	Key_p = Fr;
	Key_p.push_back(FD("", ""));
	Key_p[Key_p.size() - 1].right = "&";
	for (int i = 0; i < X.size(); i++)//"разворачиваем" множество атрибутов, чтобы получить ключ справа
	{
		Key_p[Key_p.size() - 1].left.push_back(X[X.size() - i - 1]);
	}
	LRED_for_key(Key_p, Key_r);
	sort(Key_r[Key_r.size() - 1].left.begin(), Key_r[Key_r.size() - 1].left.end());
	if (Key_r[Key_r.size() - 1].left != Key_l[Key_l.size() - 1].left)
		cout << "Ключ(справа): " << Key_r[Key_r.size() - 1].left << endl << endl;
	else
		cout << "Ключ справа такой же\n\n";

	//синтез бетта схемы
	cout << "4-я лабораторная\nСинтез B-схемы\n";
	if (F[0].left == "" && F[0].right == "")
	{
		cout << "F=0\nro={";
		SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа красным цветом
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
			SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа красным цветом
			cout << X;
			SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
			cout << "}\n";
			system("pause");
			return 1;
		}
	}

	//поисr X->Y; X->P для создания X->YP
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

	//поиск "вложенных" схем
	//необязательно его применять
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

	if (Sweep_Board(X, Fr, Ro))
	{
		cout << "Ключ добавлять не нужно\n";
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
					SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа красным цветом
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
		cout << "Ключ добавлять нужно\n";
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
					SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа красным цветом
				cout << Ro[i][j];
				SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
			}
			cout << "); ";
		}
		cout << "R" << i << "(";
		SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа красным цветом
		cout << Key_l[Key_l.size() - 1].left;
		SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
		cout << ")";
		cout << "}\n";
	}
	cout << "\n\n";
	system("pause");
	return 0;
}

void ReadFile(string& name, string& X, vector<FD>& F)
{
	ifstream In(name);
	if (!In)
	{
		cout << "Файл Input.txt не найден!\n";
		system("pause");
		exit(9);
	}
	In >> X; //считываем множество атрибутов до \n
	X.shrink_to_fit();
	FD Func("", "");
	string input_1 = "";
	string input_2 = "";
	string A;
	char ch = 0;
	while (!In.eof())
	{
		input_1 = "";
		input_2 = "";
		ch = 0;
		while (true)
		{
			In >> ch;
			if (ch == '\n')
				break;
			if (ch == '\0')
				break;
			if (ch == '-')
				break;
			if (ch != '0')
				input_1 += ch;
			A += input_1;
			if (In.eof())
				break;
		}
		if (In.eof())
		{
			F.push_back(FD(input_1, input_2));
			break;
		}
		In >> ch;
		if (ch == '\n')
		{
			F.push_back(FD(input_1, input_2));
			break;
		}
		In >> input_2;
		if (input_2[0] == '\n')
			input_2.clear();
		if (input_2 == "0")
			input_2.clear();
		else
			A += input_2;
		F.push_back(FD(input_1, input_2));
	}
	for (int i = 0; i < F.size(); i++)
	{
		F[i].left.shrink_to_fit();
		F[i].right.shrink_to_fit();
	}
	F.shrink_to_fit();

	sort(A.begin(), A.end());
	auto last = unique(A.begin(), A.end());
	A.erase(last, A.end());
	sort(X.begin(), X.end());
	if (A.size() == 1)
		A.clear();
	if (!includes(X.begin(), X.end(), A.begin(), A.end()))
	{
		cout << "ОШИБКА!!!\nколичество атрибутов в функцинальных зависимостях больше,\nчем в алфавите(1-я строка файла)!\n";
		system("pause");
		exit(-2);
	}
}

void SX(const string& X, const vector<FD>& F, string& X_plus)
{
	string OLD = "";
	string NEW = X;
	string Q = "";
	string A;
	string B;
	string C;
	int k = 0;
	while (NEW != OLD || (NEW == "" && k < F.size()))
	{
		Q = "";
		OLD = NEW;
		k++;
		for (int i = 0; i < F.size(); i++)
		{
			A = NEW;
			B = F[i].left;
			C = F[i].right;
			sort(A.begin(), A.end());
			sort(B.begin(), B.end());
			sort(C.begin(), C.end());
			if (includes(A.begin(), A.end(), B.begin(), B.end()))//строка содержит
			{
				Q.clear();
				set_union(A.begin(), A.end(), C.begin(), C.end(), std::back_inserter(Q));//объединене множеств
				NEW = Q;
			}
		}
	}
	X_plus = NEW;
}

bool PRF(const FD& X_struct, const vector<FD>& F)
{
	string X_plus;
	string X = X_struct.left;
	SX(X, F, X_plus);
	sort(X_plus.begin(), X_plus.end());
	string X_S_R = X_struct.right;
	sort(X_S_R.begin(), X_S_R.end());
	if (includes(X_plus.begin(), X_plus.end(), X_S_R.begin(), X_S_R.end()))//строка содержит
		return true;
	else
		return false;
}

void NPOK(const vector<FD>& F, vector<FD>& G)
{
	G = F;
	vector<FD> G_minus = F;
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
				j--;//чтобы после удаления не перескочить
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
				j--;//чтобы после удаления не перескочить
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
	//R-атрибуты; F - функц.зависимости 
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
		for (int i = 0; i < F.size(); i++)//перебираем ФЗ
		{
			for (int k = 0; k < board.size() - 1; k++)//"верхняя строка"
				for (int n = k + 1; n < board.size(); n++)//"нижняя" строка
				{
					int j, q;
					for (j = 0; j < F[i].left.size(); j++)//перебираем атрибуты ФЗ
					{
						for (q = 0; q < board.size(); q++)//ищим нужный атрибут из X
							if (F[i].left[j] == R[q])//Xi равен столбцу
								if (board[k][q] == board[n][q])//строки равны
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
						for (q = 0; q < R.size(); q++)//ищим нужный атрибут из X
							if (F[i].right[j] == R[q])//Xi равен столюцу
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