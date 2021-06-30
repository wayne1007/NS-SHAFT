// T10_Project_B : Let's Play Games!
// 功能 : 提供註冊登入功能，選擇遊戲並將最高成績算入排行榜
// 遊戲 :	(1) 小朋友下樓梯  ( Developed by 黃柏翔 ) 
//	   		
// 開發時間 : 2020-06-07 最後修改時間： 2020-06-16

#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <conio.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <utility>
#include <thread>
#include <windows.h>

using namespace std;

//小朋友下樓梯
class DOWNSTAIR {
public:
	DOWNSTAIR(int h, int w);			//constructor
	~DOWNSTAIR() {						//distructor
		for (int i = 0; i < MAX_H; ++i)	// 將動態配置的記憶體釋放
			delete[] map[i];
		delete[] map;
	}

	void startGame();		// 開始遊戲
	void initailizeMap();	// 初始化畫面
	void displayTheMap();	// 輸出畫面
	void movePos();			// 移動人物
	void game();			// 遊戲本體
	void update();			// 使地圖持續往上動
	bool isFlying();		// 判斷人物是否懸空
	bool isEdge(int);		// 判斷左右是否是牆壁
	bool isDead();			// 判斷是否死亡
	int getScore() { return score; }

private:
	int MAX_H;				// 地圖高度
	int MAX_W;				// 地圖寬度
	int **map;				// 存放地圖的指標 ( 二維陣列 )
	int playerPosition_x;	// 人物的 x 座標 ( 橫向座標 ) 
	int playerPosition_y;	// 人物的 y 座標 ( 垂直座標 )
	int score;				// 分數
	string shap[7] = { "Ｖ","｜","︿","　","＝","Ｏ" }; // 地圖中各物體之圖形
};

// 會員資料
struct MemberRecord {
	char name[40];
	char password[20];
	char email[30];
	char IDNumber[10];
};

// 排行榜
struct RankRecord {
	char game[30];
	char IDNumber[5][10];
	int score[5];
};

//bool bigger(int a, int b) { return a > b; }	
void printGame(const RankRecord rankTable);				// 印出該排行榜的遊戲名稱
void printID(const RankRecord rankTable, int index);	// 印出排行榜上該名次的人的ID
void sort(RankRecord &rankTable);						// 將排行榜依照分數排序

void loadMemberInfo(MemberRecord memberDetails[], int &numMembers);					// 從檔案讀入所有會員資料
void loadRank(RankRecord rankTable[], int &numGames);								// 從檔案讀入排行榜
void signIn(MemberRecord memberDetails[], int &numMembers,							// 使用者登入 並可選擇使用者功能
	RankRecord rankTable[], int &numTables);
bool illegal(char email[], char password[],											// 判斷登入資料是否錯誤
	MemberRecord memberDetails[], int numMembers, int &recordNumber);
void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber);	// 讓使用者更改會員資料
void newMember(MemberRecord memberDetails[], int &numMembers);						// 註冊會員
bool existingID(char newID[], MemberRecord memberDetails[], int &numMembers);		// 判斷ID是否已存在
bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers);	// 判斷email是否已存在

void gameList(MemberRecord &memberDetails, RankRecord rankTable[], int &numTables);	// 提供遊戲列表
void downStairGame(MemberRecord &memberDetails, RankRecord &rankTable);				// 小朋友下樓梯
void displayRanks(RankRecord rankTable[], int numRanks);							// 展示排行榜


void saveMemberInfo(MemberRecord memberDetails[], int numMembers);					// 儲存會員資料到檔案
void saveRank(RankRecord rankTable[], int &numGames);								// 儲存排行榜到檔案

int main()
{
	srand(time(0));								// 亂數種子

	MemberRecord memberDetails[20] = {};
	int numMembers = 0;
	loadMemberInfo(memberDetails, numMembers);	// 讀取會員資料

	RankRecord rankTable[5] = {};
	//memcpy(rankTable[0].game, "小朋友下樓梯", strlen("小朋友下樓梯"));
	//memcpy(rankTable[1].game, "歌詞排列", strlen("歌詞排列"));
	int numTables = 0;
	loadRank(rankTable, numTables);				// 讀取排行榜

	cout << "Welcome to T10 Project_B : Let's Play Game!" << endl;

	int choice;

	while (true)
	{
		// 使用者選單
		cout << "\nEnter your choice:" << endl;
		cout << "1. Sign In\n";
		cout << "2. New Member\n";
		cout << "3. End\n? ";
		cin >> choice;
		cout << endl;

		switch (choice)
		{
		case 1:	// 登入
			signIn(memberDetails, numMembers, rankTable, numTables);
			break;

		case 2:	// 註冊新會員
			newMember(memberDetails, numMembers);
			break;

		case 3:	// 儲存並離開
			saveMemberInfo(memberDetails, numMembers);
			saveRank(rankTable, numTables);
			cout << "Thank you...\n\n";
			system("pause");
			return 0;

		default:
			cout << "Input Error!\n\n";
			break;
		}
	}
	return 0;
}

// 從檔案讀入所有會員資料
void loadMemberInfo(MemberRecord memberDetails[], int &numMembers)
{
	// 以二元檔開啟
	ifstream inMemberDetails("memberData.dat", ios::in | ios::binary);

	if (!inMemberDetails) {
		cout << "<< New Member >>\n";
		newMember(memberDetails,numMembers);
	}
	else{

	// 將會員資料一筆一筆讀入 同時計算會員數
	while (inMemberDetails.read(reinterpret_cast<char *>(&memberDetails[numMembers]), sizeof(MemberRecord)))
		++numMembers;
}
	inMemberDetails.close();
}

// 從檔案讀入排行榜
void loadRank(RankRecord rankTable[], int &numGames)
{
	// 以二元檔開啟
	ifstream inRankTable("rankTable.dat", ios::in | ios::binary);

	if (!inRankTable) {
		memcpy(rankTable[0].game, "小朋友下樓梯", strlen("小朋友下樓梯"));
		//memcpy(rankTable[1].game, "歌詞排列", strlen("歌詞排列"));
		numGames = 1;
	}

	else	// 將排行榜資料一筆一筆讀入
		while (inRankTable.read(reinterpret_cast<char *>(&rankTable[numGames]), sizeof(MemberRecord)))
			++numGames;

	inRankTable.close();
}

// 使用者登入 並可選擇使用者功能
void signIn(MemberRecord memberDetails[], int &numMembers, RankRecord rankTable[], int &numTables)
{
	char email[30] = {}, password[20] = {};
	int recordNumber = 0;	// 紀錄登入會員之資料位置(index)

	cout << "Enter email address: ";
	cin >> email;
	cout << "Enter password: ";
	cin >> password;

	// 是否成功登入
	int tryTimes = 3;
	while (illegal(email, password, memberDetails, numMembers, recordNumber)) {
		if (--tryTimes <= 0) {
			cout << "\n\n << Try too many times! >>\n\n";
			system("pause");
			system("CLS");
			return;
		}
		memset(email, '\0', 30);
		memset(password, '\0', 20);

		cout << "\nSorry, unrecognized email or password. There are "<<tryTimes<<" chance(s) remain.\n\n";
		cout << "Enter email address: ";
		cin >> email;
		cout << "Enter password: ";
		cin >> password;

		
	}

	system("CLS");
	while (true) {
		// 會員功能選單
		cout << "\nEnter your choice: " << endl;
		cout << "1. Account Information" << endl;
		cout << "2. Play Games" << endl;
		cout << "3. Ranks" << endl;
		cout << "4. Sign Out" << endl;
		cout << "?";
		int choice;
		cin >> choice;
		switch (choice)
		{
		case 1:	// 更改會員資料
			accountInfor(memberDetails, numMembers, recordNumber);
			break;
		case 2:	// 進入遊戲選單
			gameList(memberDetails[recordNumber], rankTable, numTables);
			break;
		case 3:	// 查看排行榜
			displayRanks(rankTable, numTables);
			break;

		case 4:	// 登出
			system("CLS");
			cout << "\n\n<< Sign Out Successfully! >>\n\n";
			system("pause");
			system("CLS");
			return;

		default:
			cout << "\nInput Error!\n";
			break;
		}
	}
}

// 判斷登入資料是否錯誤
bool illegal(char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber)
{
	for (int i = 0; i < numMembers; i++) {
		// 比對全部會員資料中的email有無相符
		if (memcmp(email, memberDetails[i].email, 30) == 0)
			// 比對該email的密碼是否正確
			if (memcmp(password, memberDetails[i].password, 20) == 0) {
				recordNumber = i;
				return false;
			}
	}
	return true;
}

// 讓使用者更改會員資料
void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber)
{
	// 會員資料
	cout << "1. Name: " << memberDetails[recordNumber].name << endl;
	cout << "2. Email Address: " << memberDetails[recordNumber].email << endl;
	cout << "3. Password: " << memberDetails[recordNumber].password << endl;
	cout << "4. ID Number: " << memberDetails[recordNumber].IDNumber << endl;

	cout << "\nWhich one do you want to modify (0 – not modify)? ";
	while (true) {
		char email[30] = {}, ID[10] = {};

		int choice;
		cin >> choice;

		switch (choice)
		{
		case 0:	// do nothing
			return;

		case 1:	// 更改名字
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber].name;
			return;

		case 2:	// 更改email
			cout << "Enter correct data: ";
			cin >> email;
			while (existingEmail(email, memberDetails, numMembers)) {
				cout << "\nAn account already exists with the Email Address!\n\n";
				cout << "Enter correct data: ";
				cin >> email;
			}
			memcpy(memberDetails[recordNumber].email, email, 30);
			return;

		case 3:	// 更改密碼
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber].password;
			return;

		case 4:	// 更改ID
			cout << "Enter correct data: ";
			cin >> ID;
			while (existingID(ID, memberDetails, numMembers)) {
				cout << "\nAn account already exists with the ID number!\n\n";

				cout << "Enter correct data: ";
				cin >> ID;
			}
			memcpy(memberDetails[recordNumber].IDNumber, ID, 10);
			return;

		default:
			cout << "Input Error! Please try again: ";
			break;
		}
	}
}

// 註冊會員
void newMember(MemberRecord memberDetails[], int &numMembers)
{
	char email[30] = {}, id[10] = {};

	cout << "Enter your ID number: ";
	cin >> id;
	while (existingID(id, memberDetails, numMembers)) {
		cout << "\nAn account already exists with the ID number!\n\n";

		cout << "Enter your ID number: ";
		cin >> id;
	}
	memcpy(memberDetails[numMembers].IDNumber, id, 10);

	cout << "Enter your name: ";
	cin >> memberDetails[numMembers].name;

	cout << "Enter an email address: ";
	cin >> email;
	while (existingEmail(email, memberDetails, numMembers)) {
		cout << "\nAn account already exists with the Email Address!\n\n";
		cout << "Enter correct data: ";
		cin >> email;
	}
	memcpy(memberDetails[numMembers].email, email, 30);

	cout << "Enter a password: ";
	cin >> memberDetails[numMembers].password;
	system("CLS");
	cout << "\nSuccessful!\n";
	numMembers++;
}

// 判斷ID是否已存在
bool existingID(char newID[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++) {
		if (memcmp(newID, memberDetails[i].IDNumber, 10) == 0)
			return true;
	}
	return false;
}

// 判斷email是否已存在
bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++) {
		if (memcmp(newEmail, memberDetails[i].email, 30) == 0)
			return true;
	}
	return false;
}

// 提供遊戲列表
void gameList(MemberRecord &memberDetails, RankRecord rankTable[], int &numTables)
{
	// 遊戲選單
	cout << "------------------------------------\n";
	cout << " 1. "; printGame(rankTable[0]); cout << endl;
	cout << "Which game do you want to play\n? ";

	int choice;
	cin >> choice;

	switch (choice) {
	case 1:	// 小朋友下樓梯
		downStairGame(memberDetails, rankTable[0]);
		break;
	
	default:
		cout << "Input Error! Please try again: ";
		break;
	}
}

// 小朋友下樓梯
void downStairGame(MemberRecord &memberDetails, RankRecord &rankTable)
{
	DOWNSTAIR newGame(15, 17);	// 宣告物件
	newGame.startGame();		//開始遊戲

	int newScore = newGame.getScore();

	// 若比第五名(最低分)高分 取代之 進入排行榜
	if (rankTable.score[4] < newScore) {
		memcpy(rankTable.IDNumber[4], memberDetails.IDNumber, 10);
		rankTable.score[4] = newScore;
		sort(rankTable); //排序
	}
}

// 展示排行榜
void displayRanks(RankRecord rankTable[], int numTables)
{
	int i = 0;
	system("CLS");
	while (numTables--) {
		cout << "----------";
		printGame(rankTable[i]);
		cout << "-----------\n";
		cout << "Rank #    ID Number     Score\n";
		for (int j = 1; j <= 5; ++j) {
			cout << left << setfill(' ') << "  #" << setw(5) << j << "   ";
			printID(rankTable[i], j - 1);
			cout << right << setw(8) << rankTable[i].score[j - 1] << endl;
		}
		++i;
	}
}

// 儲存會員資料
void saveMemberInfo(MemberRecord memberDetails[], int numMembers)
{
	// 以二元檔開啟
	ofstream outMemberInfor("memberData.dat", ios::out | ios::binary);

	if (!outMemberInfor) {
		cerr << "\"memberData.dat\" could not be opened." << endl;
		exit(1);
	}

	// 儲存每筆資料
	for (int i = 0; i < numMembers; i++) {
		outMemberInfor.write(reinterpret_cast<char *>(&memberDetails[i]), sizeof(MemberRecord));
	}
	outMemberInfor.close();
}

// 儲存排行榜到檔案
void saveRank(RankRecord rankTable[], int &numGames)
{
	// 以二元檔開啟
	ofstream outRankTable("rankTable.dat", ios::out | ios::binary);

	if (!outRankTable) {
		cerr << "\"rankTable.dat\" could not be opened." << endl;
		exit(1);
	}

	// 儲存每筆資料
	for (int i = 0; i < numGames; i++) {
		outRankTable.write(reinterpret_cast<char *>(&rankTable[i]), sizeof(MemberRecord));
	}
	outRankTable.close();
}

// 印出該排行榜的遊戲名稱
void printGame(const RankRecord rankTable)
{
	for (int i = 0; i < 30; ++i) {
		if (rankTable.game[i] == '\0')
			break;
		cout << rankTable.game[i];
	}
}

// 印出排行榜上該名次的人的ID
void printID(const RankRecord rankTable, int index)
{
	for (int i = 0; i < 10; ++i) {
		//if (rankTable.IDNumber[index][i] == ' ')
			//break;
		cout << rankTable.IDNumber[index][i];
	}
}

// 將排行榜依照分數排序
void sort(RankRecord &rankTable)
{
	int toIndex = 5;
	// bubble sort
	while (toIndex > 1) {
		--toIndex;
		for (int i = 0; i < toIndex; i++) {
			// 如果前面的元素比後面的元素要小，則交換元素位置
			if (rankTable.score[i] < rankTable.score[i + 1]) {
				int tmp = rankTable.score[i];
				rankTable.score[i] = rankTable.score[i + 1];
				rankTable.score[i + 1] = tmp;

				// ID跟著分數移動
				char tmp2[10] = {};
				memcpy(tmp2, rankTable.IDNumber[i], 10);
				memcpy(rankTable.IDNumber[i], rankTable.IDNumber[i + 1], 10);
				memcpy(rankTable.IDNumber[i + 1], tmp2, 10);
			}
		}
	}
}

// constructor
DOWNSTAIR::DOWNSTAIR(int h, int w)
	:MAX_H(h), MAX_W(w)
{
	// 設定人物初始位置
	playerPosition_y = (MAX_H + 1) / 4;
	playerPosition_x = (MAX_W + 1) / 2;

	//動態配置記憶體
	map = new int*[MAX_H];
	for (int i = 0; i < MAX_H; ++i) {
		map[i] = new int[MAX_W];
	}

}

// 開始遊戲
void DOWNSTAIR::startGame()
{
	this->initailizeMap();	// 初始化地圖

	// 多執行續
	thread t1(&DOWNSTAIR::movePos, this);// 控制人物移動
	thread t2(&DOWNSTAIR::update, this); // 控制畫面移動

	this->game();// 進入遊戲

	t1.join();// 等待t1結束
	t2.join();// 等待t2結束
}

// 初始化畫面
void DOWNSTAIR::initailizeMap()
{

	// 上界
	for (int j = 0; j < MAX_W; ++j)
		map[0][j] = 0;

	// 兩側邊界
	for (int i = 1; i < MAX_H - 1; ++i) {
		map[i][0] = 1;
		map[i][MAX_W - 1] = 1;
	}

	//	下界
	for (int j = 0; j < MAX_W; ++j)
		map[MAX_H - 1][j] = 2;

	// 內部填空
	for (int i = 1; i < MAX_H - 1; ++i) {
		for (int j = 1; j < MAX_W - 1; ++j)
			map[i][j] = 3;
	}

	// 隨機產生地板 ( 長度為 3 )
	for (int i = 6; i < MAX_H; i += 3)
		for (int j = rand() % 3 + 1; j < MAX_W - 3; ++j) {
			if (!(rand() % 4)) {
				map[i][j] = 4;
				map[i][j + 1] = 4;
				map[i][j + 2] = 4;
				j += 4;
			}
		}
}

// 顯示畫面
void DOWNSTAIR::displayTheMap()
{
	system("CLS");
	for (int i = 0; i < MAX_H; ++i) {
		for (int j = 0; j < MAX_W; ++j) {
			if (i == playerPosition_y && j == playerPosition_x)
				map[i][j] = 5;
			cout << shap[map[i][j]];
		}
		cout << endl;
	}

	cout << "目前得分: " << setw(6) << setfill('0') << score << endl;;
}

// 控制角色移動
void DOWNSTAIR::movePos()
{
	int direct;
	while (!isDead()) {
		// 方向鍵由兩碼組成
		direct = _getch();
		direct = _getch();	//讀取方向碼
		switch (direct)
		{
		case 75:	// 向左
			if (!isEdge(direct))
				--playerPosition_x;
			break;
		case 77:	// 向右
			if (!isEdge(direct))
				++playerPosition_x;
			break;
		case 80:	// 向下 懸空向下可加分
			if (isFlying()) {
				++playerPosition_y;
				score += 3;
			}
			break;
		default:
			break;
		}
	}
}

// 進行遊戲
void DOWNSTAIR::game()
{
	// 紀錄時間
	clock_t a, b, c;
	a = clock();

	while (!isDead()) {
		b = clock();
		//每隔0.1秒更新畫面
		if (b - a > 250) {
			displayTheMap();
			b = a;
		}

		//人物懸空時 自動落下
		while (isFlying()) {
			c = clock();
			if (c - a > 221) {
				a = c;
				++playerPosition_y;
				displayTheMap();
			}
		}
	}

	cout << "你死了!  最後得分: " << this->getScore() << endl;
	cout << "按任意方向鍵離開!\n";
}

// 使畫面自動向上捲動
void DOWNSTAIR::update()
{
	clock_t a, b, c;
	int flag = 0;
	a = clock();
	score = 0;
	while (!isDead()) {

		b = clock();
		// 每0.3秒 整個地圖向上一列
		if (b - a > 309) {
			if (!isFlying()) {
				--playerPosition_y;
			}
			for (int i = 1; i < MAX_H - 2; ++i)
				for (int j = 1; j < MAX_W - 1; ++j)
					map[i][j] = map[i + 1][j];

			score += 3; //分數隨時間增加

			// 新增最底下的地板
			// 每兩行才產生地板
			if (flag) {
				for (int j = rand() % 3 + 1; j < MAX_W - 3; ++j)
					if (rand() % 4 == 0) {
						map[MAX_H - 2][j] = 4;
						map[MAX_H - 2][j + 1] = 4;
						map[MAX_H - 2][j + 2] = 4;
						j += 4;
					}
			}
			else {
				for (int j = 1; j < MAX_W - 1; ++j)
					map[MAX_H - 2][j] = 3;
			}
			//每次改變 flag 狀態
			flag ? flag = 0 : flag = 1;
			a = b;
		}
	}
}

// 判斷人物是否懸空
bool DOWNSTAIR::isFlying()
{
	if (map[playerPosition_y + 1][playerPosition_x] == 3)
		return 1;
	return 0;
}

// 判斷左右移動是否撞到牆壁或地板
bool DOWNSTAIR::isEdge(int direct)
{
	if ((direct == 77 && map[playerPosition_y][playerPosition_x + 1] == 4) || (direct == 75 && map[playerPosition_y][playerPosition_x - 1] == 4))
		return 1;
	if ((direct == 77 && playerPosition_x == MAX_W - 2) || (direct == 75 && playerPosition_x == 1))
		return 1;
	return 0;

}

// 判斷是否碰到上界或下界
bool DOWNSTAIR::isDead()
{
	if (playerPosition_y == 0 || playerPosition_y == MAX_H - 2)
		return 1;
	return 0;
}
