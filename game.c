#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
	��Ϸ���������Ϸ�������Ϊ��׼��1����ʤ����0����ƽ�֣�-1����ʧ�ܣ������߼�������ʾ��
	+------+------+------+----+
	|      | ���� | ʯͷ | �� |
	+------+------+------+----+
	| ���� |  ƽ  |  ��  | ʤ |
	+------+------+------+----+
	| ʯͷ |  ʤ  |  ƽ  | �� |
	+------+------+------+----+
	| ��   |  ��  |  ʤ  | ƽ |
	+------+------+------+----+

*/
const char game[3][3] =
{
	{ 0,-1,1 },
	{ 1,0,-1 },
	{ -1,1,0 }
};

/*
	������Ϸ�Ĵ�ӡ��Ϣ��ͨ����������������
*/
const char *mess[5] = { "����","ʯͷ","��" ,"ը��","�����" };

/*
	������Ϣ���ܹ�30���ƣ�������ʯͷ������10�ţ�����init�����н��г�ʼ��
*/
char cards[30];

/*
	player1��ʾ������ң�player2��ʾ������ҡ�
	����һ�����飬��������0��������Ƶ�����������1����ʯͷ�Ƶ�����������2���沼������������3����ը��������������4���滤���������
*/
char player1[5];
char player2[3];

/*
	��ʼ��������Ϣ���߼����£�
	�趨һ��30�ε�ѭ����ÿһ��ѭ�������棬����ѭ����������3�����ѽ����ֵ�������ĳ��������
	���������ͳ�����iС��10�����Ϊ0����ʾ������i����10��С��20�����Ϊ1����ʾʯͷ��i����20��С��30�����Ϊ2����ʾ����

	ϴ���߼����£�
	1.�Ե�ǰʱ��Ϊ��������ӣ���ʼ�������
	2.�趨100�ε�ѭ������
	3.ÿ��ѭ������ȡa��b�����������������30ȡģ����Ϊ���������ѿ���������ƽ���
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
	���ƺ�����ǰ15�Ÿ����1����15�Ÿ����2��
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
	��ȡ���1�����ƣ����Գ�ʾ���ƣ��ֱ��Ǽ�����ʯͷ������ը������Ӧ��ŷֱ���1��2��3��4.
	������1��2��3��4���������ʱ���ᱨ������Ϣ����
	�������Ϊ�Ϸ���Ϣ�����ж϶�Ӧ����Ϣ�Ƿ�Ϊ0��Ϊ0����ʾ���û�������ƣ����������������
	������������ַ�������ascii���У�0��9��ʮ��������ֱ���0x30��0x39�����Է��ص�ʱ���ȥ0x31
*/
char get_player1_card()
{
	char card;
	while (card = getchar())
	{
		getchar();
		if (card != '1' && card != '2' && card != '3'&& card != '4')
		{
			printf("������������³���\n");
			continue;
		}

		if (player1[card - 0x31] <= 0)
		{
			printf("��û�������ƣ������³���\n");
			continue;
		}
		break;
	}
	return card - 0x31;
}

/*
	���2���ƣ��������2�ǵ�����ң�����ͨ��������ƽ������������1��������
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
	������ƣ������1����ʧ�ܻ���ƽ��ʱ��������1ӵ�л�����ƣ�����Զ�ѯ���Ƿ�ʹ�û�����ƣ����ѡ���ǣ�������һ�Ż�����ƣ���ʧ�ܵ����û����С�
*/
void amulet(char card)
{
	char ch;
	if (player1[4] > 0)
	{
		printf("����%d�Ż�������Ƿ�ʹ�ã�(y/N)\n", player1[4]);
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
			printf("����������������룡\n");
			goto a;
		}
	}
}

/*
	������Ϸ��������ǰ���ȳ�ʼ����Ϸ��Ϣ������ϴ�ƺͷ��ơ�
	��Ϸ��ʼʱ���Ȼ�ȡ˫�������ƣ�������1����ը��������һ�ְ�ƽ����
	������1����ʧ�ܻ���ƽ�֣��������л������ʱ����ѯ���Ƿ�ʹ�ã�������1ѡ��ʹ�ã���ɰ�ʧ�ܵ���һ�����ûأ�������һ�Ż������
	������һ��������Ϊ0ʱ������һ����ʧ�ܷ�
*/
void run_game()
{
	char a, b;
	init_cards();
	give_cards();
	printf("��ʣ��15����\n");
	printf("�Է�ʣ��15����\n");
	printf("\n");

	while (1)
	{
		printf("������%d\n", player1[0]);
		printf("ʯͷ��%d\n", player1[1]);
		printf("����%d\n", player1[2]);
		printf("ը����%d\n", player1[3]);
		printf("�������%d\n", player1[4]);
		printf("\n");

		printf("����ƣ�\n");
		printf("\t1.����\n");
		printf("\t2.ʯͷ\n");
		printf("\t3.��\n");
		printf("\t4.ը��\n");
		a = get_player1_card();
		b = get_player2_card();
		system("cls");
		printf("%s vs %s\n", mess[a], mess[b]);
		if (a == 3)
		{
			player1[a]--;
			player2[b]--;
			printf("��ʹ����ը����\n");
			goto a;
		}
		switch (game[a][b])
		{
		case -1:
			player1[a]--;
			printf("�ܣ�\n");
			amulet(a);
			break;
		case 0:
			player1[a]--;
			player2[b]--;
			printf("ƽ��\n");
			amulet(a);
			break;
		case 1:
			player2[b]--;
			printf("ʤ��\n");
			break;
		default:
			break;
		}
	a:
		a = player1[0] + player1[1] + player1[2];
		b = player2[0] + player2[1] + player2[2];
		if (a <= 0)
		{
			printf("������!\n");
			break;
		}
		else if (b <= 0)
		{
			printf("��Ӯ��!\n");
			break;
		}
		else
		{
			printf("\n");
			printf("��ʣ��%d����\n", a);
			printf("�Է�ʣ��%d����\n", b);
			printf("\n");
		}
	}
}

/*
	main��������Ϸ����ں���
*/
int main()
{
	run_game();
	system("pause");
	return 0;
}