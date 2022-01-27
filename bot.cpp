#include "headers.h"

pair<int, int> prev1;
int countMy = 0; // number of my hits
int countBot = 0; // number of bot's hits
vector<pair<int, int>> pos;
int oneByOne = 0;

bool checkStripe(int x, int y, int dir, int type) { // dir: 0 - up, 1-down,2-right, 3-left
	if (dir == 0) {
		if (IsOk({x, y - type + 1 })) {
			for (int i = y; i >= y - type + 1; i--) {
				if (!allowed[x][i])
					return false;
			}
			for (int i = y; i >= y - type + 1; i--) {
				EnemyField[x][i] = true;
				allowed[x][i] = false;
			}
			return true;
		}
	}
	else if (dir == 1) {
		if (IsOk({ x, y - type + 1 })) {
			for (int i = y; i >= y - type + 1; i--) {
				if (!allowed[x][i])
					return false;
			}
			for (int i = y; i >= y - type + 1; i--) {
				EnemyField[x][i] = true;
				allowed[x][i] = false;
			}
			return true;
		}
	}
	else if (dir == 2) {
		if (IsOk({ x + type - 1, y })) {
			for (int i = x; i <= x + type - 1; i++) {
				if (!allowed[i][y])
					return false;
			}
			for (int i = x; i <= x + type - 1; i++) {
				EnemyField[i][y] = true;
				allowed[i][y] = false;
			}
			return true;
		}
	}
	else {
		if (IsOk({ x - type + 1, y })) {
			for (int i = x; i >= x - type + 1; i--) {
				if (!allowed[i][y])
					return false;
			}
			for (int i = x; i >= x - type + 1; i--) {
				EnemyField[i][y] = true;
				allowed[i][y] = false;
			}
			return true;
		}
	}
	return false;
}

bool PutShip(int x, int y, int type) {
	vector<int> orderDir = { 0,1,2,3 };
	random_shuffle(orderDir.begin(), orderDir.end());
	for (int i = 0; i < 3; i++) {
		if (checkStripe(y, x, orderDir[i], type)) {
			return true;
		}
	}
	return false;
}

bool check(int x, int y) {
	return (x >= 0 && x < 10 && y >= 0 && y < 10);
}

bool usedEnemy[10][10];

void CircleShip(int x, int y) {
	usedEnemy[y][x] = true;
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y) {
				continue;
			}
			if (check(j, i)) {
				allowed[j][i] = false;
			}
		}
	}
	for (pair<int,int> i : vector<pair<int,int>> { { y, x - 1 }, { y, x + 1 }, {y - 1, x},{y + 1, x} }) {
		if (check(i.first, i.second) && EnemyField[i.first][i.second] && !usedEnemy[i.first][i.second])
			CircleShip(i.second, i.first);
	}
}

void CircleSingle(int x, int y) {
	for (int i = x - 1; i <= x + 1; i++) {
		for (int j = y - 1; j <= y + 1; j++) {
			if (i == x && j == y)
				continue;
			if (IsOk({ j,i }))
				allowed[j][i] = false;
		}
	}
}

void PrintBotA() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << int(allowed[i][j]) << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void PrintBot() {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			cout << int(EnemyField[i][j]) << " ";
		}
		cout << endl;
	}
	cout << endl;
}

bool allowed[10][10];

void FillBot() {
	bool flagTest = true;
	while (flagTest) {
		vector<int> ships = { 0, 4,3,2,1 }; // a[i] ships (are made from i cells)
											// are remaining to be put
		srand(time(0));
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				allowed[i][j] = true;
		for (int i = 4; i >= 1; i--) {
			bool TEST_ = true;
			while (ships[i] > 0) {
				int count = 0;
				bool flag = true;
				while (flag) {
					srand(time(0));
					int x = rand() % 10, y = rand() % 10;
					while (EnemyField[y][x] == true || !allowed[y][x]) {
						x = rand() % 10, y = rand() % 10;
					}
					bool temp = PutShip(x, y, i);
					count++;
					bool flagPut = true;
					if (!temp && count > 10 && i == 1) {
						int tx = rand() % 10, ty = rand() % 10;
						while (!allowed[ty][tx]) {
							tx = rand() % 10, ty = rand() % 10;
						}
						EnemyField[ty][tx] = true;
						temp = true;
					}
					if (!temp)
						continue;
					if (i!=1)
						CircleShip(x, y);
					else
						CircleSingle(x, y);
					ships[i]--;
					flag = false;
				}
			}
		}
		flagTest = false;
	}
	if (CheckingDiag(EnemyField) && CheckingCnt(EnemyField)) {
		return;
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++)
			EnemyField[i][j] = false;
	}
	FillBot();
}

// YOU NEED TO KNOW ABOUT THE PREVIOUS MOVE THE COORDINATES OF THE PREVIOUS MOVE, THE RESULT (hit or kill)

int prevx = -1, prevy = -1;
int prevRes;

enum result {lost, injured, killed} a;
enum orient {vert, gor} b; // ship orientation
vector<pair<int, int>> coord;
bool used_ship[10][10];
vector<pair<int, int>> wanted;
void search_ship(int x, int y, bool f[10][10]) {
	if (used_ship[y][x] || !f[y][x]) // we have already seen this cell or there's no ship here
		return;
	used_ship[y][x] = true;
	coord.push_back({ x, y });
	if (IsOk({ x-1,y })) {
		search_ship(x - 1, y, f);
	}
	if (IsOk({ x + 1,y })) {
		search_ship(x + 1, y, f);
	}
	if (IsOk({ x ,y - 1 })) {
		search_ship(x , y - 1, f);
	}
	if (IsOk({ x ,y + 1 })) {
		search_ship(x, y + 1, f);
	}
}

// the coordinates of the last shot, the opponent's field  and my version of the opponent's field
bool isKilled(int x, int y, bool f[10][10], bool alreadyShot[10][10]) {
	coord.clear();
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			used_ship[i][j] = false;
		}
	}
	search_ship(x, y, f);
	for (auto x : coord) {
		if (alreadyShot[x.second][x.first] != true) {
			return false;
		}
	}
	return true;
}

void rand_shot(int &tx, int &ty) {
	tx = rand() % 10, ty = rand() % 10;
	while (UBot[tx][ty]) {
		tx = rand() % 10, ty = rand() % 10;
	}
	if (!MyField[ty][tx]) {
		prevRes = lost;
		FillMiss(tx, ty, Frame);
	}
	else {
		countBot++;
		if (isKilled(tx, ty, MyField, UBot)) {
			prevRes = killed;
			for (auto x : coord) {
				FillKill(x.first, x.second, Frame);
			}
			wanted.clear();
		}
		else {
			prevRes = injured;
			FillHit(tx, ty, Frame);
		}
	}
	prevx = tx, prevy = ty;
	if (prevRes == lost) {
		myStep = !myStep;
	}
}

void count_shots(int x, int y, bool f[10][10]) {

}

int how_many_shot(int x, int y, bool f[10][10], bool alreadyShot[10][10]) {
	coord.clear();
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			used_ship[i][j] = false;
		}
	}
	search_ship(x,y,f);
	int counter = 0;
	for (auto x : coord) {
		if (alreadyShot[x.second][x.first] == true) {
			counter++;
		}
	}
	return counter;
}

vector<pair<int, int>> possible;

void easy_bot() {
	int tx = possible[0].first, ty = possible[0].second;
	UBot[ty][tx] = true;
	possible.erase(possible.begin());
	if (MyField[ty][tx] == true) {
		countBot++;
		if (isKilled(tx, ty, MyField, UBot)) {
			search_ship(tx, ty, MyField);
			for (auto x : coord) {
				FillKill(x.first, x.second, Frame);
			}
		}
		else {
			FillHit(tx, ty, Frame);
		}
		easy_bot();
	}
	else {
		FillMiss(tx, ty, Frame);
		myStep = !myStep;
	}
	//Mouse1(0,0,0,0);
}

void step_bot()
{
	srand(time(0));	
	int tx = 0, ty = 0; // bot target
	if (prevx == -1 && prevy == -1) { // very first step
		rand_shot(tx, ty);
		if (prevRes == injured) {
			vector<pair<int, int>> a = { {tx - 1,ty},{tx + 1,ty},{tx, ty + 1},{tx,ty - 1} };
			random_shuffle(a.begin(), a.end());
			for (int i = 0; i < 4; i++)
				wanted.push_back(a[i]);
		}
	}
	else { // not the first move and the last move killed or missed
		if (prevRes == lost || prevRes == killed) {
			rand_shot(tx, ty);
			if (prevRes == injured) {
				vector<pair<int, int>> a = { {tx - 1,ty},{tx + 1,ty},{tx, ty + 1},{tx,ty - 1} };
				random_shuffle(a.begin(), a.end());
				for (int i = 0; i < 4; i++) {
					if (IsOk(a[i]))
						wanted.push_back(a[i]);
				}
			}
		}
		else { // we've injured by previous step
			int howManyShot = how_many_shot(prevx, prevy, MyField, UBot);
			if (howManyShot == 1) {
				tx = wanted[0].first;
				ty = wanted[0].second;
				wanted.erase(wanted.begin());
				if (!MyField[ty][tx]) {
					prevRes = lost;
					FillMiss(tx, ty, Frame);
				}
				else {
					countBot++;
					if (isKilled(tx, ty, MyField, UBot)) {
						prevRes = killed;
						for (auto x : coord) {
							FillKill(x.first, x.second, Frame);
						}
						wanted.clear();
					}
					else {
						int orientation;
						int cnt = coord[0].first;
						int cnt2 = coord[0].second;
						for (int i = 0; i < (int)coord.size(); i++) {
							if (coord[i].first == cnt) {
								orientation = gor;
							}
							if (coord[i].second == cnt) {
								orientation = vert;
							}
						}
						search_ship(tx, ty, MyField);
						if (orientation == vert) {
							sort(coord.begin(), coord.end());
							if (IsOk({ coord[0].first, coord[0].second - 1 })) {
								wanted.push_back({ coord[0].first,coord[0].second - 1 });
							}
							int size = coord.size();
							if (IsOk({ coord[size - 1].first, coord[size - 1].second + 1 })) {
								wanted.push_back({ coord[size - 1].first, coord[size - 1].second + 1 });
							}
						}
						else { // horizontal orientation
							sort(coord.begin(), coord.end());
							if (IsOk({ coord[0].first-1, coord[0].second})) {
								wanted.push_back({ coord[0].first - 1,coord[0].second });
							}
							int size = coord.size();
							if (IsOk({ coord[size - 1].first+1, coord[size - 1].second })) {
								wanted.push_back({ coord[size - 1].first + 1, coord[size - 1].second });
							}
						}
						prevRes = injured;
						FillHit(tx, ty, Frame);
					}
				}
				prevx = tx, prevy = ty;
				if (prevRes == lost) {
					myStep = !myStep;
				}
			}
			else {
				coord.clear();
				search_ship(prevx, prevy, MyField);
			}
		}
	}
	UBot[tx][ty] = true;
	if (!myStep) { // now it's bot turn
		step_bot();
	}
}