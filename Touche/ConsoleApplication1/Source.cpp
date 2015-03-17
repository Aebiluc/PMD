
#include<iostream>

#include<Windows.h>
using namespace std;
void main(void)
{
	short Haut, Bas, Gauche, Droite;
	while (1){
		Gauche = GetAsyncKeyState(VK_LEFT);
		Droite = GetAsyncKeyState(VK_RIGHT);
		Haut = GetAsyncKeyState(VK_UP);
		Bas = GetAsyncKeyState(VK_DOWN);
		if (Gauche != 0)
		{
			cout << "Gauche" << endl;
		}
		if (Droite != 0)
		{
			cout << "Droite" << endl;
		}
		if (Haut != 0)
		{
			cout << "Haut" << endl;
		}
		if (Bas != 0)
		{
			cout << "Bas" << endl;
		}
		Sleep(100);
	}
}
