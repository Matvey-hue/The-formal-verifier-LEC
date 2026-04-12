#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
using namespace std;
string ans = "";
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << "Выберите задачу:" << endl;
	cout << "1. Построение ДНФ функции F" << endl;
	cout << "2. Построение ДНФ расхождения F1 и F2" << endl;
	int K = 0;
	cout << "Выберите: ";
	cin >> K;
	cout << "Куда сохранить выражение [Полный адрес файла]: ";
	string inpfile = "";
	cin >> inpfile;
	while (K > 0) {
		cout << "Загрузите ещё " << K << " файлов:" << endl;
		int d = 0;
		while (d != 1 && d != 2) {
			cout << "1. *.aag" << endl;
			cout << "2. *.txt" << endl;
			cout << "Выберите: ";
			cin >> d;
			if (d == 1) {
				string outfile1 = "";
				cout << "Введите полный адрес файла: ";
				cin >> outfile1;
				ifstream lin(outfile1);
				int M = 0, I = 0, L = 0, O = 0, A = 0;
				lin >> M >> I >> L >> O >> A;
				int* E = new int[I];
				int* R = new int[O];
				if (L != 0) {
					// L != 0 => В схеме есть защёлки => ОТКАЗ
					cout << "Программа рассматривает только комбинационную логику. Такой файл не обработуется." << endl;
					break;
				}
				string* arr = new string[2 * M + 2];
				for (int i = 0; i < I; i++) {
					lin >> E[i];
					arr[E[i]] = "x";
					arr[E[i]] += to_string(E[i]);
					arr[E[i] + 1] = "!(x";
					arr[E[i] + 1] += to_string(E[i]);
					arr[E[i] + 1] += ")";
				}
				for (int i = 0; i < O; i++) {
					lin >> R[i];
				}
				int k = 0;
				cout << "Сколько выходов учесть в этом графе: ";
				cin >> k;
				int* ex = new int[k];
				for (int i = 0; i < k; i++) {
					// Ввод литералов выхода
					bool OK = false;
					while (OK == false) {
						cout << "Введите литерал выхода " << i << ": ";
						cin >> ex[i];
						for (int j = 0; j < O; j++) {
							if (R[j] == ex[i]) {
								OK = true;
							}
						}
						if (OK == false) {
							cout << "Ошибка. Литарал " << ex[i] << " не записан как выход" << endl;
						}
					}
				}
				// На выходе берём результат из ячеек arr[ex[0]] и arr[ex[1]]
				int** V = new int* [A];
				for (int i = 0; i < A; i++) {
					V[i] = new int[3];
					lin >> V[i][0] >> V[i][1] >> V[i][2];
				}
				while ((k == 2 && (arr[ex[1]][0] == 0 || arr[ex[0]][0] == 0)) || (k == 1 && arr[ex[0]][0] == 0)) {
					for (int i = 0; i < A; i++) {
						if (arr[V[i][1]][0] != 0 && arr[V[i][2]][0] != 0) {
							arr[V[i][0]] += arr[V[i][1]];
							arr[V[i][0]] += "&";
							arr[V[i][0]] += arr[V[i][2]];
							arr[V[i][0] + 1] += "!(";
							arr[V[i][0] + 1] += arr[V[i][1]];
							arr[V[i][0] + 1] += "&";
							arr[V[i][0] + 1] += arr[V[i][2]];
							arr[V[i][0] + 1] += ")";
						}
					}
				}
				if (k == 2) {
					if (ex[0] % 2 != 0) {
						ans += "!";
					}
					ans += "(";
					ans += arr[ex[0]];
					ans += ")^";
					if (ex[1] % 2 != 0) {
						ans += "!";
					}
					ans += "(";
					ans += arr[ex[1]];
					ans += ")";

				}
				else if (k == 1) {
					if (ex[0] % 2 != 0) {
						ans += "!";
					}
					ans += "(";
					ans += arr[ex[0]];
					ans += ")";
					if (K == 2) {
						ans += "^";
					}
				}
				lin.close();
				K -= k;
			}
			else if (d == 2) {
				string outfile1 = "";
				cout << "Введите полный адрес файла: ";
				cin >> outfile1;
				ifstream fin(outfile1);
				fin >> ans;
				fin.close();
				if (K == 2) {
					ans += "^";  // XOR
				}
				K--;
			}
		}
	}
	ofstream fout(inpfile);
	fout << ans << endl;
	fout.close();
	system("pause");
	return 0;
}