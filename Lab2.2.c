#include <stdio.h>
#include <windows.h>


int main () {//							 	  0     1    2    3    4     5     6   7    8    9
	const int MATRIX_WITH_DATA[7][10] = {	{-1  ,  19 , 2 ,  2 ,  0  ,  3  ,  2 , 1 , -1 , -3 /*0*/}, 
											{-12 , -40 , 4 ,  2 ,  0  , 230 , -4 , 1 , -1 , -8 /*1*/}, 
											{ 0  ,  4  , 2 , -2 ,  0  ,  1  ,  1 , 1 , -1 , -1 /*2*/},
											{ 2  ,  0  , 4 ,  9 ,  2  ,  4  ,  1 , 0 , -2 , -5 /*3*/},
											{-1  , -2  , 5 ,  3 , -1  , -2  ,  8 , 1 , -1 , -2 /*4*/},
											{ 5  ,  0  , 5 ,  0 ,  3  ,  4  ,  2 , 1 , -1 , -7 /*5*/},
											{ 20 ,  2  , 5 , -3 , -23 ,  4  ,  1 , 1 , -1 , -1 /*6*/}	};
	const int RAW_LENGTH = 10; //Довжина рядка
	const int COL_LENGTH = 7; //Довжина стовпчика
	

	COORD GetConsoleCursorPosition(HANDLE hConsoleOutput) {
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		if (GetConsoleScreenBufferInfo(hConsoleOutput, &csbi)) {
			return csbi.dwCursorPosition;
		} else {
			COORD invalid = { 0, 0 };
			return invalid;
		}
	}

	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);

	void gotoX(int x){
		x += GetConsoleCursorPosition(hout).X;		
		COORD pos = {x, GetConsoleCursorPosition(hout).Y};
		SetConsoleCursorPosition(hout, pos);
	}

	void gotoY(int y){	
		y += GetConsoleCursorPosition(hout).Y;
		COORD pos = {GetConsoleCursorPosition(hout).X, y};
		SetConsoleCursorPosition(hout, pos);
	}

	void drawMatrix(int matrix[COL_LENGTH][RAW_LENGTH]) {
		printf("\n\n");
		for (int i = 0; i < COL_LENGTH; i++) {
			for (int j = 0; j < RAW_LENGTH; j++){
				gotoX(2);
				printf("%3d", matrix[i][j]);
			}
			printf("\n\n");
		}
		printf("\n");
	}

	int getReversedMatrix(int matrix[COL_LENGTH][RAW_LENGTH]) {
		int buffer[COL_LENGTH][RAW_LENGTH];
		for (int i = 0; i <= COL_LENGTH / 2; i++) {
			for (int j = 0; j <= COL_LENGTH / 2; j++)
			{
				buffer[i][j] = matrix[COL_LENGTH - i][j];
				matrix[COL_LENGTH - i][j] = matrix[i][j];
				matrix[i][j] = buffer[i][j];		
			}
		}
		return matrix;
	}

	printf("Original matrix: \n");
	drawMatrix(MATRIX_WITH_DATA);

	int sortMatrixByHoar(int matrix[COL_LENGTH][RAW_LENGTH]) {
		int useHoar(int m[COL_LENGTH][RAW_LENGTH], int L, int R, int i) {
			int K = L;
			int M = R;

			int T = m[L][i]; //опорний елемент
			while (L < R) {
				while (m[R][i] < T && L < R) { //рухаємо правий вказівник
					R--;
				}

				//знайшли число, що неправильно розташований відносно опорного елементу правим вказівником
				if (L != R) {
					m[L][i] = m[R][i];
					L++;
				}

				while (m[L][i] > T && L < R) { //рухаємо лівий вказівник
					L++;
				}

				//знайшли число, що неправильно розташований відносно опорного елементу лівим вказівником
				if (L != R) { 
					m[R][i] = m[L][i];
					R--;
				}
			} //(L <= R) -> розміщаємо опорний елемент

			m[L][i] = T;
			int P = L; // координата опорного елементу

			// рекурсивно визиваємо для лівої частини
			L = K;
			R = P - 1;
			if (P != L && P != R) {
				m = useHoar(m, L, R, i);
			}

			//рекурсивно визиваємо для правої частини
			L = P + 1;
			R = M;
			if (P != L && P != R) {
				m = useHoar(m, L, R, i);
			}
			// немає елементів по один бік від опорного елемента -> повертаємо відсортований масив
			return m;
		}

		//сортуємо кожен стовпчик масиву
		for (int i = 0; i < RAW_LENGTH; i++) {
			matrix = useHoar(matrix, 0, COL_LENGTH - 1, i);
		}

		return matrix;
	}


	printf("Sorted matrix: \n");
	drawMatrix(sortMatrixByHoar(MATRIX_WITH_DATA));
	
	const int REVERSED_SORTED_MATRIX = getReversedMatrix(sortMatrixByHoar(MATRIX_WITH_DATA));
	printf("Reversed sorted matrix: \n");
	drawMatrix(REVERSED_SORTED_MATRIX);

	printf("Re-sorted reversed matrix: \n");
	drawMatrix(sortMatrixByHoar(REVERSED_SORTED_MATRIX));

	system("pause");
	return 0;
}