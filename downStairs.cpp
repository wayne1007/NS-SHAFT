// T10_Project_B : Let's Play Games!
// �\�� : ���ѵ��U�n�J�\��A��ܹC���ñN�̰����Z��J�Ʀ�]
// �C�� :	(1) �p�B�ͤU�ӱ�  ( Developed by ���f�� ) 
//	   		
// �}�o�ɶ� : 2020-06-07 �̫�ק�ɶ��G 2020-06-16

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

//�p�B�ͤU�ӱ�
class DOWNSTAIR {
public:
	DOWNSTAIR(int h, int w);			//constructor
	~DOWNSTAIR() {						//distructor
		for (int i = 0; i < MAX_H; ++i)	// �N�ʺA�t�m���O��������
			delete[] map[i];
		delete[] map;
	}

	void startGame();		// �}�l�C��
	void initailizeMap();	// ��l�Ƶe��
	void displayTheMap();	// ��X�e��
	void movePos();			// ���ʤH��
	void game();			// �C������
	void update();			// �Ϧa�ϫ��򩹤W��
	bool isFlying();		// �P�_�H���O�_�a��
	bool isEdge(int);		// �P�_���k�O�_�O���
	bool isDead();			// �P�_�O�_���`
	int getScore() { return score; }

private:
	int MAX_H;				// �a�ϰ���
	int MAX_W;				// �a�ϼe��
	int **map;				// �s��a�Ϫ����� ( �G���}�C )
	int playerPosition_x;	// �H���� x �y�� ( ��V�y�� ) 
	int playerPosition_y;	// �H���� y �y�� ( �����y�� )
	int score;				// ����
	string shap[7] = { "��","�U","�s","�@","��","��" }; // �a�Ϥ��U���餧�ϧ�
};

// �|�����
struct MemberRecord {
	char name[40];
	char password[20];
	char email[30];
	char IDNumber[10];
};

// �Ʀ�]
struct RankRecord {
	char game[30];
	char IDNumber[5][10];
	int score[5];
};

//bool bigger(int a, int b) { return a > b; }	
void printGame(const RankRecord rankTable);				// �L�X�ӱƦ�]���C���W��
void printID(const RankRecord rankTable, int index);	// �L�X�Ʀ�]�W�ӦW�����H��ID
void sort(RankRecord &rankTable);						// �N�Ʀ�]�̷Ӥ��ƱƧ�

void loadMemberInfo(MemberRecord memberDetails[], int &numMembers);					// �q�ɮ�Ū�J�Ҧ��|�����
void loadRank(RankRecord rankTable[], int &numGames);								// �q�ɮ�Ū�J�Ʀ�]
void signIn(MemberRecord memberDetails[], int &numMembers,							// �ϥΪ̵n�J �åi��ܨϥΪ̥\��
	RankRecord rankTable[], int &numTables);
bool illegal(char email[], char password[],											// �P�_�n�J��ƬO�_���~
	MemberRecord memberDetails[], int numMembers, int &recordNumber);
void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber);	// ���ϥΪ̧��|�����
void newMember(MemberRecord memberDetails[], int &numMembers);						// ���U�|��
bool existingID(char newID[], MemberRecord memberDetails[], int &numMembers);		// �P�_ID�O�_�w�s�b
bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers);	// �P�_email�O�_�w�s�b

void gameList(MemberRecord &memberDetails, RankRecord rankTable[], int &numTables);	// ���ѹC���C��
void downStairGame(MemberRecord &memberDetails, RankRecord &rankTable);				// �p�B�ͤU�ӱ�
void displayRanks(RankRecord rankTable[], int numRanks);							// �i�ܱƦ�]


void saveMemberInfo(MemberRecord memberDetails[], int numMembers);					// �x�s�|����ƨ��ɮ�
void saveRank(RankRecord rankTable[], int &numGames);								// �x�s�Ʀ�]���ɮ�

int main()
{
	srand(time(0));								// �üƺؤl

	MemberRecord memberDetails[20] = {};
	int numMembers = 0;
	loadMemberInfo(memberDetails, numMembers);	// Ū���|�����

	RankRecord rankTable[5] = {};
	//memcpy(rankTable[0].game, "�p�B�ͤU�ӱ�", strlen("�p�B�ͤU�ӱ�"));
	//memcpy(rankTable[1].game, "�q���ƦC", strlen("�q���ƦC"));
	int numTables = 0;
	loadRank(rankTable, numTables);				// Ū���Ʀ�]

	cout << "Welcome to T10 Project_B : Let's Play Game!" << endl;

	int choice;

	while (true)
	{
		// �ϥΪ̿��
		cout << "\nEnter your choice:" << endl;
		cout << "1. Sign In\n";
		cout << "2. New Member\n";
		cout << "3. End\n? ";
		cin >> choice;
		cout << endl;

		switch (choice)
		{
		case 1:	// �n�J
			signIn(memberDetails, numMembers, rankTable, numTables);
			break;

		case 2:	// ���U�s�|��
			newMember(memberDetails, numMembers);
			break;

		case 3:	// �x�s�����}
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

// �q�ɮ�Ū�J�Ҧ��|�����
void loadMemberInfo(MemberRecord memberDetails[], int &numMembers)
{
	// �H�G���ɶ}��
	ifstream inMemberDetails("memberData.dat", ios::in | ios::binary);

	if (!inMemberDetails) {
		cout << "<< New Member >>\n";
		newMember(memberDetails,numMembers);
	}
	else{

	// �N�|����Ƥ@���@��Ū�J �P�ɭp��|����
	while (inMemberDetails.read(reinterpret_cast<char *>(&memberDetails[numMembers]), sizeof(MemberRecord)))
		++numMembers;
}
	inMemberDetails.close();
}

// �q�ɮ�Ū�J�Ʀ�]
void loadRank(RankRecord rankTable[], int &numGames)
{
	// �H�G���ɶ}��
	ifstream inRankTable("rankTable.dat", ios::in | ios::binary);

	if (!inRankTable) {
		memcpy(rankTable[0].game, "�p�B�ͤU�ӱ�", strlen("�p�B�ͤU�ӱ�"));
		//memcpy(rankTable[1].game, "�q���ƦC", strlen("�q���ƦC"));
		numGames = 1;
	}

	else	// �N�Ʀ�]��Ƥ@���@��Ū�J
		while (inRankTable.read(reinterpret_cast<char *>(&rankTable[numGames]), sizeof(MemberRecord)))
			++numGames;

	inRankTable.close();
}

// �ϥΪ̵n�J �åi��ܨϥΪ̥\��
void signIn(MemberRecord memberDetails[], int &numMembers, RankRecord rankTable[], int &numTables)
{
	char email[30] = {}, password[20] = {};
	int recordNumber = 0;	// �����n�J�|������Ʀ�m(index)

	cout << "Enter email address: ";
	cin >> email;
	cout << "Enter password: ";
	cin >> password;

	// �O�_���\�n�J
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
		// �|���\����
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
		case 1:	// ���|�����
			accountInfor(memberDetails, numMembers, recordNumber);
			break;
		case 2:	// �i�J�C�����
			gameList(memberDetails[recordNumber], rankTable, numTables);
			break;
		case 3:	// �d�ݱƦ�]
			displayRanks(rankTable, numTables);
			break;

		case 4:	// �n�X
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

// �P�_�n�J��ƬO�_���~
bool illegal(char email[], char password[], MemberRecord memberDetails[], int numMembers, int &recordNumber)
{
	for (int i = 0; i < numMembers; i++) {
		// �������|����Ƥ���email���L�۲�
		if (memcmp(email, memberDetails[i].email, 30) == 0)
			// ����email���K�X�O�_���T
			if (memcmp(password, memberDetails[i].password, 20) == 0) {
				recordNumber = i;
				return false;
			}
	}
	return true;
}

// ���ϥΪ̧��|�����
void accountInfor(MemberRecord memberDetails[], int numMembers, int recordNumber)
{
	// �|�����
	cout << "1. Name: " << memberDetails[recordNumber].name << endl;
	cout << "2. Email Address: " << memberDetails[recordNumber].email << endl;
	cout << "3. Password: " << memberDetails[recordNumber].password << endl;
	cout << "4. ID Number: " << memberDetails[recordNumber].IDNumber << endl;

	cout << "\nWhich one do you want to modify (0 �V not modify)? ";
	while (true) {
		char email[30] = {}, ID[10] = {};

		int choice;
		cin >> choice;

		switch (choice)
		{
		case 0:	// do nothing
			return;

		case 1:	// ���W�r
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber].name;
			return;

		case 2:	// ���email
			cout << "Enter correct data: ";
			cin >> email;
			while (existingEmail(email, memberDetails, numMembers)) {
				cout << "\nAn account already exists with the Email Address!\n\n";
				cout << "Enter correct data: ";
				cin >> email;
			}
			memcpy(memberDetails[recordNumber].email, email, 30);
			return;

		case 3:	// ���K�X
			cout << "Enter correct data: ";
			cin >> memberDetails[recordNumber].password;
			return;

		case 4:	// ���ID
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

// ���U�|��
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

// �P�_ID�O�_�w�s�b
bool existingID(char newID[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++) {
		if (memcmp(newID, memberDetails[i].IDNumber, 10) == 0)
			return true;
	}
	return false;
}

// �P�_email�O�_�w�s�b
bool existingEmail(char newEmail[], MemberRecord memberDetails[], int &numMembers)
{
	for (int i = 0; i < numMembers; i++) {
		if (memcmp(newEmail, memberDetails[i].email, 30) == 0)
			return true;
	}
	return false;
}

// ���ѹC���C��
void gameList(MemberRecord &memberDetails, RankRecord rankTable[], int &numTables)
{
	// �C�����
	cout << "------------------------------------\n";
	cout << " 1. "; printGame(rankTable[0]); cout << endl;
	cout << "Which game do you want to play\n? ";

	int choice;
	cin >> choice;

	switch (choice) {
	case 1:	// �p�B�ͤU�ӱ�
		downStairGame(memberDetails, rankTable[0]);
		break;
	
	default:
		cout << "Input Error! Please try again: ";
		break;
	}
}

// �p�B�ͤU�ӱ�
void downStairGame(MemberRecord &memberDetails, RankRecord &rankTable)
{
	DOWNSTAIR newGame(15, 17);	// �ŧi����
	newGame.startGame();		//�}�l�C��

	int newScore = newGame.getScore();

	// �Y��Ĥ��W(�̧C��)���� ���N�� �i�J�Ʀ�]
	if (rankTable.score[4] < newScore) {
		memcpy(rankTable.IDNumber[4], memberDetails.IDNumber, 10);
		rankTable.score[4] = newScore;
		sort(rankTable); //�Ƨ�
	}
}

// �i�ܱƦ�]
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

// �x�s�|�����
void saveMemberInfo(MemberRecord memberDetails[], int numMembers)
{
	// �H�G���ɶ}��
	ofstream outMemberInfor("memberData.dat", ios::out | ios::binary);

	if (!outMemberInfor) {
		cerr << "\"memberData.dat\" could not be opened." << endl;
		exit(1);
	}

	// �x�s�C�����
	for (int i = 0; i < numMembers; i++) {
		outMemberInfor.write(reinterpret_cast<char *>(&memberDetails[i]), sizeof(MemberRecord));
	}
	outMemberInfor.close();
}

// �x�s�Ʀ�]���ɮ�
void saveRank(RankRecord rankTable[], int &numGames)
{
	// �H�G���ɶ}��
	ofstream outRankTable("rankTable.dat", ios::out | ios::binary);

	if (!outRankTable) {
		cerr << "\"rankTable.dat\" could not be opened." << endl;
		exit(1);
	}

	// �x�s�C�����
	for (int i = 0; i < numGames; i++) {
		outRankTable.write(reinterpret_cast<char *>(&rankTable[i]), sizeof(MemberRecord));
	}
	outRankTable.close();
}

// �L�X�ӱƦ�]���C���W��
void printGame(const RankRecord rankTable)
{
	for (int i = 0; i < 30; ++i) {
		if (rankTable.game[i] == '\0')
			break;
		cout << rankTable.game[i];
	}
}

// �L�X�Ʀ�]�W�ӦW�����H��ID
void printID(const RankRecord rankTable, int index)
{
	for (int i = 0; i < 10; ++i) {
		//if (rankTable.IDNumber[index][i] == ' ')
			//break;
		cout << rankTable.IDNumber[index][i];
	}
}

// �N�Ʀ�]�̷Ӥ��ƱƧ�
void sort(RankRecord &rankTable)
{
	int toIndex = 5;
	// bubble sort
	while (toIndex > 1) {
		--toIndex;
		for (int i = 0; i < toIndex; i++) {
			// �p�G�e����������᭱�������n�p�A�h�洫������m
			if (rankTable.score[i] < rankTable.score[i + 1]) {
				int tmp = rankTable.score[i];
				rankTable.score[i] = rankTable.score[i + 1];
				rankTable.score[i + 1] = tmp;

				// ID��ۤ��Ʋ���
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
	// �]�w�H����l��m
	playerPosition_y = (MAX_H + 1) / 4;
	playerPosition_x = (MAX_W + 1) / 2;

	//�ʺA�t�m�O����
	map = new int*[MAX_H];
	for (int i = 0; i < MAX_H; ++i) {
		map[i] = new int[MAX_W];
	}

}

// �}�l�C��
void DOWNSTAIR::startGame()
{
	this->initailizeMap();	// ��l�Ʀa��

	// �h������
	thread t1(&DOWNSTAIR::movePos, this);// ����H������
	thread t2(&DOWNSTAIR::update, this); // ����e������

	this->game();// �i�J�C��

	t1.join();// ����t1����
	t2.join();// ����t2����
}

// ��l�Ƶe��
void DOWNSTAIR::initailizeMap()
{

	// �W��
	for (int j = 0; j < MAX_W; ++j)
		map[0][j] = 0;

	// �ⰼ���
	for (int i = 1; i < MAX_H - 1; ++i) {
		map[i][0] = 1;
		map[i][MAX_W - 1] = 1;
	}

	//	�U��
	for (int j = 0; j < MAX_W; ++j)
		map[MAX_H - 1][j] = 2;

	// �������
	for (int i = 1; i < MAX_H - 1; ++i) {
		for (int j = 1; j < MAX_W - 1; ++j)
			map[i][j] = 3;
	}

	// �H�����ͦa�O ( ���׬� 3 )
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

// ��ܵe��
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

	cout << "�ثe�o��: " << setw(6) << setfill('0') << score << endl;;
}

// ����Ⲿ��
void DOWNSTAIR::movePos()
{
	int direct;
	while (!isDead()) {
		// ��V��Ѩ�X�զ�
		direct = _getch();
		direct = _getch();	//Ū����V�X
		switch (direct)
		{
		case 75:	// �V��
			if (!isEdge(direct))
				--playerPosition_x;
			break;
		case 77:	// �V�k
			if (!isEdge(direct))
				++playerPosition_x;
			break;
		case 80:	// �V�U �a�ŦV�U�i�[��
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

// �i��C��
void DOWNSTAIR::game()
{
	// �����ɶ�
	clock_t a, b, c;
	a = clock();

	while (!isDead()) {
		b = clock();
		//�C�j0.1���s�e��
		if (b - a > 250) {
			displayTheMap();
			b = a;
		}

		//�H���a�Ů� �۰ʸ��U
		while (isFlying()) {
			c = clock();
			if (c - a > 221) {
				a = c;
				++playerPosition_y;
				displayTheMap();
			}
		}
	}

	cout << "�A���F!  �̫�o��: " << this->getScore() << endl;
	cout << "�����N��V�����}!\n";
}

// �ϵe���۰ʦV�W����
void DOWNSTAIR::update()
{
	clock_t a, b, c;
	int flag = 0;
	a = clock();
	score = 0;
	while (!isDead()) {

		b = clock();
		// �C0.3�� ��Ӧa�ϦV�W�@�C
		if (b - a > 309) {
			if (!isFlying()) {
				--playerPosition_y;
			}
			for (int i = 1; i < MAX_H - 2; ++i)
				for (int j = 1; j < MAX_W - 1; ++j)
					map[i][j] = map[i + 1][j];

			score += 3; //�����H�ɶ��W�[

			// �s�W�̩��U���a�O
			// �C���~���ͦa�O
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
			//�C������ flag ���A
			flag ? flag = 0 : flag = 1;
			a = b;
		}
	}
}

// �P�_�H���O�_�a��
bool DOWNSTAIR::isFlying()
{
	if (map[playerPosition_y + 1][playerPosition_x] == 3)
		return 1;
	return 0;
}

// �P�_���k���ʬO�_��������Φa�O
bool DOWNSTAIR::isEdge(int direct)
{
	if ((direct == 77 && map[playerPosition_y][playerPosition_x + 1] == 4) || (direct == 75 && map[playerPosition_y][playerPosition_x - 1] == 4))
		return 1;
	if ((direct == 77 && playerPosition_x == MAX_W - 2) || (direct == 75 && playerPosition_x == 1))
		return 1;
	return 0;

}

// �P�_�O�_�I��W�ɩΤU��
bool DOWNSTAIR::isDead()
{
	if (playerPosition_y == 0 || playerPosition_y == MAX_H - 2)
		return 1;
	return 0;
}
