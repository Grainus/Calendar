/************************************************************************************
Auteur		: ###############
Description : Affiche le jour de la semaine d'une date, puis un calendrier du mois
				si l'utilisateur le d�sire, tout en v�rifiant la validit� de l'input
Input		: Date
Output		: Jour de la semaine, calendrier du mois
Version		: 1.0
************************************************************************************/

#include <iostream>		//cout et cin
#include <conio.h>		//_getch()
#include <string>
#include <sstream>
#include <Windows.h>

void OutputDown(const std::string str, const HANDLE handle, bool clear = true)
{
	if (clear)
	{
		DWORD dummy;
		FillConsoleOutputCharacter(handle, 32, 0xFFFFF, {}, &dummy);
	}

	CONSOLE_SCREEN_BUFFER_INFO buffInfo;
	GetConsoleScreenBufferInfo(handle, &buffInfo);
	short consoleHeight = buffInfo.srWindow.Bottom - buffInfo.srWindow.Top + 1;
	SetConsoleCursorPosition(handle, { 0, consoleHeight - 1 }); 

	std::cout << str;
	if (clear)
		SetConsoleCursorPosition(handle, {});
}

void Center(const std::string str, const HANDLE handle, short y)
{
	CONSOLE_SCREEN_BUFFER_INFO buffInfo;
	GetConsoleScreenBufferInfo(handle, &buffInfo);
	unsigned int consoleWidth = buffInfo.srWindow.Right - buffInfo.srWindow.Left + 1;

	short x = consoleWidth / 2 - str.size() / 2;
	SetConsoleCursorPosition(handle, { x, y });
	std::cout << str;
}

int main()
{
	//Variables - Constantes
	const std::string JOURS[7] =	{ "Dimanche",	"Lundi",	"Mardi",	"Mercredi", "Jeudi",	"Vendredi", "Samedi" };
	const std::string MOIS[12] =	{ "Janvier",	"F\202vrier",	"Mars", 
									"Avril",		"Mai",			"Juin", 
									"Juillet",		"Ao\226t",		"Septembre", 
									"Octobre",		"Novembre",		"D\202cembre" };
	const int JOURSMOIS[12] =		{31,			0,				31,
									30,				31,				30,
									31,				31,				30,
									31,				30,				31 };

	//Variables - Input
	int jour, mois, annee;
	char seperateur[2];
	bool encore = false;

	//Variables - Traitement
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dummy;
	int jourSemaine, nbJours, joursFevrier;
	char getc;


	do
	{
		FillConsoleOutputCharacter(handle, 32, 0xFFFFF, {}, &dummy);
		SetConsoleCursorPosition(handle, {});
		//Input
		do
		{
			std::cin.ignore(std::cin.rdbuf()->in_avail());
			std::cout << "\n\tEntrez une date (JJ/MM/AAAA) : ";
			std::cin >> jour >> seperateur[0] >> mois >> seperateur[1] >> annee;

			if (std::cin.fail())
			{
				OutputDown("\tVous devez entrer des nombres seulement", handle);
				std::cin.clear();
				continue;
			}
			else if (seperateur[0] != seperateur[1])
			{
				OutputDown("\tVous devez utiliser le m\210me s\202parateur", handle);
				continue;
			}
			else if (seperateur[0] != '-' && seperateur[0] != '/')
			{
				OutputDown("\tVous devez utiliser le s\202parateur '/' ou '-' seulement", handle);
				continue;
			}
			else if (jour <= 0 || jour > 31)
			{
				OutputDown("\tLe jour doit \210tre entre 1 et 31 inclusivement", handle);
				continue;
			}
			else if (mois <= 0 || mois > 12)
			{
				OutputDown("\tLe mois est erron\202 (doit \210tre entre 1 et 12 inclusivement)", handle);
				continue;
			}
			else if (annee < 1901 || annee > 2099)
			{
				OutputDown("\tL'ann\202e est erron\202e (de 1901 \205 2099 seulement)", handle);
				continue;
			}
			else if (mois == 2 && jour > 28)
			{
				int errormax = 0;
				std::string errorstr = "\tLe mois de f\202vrier ne peut compter plus de ";
				if (annee % 4 == 0)
					if (jour > 29)
						errormax = 29;
					else
						break;
				else
					if (jour > 28)
						errormax = 28;
					else
						break;
				errorstr += std::to_string(errormax) + " jours";
				OutputDown(errorstr, handle);
			}
			else if (jour == 31 && JOURSMOIS[mois - 1] == 30)
			{
				OutputDown("\tCe mois ne compte que 30 jours", handle);
				continue;
			}
			else
				break;
		} while (true);
		
		//Traitement
		joursFevrier = (annee % 4 == 0 ? 29 : 28);
		nbJours = joursFevrier + jour;
		for (int i = mois - 1; i > 0; --i)
			nbJours += JOURSMOIS[i - 1];
		jourSemaine = (nbJours + (annee - 1900) + (annee - 1901) / 4) % 7;

		//Output
		FillConsoleOutputCharacter(handle, 32, 0xFFFFF, {}, & dummy);
		SetConsoleCursorPosition(handle, { 8, 3 });		//Affichage
		std::cout << "Le " << jour << '/' << mois << '/' << annee << " est un : " << JOURS[jourSemaine];
		SetConsoleCursorPosition(handle, { 8, 6 });	//Affichage
		std::cout << "Voulez-vous voir le calendrier complet du mois (O/N) ? ";

		getc = ' ';
		while (getc != 'O' && getc != 'N')
		{
			getc = toupper(_getch());
			std::cout << getc << '\b';
		}

		if (getc == 'O')
		{
			FillConsoleOutputCharacter(handle, 32, 0xFFFFF, {}, &dummy);

			//Variables
			const int PADDING = 10;
			const char empty = ' ', filling = ' ';
			int ligne;
			std::stringstream str;
			jourSemaine = (nbJours - (jour - 1) + (annee - 1900) + (annee - 1901) / 4) % 7;		//Premier jours du mois

			str << MOIS[mois - 1] + ' ' + std::to_string(annee);
			const int jourmois = (mois == 2 ? joursFevrier : JOURSMOIS[mois - 1]);
			Center(str.str(), handle, 3);

			str.str(std::string());
			str << JOURS[0].substr(0, 2);
			for (int i = 1; i < 7; ++i)
				str << std::string(PADDING - 2, filling) << JOURS[i].substr(0, 2);
			Center(str.str(), handle, 6);
			

			str.str( std::string(str.str().size(), '_') );
			Center(str.str(), handle, 7);
	
			ligne = 10;
			str.str(std::string());
			int max = jourmois + 7 - ((jourmois + jourSemaine) % 7 - jourSemaine);
			for (int i = 1 - jourSemaine; i <= max; ++i)
			{
				if (str.str().empty())
					str << (i < 10 ? " " : "");
				else
					str << std::string(PADDING - std::to_string(abs(i)).size(), filling); //�quivalent de setw
				if (i > 0 && i <= jourmois)
					str << i;
				else
					str << std::string(std::to_string(abs(i)).size(), empty);
				if ((jourSemaine + i) % 7 == 0)
				{
					Center(str.str(), handle, ligne);
					ligne += 2;
					str.str(std::string());
				}
			}

		}
		OutputDown("Voulez-vous traiter une autre date (O/N) ? ", handle, false);

		getc = ' ';
		while (getc != 'O' && getc != 'N')
		{
			getc = toupper(_getch());
			std::cout << getc << '\b';
		}

	} while (getc == 'O');

	short offset;
	{
		CONSOLE_SCREEN_BUFFER_INFO buffInfo;
		GetConsoleScreenBufferInfo(handle, &buffInfo);
		offset = buffInfo.srWindow.Bottom - buffInfo.srWindow.Top + 1;
		FillConsoleOutputCharacter(handle, 32, 0xFFFFF, {}, &dummy);
	}
	
	Center("A U  R E V O I R !", handle, offset / 2);
	Sleep(1000);
	return 0;
}





/*
str.str(std::string());
dispjour = 1 - jourSemaine;
for (int s = 0; s < (jourSemaine + 1 + jourmois) % 7 + 1; ++s)
{
	for (int j = 1; j <= 7; ++j, ++dispjour)
	{
		if (dispjour > 0 && dispjour <= jourmois)
		{
			str << (dispjour < 10 ? " " : "") << dispjour;
			if (j != 7)
				str << std::string(8, filling);
		}
		else if (j != 7)
		{
			if (dispjour == jourmois + 1)
				str << std::string(2, filling);
			str << std::string(10, empty);
		}
	}
	Center(str.str(), handle, 10 + s * 2);
	str.str(std::string());
}
*/