#include <iostream>
#include <string>
#include <ctime>
#include <Windows.h>

// #include <chrono>
// #include <conio.h>

using namespace std;

#define SIZE 9
#define GRID_FULL std::make_pair(SIZE, SIZE)

bool editMode = true;
bool solvingMode = false;
void gotoxy(int x, int y)
{
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &lpCursor);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
}

void draw(int[SIZE][SIZE]);

bool is_allowed_in_row(int area[SIZE][SIZE], int row, int number)
{
	for (int col = 0; col < SIZE; col++)
	{
		if (area[row][col] == number)
		{
			return false;
		}
	}
	return true;
}

bool is_allowed_in_col(int area[SIZE][SIZE], int col, int number)
{
	for (int row = 0; row < SIZE; row++)
	{
		if (area[row][col] == number)
		{
			return false;
		}
	}
	return true;
}

bool is_allowed_in_box(int area[SIZE][SIZE], int start_row, int start_col, int number)
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (area[start_row + row][start_col + col] == number)
			{
				return false;
			}
		}
	}
	return true;
}

bool is_safe(int area[SIZE][SIZE], int row, int col, int number)
{
	bool cond = is_allowed_in_row(area, row, number);
	cond = cond && is_allowed_in_col(area, col, number);
	cond = cond && is_allowed_in_box(area, row - row % 3, col - col % 3, number);
	return cond;
}

bool not_filled(int area[SIZE][SIZE], int row, int col)
{
	return area[row][col] == 0;
}

bool is_full(int area[SIZE][SIZE])
{
	for (int row = 0; row < SIZE; row++)
	{
		for (int col = 0; col < SIZE; col++)
		{
			if (area[row][col] == 0)
			{
				return false;
			}
		}
	}

	return true;
}

std::pair<int, int> get_unassigned_location(int grid[SIZE][SIZE])
{
	for (int row = 0; row < SIZE; row++)
		for (int col = 0; col < SIZE; col++)
			if (grid[row][col] == 0)
			{
				return std::make_pair(row, col);
			}
	return GRID_FULL;
}

bool solve(int area[SIZE][SIZE])
{
	/*
	if (is_full(area)) {
		return true;
	};
	*/

	// If the Soduko grid has been filled, we are done
	if (GRID_FULL == get_unassigned_location(area))
	{
		return true;
	}

	// Get an unassigned Soduko grid location
	std::pair<int, int> row_and_col = get_unassigned_location(area);
	int row = row_and_col.first;
	int col = row_and_col.second;

	for (int number = 1; number <= 9; number++)
	{
		if (is_safe(area, row, col, number))
		{
			area[row][col] = number;
			//draw(area);
			if (solve(area))
			{
				return true;
			}
			area[row][col] = 0;
		}
	}
	return false;
}

class Cursor
{
public:
	int x, y;

	Cursor(int xPos, int yPos)
	{
		x = xPos;
		y = yPos;
	}
};

Cursor cursor(0, 0);

void draw(int area[SIZE][SIZE])
{
	string name = "SODUKO";
	//gotoxy((int)(SIZE - name.length())/2, 1);
	//cout << name;

	for (int k = 0; k < SIZE; k++)
	{
		gotoxy(4 * k, 0);
		cout << "+---+";

		gotoxy(0, 2 * k + 1);
		cout << "|";
	}
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{

			gotoxy(4 * j + 1, 2 * i + 2);
			if ((i + 1) % 3 == 0)
				cout << "===+";
			else
				cout << "---+";

			gotoxy(4 * j + 4, 2 * i + 1);
			if ((j + 1) % 3 == 0)
				cout << "\xba";
			else
				cout << "|";
		}
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			gotoxy(4 * j + 2, 2 * i + 1);

			if (area[i][j] == 0)
				cout << " ";
			else
				cout << area[i][j];
		}
	}

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			gotoxy(4 * j + 1, 2 * i + 1);
			cout << " ";

			gotoxy(4 * j + 3, 2 * i + 1);
			cout << " ";
		}
	}

	gotoxy(4 * cursor.x + 1, 2 * cursor.y + 1);
	cout << ">";
	gotoxy(4 * cursor.x + 3, 2 * cursor.y + 1);
	cout << "<";
}

void keyHandler(int area[SIZE][SIZE])
{
	if (!solvingMode)
	{
		/*
			if (GetAsyncKeyState(VK_SPACE)) {
				while (GetAsyncKeyState(VK_SPACE));
				editMode = !editMode;
			}
		*/

		if (GetAsyncKeyState(VK_RETURN))
		{
			while (GetAsyncKeyState(VK_RETURN))
				;
			editMode = !editMode;
			solvingMode = !solvingMode;
		}

		if (editMode)
		{

			if (GetAsyncKeyState(VK_LEFT) && cursor.x > 0)
			{
				while (GetAsyncKeyState(VK_LEFT))
					;
				cursor.x--;
			}
			if (GetAsyncKeyState(VK_RIGHT) && cursor.x < SIZE - 1)
			{
				while (GetAsyncKeyState(VK_RIGHT))
					;
				cursor.x++;
			}
			if (GetAsyncKeyState(VK_UP) && cursor.y > 0)
			{
				while (GetAsyncKeyState(VK_UP))
					;
				cursor.y--;
			}
			if (GetAsyncKeyState(VK_DOWN) && cursor.y < SIZE - 1)
			{
				while (GetAsyncKeyState(VK_DOWN))
					;
				cursor.y++;
			}

			if (GetAsyncKeyState(VK_NUMPAD0))
			{
				while (GetAsyncKeyState(VK_NUMPAD0))
					;
				area[cursor.y][cursor.x] = 0;
			}
			if (GetAsyncKeyState(VK_NUMPAD1))
			{
				while (GetAsyncKeyState(VK_NUMPAD1))
					;
				area[cursor.y][cursor.x] = 1;
			}
			if (GetAsyncKeyState(VK_NUMPAD2))
			{
				while (GetAsyncKeyState(VK_NUMPAD2))
					;
				area[cursor.y][cursor.x] = 2;
			}
			if (GetAsyncKeyState(VK_NUMPAD3))
			{
				while (GetAsyncKeyState(VK_NUMPAD3))
					;
				area[cursor.y][cursor.x] = 3;
			}
			if (GetAsyncKeyState(VK_NUMPAD4))
			{
				while (GetAsyncKeyState(VK_NUMPAD4))
					;
				area[cursor.y][cursor.x] = 4;
			}
			if (GetAsyncKeyState(VK_NUMPAD5))
			{
				while (GetAsyncKeyState(VK_NUMPAD5))
					;
				area[cursor.y][cursor.x] = 5;
			}
			if (GetAsyncKeyState(VK_NUMPAD6))
			{
				while (GetAsyncKeyState(VK_NUMPAD6))
					;
				area[cursor.y][cursor.x] = 6;
			}
			if (GetAsyncKeyState(VK_NUMPAD7))
			{
				while (GetAsyncKeyState(VK_NUMPAD7))
					;
				area[cursor.y][cursor.x] = 7;
			}
			if (GetAsyncKeyState(VK_NUMPAD8))
			{
				while (GetAsyncKeyState(VK_NUMPAD8))
					;
				area[cursor.y][cursor.x] = 8;
			}
			if (GetAsyncKeyState(VK_NUMPAD9))
			{
				while (GetAsyncKeyState(VK_NUMPAD9))
					;
				area[cursor.y][cursor.x] = 9;
			}

			/*
				if (GetAsyncKeyState(VK_UP)) {
					if (area[cursor.y][cursor.x] < 9) area[cursor.y][cursor.x] += 1;
					while (GetAsyncKeyState(VK_UP));
				}
				if (GetAsyncKeyState(VK_DOWN)) {
					if (area[cursor.y][cursor.x] > 0) area[cursor.y][cursor.x] -= 1;
					while (GetAsyncKeyState(VK_DOWN));
				}
			*/
		}
	}
}

int main()
{
	system("cls");
	bool end = false;
	int fps = 1;
	int area[SIZE][SIZE];
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			area[i][j] = 0;
		}
	}

	do
	{
		fps++;
		if (fps % 10000 == 0)
		{
			keyHandler(area);
			draw(area);
		}

	} while (!end && !solvingMode);
	if (solvingMode == true)
	{
		system("cls");
		gotoxy(SIZE, 0);
		cout << "Solving...";
		clock_t time = clock();
		if (solve(area))
		{
			draw(area);
			time = clock() - time;
			int ms = double(time) / CLOCKS_PER_SEC * 1000;
			gotoxy(5, SIZE * 2 + 2);
			cout << "Solving time: " << ms << "ms" << endl
				 << endl;
			system("pause");
		}
		else
		{
			system("cls");
			cout << "No solution exists for the given Soduko" << endl
				 << endl;
		}
	}
}