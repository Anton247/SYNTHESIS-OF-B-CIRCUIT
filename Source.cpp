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

struct FD//Функциональная зависимость 
{
	vector<string> left;
	vector<string> right;
	FD(const vector<string>& _l, const vector<string>& _r) : left(_l), right(_r) {};
};

void printInputData(vector<string> &X, vector<FD> &F);
void printData(vector<FD>& F);
void ReadFile(string& name, vector<string>& X, vector<FD>& F);
void SX(const vector<string>& X, const vector<FD>& F, vector<string>& X_plus);
bool PRF(const FD& X_struct, const vector<FD>& F);
void NPOK(const vector<FD>& F, vector<FD>& G);
void LRED(const vector<FD>& F, vector<FD>& Fl);
void PRED(const vector<FD>& Fl, vector<FD>& G);
void LRED_for_key(const vector<FD>& F, vector<FD>& Fl);
void PRED_for_key(const vector<FD>& Fl, vector<FD>& G);
void synthesisOfBScheme(vector<FD> &F, vector<string> &X, vector<FD> Fred, vector<FD>& Key);
bool Sweep_Board(vector<string>& R, const vector<FD>& F, const vector<vector<string>>& Scheme);

int main()
{
	setlocale(0, "");
	string file("Input.txt");//файл для считывания данных
	vector<string> X; X;//множество атрибутов (первая строка в файле!)
	vector<FD> F;//множество функциональных зависимостей (F=пустое множесто обозначается как 0)
	vector<FD> G;//неизбыточное покрытие
	ReadFile(file, X, F);
	printInputData(X, F);

	NPOK(F, G); //построение неизбыточного покрытия
	cout << "\n1-я лабораторная\nНеизбыточное покрытие\n";
	printData(G);

	vector<FD> Fl;//левое редуцирование
	vector<FD> Fr;//правое редуцирование
	cout << endl;
	LRED(G, Fl);

	cout << "\n2-я лабораторная\n";
	cout << "Левое редуцирование\n";
	printData(Fl);

	PRED(Fl, Fr);
	cout << "\nПравое редуцирование";
	cout << endl;
	printData(Fr);

	vector<FD> Key_p = Fr;//"ключевые" атрибуты
	vector<FD> Key_l;//ключ левый
	vector<string> enemy;
	Key_p.push_back(FD(enemy, enemy));
	Key_p[Key_p.size() - 1].left = X;
	Key_p[Key_p.size() - 1].right.push_back("&");//Сивол которого нет в алфавите

	LRED_for_key(Key_p, Key_l);
	cout << "\nКлюч(слева): ";
	for (auto k : Key_l[Key_l.size() - 1].left)
		cout << k;
	cout << "\n";

	vector<FD> Key_r;//ключ правый
	Key_p.clear();
	Key_p = Fr;
	Key_p.push_back(FD(enemy, enemy));
	Key_p[Key_p.size() - 1].right.push_back("&");
	for (int i = 0; i < X.size(); i++) { //"разворачиваем" множество атрибутов, чтобы получить ключ справа
		Key_p[Key_p.size() - 1].left.push_back(X[X.size() - i - 1]);
	}
	LRED_for_key(Key_p, Key_r);
	sort(Key_r[Key_r.size() - 1].left.begin(), Key_r[Key_r.size() - 1].left.end());
	if (Key_r[Key_r.size() - 1].left != Key_l[Key_l.size() - 1].left) {
		cout << "Ключ(справа): ";
		for (auto k : Key_r[Key_r.size() - 1].left)
			cout << k;
		cout << "\n";
	}
	else
		cout << "Ключ справа такой же как и слева\n\n";

	//синтез бетта схемы
	cout << "4-я лабораторная\nСинтез B-схемы\n";
	synthesisOfBScheme(F, X, Fr, Key_l);
	cout << "\n\n";

	system("pause");
	return 0;
}

void printInputData(vector<string>& X, vector<FD>& F) {
	cout << "Введённые данные\n";
	cout << "R=";
	for (auto x : X)
		cout << x;
	cout << "\n";
	cout << "F={";
	for (int i = 0; i < F.size(); i++) {
		if (F[i].left.size() == 0 && F[i].right.size() == 0)
			cout << "0" << "}\n";
		else {
			if (F[i].left.size() == 1 && F[i].left[0] == "")
				cout << "0";
			else
				for (int j = 0; j < F[i].left.size(); j++)
					cout << F[i].left[j];
			cout << "->";
			if (F[i].right.size() == 1 && F[i].right[0] == "")
				cout << "0";
			else
				for (int j = 0; j < F[i].right.size(); j++)
					cout << F[i].right[j];
			if (i != F.size() - 1)
				cout << ", ";
			else
				cout << "}\n";
		}
	}
}

void printData(vector<FD>& F) {
	if (F.size() == 0)
		cout << "0\n";
	for (int i = 0; i < F.size(); i++) {
		if (F[i].left.size() == 0)
			cout << "0";
		else {
			for (int j = 0; j < F[i].left.size(); j++)
				cout << F[i].left[j];
		}
		cout << "->";
		if (F[i].right.size() == 0)
			cout << "0";
		else {
			for (int j = 0; j < F[i].right.size(); j++)
				cout << F[i].right[j];
		}
		cout << "\n";
	}
}

void ReadFile(string& name, vector<string>& X, vector<FD>& F) {
	set<string> tX;
	set<string> Atr;
	set<string> ZERO;
	ZERO.insert("0");
	ifstream In(name);
	if (!In) {
		cout << "Файл не найден!";
		exit(-1);
	}
	//читаем атрибуты
	char ch;
	string buff;

	getline(In, buff, '\n');
	for (int i = 0; i < buff.length(); i++) {
		if (buff[i] == ' ') {
			buff.erase(i, 1);
			i--;
		}
	}
	//атрибуты в buff
	//разбиваем buff на отдельные атрибуты
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
	//Формирование множества атрибутов
	X.assign(tX.begin(), tX.end());

	//начало чтения функциональных зависимостей
	while (!In.eof()) {

		buff.clear();
		getline(In, buff, '-');
		for (int i = 0; i < buff.length(); i++) {
			if (buff[i] == ' ') {
				buff.erase(i, 1);
				i--;
			}
		}
		//разбиваем buff на отдельные атрибуты (левая часть ФЗ)
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
			FLeft.insert(A);
			Atr.insert(A);
			A.clear();
			j++;
		}
		if (FLeft == ZERO)
			FLeft.clear();
		In.get(ch);
		buff.clear();
		getline(In, buff, '\n');
		for (int i = 0; i < buff.length(); i++) {
			if (buff[i] == ' ') {
				buff.erase(i, 1);
				i--;
			}
		}
		//разбиваем buff на отдельные атрибуты (правая часть ФЗ)
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
			FRight.insert(A);
			Atr.insert(A);
			A.clear();
			j++;
		}
		if (FRight == ZERO)
			FRight.clear();
		vector<string> left;
		vector<string> right;
		left.assign(FLeft.begin(), FLeft.end());
		right.assign(FRight.begin(), FRight.end());
		F.push_back(FD(left, right));
	}
	vector<string> AtrV;
	AtrV.assign(Atr.begin(), Atr.end());
	if (X.size() < AtrV.size()) {
		cout << "ОШИБКА!!!\nНе полный список атрибутов\nВ ФЗ используется необъявленный атрибут\n\n";
		cout << "Нажми 1 чтобы автоматически дополнить множество атрибутов\nНажми 0 чтобы выйти из программы\nВвод:";
		int q;
		cin >> q;
		if (q == 1)
			X = AtrV;
		else
			exit(-3);
	}
}

void SX(const vector<string>& X, const vector<FD>& F, vector<string>& X_plus) {
	vector<string> OLD;
	vector<string> NEW = X;
	vector<string> Q;
	vector<string> A;
	vector<string> B;
	vector<string> C;
	while (NEW != OLD){
		Q.clear();
		OLD = NEW;
		for (int i = 0; i < F.size(); i++){
			A = NEW;
			B = F[i].left;
			C = F[i].right;
			sort(A.begin(), A.end());
			sort(B.begin(), B.end());
			sort(C.begin(), C.end());
			if (includes(A.begin(), A.end(), B.begin(), B.end())){ //строка содержит
				Q.clear();
				set_union(A.begin(), A.end(), C.begin(), C.end(), back_inserter(Q));//объединение множеств
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
	if (includes(X_plus.begin(), X_plus.end(), X_struct.right.begin(), X_struct.right.end()))//строка содержит
		return true;
	else
		return false;
}

void NPOK(const vector<FD>& F, vector<FD>& G)
{
	G = F;
	vector<FD> G_minus;
	for (int i = 0; i < F.size(); i++){
		G_minus = G;
		int k = 0;
		while (true){
			if (k < G_minus.size()){
				vector<string> check;
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
	for (int i = 0; i < F.size(); i++) {
		for (int j = 0; j < F[i].left.size(); j++) {
			vector<string>::iterator n;
			vector<string> S;
			S = F[i].left;
			string A;
			A = F[i].left[j];
			n = find(S.begin(), S.end(), A);
			if (n != S.end())
				S.erase(n);
			vector<string> Av;
			Av.push_back(A);
			FD X(S, Av); //(X\A)->A
			if (PRF(X, Fl)) {
				n = find(Fl[i].left.begin(), Fl[i].left.end(), A);
				if (n != Fl[i].left.end())
					Fl[i].left.erase(n);
			}
		}
	}
}

void LRED_for_key(const vector<FD>& F, vector<FD>& Fl)
{
	Fl = F;
	for (int i = 0; i < F.size(); i++){
		for (int j = 0; j < Fl[i].left.size(); j++){
			vector<string>::iterator n;
			vector<string> S;
			S = Fl[i].left;
			string A;
			A = Fl[i].left[j];
			n = find(S.begin(), S.end(), A);
			if (n != S.end())
				S.erase(n);
			vector<string> Av;
			Av.push_back(A);
			FD X(S, Av); //(X\A)->A
			if (PRF(X, Fl)){
				n = find(Fl[i].left.begin(), Fl[i].left.end(), A);
				if (n != Fl[i].left.end()) {
					Fl[i].left.erase(n);
					j--;
				}
			}
		}
	}
}

void PRED(const vector<FD>& Fl, vector<FD>& G)
{
	G = Fl;
	vector<FD> G_minus = G;
	for (int i = 0; i < G.size(); i++){
		for (int j = 0; j < G[i].right.size(); j++){
			vector<string>::iterator n;
			vector<string> S;
			S = Fl[i].right;
			string A;
			A = Fl[i].right[j];
			n = find(S.begin(), S.end(), A);
			S.erase(n);

			G_minus = G;
			int k = 0;
			while (true){
				if (k < G_minus.size()){
					if (G[i].left == G_minus[k].left && G[i].right == G_minus[k].right)
						G_minus.erase(G_minus.begin() + k);
					else
						k++;
				}
				else
					break;
			}

			vector<string> Av;
			Av.push_back(A);
			FD X(Fl[i].left, Av); //(X->A)
			FD Xn(Fl[i].left, S); //X->Y\A
			k = 0;
			for (; k < G_minus.size(); k++)
				if (G_minus[k].left == Xn.left && G_minus[k].right == Xn.right)
					break;
			if (k == G_minus.size())
				G_minus.push_back(Xn);

			if (PRF(X, G_minus)) {
				n = find(G[i].right.begin(), G[i].right.end(), A);
				if (n != G[i].right.end()) {
					G[i].right.erase(n);
					j--;//чтобы после удаления не перескочить
				}
			}
		}
	}

	for (int i = 0; i < G.size(); i++) {
		if (G[i].right.size() == 0) {
			G.erase(G.begin() + i);
			i--;
		}
	}
}

void PRED_for_key(const vector<FD>& Fl, vector<FD>& G)
{
	G = Fl;
	vector<FD> G_minus = G;
	for (int i = 0; i < G.size(); i++){
		for (int j = 0; j < G[i].right.size(); j++){
			vector<string>::iterator n;
			vector<string> S;

			S = Fl[i].right;
			string A;
			A = Fl[i].right[j];
			n = find(S.begin(), S.end(), A);
			S.erase(n);

			G_minus = G;
			int k = 0;
			while (true){
				if (k < G_minus.size()){
					if (G[i].left == G_minus[k].left && G[i].right == G_minus[k].right)
						G_minus.erase(G_minus.begin() + k);
					else
						k++;
				}
				else
					break;
			}

			vector<string> Av;
			Av.push_back(A);
			FD X(G[i].left, Av); //(X->A)
			FD Xn(G[i].left, S); //X->Y\A
			k = 0;
			for (; k < G_minus.size(); k++)
				if (G_minus[k].left == Xn.left && G_minus[k].right == Xn.right)
					break;
			if (k == G_minus.size())
				G_minus.push_back(Xn);

			if (PRF(X, G_minus)){
				n = find(G[i].right.begin(), G[i].right.end(), A);
				G[i].right.erase(n);
				j--;//чтобы после удаления не перескочить
			}
		}
	}

	for (int i = 0; i < G.size(); i++) {
		vector<string>::iterator n;
		n = find(G[i].right.begin(), G[i].right.end(), "");
		G.erase(G.begin() + i);
		i--;
	}
}

void synthesisOfBScheme(vector<FD>& F, vector<string>& X, vector<FD> Fred, vector<FD>& Key) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (F[0].left.size() == 0 && F[0].right.size() == 0) {
		cout << "F=0\nro={";
		SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа
		for (auto x : X)
			cout << x;
		SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
		cout << "}\n";
		system("pause");
		return;
	}
	vector<string> check;
	//Fred - отредуцирован уже
	for (int i = 0; i < Fred.size(); i++) {
		check.clear();
		check = Fred[i].left;
		for (auto r : Fred[i].right)
			check.push_back(r);
		sort(check.begin(), check.end());
		auto last = unique(check.begin(), check.end());
		check.erase(last, check.end());
		sort(X.begin(), X.end());
		if (check == X) {
			cout << "ro={";
			SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа
			for (auto x : X)
				cout << x;
			SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
			cout << "}\n";
			system("pause");
			return;
		}
	}

	//поисr X->Y; X->P для создания X->YP
	for (int i = 0; Fred.size() != 0 && i < (Fred.size() - 1); i++) {
		for (int j = i + 1; j < Fred.size(); j++)
			if (Fred[i].left == Fred[j].left) {
				for (auto r : Fred[j].right)
					Fred[i].right.push_back(r);
				sort(Fred[i].right.begin(), Fred[i].right.end());
				auto last = unique(Fred[i].right.begin(), Fred[i].right.end());
				Fred[i].right.erase(last, Fred[i].right.end());
				Fred.erase(Fred.begin() + j);
				j--;
			}
	}

	vector<vector<string>> Ro(Fred.size());
	for (int i = 0; i < Fred.size(); i++) {
		Ro[i] = Fred[i].left;
		for (auto r : Fred[i].right)
			Ro[i].push_back(r);
		sort(Ro[i].begin(), Ro[i].end());
	}

	//поиск "вложенных" схем
	//необязательно его применять, но лучше сделать
	for (int i = 0; i < Ro.size() - 1; i++) {
		for (int j = i + 1; j < Ro.size(); j++) {
			sort(Ro[i].begin(), Ro[i].end());
			sort(Ro[j].begin(), Ro[j].end());
			if (includes(Ro[i].begin(), Ro[i].end(), Ro[j].begin(), Ro[j].end())) {
				Ro.erase(Ro.begin() + j);
				j--;
			}
		}
	}

	if (Sweep_Board(X, Fred, Ro)) {

		cout << "Ключ добавлять не нужно\n";
		cout << "ro= {";
		for (int i = 0; i < Ro.size(); i++)
		{
			cout << "R" << i << "(";
			for (int j = 0; j < Ro[i].size(); j++)
			{
				vector<string> S;
				S.push_back(Ro[i][j]);
				sort(Fred[i].left.begin(), Fred[i].left.end());
				sort(S.begin(), S.end());
				if (includes(Fred[i].left.begin(), Fred[i].left.end(), S.begin(), S.end()))
					SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа
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

	else {
		cout << "Ключ добавлять нужно\n";
		cout << "ro= {";
		int i;
		for (i = 0; i < Ro.size(); i++) {
			cout << "R" << i << "(";
			for (int j = 0; j < Ro[i].size(); j++) {
				vector<string> S;
				S.push_back(Ro[i][j]);
				sort(Fred[i].left.begin(), Fred[i].left.end());
				if (includes(Fred[i].left.begin(), Fred[i].left.end(), S.begin(), S.end()))
					SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа
				cout << Ro[i][j];
				SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
			}
			cout << "); ";
		}
		cout << "R" << i << "(";
		SetConsoleTextAttribute(consoleHandle, (WORD)((15 << 4) | 0));//выделение ключа
		for (auto k : Key[Key.size() - 1].left)
			cout << k;
		SetConsoleTextAttribute(consoleHandle, (WORD)((0 << 4) | 15));
		cout << ")";
		cout << "}\n";
	}
}

bool Sweep_Board(vector<string>& R, const vector<FD>& F, const vector<vector<string>>& Scheme)
{
	//R-атрибуты; F - функц.зависимости 
	vector<vector<int>> board;
	board.resize(Scheme.size());
	for (int i = 0; i < Scheme.size(); i++)
		board[i].resize(R.size());
	
	int N = 1;//a_j
	int m_N = -1;//b_j
	for (int i = 0; i < Scheme.size(); i++)
		for (int j = 0; j < R.size(); j++){
			vector<string> Rj;
			Rj.push_back(R[j]);
			vector<string> Schem = Scheme[i];
			sort(Schem.begin(), Schem.end());
			if (includes(Schem.begin(), Schem.end(), Rj.begin(), Rj.end()))
				board[i][j] = j + 1;
			else
				board[i][j] = m_N--;
		}
	while (true){
		vector<vector<int>> board_old = board;
		for (int i = 0; i < F.size(); i++){//перебираем ФЗ
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

					for (j = 0; j < F[i].right.size(); j++){
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

	for (int i = 0; i < Scheme.size(); i++){
		int j;
		for (j = 0; j < R.size(); j++){
			if (board[i][j] < 0)
				break;
		}
		if (j == R.size())
			return true;
	}
	return false;
}