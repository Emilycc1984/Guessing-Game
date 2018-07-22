#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
	游戏规则表，以游戏人类玩家为基准，1代表胜利，0代表平局，-1代表失败，具体逻辑如下所示：
	+------+------+------+----+
	|      | 剪刀 | 石头 | 布 |
	+------+------+------+----+
	| 剪刀 |  平  |  败  | 胜 |
	+------+------+------+----+
	| 石头 |  胜  |  平  | 败 |
	+------+------+------+----+
	| 布   |  败  |  胜  | 平 |
	+------+------+------+----+

*/
const char game[3][3] =
{
	{ 0,-1,1 },
	{ 1,0,-1 },
	{ -1,1,0 }
};

/*
	这是游戏的打印信息，通过查表输出道具名称
*/
const char *mess[5] = { "剪刀","石头","布" ,"炸弹","护身符" };

/*
	卡牌信息，总共30张牌，剪刀，石头，布各10张，将在init函数中进行初始化
*/
char cards[30];

/*
	player1表示人类玩家，player2表示电脑玩家。
	这是一个数组，其中索引0保存剪刀牌的数量，索引1保存石头牌的数量，索引2保存布牌数量，索引3保存炸弹牌数量，索引4保存护身符牌数量
*/
char player1[5];
char player2[3];

/*
	初始化卡牌信息，逻辑如下：
	设定一个30次的循环，每一次循环体里面，都将循环次数除以3，并把结果赋值给数组的某个索引。
	由于是整型除法，i小于10，结果为0，表示剪刀；i大于10，小于20，结果为1，表示石头；i大于20，小于30，结果为2，表示布。

	洗牌逻辑如下：
	1.以当前时间为随机数种子，初始化随机数
	2.设定100次的循环次数
	3.每次循环，获取a和b两个随机数，用整数30取模后，作为数组索引把卡组的两处牌交换
*/
void init_cards()
{
	int a, b, c;
	for (int i = 0; i<30; i++)
		cards[i] = i / 10;

	srand((unsigned int)time(NULL));
	for (int i = 0; i<100; i++)
	{
		a = rand() % 30;
		b = rand() % 30;
		c = cards[a];
		cards[a] = cards[b];
		cards[b] = c;
	}
}

/*
	发牌函数，前15张给玩家1，后15张给玩家2。
*/

void give_cards()
{
	int a;
	for (int i = 0; i<15; i++)
	{
		a = cards[i];
		player1[a]++;
	}
	player1[3] = 3;
	player1[4] = 3;

	for (int i = 15; i<30; i++)
	{
		a = cards[i];
		player2[a]++;
	}
}

/*
	获取玩家1出的牌，可以出示的牌，分别是剪刀，石头，布和炸弹，对应编号分别是1，2，3，4.
	当输入1，2，3，4以外的数字时，会报输入信息错误
	如果输入为合法信息，则判断对应牌信息是否为0，为0则提示玩家没有这张牌，并让玩家重新输入
	由于输入的是字符串，在ascii码中，0到9的十六进制码分别是0x30到0x39，所以返回的时候减去0x31
*/
char get_player1_card()
{
	char card;
	while (card = getchar())
	{
		getchar();
		if (card != '1' && card != '2' && card != '3'&& card != '4')
		{
			printf("输入错误，请重新出牌\n");
			continue;
		}

		if (player1[card - 0x31] <= 0)
		{
			printf("你没有这张牌，请重新出牌\n");
			continue;
		}
		break;
	}
	return card - 0x31;
}

/*
	玩家2出牌，由于玩家2是电脑玩家，出牌通过随机数推进，过程与玩家1出牌类似
*/
char get_player2_card()
{
	char card;
	while (card = rand() % 3)
	{
		if (card != 0 && card != 1 && card != 2)
		{
			continue;
		}

		if (player1[card] <= 0)
		{
			continue;
		}
		break;
	}
	return card;
}

/*
	护身符牌，当玩家1遇到失败或者平局时，如果玩家1拥有护身符牌，则会自动询问是否使用护身符牌，如果选择是，则消耗一张护身符牌，把失败的牌拿回手中。
*/
void amulet(char card)
{
	char ch;
	if (player1[4] > 0)
	{
		printf("你有%d张护身符，是否使用？(y/N)\n", player1[4]);
	a:
		ch = getchar();
		if (ch == 'y' || ch == 'Y')
		{
			player1[card]++;
			player1[4]--;
			getchar();
		}
		else if (ch == 'n' || ch == 'N')
			getchar();
		else
		{
			if (ch == '\n')
				return;
			printf("输入错误，请重新输入！\n");
			goto a;
		}
	}
}

/*
	运行游戏，在运行前，先初始化游戏信息，包括洗牌和发牌。
	游戏开始时，先获取双方出的牌，如果玩家1出了炸弹，则这一局按平局算
	如果玩家1遇到失败或者平局，当手上有护身符牌时，会询问是否使用，如果玩家1选择使用，则可把失败的那一张牌拿回，并消耗一张护身符牌
	当任意一方的牌数为0时，则那一方算失败方
*/
void run_game()
{
	char a, b;
	init_cards();
	give_cards();
	printf("你剩余15张牌\n");
	printf("对方剩余15张牌\n");
	printf("\n");

	while (1)
	{
		printf("剪刀：%d\n", player1[0]);
		printf("石头：%d\n", player1[1]);
		printf("布：%d\n", player1[2]);
		printf("炸弹：%d\n", player1[3]);
		printf("护身符：%d\n", player1[4]);
		printf("\n");

		printf("请出牌：\n");
		printf("\t1.剪刀\n");
		printf("\t2.石头\n");
		printf("\t3.布\n");
		printf("\t4.炸弹\n");
		a = get_player1_card();
		b = get_player2_card();
		system("cls");
		printf("%s vs %s\n", mess[a], mess[b]);
		if (a == 3)
		{
			player1[a]--;
			player2[b]--;
			printf("你使用了炸弹！\n");
			goto a;
		}
		switch (game[a][b])
		{
		case -1:
			player1[a]--;
			printf("败！\n");
			amulet(a);
			break;
		case 0:
			player1[a]--;
			player2[b]--;
			printf("平！\n");
			amulet(a);
			break;
		case 1:
			player2[b]--;
			printf("胜！\n");
			break;
		default:
			break;
		}
	a:
		a = player1[0] + player1[1] + player1[2];
		b = player2[0] + player2[1] + player2[2];
		if (a <= 0)
		{
			printf("你输了!\n");
			break;
		}
		else if (b <= 0)
		{
			printf("你赢了!\n");
			break;
		}
		else
		{
			printf("\n");
			printf("你剩余%d张牌\n", a);
			printf("对方剩余%d张牌\n", b);
			printf("\n");
		}
	}
}

/*
	main函数，游戏的入口函数
*/
int main()
{
	run_game();
	system("pause");
	return 0;
}