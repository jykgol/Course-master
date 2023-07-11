#include <iostream>
#include <string>
#include <functional>
#include <windows.h>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <fstream>


//#include <random>
using namespace std;

int menu, menu1, menuFirst,menuchange;
string baza[10][10][4];//[0][0][0] login; [0][0][1] parol; [0][0][2] количество входов в систему
string bazadmin[2];

char ch_pas1[]="abcdefghijklmnopqrst";

string shifr(string pas) 
{
	char mat[20][20];
	string shifrpas;

	pas = pas+'0';		//добавляем символ конца строки
	int l,i,j,k,y;

	l = pas.length();
	if(l%3==0)
		y=(l/3)+1;
	else
		y=(l/3)+2;

	for (i=0;i<y;i++)		//заполняем матрицу мусором из маленьких английских букв
	{
		for (j=0;j<y;j++)
		{	mat[i][j]=(rand()%26+0x61);
		//cout<<mat[i][j]<<"   ";
		}
		//cout<<endl;
	}
	//cout<<endl;

	for (i=0, j=y-1, k=0; i<y && k<l; i++,k++)	//начинаем записывать наш пароль в матрицу (последний столбец сверху вниз)
	{
		mat [i][j] = pas[k];
	}

	for (i=y-1, j=y-2,k=y ; j>-1 && k<l ;k++, j--)		//нижняя строка справо налево
	{
		mat [i][j]=pas[k];
	}

	for (i=y-2, j=1, k=2*y-1 ;i>-1 && k<l ;k++, i--, j++)		//побочная диагональ снизу вверх
	{
		mat [i][j]=pas[k];
	}


	/*	for (i=0;i<y;i++)			//выводим получившуюся матрицу с нашим паролем в ней
	{
	for (j=0;j<y;j++)
	{
	cout<<mat[i][j]<<"   ";
	}
	cout<<endl;
	}*/

	for (i=0;i<y;i++)		//записываем матрицу в вектор как зашифрованный пароль
	{
		for(j=0;j<y;j++)
		{
			shifrpas=shifrpas+mat[i][j];
		}
	}
	//cout<<shifrpas<<endl;

	return shifrpas;
}

string deshifr(string shifrpas)
{
	cout<<endl;
	char mat[20][20];
	string pas;
	int i,j,k,l,y;

	k=0;
	l=shifrpas.length();
	y=sqrt((double)l);
	for (i=0;i<y;i++)  //получили нашу матрицу которая была при шифровании
	{
		for (j=0;j<y;j++)
		{
			mat[i][j]=shifrpas[k];
			k++;
		}
	}

	/*	for (i=0;i<y;i++)		//просто проверка (вывод получившейся матрицы)
	{
	for (j=0;j<y;j++)
	{
	cout<<mat[i][j]<<"   ";
	}
	cout<<endl;
	}

	cout<<endl;*/


	for (i=0, j=y-1 ; i<y ;i++)	//начинаем записывать наш пароль из матрицы (последний столбец) 
	{
		pas = pas+mat [i][j];
	}

	for (i=y-1, j=y-2 ; j>-1 && mat[i][j]!='0' ;j--)		//нижняя строка справо налево пишем до символа "0"
	{
		pas = pas+mat [i][j];
	}

	for (i=y-2, j=1; i>-1 && mat[i][j]!='0'; i--, j++)		//побочная диагональ снизу вверх  пишем до символа "0"
	{
		pas = pas+mat [i][j];
	}

	//cout<<pas<<"  <- parol"<<endl;  //выводим получившийся пароль

	return pas;

}

int baz_pas_check(string pas)
{	int sovpal;
if (pas.length()<8)
{cout<<" error: Пароль не меньше 8 символов"<<endl;return 0;} 

for (int i=0;i<pas.length();i=i+2)
{
	sovpal=0;
	for (int j=0;j<sizeof(ch_pas1);j++)
	{
		if (pas[i]==ch_pas1[j])
		{	
			sovpal=1;
			break;
		}
	}
	if (sovpal==0)
	{
		cout<<"error: Первые 20 букв латинского алфавита"<<endl;
		return 0;
	}
}

return 1;

}

int baz_log_equal(string log) //проверка занятости или сравнение логина 
{
	for(int i=0;i<10;i++) //stroka
	{
		for(int j=0;j<10;j++) //stolbec
		{
			if (log==baza[i][j][0])
			{return 1;}
		}
	}
	return 0;
}
int baz_log_reset(string log) //проверка занятости или сравнение логина 
{
	for(int i=0;i<10;i++) //stroka
	{
		for(int j=0;j<10;j++) //stolbec
		{
			if (log==baza[i][j][0])
			{
				baza[i][j][3].clear();
				return 1;
			}
		}
	}
	return 0;
}
int baz_aut_check(string log,string pas) //проверка занятости или сравнение логина 
{
	for(int i=0;i<10;i++) //stroka
	{
		for(int j=0;j<10;j++) //stolbec
		{
			if (log==baza[i][j][0])
			{
				if (baza[i][j][3].length()>2)
				{
					cout<<"Вы превысили количество попыток входа, обратитесь к admin"<<endl;
					return 0;
				}

				if(pas==deshifr(baza[i][j][1]))
				{

					baza[i][j][2]=baza[i][j][2]+'a';
					cout<<"Вы успешно зашли в систему"<<endl;
					return 1;
				}
				else
				{	
					baza[i][j][3]=baza[i][j][3]+'a';
					cout<<"Неправильный пароль"<<endl;
					return 0;
				}
			}
		}
	}
	cout<<"Неправильный логин"<<endl;
	return 0;
}

int baz_zapis(string log, string shifrpas)
{
	for(int i=0;i<10;i++) //stroka
	{	
		for(int j=0;j<10;j++) //stolbec
		{
			if (baza[i][j][0].empty())
			{
				baza[i][j][0]=log;
				baza[i][j][1]=shifrpas;
				return 1;
			}

		}
	}

	return 0;
}

int reg()
{
	string log;
	string pas,shifrpas;

	cout<<"vvedite login"<<endl;
	cin>>log;

	if (baz_log_equal(log)==1) //check login na sovpadenie s bazoy
	{cout<<"error: Твой логин уже занят"<<endl;return 0;}
	else 
		cout<<" логин свободен "<<endl;

	cout<<"Введите пароль по правилам только первые 20 букв латинского алфавита"<<endl;
	cin>>pas;
	if(baz_pas_check(pas)==1)
		cout<<"password ok"<<endl;
	else
	{cout<<"error: Начни заново"<<endl;return 0;}

	if(log==pas)
	{cout<<"error: Пароль и логин должны быть разные"<<endl;return 0;}

	shifrpas=shifr(pas);

	if (baz_zapis(log,shifrpas)==1)
		cout<<"Ты зарегистрирован"<<endl;

	return 1;
}//end of reg

int aut()
{
	string log, pas;

	cout<<"Введите логин"<<endl;
	cin>>log;
	cout<<"Введите пароль"<<endl;
	cin>>pas;
	if(baz_aut_check(log,pas)==1)
	{
		cout<<"Поздравляем"<<endl;
		return 1;
	} 
	else 
		return 0;
}

int autadmin()
{

	string log,pas;

	cout<<"Введите логин"<<endl;
	cin>>log;
	cout<<"Введите пароль"<<endl;
	cin>>pas;

	if (log==bazadmin[0])
	{
		if(pas==deshifr(bazadmin[1]))
		{
			cout<<"Вы успешно зашли в систему"<<endl;
			return 1;
		}
		else
		{
			cout<<"Неправильный пароль"<<endl;

			return 0;
		}
	}
	cout<<"Неправильный логин"<<endl;
	return 0;
}
int task ()
{
	int min = baza[0][0][2].length(); //по умолчанию первый пользователь имеет min входов
	string min_user = baza[0][0][0];

	for(int i=0;i<10;i++) //stroka
	{
		for(int j=0;j<10;j++) //stolbec
		{
			if(baza[i][j][0].empty()==0 && baza[i][j][2].length()<min)	
			{
				min = baza[i][j][2].length();
				min_user = baza[i][j][0];
			}
		}
	}
	cout<<min<<" <- количество входов"<<endl<<endl;
	cout<<min_user<<" <- пользователь с минимальным количеством входов в систему"<<endl;
	return 0;
}


int change()
{
	string log,pas,pas2;
	cout<<"Введите логин пользователя -> ";
	cin>>log;
	cout<<endl;

	for(int i=0;i<10;i++) //stroka
	{
		for(int j=0;j<10;j++) //stolbec
		{
			if (log==baza[i][j][0])
			{
				do
				{
					menuchange=0;
					cout<<"1:изменить логин"<<endl;
					cout<<"2:изменить пароль"<<endl;
					cout<<"0:exit"<<endl;
					cin>>menuchange;

					switch(menuchange)
					{
					case 1:
						cout<<"введите новый логин"<<endl;
						cin>>log;
						if (!baz_log_equal(log))
							baza[i][j][0]=log;
						else
							cout<<"такой логин уже занят"<<endl;
						break;
					case 2:
						cout<<"введите старый пароль"<<endl;
						cin>>pas;
						if(pas==deshifr(baza[i][j][1]))
						{
							cout<<"введите новый пароль"<<endl;
							cin>>pas;
							if (baz_pas_check(pas)==1)
							{
								cout<<"введите новый пароль снова"<<endl;
								cin>>pas2;
								if (pas==pas2)
								{
									baza[i][j][1]=shifr(pas);
								}
								else
								{
									cout<<"error: очепатка, и ты не попал на тв"<<endl;
									Sleep(2000);
									cout<<"в смысле я хочу сказать пароли не совпадают"<<endl;
								}
							}

						}
						else
						{
							cout<<"неверный пароль"<<endl;
						}
						break;


					case 0:break;

					default:break;
					}

				}while(menuchange!=0);
				return 0;
			}
		}
	}
	return 0;
}
int get_users_shifrpas()
{
	for(int i=0;i<10;i++) //stroka
	{
		for(int j=0;j<10;j++) //stolbec
			if(baza[i][j][0].empty()==0)
				cout<<baza[i][j][0]<<"	"<<baza[i][j][1]<<endl;

	}
	return 0;
}
int menuaut()
{
	cout<<" Теперь ты в системе"<<endl;
	return 0;
}
int menuok()
{	string log;
do{
	cout<<"1:Registration"<<endl;
	cout<<"2:My task"<<endl;
	cout<<"3:Users"<<endl;
	cout<<"4:Reset number of tries"<<endl;
	cout<<"5:Change password"<<endl;
	cout<<"6:cls"<<endl;

	cin>>menu;

	switch(menu)
	{

	case 1:reg();break;

	case 2:task();break;

	case 3:get_users_shifrpas();break;

	case 4:	
		cout<<"Введите логин"<<endl;
		cin>>log;
		if (baz_log_reset(log)==1)
			cout<<"Пользователь разблокирован, сброшены попытки входа"<<endl;
		else
			cout<<"Ошибка: Пользователь не найден"<<endl;
		break;

	case 5:	change();break;
	case 6:	system("cls");break;

	default:break;
	}
}while(menu!=0);
return 0;
}

int menuadmin()
{
	cout<<"Авторизация"<<endl;
	if(autadmin()==1)
		menuok();
	return 0;
}
int menuuser()
{
	cout<<"1:Autorization "<<endl;
	if (aut()==1)
		menuaut();

	return 0;
}

int load()
{
	string log,pas;
	ifstream loadlog("log.txt",ios::in);
	ifstream loadpas("pas.txt",ios::in);

	for(int i=0;i<10;i++) //stroka
	{
		for(int j=0;j<10;j++) //stolbec
		{
			loadlog>>log;
			loadpas>>pas;
			if (!loadlog.eof())
			{
				baza[i][j][0]=log;
				baza[i][j][1]=pas;
			}
			else
			{
				cout<<"база успешно загружена"<<endl;
				return 1;
			}
		}
	}
	return 0;

}
int save()
{
	ofstream savelog("log.txt",ios::out);
	ofstream savepas("pas.txt",ios::out);

	for(int i=0;i<10;i++) //stroka
	{
		for(int j=0;j<10;j++) //stolbec
		{
			if (!baza[i][j][0].empty())
			{
				savelog<<baza[i][j][0]+"\n";
				savepas<<baza[i][j][1]+"\n";
			}
			else
			{
				cout<<"база успешно сохранена"<<endl;
				return 1;
			}
		}
	}
	return 0;

}
int main()
{	
	setlocale(LC_ALL,"Russian");
	string admin="admin";
	bazadmin[0]=admin;
	bazadmin[1]=shifr(admin);
	load();
	do
	{
		menuFirst = 0;
		cout<<"1: admin"<<endl;
		cout<<"2: user"<<endl;
		cin>>menuFirst;

		switch(menuFirst)
		{
		case 1: menuadmin();	

			break;

		case 2: menuuser();break;

		default:break;
		}
		save();

	}while(menuFirst!=0);
	system("pause");
	return 0;
}