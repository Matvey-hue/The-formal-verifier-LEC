#include <iostream>
#include <Windows.h>
using namespace std;
const long long fizic = 16777216;  // Константа, физический размер длины строки (максимально обрабатываемый, включая промежуточные вычисления)
char str[fizic];  // Выделение статического массива в памяти под хранение (и обработку) строки - исходного логического выражения
long long logic = 0;  // Логический размер (сколько байт в каждый момент времени занято для хранения строки)
int stack[fizic];  // Стек для определения глубины выражения (в скобках)
int upp = fizic - 1;  // Верхняя граница стека stack на каждый момент (сколько осталось элементов до полного заполнения)
int stack2[fizic];  // Массив порядка операций (строгая проекция с str)
char str2[fizic];  // Дублирующая строка для промежуточных вычислений
const int maxterm = 8192;
const int maxper = 256;
short BASE[maxterm][maxper * 2];  // Основной массив: таблица, обрабатываемая по принципу MMU, содержит логические таблицы на всех промежуточных действиях
short KON[524290][101];  // Массив хранит границы результата операции в таблице BASE - по принципу MMU
char SOO[maxper][10];  // Соотношение ID переменной и её названия (индекс - значение: не более 10 символов в названии переменной)
int IDD = 0;  // Размер массива SOO (количество строк)
int ID = 0;  // Размер массива BASE (количество э. к. - элементарных конъюнкций)
short stack3[maxterm];
int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	// Пункт 1 - СЧИТЫВАНИЕ И ПРОВЕРКА НА КОРРЕКТНОСТЬ
	// Считываем строку в память. Это единственное место, где программе подаются входные данные
	cin >> str;
	// Подсчитываем количество символов в строке бинарным поиском, O(logN) - для определения логического размера исходной строки, N - физический размер строки, по умолчанию - 2**27 (изменяется до компиляции)
	long long min = 1, max = fizic - 1, med = fizic / 2;  
	while (!(med >= 1 && str[med] == 0 && str[med - 1] != 0) || (med == 0 && str[med] == 0)) {
		if (str[med] == 0) { max = (max + min) / 2; med = (max + min) / 2; }
		else { min = (max + min) / 2; med = (max + min) / 2; }
	}
	logic = med;
	// Проверка корректности выражения, O(N), N - логический размер исходной строки
	bool kontr = true, ekv = false;
	for (int i = 0; i < logic; i++) {
		// Проверка на соблюдение алфавита: допускаются только символы &, \, +, !, x, y, z, w, a, b, c, d, (, ), 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
		if (str[i] != '&' && str[i] != '|' && str[i] != '^' && str[i] != '!' && str[i] != 'x' && str[i] != 'y' && str[i] != 'z' && str[i] != 'w' && str[i] != 'a' && str[i] != 'b' && str[i] != 'c' && str[i] != 'd' && str[i] != '(' && str[i] != ')' && str[i] != '0' && str[i] != '1' && str[i] != '2' && str[i] != '3' && str[i] != '4' && str[i] != '5' && str[i] != '6' && str[i] != '7' && str[i] != '8' && str[i] != '9') { kontr = false; break; }
		// Проверка на последовательность символов (после таких-то символов никогда не могут идти такие-то)
		if (str[i] == '!' && (str[i + 1] != '(' || str[i + 1] == 0)) { kontr = false; break; }
		if (str[i] == '(' && (str[i + 1] == 0 || str[i + 1] == '|' || str[i + 1] == '&' || str[i + 1] == '^' || str[i + 1] == '0' || str[i + 1] == '1' || str[i + 1] == '2' || str[i + 1] == '3' || str[i + 1] == '4' || str[i + 1] == '5' || str[i + 1] == '6' || str[i + 1] == '7' || str[i + 1] == '8' || str[i + 1] == '9')) { kontr = false; break; }
		if ((str[i] == '&' || str[i] == '|' || str[i] == '^') && (i == 0 || str[i + 1] == 0 || str[i + 1] == '0' || str[i + 1] == '1' || str[i + 1] == '2' || str[i + 1] == '3' || str[i + 1] == '4' || str[i + 1] == '5' || str[i + 1] == '6' || str[i + 1] == '7' || str[i + 1] == '8' || str[i + 1] == '9' || str[i + 1] == '&' || str[i + 1] == '|' || str[i + 1] == '^' || str[i + 1] == ')')) { kontr = false; break; }
		if ((str[i] == 'x' || str[i] == 'y' || str[i] == 'z' || str[i] == 'w' || str[i] == 'a' || str[i] == 'b' || str[i] == 'c' || str[i] == 'd') && (str[i + 1] == '(' || str[i + 1] == '!' || str[i + 1] == 'x' || str[i + 1] == 'y' || str[i + 1] == 'z' || str[i + 1] == 'w' || str[i + 1] == 'a' || str[i + 1] == 'b' || str[i + 1] == 'c' || str[i + 1] == 'd')) { kontr = false; break; }
		if (str[i] == '0' || str[i] == '1' && (i == 0 || (i >= 1 && str[i - 1] != 'x' && str[i - 1] != 'y' && str[i - 1] != 'z' && str[i - 1] != 'w' && str[i - 1] != 'a' && str[i - 1] != 'b' && str[i - 1] != 'c' && str[i - 1] != 'd'))) {
			kontr = true;
		}
		else if ((str[i] == '0' || str[i] == '1' || str[i] == '2' || str[i] == '3' || str[i] == '4' || str[i] == '5' || str[i] == '6' || str[i] == '7' || str[i] == '8' || str[i] == '9') && (i == 0 || str[i + 1] == 'x' || str[i + 1] == 'y' || str[i + 1] == 'z' || str[i + 1] == 'w' || str[i + 1] == 'a' || str[i + 1] == 'b' || str[i + 1] == 'c' || str[i + 1] == 'd' || str[i + 1] == '(' || str[i + 1] == '!')) { kontr = false; break; }
		// Проверка корректности скобочного выражения (реализовано через структуру данных: стек)
		if (str[i] == '(' && (i == 0 || str[i - 1] != '!')) { stack[upp] = 1; upp--; }
		else if (i >= 1 && str[i] == '(' and str[i - 1] == '!') { stack[upp] = 2; upp--; }
		else if (str[i] == ')') { 
			if (stack[upp + 1] != 0) { stack[upp + 1] = 0; upp++; }
			else {kontr = false; break;}
		}
		// Проверка на корректность применения операции ^: НЕ БОЛЕЕ одной операции в выражении и НЕ ВНУТРИ скобок
		if (str[i] == '^') {
			if (ekv == true) { kontr = false; break; }
			ekv = true;
			if (str[i] == '^' && upp != fizic - 1) { kontr = false; break; }
		}
	}
	if (upp != fizic - 1 || kontr == false) cout << "НЕКОРРЕКТНОЕ ВЫРАЖЕНИЕ" << endl;
	else {
		// Пункт 2 - ПРЕОБРАЗОВАНИЕ ИСХОДНОЙ СТРОКИ И СВЕДЕНИЕ К БАЗИСУ {F1&F2, F1|F2, !P}		
		for (int i = 0; i < logic; i++) {	
			// Сводим операцию ^ к базису {F1&F2, F1|F2, !F}
			if (str[i] == '^') {
				int j = 0;
				str2[j] = '('; j++;
				str2[j] = '!'; j++;
				str2[j] = '('; j++;
				for (int k = 0; k < i; j++, k++) {
					str2[j] = str[k];
				}
				str2[j] = ')'; j++;
				str2[j] = '&'; j++;
				str2[j] = '('; j++;
				for (int k = i + 1; k < logic; j++, k++) {
					str2[j] = str[k];
				}
				str2[j] = ')'; j++;
				str2[j] = ')'; j++;
				str2[j] = '|'; j++;
				str2[j] = '('; j++;
				str2[j] = '('; j++;
				for (int k = 0; k < i; j++, k++) {
					str2[j] = str[k];
				}
				str2[j] = ')'; j++;
				str2[j] = '&'; j++;
				str2[j] = '!'; j++;
				str2[j] = '('; j++;
				for (int k = i + 1; k < logic; j++, k++) {
					str2[j] = str[k];
				}
				str2[j] = ')'; j++;
				str2[j] = ')'; j++;
				for (int k = 0; k < logic; k++) str[k] = 0;
				for (int k = 0; k < j; k++) {
					str[k] = str2[k];
					str2[k] = 0;
				}
				logic = j;
				break;
			}
		}
		// Выполняем упрощение по законам де Морагна, цель - свести к базису {F1&F2, F1\F2, !P}
		// Избавляемся от дублирующего НЕ и дублирующих скобок
		bool kontrol = true;
		do {
			kontrol = true;
			int stack_len = 0;
			for (int i = 0; i < logic; i++) {
				if (str[i] == '(') { stack[stack_len] = i; stack_len++; }
				else if (str[i] == ')') { stack[stack_len] = fizic + i; stack_len++; }
			}
			for (int i = 0; i < stack_len; i++) {
				int bz = -1;  // Ближайшая закрытая скобка справа
				int bo = -1;  // Ближайшая открытая скобка слева
				int bom1 = -1;  // Ближайшая открытая к ближайшей открытой скобке слева
				for (int j = i + 1; j < stack_len; j++) {
					if (stack[j] != -1 && stack[j] >= fizic) {
						bz = j;
						break;
					}
				}
				for (int j = i - 1; j >= 0; j--) {
					if (stack[j] != -1 && bo == -1 && stack[j] < fizic) {
						bo = j;
					}
					if (stack[j] != -1 && bo != -1 && bo != j && stack[j] < fizic) {
						bom1 = j;
						break;
					}
				}
				if (stack[i] >= fizic) {
					if (bz != -1 && bom1 != -1 && bo != -1 && stack[bz] == stack[i] + 1 && stack[bo] == stack[bom1] + 2 && bom1 >= 1 && !(str[stack[bom1]] == '(' && str[stack[bo] - 1] == '!' && str[stack[bo]] == '(' && str[stack[bom1] - 1] != '!')) {
						// Очищаем str от дублирующего НЕ
						str[stack[i] - fizic] = 0;
						str[stack[bo] - 1] = 0;
						str[stack[bom1]] = 0;						
						str[stack[bom1] - 1] = 0;								
					}
					else if (bz != -1 && bom1 != -1 && bo != -1 && stack[bz] == stack[i] + 1 && stack[bo] == stack[bom1] + 1) {
						// Очищаем str от дублирования обычных скобок
						str[stack[i] - fizic] = 0;
						str[stack[bo]] = 0;
					}
					stack[i] = -1;
					stack[bo] = -1;
				}
				bz = -1;
				bo = -1;
				bom1 = -1;
			}
			// В результате избавления от дублирующих НЕ и скобок, в str образовался ряд пустых (0) элементов, от них нужно избавиться
			int l2 = 0;  // Считаем количество элементов в строке без 0
			for (int i = 0; i < logic; i++) {
				if (str[i] != 0) {
					str2[l2] = str[i];
					l2++;
				}
			}
			for (int i = l2; i < logic; i++) {
				str[i] = 0;
			}
			logic = l2;
			for (int i = 0; i < logic; i++) {
				str[i] = str2[i];
				str2[i] = 0;
			}
			int i1 = -1, j1 = -1, j2 = -1, i2 = -1;
			int m = -1;
			char t = 0;
			int str2_len = 0;
			for (int i = 0, j = 0; i < logic; i++) {
				// Определение состояния
				if (str[i] == '(' && (i == 0 || str[i - 1] != '!')) {
					stack[upp] = 1;
					upp--;
				}
				if (str[i] == '!') {
					if (i1 == -1) {
						i1 = i;
						m = upp;
					}
					stack[upp] = 2;
					upp--;
				}
				if (str[i] == ')') {
					if (m == upp + 1) {
						i2 = i;
					}
					upp++;
					stack[upp] = 0;
				}
				if (m != -1 && j2 == -1 && stack[m - 1] == 0 && (str[i] == '&' || str[i] == '|')) {
					if (str[i] == '&') {
						j1 = i;
						t = str[i];
					}
					else if (str[i] == '|') {
						j2 = i;
						t = str[i];
						j1 = -1;
					}					
				}
				// Выполнение состояния
				if (i1 == -1) {
					// Простое копирование
					str2[j] = str[i];
					j++;
				}
				else if (i1 != -1 && i2 == -1) {
					// Запись в буфер => Ничего не делаем
				}
				else if (i2 != -1) {
					if (j2 != -1 || j1 != -1) {
						// Выгрузка по законам де Моргана
						kontrol = false; // Выражение пока не соответствует NNF
						if (j2 == -1 && j1 != -1) {
							j2 = j1;
						}
						str2[j] = '(';
						j++;
						str2[j] = '!';
						j++;
						str2[j] = '(';
						j++;
						for (int k = i1 + 2; k < j2; k++, j++) {
							str2[j] = str[k];
						}
						str2[j] = ')';
						j++;
						if (t == '&') {
							str2[j] = '|';
							j++;
						}
						else if (t == '|') {
							str2[j] = '&';
							j++;
						}
						str2[j] = '!';
						j++;
						str2[j] = '(';
						j++;
						for (int k = j2 + 1; k < i2; k++, j++) {
							str2[j] = str[k];
						}
						str2[j] = ')';
						j++;
						str2[j] = ')';
						j++;
					}
					else if (j2 == -1) {
						// Простая выгрузка
						for (int k = i1; k <= i2; k++, j++) {
							str2[j] = str[k];
						}
					}
					i1 = -1;
					i2 = -1;
					j1 = -1;
					j2 = -1;
					t = 0;
					m = -1;
				}
				str2_len = j;
			}
			for (int k = logic; k < str2_len; k++) {
				str[k] = 0;
			}
			for (int k = 0; k < str2_len; k++) {
				str[k] = str2[k];
				str2[k] = 0;
			}
			logic = str2_len;
		} while (kontrol == false);
		// Пункты 3 и 4 - РАССТАВЛЕНИЕ ПРИОРИТЕТОВ ОПЕРАЦИЙ и ВЫЧИСЛЕНИЕ ЛОГИЧЕСКИХ ТАБЛИЦ
		// Добавление ( и ) по бокам NNF
		char s[2]{ 0, 0 };
		for (int i = 0; i <= logic; i++) {
			if (s[0] == 0) {
				s[0] = str[i];
				str[i] = s[1];
				s[1] = 0;
			}
			else {
				s[1] = str[i];
				str[i] = s[0];
				s[0] = 0;
			}
		}
		str[0] = '(';
		str[logic + 1] = ')';
		logic += 2;
		// Читаем строку в формате NNF и вводим её в рассмотрение: Проходим по массиву str и заполняем массив SOO соотношением IDD и названия переменной
		for (int i = 0, m = 0; i < logic; i++) {
			if (str[i] == 'x' || str[i] == 'y' || str[i] == 'z' || str[i] == 'w' || str[i] == 'a' || str[i] == 'b' || str[i] == 'c' || str[i] == 'd') {
				SOO[IDD][m] = str[i];
				m = 1;
			}
			else if (m > 0) {
				if (str[i] == '0' || str[i] == '1' || str[i] == '2' || str[i] == '3' || str[i] == '4' || str[i] == '5' || str[i] == '6' || str[i] == '7' || str[i] == '8' || str[i] == '9') {
					SOO[IDD][m] = str[i];
					m++;
				}
				else {
					// Наткнулись на ), &, | => проверяем, не было ли уже такой переменной в SOO
					for (int j = 0; j < IDD; j++) {
						bool OK = true;
						for (int k = 0; k < 10; k++) {
							if (SOO[j][k] != SOO[IDD][k]) {
								OK = false;
								break;
							}
						}
						if (OK == true) {
							for (int k = 0; k < 10; k++) {
								SOO[IDD][k] = 0;
							}
							IDD--;
							break;
						}
					}
					m = 0;
					IDD++;
				}
			}
		}
		// Расставляем приоритеты операций	
		int oper = 1;
		int OOOKKK[2]{ 0,0 };
		for (int i = 0; i < logic; i++) {
			if (str[i] == '(' && (i == 0 || (i >= 1 && str[i - 1] != '!'))) {
				stack[upp] = i;
				upp--;
			}
			else if (str[i] == '(' && i >= 1 && str[i - 1] == '!') {
				stack[upp] = fizic + i - 1;
				upp--;
			}
			else if (str[i] == ')') {		
				upp++;
				int j = 0;
				if (stack[upp] >= fizic) {
					j = stack[upp] - fizic;
				}
				else if (stack[upp] < fizic) {
					j = stack[upp];
				}
				if (str[i + 1] == 0) {
					j = 0;
				}
				// [1] - !P
				int cpj = 0;  // Сюда запишем место, откуда начинается название переменной, которую нужно инвертировать				
				for (bool k = false; j <= i; j++) {
					// Расставляем приоритеты для !P
					if (str[j] == '!' && stack2[j] == 0) {
						cpj = j + 2;
						stack2[j] = oper;
						k = true;
					}
					else if (k == true && str[j] == ')' && stack2[j] == 0) {
						stack2[j] = oper;
						// Здесь нам нужно определить, какую переменную (по IDD) инвертировать						
						for (int t = 0; t < IDD; t++) {
							bool OK = true;
							for (int p = 0, h = cpj; h < j || p < strlen(SOO[t]); p++, h++) {
								if (SOO[t][p] != str[h]) {
									OK = false;
									break;
								}
							}
							if (OK == true) {
								// Адресация в BASE: 2*t - столбец переменной с индексом t, 2*t+1 - столбец инвертированной переменной с индексом t
								// В BASE первая строка остаётся занулённой и никогда не используется
								int treb = 1;
								for (int p = 1; p < maxterm; p++) {
									// Выделение памяти универсально для НЕ, И, ИЛИ; зависит от количества требуемых действий до сокращений (НЕ = 1, ИЛИ = сумма, И = умножение) 
									// Здесь мы "выделяем память" под текущую операцию (=действие), сообщая массиву KON, какие термы (строки в массиве BASE) относятся к текущей операции
									if (treb == 0) {
										break;
									}									
									if (stack3[p] == 0) {
										if (KON[oper][100] >= 2 && p == KON[oper][KON[oper][100] - 1] + 1) { 
											KON[oper][KON[oper][100] - 1]++;
										}
										else {
											KON[oper][KON[oper][100]] = p;
											KON[oper][KON[oper][100] + 1] = p;
											KON[oper][100] += 2;
										}		
										if (p > ID) {
											ID = p;
										}
										stack3[p] = -1;
										treb--;
									}
								}
								BASE[KON[oper][0]][2 * t] = 0;
								BASE[KON[oper][0]][2 * t + 1] = 1;
								break;
							}
						}
						oper++;
						k = false;
					}
					else if (k == true && stack2[j] == 0) {
						stack2[j] = -1;
					}
				}
				// Техническая часть
				if (stack[upp] >= fizic) {
					j = stack[upp] - fizic;
				}
				else if (stack[upp] < fizic) {
					j = stack[upp];
				}
				if (str[i + 1] == 0) {
					j = 0;
				}
				// [2] - F1 & F2
				for (; j <= i; j++) {
					// Расставляем приоритеты для &
					if (str[j] == '&' && stack2[j] == 0) {
						stack2[j] = oper;			
						int LR[2]{ 0,0 };  // Адрес операции (или доп. операции) левой и правой части соответственно
						int cpl1 = -1, cpl2 = -1;  // Координаты начала и конца строки str - имя переменной-левого множества
						for (int l = j - 1, k = -1; l >= 0; l--) {
							if ((k == -2 || k == -1) && ((stack2[l] != -1 && stack2[l] != 0) || (stack2[l] == 0 && str[l] != '(' && str[l] != ')'))) {
								k = stack2[l];							
								if (stack2[l] == 0) {
									// Левое множество - просто переменная
									// Нам нужно получить IDD этой переменной									
									if (cpl1 == -1) {
										cpl1 = l;
									}
									stack2[l] = -1;
									if (str[l] == 'x' || str[l] == 'y' || str[l] == 'z' || str[l] == 'w' || str[l] == 'a' || str[l] == 'b' || str[l] == 'c' || str[l] == 'd') {
										k = -1;
										cpl2 = l;
										// Теперь по координатам cpl1 и cpl2 получаем IDD левой переменной из базы SOO 
										for (int t = 0; t < IDD; t++) {
											bool OK = true;
											for (int p = 0, h = cpl2; h <= cpl1 || p < strlen(SOO[t]); p++, h++) {
												if (SOO[t][p] != str[h]) {
													OK = false;
													break;
												}
											}
											if (OK == true) {
												int treb = 1;
												for (int p = 1; p < maxterm; p++) {
													// Выделение памяти универсально для НЕ, И, ИЛИ; зависит от количества требуемых действий до сокращений (НЕ = 1, ИЛИ = сумма, И = умножение) 
													// Здесь мы "выделяем память" под текущую операцию (=действие), сообщая массиву KON, какие термы (строки в массиве BASE) относятся к текущей операции
													if (treb == 0) {
														break;
													}
													if (stack3[p] == 0) {
														for (int o = 524288; o < 524290; o++) {
															if (KON[o][100] == 0) {
																LR[0] = o;
																KON[o][KON[o][100]] = p;
																KON[o][KON[o][100] + 1] = p;
																KON[o][100] += 2;
																BASE[KON[o][0]][2 * t] = 1;
																break;
															}
														}
														stack3[p] = -1;
														treb--;
													}
												}
												break;
											}
										}
										break;
									}
									k = -2;									
								}
								else {
									// Левое множество - предыдущее действие stack2[l]
									LR[0] = stack2[l];  // Получили координату - номер операции, которая уже выполнена, но ещё не укрупнена
								}
								stack2[l] = -1;
							}
							if (k != -1 && k != -2) {
								if (stack2[l] == k) {
									stack2[l] = -1;
									break;
								}
								if (stack2[l] != -1) {
									break;
								}
							}
						}
						int cpr1 = -1, cpr2 = -1;  // Координаты начала и конца строки str - имя переменной-правого множества
						for (int r = j + 1, k = -1; r < i; r++) {
							if ((k == -2 || k == -1) && ((stack2[r] != -1 && stack2[r] != 0) || (stack2[r] == 0 && str[r] != '(' && str[r] != ')'))) {
								k = stack2[r];								
								if (stack2[r] == 0) {
									// Правое множество - просто переменная									
									if (str[r] == 'x' || str[r] == 'y' || str[r] == 'z' || str[r] == 'w' || str[r] == 'a' || str[r] == 'b' || str[r] == 'c' || str[r] == 'd') {
										cpr1 = r;
									}
									if (str[r + 1] != '0' && str[r + 1] != '1' && str[r + 1] != '2' && str[r + 1] != '3' && str[r + 1] != '4' && str[r + 1] != '5' && str[r + 1] != '6' && str[r + 1] != '7' && str[r + 1] != '8' && str[r + 1] != '9') {
										k = -1;
										cpr2 = r;
										stack2[r] = -1;
										// Теперь по координатам cpr1 и cpr2 получаем IDD левой переменной из базы SOO 
										for (int t = 0; t < IDD; t++) {
											bool OK = true;
											for (int p = 0, h = cpr1; h <= cpr2 || p < strlen(SOO[t]); p++, h++) {
												if (SOO[t][p] != str[h]) {
													OK = false;
													break;
												}
											}
											if (OK == true) {
												// t - это индекс переменной (столбец в BASE)
												int treb = 1;
												for (int p = 1; p < maxterm; p++) {
													// Выделение памяти универсально для НЕ, И, ИЛИ; зависит от количества требуемых действий до сокращений (НЕ = 1, ИЛИ = сумма, И = умножение) 
													// Здесь мы "выделяем память" под текущую операцию (=действие), сообщая массиву KON, какие термы (строки в массиве BASE) относятся к текущей операции
													if (treb == 0) {
														break;
													}
													if (stack3[p] == 0) {
														for (int o = 524288; o < 524290; o++) {
															if (KON[o][100] == 0) {
																LR[1] = o;
																KON[o][KON[o][100]] = p;
																KON[o][KON[o][100] + 1] = p;
																KON[o][100] += 2;
																BASE[KON[o][0]][2 * t] = 1;
																break;
															}
														}
														stack3[p] = -1;
														treb--;
													}
												}
												break;
											}
										}
										break;
									}
									k = -2;									
								}	
								else {
									// Правое множество - предыдущее действие stack2[r]
									LR[1] = stack2[r];  // Получили координату - номер операции, которая уже выполнена, но ещё не укрупнена
								}
								stack2[r] = -1;
							}
							if (k != -1 && k != -2) {								
								if (stack2[r] == k) {
									stack2[r] = -1;
									break;
								}
								if (stack2[r] != -1) {
									break;
								}
							}
						}
						// Здесь мы уже знаем координаты функций (номер порядка операции), с которыми работаем - они записаны в BASE и определены в KON
						// Эти координаты в stack2 уже удалены, но в LR, BASE и KON они ещё хранятся
						int trebL = 0;
						int trebR = 0;
						int treb = 0;
						for (int o = 1; o < 100; o += 2) {
							if (KON[LR[0]][o] != 0) {
								trebL += KON[LR[0]][o] - KON[LR[0]][o - 1] + 1;
							}
							else {
								break;
							}
						}
						for (int o = 1; o < 100; o += 2) {
							if (KON[LR[1]][o] != 0) {
								trebR += KON[LR[1]][o] - KON[LR[1]][o - 1] + 1;
							}
							else {
								break;
							}
						}
						treb = trebL * trebR;
						// Выделение памяти универсально для НЕ, И, ИЛИ; зависит от количества требуемых действий до сокращений (НЕ = 1, ИЛИ = сумма, И = умножение) 
						// Здесь мы "выделяем память" под текущую операцию (=действие), сообщая массиву KON, какие термы (строки в массиве BASE) относятся к текущей операции
						for (int p = 1; p < maxterm; p++) {							
							if (treb == 0) {
								break;
							}
							if (stack3[p] == 0) {
								if (KON[oper][100] >= 2 && p == KON[oper][KON[oper][100] - 1] + 1) {
									KON[oper][KON[oper][100] - 1]++;
								}
								else if (KON[oper][100] == 98) {
									// Строка KON[oper] почти заполнена: остался 1 свободный отрезок [KON[oper][98];KON[oper][99]]
									// Выделяем свободную память в BASE (размером treb) после самой последней занятой э. к. (строки BASE)
									KON[oper][98] = ID;
									ID += treb;
									KON[oper][99] = ID - 1;
									KON[oper][100] = 100;
									for (int k = KON[oper][98]; k <= KON[oper][99]; k++) {
										stack3[k] = -1;
									}
									break; // treb = 0; не нужен, т. к. break;
								}
								else {
									KON[oper][KON[oper][100]] = p;
									KON[oper][KON[oper][100] + 1] = p;
									KON[oper][100] += 2;
								}
								if (p > ID) {
									ID = p;
								}
								stack3[p] = -1;
								treb--;
							}
						}
						// Здесь мы уже имеем выделенную память под операцию oper
						int q = KON[oper][0];
						int k = 1;
						// Начало главного блока операции & (624): заполнение BASE в стандартном виде						
						for (int p1 = 1; p1 < KON[LR[0]][100]; p1 += 2) {
							for (int f = KON[LR[0]][p1 - 1]; f <= KON[LR[0]][p1]; f++) {
								for (int p2 = 1; p2 < KON[LR[1]][100]; p2 += 2) {
									for (int g = KON[LR[1]][p2 - 1]; g <= KON[LR[1]][p2]; g++) {
										for (int t = 0; t < IDD; t++) {
											BASE[q][2 * t] = BASE[f][2 * t] | BASE[g][2 * t];
											BASE[q][2 * t + 1] = BASE[f][2 * t + 1] | BASE[g][2 * t + 1];
										}
										if (q == KON[oper][k]) {
											if ((k + 1) == KON[oper][100]) {
												break;
											}
											q = KON[oper][k + 1];
											k += 2;
										}
										else {
											q++;
										}										
									}
								}
							}
						}
						// Конец главного блока операции & (647): здесь мы имеем массив BASE до сокращений и поглащений						
						// Теперь пора освободить память с помощью KON и очистить (занулить) в BASE укрупнённые термы, а также осободить соответствующие ячейки массива stack3
						for (int r = 1; r < 100; r += 2) {
							if (KON[LR[0]][r] == 0) {
								break;
							}
							for (int t = KON[LR[0]][r - 1]; t <= KON[LR[0]][r]; t++) {
								for (int h = 0; h < IDD; h++) {
									BASE[t][2 * h] = 0;
									BASE[t][2 * h + 1] = 0;									
								}	
								stack3[t] = 0;
								// Передвижение наверх нижней границы BASE
								if (t + 1 == ID) {
									for (int m = t; m >= 0; m--) {
										if (stack3[m] == 0) {
											ID--;
										}
										else {
											break;
										}
									}
								}
							}
							KON[LR[0]][r - 1] = 0;
							KON[LR[0]][r] = 0;
						}
						KON[LR[0]][100] = 0;
						for (int r = 1; r < 100; r += 2) {
							if (KON[LR[1]][r] == 0) {
								break;
							}
							for (int t = KON[LR[1]][r - 1]; t <= KON[LR[1]][r]; t++) {
								for (int h = 0; h < IDD; h++) {
									BASE[t][2 * h] = 0;
									BASE[t][2 * h + 1] = 0;
								}
								stack3[t] = 0;
								// Передвижение наверх нижней границы BASE
								if (t + 1 == ID) {
									for (int m = t; m >= 0; m--) {
										if (stack3[m] == 0) {
											ID--;
										}
										else {
											break;
										}
									}
								}
							}
							KON[LR[1]][r - 1] = 0;
							KON[LR[1]][r] = 0;
						}
						KON[LR[1]][100] = 0;
						// Память уже обработанного и укрупнённого действия освобождена
						// Выполняем сокращения (684) внутри терма (строки): x&!(x) = 0 - весь такой терм в BASE зануляется, в KON и stack3 такая строка освобождается
						q = 0;
						int cpKON3[101]{};
						for (int c = 0; c < KON[oper][100]; c++) {
							cpKON3[c] = KON[oper][c];
						}
						cpKON3[100] = KON[oper][100];
						for (int p = 1; p < 100; p += 2) {
							for (q = KON[oper][p - 1]; q <= cpKON3[p]; q++) {
								for (int t = 0; t < IDD; t++) {
									if (BASE[q][2 * t] == 1 && BASE[q][2 * t] == BASE[q][2 * t + 1]) {
										// Нашли x&!(x)
										for (int e = 0; e < IDD; e++) {
											BASE[q][2 * e] = 0;
											BASE[q][2 * e + 1] = 0;
										}
										stack3[q] = 0;
										// Передвижение наверх нижней границы BASE
										if (q + 1 == ID) {
											for (int m = q; m >= 0; m--) {
												if (stack3[m] == 0) {
													ID--;
												}
												else {
													break;
												}
											}
										}
										for (int e = 1; e < 100; e += 2) {
											if (KON[oper][e - 1] == q && q == KON[oper][e]) {
												KON[oper][e - 1] = 0;
												KON[oper][e] = 0;
												for (int g = e + 1; g < 100; g++) {
													KON[oper][g - 2] = KON[oper][g];
												}
												KON[oper][100] -= 2;
												break;
											}
											else if (KON[oper][e - 1] == q && q < KON[oper][e]) {
												KON[oper][e - 1]++;
												break;
											}
											else if (KON[oper][e - 1] < q && q == KON[oper][e]) {
												KON[oper][e]--;
												break;
											}
											else if (KON[oper][e - 1] < q && q < KON[oper][e]) {
												for (int k = KON[oper][100] - 1; k > e; k--) {
													KON[oper][k + 2] = KON[oper][k];
												}
												KON[oper][e + 2] = KON[oper][e];
												KON[oper][e] = q - 1;
												KON[oper][e + 1] = q + 1;
												KON[oper][100] += 2;
												break;
											}
										}
										break;
									}
								}
							}
						}						
						// Здесь сокращения в пределах одного терма выполнены (742)
						// Переходим к поглощениям "один внутри другого" (A & B) | A = A, в том числе дублирование термов (строк): O(N^2) по времени, где N - количество термов на момент начала поглощения
						int cpKON1[101]{};
						for (int c = 0; c < KON[oper][100]; c++) {
							cpKON1[c] = KON[oper][c];
						}
						cpKON1[100] = KON[oper][100];
						for (int p = 1; p < 100; p += 2) {
							for (int f = cpKON1[p - 1]; f <= cpKON1[p]; f++) {
								int cpKON2[101]{};
								for (int c = 0; c < KON[oper][100]; c++) {
									cpKON2[c] = KON[oper][c];
								}
								cpKON2[100] = KON[oper][100];
								for (int r = 1; r < 100; r += 2) {
									for (int g = cpKON2[r - 1]; g <= cpKON2[r]; g++) {
										if (f != g && f != 0 && g != 0) {
											short OK = 0;
											for (int k = 0; k < IDD; k++) {
												if (BASE[g][2 * k] == 1 && BASE[g][2 * k] == BASE[f][2 * k]) {
													OK = 1;
												}
												if (BASE[g][2 * k + 1] == 1 && BASE[g][2 * k + 1] == BASE[f][2 * k + 1]) {
													OK = 1;
												}
												if (BASE[g][2 * k] == 1 && BASE[g][2 * k] != BASE[f][2 * k]) {
													OK = -1;
													break;
												}
												if (BASE[g][2 * k + 1] == 1 && BASE[g][2 * k + 1] != BASE[f][2 * k + 1]) {
													OK = -1;
													break;
												}
											}
											if (OK == 1) {
												// Удаляем терм BASE[f]
												for (int k = 0; k < IDD; k++) {
													BASE[f][2 * k] = 0;
													BASE[f][2 * k + 1] = 0;
												}
												stack3[f] = 0;
												// Передвижение наверх нижней границы BASE
												if (f + 1 == ID) {
													for (int m = f; m >= 0; m--) {
														if (stack3[m] == 0) {
															ID--;
														}
														else {
															break;
														}
													}
												}
												for (int e = 1; e < 100; e += 2) {
													if (KON[oper][e - 1] == f && f == KON[oper][e]) {
														KON[oper][e - 1] = 0;
														KON[oper][e] = 0;
														for (int k = e + 1; k < 100; k++) {
															KON[oper][k - 2] = KON[oper][k];
														}
														KON[oper][100] -= 2;
														break;
													}
													else if (KON[oper][e - 1] == f && f < KON[oper][e]) {
														KON[oper][e - 1]++;
													}
													else if (KON[oper][e - 1] < f && f == KON[oper][e]) {
														KON[oper][e]--;
													}
													else if (KON[oper][e - 1] < f && f < KON[oper][e]) {
														for (int k = KON[oper][100] - 1; k > e; k--) {
															KON[oper][k + 2] = KON[oper][k];
														}
														KON[oper][e + 2] = KON[oper][e];
														KON[oper][e] = f - 1;
														KON[oper][e + 1] = f + 1;
														KON[oper][100] += 2;
													}
												}
											}
										}
									}
								}
							}
						}
						// Здесь все сокращения и поглощения уже выполнены
						oper++;
					}
				}
				// Техническая часть
				if (stack[upp] >= fizic) {
					j = stack[upp] - fizic;
				}
				else if (stack[upp] < fizic) {
					j = stack[upp];
				}
				if (str[i + 1] == 0) {
					j = 0;
				}
				// [3] - F1 | F2
				for (; j <= i; j++) {
					// Расставляем приоритеты для |
					if (str[j] == '|' && stack2[j] == 0) {
						stack2[j] = oper;
						int LR[2]{ 0,0 };  // Адрес операции (или доп. операции) левой и правой части соответственно
						int cpl1 = -1, cpl2 = -1;  // Координаты начала и конца строки str - имя переменной-левого множества
						for (int l = j - 1, k = -1; l >= 0; l--) {
							if ((k == -2 || k == -1) && ((stack2[l] != -1 && stack2[l] != 0) || (stack2[l] == 0 && str[l] != '(' && str[l] != ')'))) {
								k = stack2[l];
								if (stack2[l] == 0) {
									// Левое множество - просто переменная
									// Нам нужно получить IDD этой переменной									
									if (cpl1 == -1) {
										cpl1 = l;
									}
									stack2[l] = -1;
									if (str[l] == 'x' || str[l] == 'y' || str[l] == 'z' || str[l] == 'w' || str[l] == 'a' || str[l] == 'b' || str[l] == 'c' || str[l] == 'd') {
										k = -1;
										cpl2 = l;
										// Теперь по координатам cpl1 и cpl2 получаем IDD левой переменной из базы SOO 
										for (int t = 0; t < IDD; t++) {
											bool OK = true;
											for (int p = 0, h = cpl2; h <= cpl1 || p < strlen(SOO[t]); p++, h++) {
												if (SOO[t][p] != str[h]) {
													OK = false;
													break;
												}
											}
											if (OK == true) {
												int treb = 1;
												for (int p = 1; p < maxterm; p++) {
													// Выделение памяти универсально для НЕ, И, ИЛИ; зависит от количества требуемых действий до сокращений (НЕ = 1, ИЛИ = сумма, И = умножение) 
													// Здесь мы "выделяем память" под текущую операцию (=действие), сообщая массиву KON, какие термы (строки в массиве BASE) относятся к текущей операции
													if (treb == 0) {
														break;
													}
													if (stack3[p] == 0) {
														for (int o = 524288; o < 524290; o++) {
															if (KON[o][100] == 0) {
																LR[0] = o;
																KON[o][KON[o][100]] = p;
																KON[o][KON[o][100] + 1] = p;
																KON[o][100] += 2;
																BASE[KON[o][0]][2 * t] = 1;
																break;
															}
														}
														stack3[p] = -1;
														treb--;
													}
												}
												break;
											}
										}
										break;
									}
									k = -2;
								}
								else {
									// Левое множество - предыдущее действие stack2[l]
									LR[0] = stack2[l];  // Получили координату - номер операции, которая уже выполнена, но ещё не укрупнена
								}
								stack2[l] = -1;
							}
							if (k != -1 && k != -2) {
								if (stack2[l] == k) {
									stack2[l] = -1;
									break;
								}
								if (stack2[l] != -1) {
									break;
								}
							}
						}
						int cpr1 = -1, cpr2 = -1;  // Координаты начала и конца строки str - имя переменной-правого множества
						for (int r = j + 1, k = -1; r < i; r++) {
							if ((k == -2 || k == -1) && ((stack2[r] != -1 && stack2[r] != 0) || (stack2[r] == 0 && str[r] != '(' && str[r] != ')'))) {
								k = stack2[r];
								if (stack2[r] == 0) {
									// Правое множество - просто переменная									
									if (str[r] == 'x' || str[r] == 'y' || str[r] == 'z' || str[r] == 'w' || str[r] == 'a' || str[r] == 'b' || str[r] == 'c' || str[r] == 'd') {
										cpr1 = r;
									}
									if (str[r + 1] != '0' && str[r + 1] != '1' && str[r + 1] != '2' && str[r + 1] != '3' && str[r + 1] != '4' && str[r + 1] != '5' && str[r + 1] != '6' && str[r + 1] != '7' && str[r + 1] != '8' && str[r + 1] != '9') {
										k = -1;
										cpr2 = r;
										stack2[r] = -1;
										// Теперь по координатам cpr1 и cpr2 получаем IDD левой переменной из базы SOO 
										for (int t = 0; t < IDD; t++) {
											bool OK = true;
											for (int p = 0, h = cpr1; h <= cpr2 || p < strlen(SOO[t]); p++, h++) {
												if (SOO[t][p] != str[h]) {
													OK = false;
													break;
												}
											}
											if (OK == true) {
												// t - это индекс переменной (столбец в BASE)
												int treb = 1;
												for (int p = 1; p < maxterm; p++) {
													// Выделение памяти универсально для НЕ, И, ИЛИ; зависит от количества требуемых действий до сокращений (НЕ = 1, ИЛИ = сумма, И = умножение) 
													// Здесь мы "выделяем память" под текущую операцию (=действие), сообщая массиву KON, какие термы (строки в массиве BASE) относятся к текущей операции
													if (treb == 0) {
														break;
													}
													if (stack3[p] == 0) {
														for (int o = 524288; o < 524290; o++) {
															if (KON[o][100] == 0) {
																LR[1] = o;
																KON[o][KON[o][100]] = p;
																KON[o][KON[o][100] + 1] = p;
																KON[o][100] += 2;
																BASE[KON[o][0]][2 * t] = 1;
																break;
															}
														}
														stack3[p] = -1;
														treb--;
													}
												}
												break;
											}
										}
										break;
									}
									k = -2;
								}
								else {
									// Правое множество - предыдущее действие stack2[r]
									LR[1] = stack2[r];  // Получили координату - номер операции, которая уже выполнена, но ещё не укрупнена
								}
								stack2[r] = -1;
							}
							if (k != -1 && k != -2) {
								if (stack2[r] == k) {
									stack2[r] = -1;
									break;
								}
								if (stack2[r] != -1) {
									break;
								}
							}
						}
						// Здесь мы уже знаем координаты функций (номер порядка операции), с которыми работаем - они записаны в BASE и определены в KON
						// Эти координаты в stack2 уже удалены, но в LR, BASE и KON они ещё хранятся
						int trebL = 0;
						int trebR = 0;
						int treb = 0;
						for (int o = 1; o < 100; o += 2) {
							if (KON[LR[0]][o] != 0) {
								trebL += KON[LR[0]][o] - KON[LR[0]][o - 1] + 1;
							}
							else {
								break;
							}
						}
						for (int o = 1; o < 100; o += 2) {
							if (KON[LR[1]][o] != 0) {
								trebR += KON[LR[1]][o] - KON[LR[1]][o - 1] + 1;
							}
							else {
								break;
							}
						}
						treb = trebL + trebR;
						// Выделение памяти универсально для НЕ, И, ИЛИ; зависит от количества требуемых действий до сокращений (НЕ = 1, ИЛИ = сумма, И = умножение) 
						// Здесь мы "выделяем память" под текущую операцию (=действие), сообщая массиву KON, какие термы (строки в массиве BASE) относятся к текущей операции
						for (int p = 1; p < maxterm; p++) {							
							if (treb == 0) {
								break;
							}
							if (stack3[p] == 0) {
								if (KON[oper][100] >= 2 && p == KON[oper][KON[oper][100] - 1] + 1) {
									KON[oper][KON[oper][100] - 1]++;
								}
								else if (KON[oper][100] == 98) {
									// Строка KON[oper] почти заполнена: остался 1 свободный отрезок [KON[oper][98];KON[oper][99]]
									// Выделяем свободную память в BASE (размером treb) после самой последней занятой э. к. (строки BASE)
									KON[oper][98] = ID;
									ID += treb;
									KON[oper][99] = ID - 1;
									KON[oper][100] = 100;
									for (int k = KON[oper][98]; k <= KON[oper][99]; k++) {
										stack3[k] = -1;
									}
									break; // treb = 0; не нужен, т. к. break;
								}
								else {
									KON[oper][KON[oper][100]] = p;
									KON[oper][KON[oper][100] + 1] = p;
									KON[oper][100] += 2;
								}
								if (p > ID) {
									ID = p;
								}
								stack3[p] = -1;
								treb--;
							}
						}
						// Здесь мы уже имеем выделенную память под операцию oper
						int q = KON[oper][0];
						// Начало главного блока операции | (1056): заполнение BASE в стандартном виде						
						for (int p = 1; p < 100; p += 2) {
							for (int f = KON[LR[0]][p - 1]; f <= KON[LR[0]][p]; f++) {
								short len = 0;
								for (int k = 0; k < maxper; k++) {
									if (SOO[k][0] == 0) {
										break;
									}
									if (BASE[f][2 * k] == 1 || BASE[f][2 * k + 1] == 1) {
										len = 1;
									}
									BASE[q][2 * k] = BASE[f][2 * k];
									BASE[q][2 * k + 1] = BASE[f][2 * k + 1];
								}
								bool OK = false;
								do {
									if (len > 0) {
										q++;
									}									
									for (int r = 1; r < 100; r += 2) {
										if (KON[oper][r - 1] <= q && q <= KON[oper][r]) {
											OK = true;
											break;
										}
									}
									if (q == maxterm) {
										break;
									}
								} while (OK == false);
								if (q == maxterm) {
									p = 100;
									break;
								}
							}
							if (q < maxterm) {
								for (int g = KON[LR[1]][p - 1]; g <= KON[LR[1]][p]; g++) {
									short len = 0;
									for (int k = 0; k < maxper; k++) {
										if (SOO[k][0] == 0) {
											break;
										}
										if (BASE[g][2 * k] == 1 || BASE[g][2 * k + 1] == 1) {
											len = 1;
										}
										BASE[q][2 * k] = BASE[g][2 * k];
										BASE[q][2 * k + 1] = BASE[g][2 * k + 1];
									}
									bool OK = false;
									do {
										if (len > 0) {
											q++;
										}
										for (int r = 1; r < 100; r += 2) {
											if (KON[oper][r - 1] <= q && q <= KON[oper][r]) {
												OK = true;
												break;
											}
										}
										if (q == maxterm) {
											break;
										}
									} while (OK == false);
									if (q == maxterm) {
										p = 100;
										break;
									}
								}
							}
						}
						// Конец главного блока операции | (1125): здесь мы имеем массив BASE до сокращений и поглащений						
						// Теперь пора освободить память с помощью KON и очистить (занулить) в BASE укрупнённые термы, а также осободить соответствующие ячейки массива stack3
						for (int r = 1; r < 100; r += 2) {
							if (KON[LR[0]][r] == 0) {
								break;
							}
							for (int t = KON[LR[0]][r - 1]; t <= KON[LR[0]][r]; t++) {
								for (int h = 0; h < IDD; h++) {
									BASE[t][2 * h] = 0;
									BASE[t][2 * h + 1] = 0;
								}
								stack3[t] = 0;
								// Передвижение наверх нижней границы BASE
								if (t + 1 == ID) {
									for (int m = t; m >= 0; m--) {
										if (stack3[m] == 0) {
											ID--;
										}
										else {
											break;
										}
									}
								}
							}
							KON[LR[0]][r - 1] = 0;
							KON[LR[0]][r] = 0;
						}
						KON[LR[0]][100] = 0;
						for (int r = 1; r < 100; r += 2) {
							if (KON[LR[1]][r] == 0) {
								break;
							}
							for (int t = KON[LR[1]][r - 1]; t <= KON[LR[1]][r]; t++) {
								for (int h = 0; h < IDD; h++) {
									BASE[t][2 * h] = 0;
									BASE[t][2 * h + 1] = 0;
								}
								stack3[t] = 0;
								// Передвижение наверх нижней границы BASE
								if (t + 1 == ID) {
									for (int m = t; m >= 0; m--) {
										if (stack3[m] == 0) {
											ID--;
										}
										else {
											break;
										}
									}
								}
							}
							KON[LR[1]][r - 1] = 0;
							KON[LR[1]][r] = 0;
						}
						KON[LR[1]][100] = 0;
						// Память уже обработанного и укрупнённого действия освобождена
						// x&!(x) не может быть после операции | (дизъюнкции) => сокращений не делаем
						// Переходим к поглощениям "один внутри другого" (A & B) | A = A, в том числе дублирование термов (строк): O(N^2) по времени, где N - количество термов на момент начала поглощения
						int cpKON1[101]{};
						for (int c = 0; c < KON[oper][100]; c++) {
							cpKON1[c] = KON[oper][c];
						}
						cpKON1[100] = KON[oper][100];
						for (int p = 1; p < 100; p += 2) {
							for (int f = cpKON1[p - 1]; f <= cpKON1[p]; f++) {
								int cpKON2[101]{};
								for (int c = 0; c < KON[oper][100]; c++) {
									cpKON2[c] = KON[oper][c];
								}
								cpKON2[100] = KON[oper][100];
								for (int r = 1; r < 100; r += 2) {
									for (int g = cpKON2[r - 1]; g <= cpKON2[r]; g++) {
										if (f != g && f != 0 && g != 0) {
											short OK = 0;
											for (int k = 0; k < IDD; k++) {
												if (BASE[g][2 * k] == 1 && BASE[g][2 * k] == BASE[f][2 * k]) {
													OK = 1;
												}
												if (BASE[g][2 * k + 1] == 1 && BASE[g][2 * k + 1] == BASE[f][2 * k + 1]) {
													OK = 1;
												}
												if (BASE[g][2 * k] == 1 && BASE[g][2 * k] != BASE[f][2 * k]) {
													OK = -1;
													break;
												}
												if (BASE[g][2 * k + 1] == 1 && BASE[g][2 * k + 1] != BASE[f][2 * k + 1]) {
													OK = -1;
													break;
												}
											}
											if (OK == 1) {
												// Удаляем терм BASE[f]
												for (int k = 0; k < IDD; k++) {
													BASE[f][2 * k] = 0;
													BASE[f][2 * k + 1] = 0;
												}
												stack3[f] = 0;
												// Передвижение наверх нижней границы BASE
												if (f + 1 == ID) {
													for (int m = f; m >= 0; m--) {
														if (stack3[m] == 0) {
															ID--;
														}
														else {
															break;
														}
													}
												}
												for (int e = 1; e < 100; e += 2) {
													if (KON[oper][e - 1] == f && f == KON[oper][e]) {
														KON[oper][e - 1] = 0;
														KON[oper][e] = 0;
														for (int k = e + 1; k < 100; k++) {
															KON[oper][k - 2] = KON[oper][k];
														}
														KON[oper][100] -= 2;
														break;
													}
													else if (KON[oper][e - 1] == f && f < KON[oper][e]) {
														KON[oper][e - 1]++;
													}
													else if (KON[oper][e - 1] < f && f == KON[oper][e]) {
														KON[oper][e]--;
													}
													else if (KON[oper][e - 1] < f && f < KON[oper][e]) {
														for (int k = KON[oper][100] - 1; k > e; k--) {
															KON[oper][k + 2] = KON[oper][k];
														}
														KON[oper][e + 2] = KON[oper][e];
														KON[oper][e] = f - 1;
														KON[oper][e + 1] = f + 1;
														KON[oper][100] += 2;
													}
												}
											}
										}
									}
								}
							}
						}
						// Здесь все сокращения и поглощения уже выполнены
						oper++;
					}
				}
				stack[upp] = 0;
			}
		}	
		// Финал: вывод результата на экран
		oper--;  // Получаем индекс последней операции 
		cout << oper << endl;
		cout << IDD << endl;  // IDD - количество переменных в выражении
		for (int i = 1; i < KON[oper][100]; i += 2) {
			for (int j = KON[oper][i - 1]; j <= KON[oper][i]; j++) {
				for (int k = 0, t = 0; k < IDD; k++) {
					if (BASE[j][k + k] == 1) {
						if (t > 0) {
							cout << "&";
						}
						cout << SOO[k];
						t = 1;
					}
					else if (BASE[j][k + k + 1] == 1) {
						if (t > 0) {
							cout << "&";
						}
						cout << "!(" << SOO[k] << ")";
						t = 1;
					}
				}
				cout << "|" << endl;
			}
		}
		cout << "." << endl;
	}	
	return 0;
}