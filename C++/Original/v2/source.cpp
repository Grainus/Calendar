/**********************************************************************
Author      : Grainus
Description : Displays the day of the week for a date, then, if needed,
    a calendar for the month of the date. Also validates user input.
Input       : Date
Output      : Day of the week for the date, Calendar of the month
Version     : 1.1
**********************************************************************/

#include <iostream>     // std::cout and std::cin
#include <conio.h>      // _getch()
#include <string>
#include <sstream>
#include <Windows.h>

COORD getsize();
void gotoxy(short x, short y);
void clrscr();
void OutputBottom(const std::string& str, bool clear = true);
void Center(const std::string& str, short y);

const HANDLE OUTHANDLE = GetStdHandle(STD_OUTPUT_HANDLE);


int main() {
    // Variables - Constants
    const std::string DAYS[7] = {
        "Sunday", "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday"
    };
    const std::string MONTHS[12] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };
    const int MONTHDAYS[12] = { 31, 0, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Variables - Input
    int day, month, year;
    char separator[2];

    // Variables - Computing
    int dayOfWeek, daysCounter, daysFebruary;
    char getc;


    do {
        clrscr();
        // Input
        do {
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cout << "\n\tEnter a date (DD/MM/YYYY) : ";
            std::cin >> day >> separator[0] >> month >> separator[1] >> year;

            if (std::cin.fail()) {
                OutputBottom("\tYou must enter only numbers");
                std::cin.clear();
                continue;
            }
            else if (separator[0] != separator[1]) {
                OutputBottom("\tYou must use the same separator");
                continue;
            }
            else if (separator[0] != '-' && separator[0] != '/') {
                OutputBottom("\tYou can only use '/' or '-' as separators");
                continue;
            }
            else if (day <= 0 || day > 31) {
                OutputBottom("\tThe day must be between 1 and 31 inclusively");
                continue;
            }
            else if (month <= 0 || month > 12) {
                OutputBottom("\tThe month is invalid (must be between 1 and 12 inclusively)");
                continue;
            }
            else if (year < 1901 || year > 2099) {
                OutputBottom("\tThe year is invalid (must be between 1901 and 2099 inclusively)");
                continue;
            }
            else if (month == 2 && day > 28) {
                int errormax = 0;
                std::string errorstr = "\tThe month of February cannot have more than ";
                if (year % 4 == 0)
                    if (day > 29)
                        errormax = 29;
                    else
                        break;
                else
                    if (day > 28)
                        errormax = 28;
                    else
                        break;
                errorstr += std::to_string(errormax) + " days";
                OutputBottom(errorstr);
            }
            else if (day == 31 && MONTHDAYS[month - 1] == 30) {
                OutputBottom("\tThis month only has 30 days");
                continue;
            }
            else
                break;
        } while (true);

        // Computing
        daysFebruary = (year % 4 == 0 ? 29 : 28);
        daysCounter = daysFebruary + day;
        for (int i = month - 1; i > 0; --i)
            daysCounter += MONTHDAYS[i - 1];
        dayOfWeek = (daysCounter + (year - 1900) + (year - 1901) / 4) % 7;

        // Output
        clrscr();
        gotoxy(8, 3);
        std::cout << day << '/' << month << '/' << year << " is a : " << DAYS[dayOfWeek];
        gotoxy(8, 6);
        std::cout << "Do you want to see the complete calendar for the month (Y/N) ? ";

        getc = ' ';
        while (getc != 'Y' && getc != 'N') {
            getc = toupper(_getch());
            std::cout << getc << '\b';
        }

        if (getc == 'Y') {
            clrscr();

            // Variables
            const int PADDING = 10;
            const char empty = ' ', filling = ' ';
            int line;
            std::stringstream str;
            dayOfWeek = (daysCounter - (day - 1) + (year - 1900) + (year - 1901) / 4) % 7; // First day of the month

            str << MONTHS[month - 1] + ' ' + std::to_string(year);
            const int monthdays = (month == 2 ? daysFebruary : MONTHDAYS[month - 1]);
            Center(str.str(), 3);

            str.str(std::string());
            str << DAYS[0].substr(0, 2);
            for (int i = 1; i < 7; ++i)
                str << std::string(PADDING - 2, filling) << DAYS[i].substr(0, 2);
            Center(str.str(), 6);


            str.str( std::string(str.str().size(), '_') );
            Center(str.str(), 7);

            line = 10;
            str.str(std::string());
            int max = monthdays + 7 - ((monthdays + dayOfWeek) % 7 - dayOfWeek);
            for (int i = 1 - dayOfWeek; i <= max; ++i) {
                if (str.str().empty())
                    str << (i < 10 ? " " : "");
                else
                    str << std::string(PADDING - std::to_string(abs(i)).size(), filling); // equivalent to setw
                if (i > 0 && i <= monthdays)
                    str << i;
                else
                    str << std::string(std::to_string(abs(i)).size(), empty);
                if ((dayOfWeek + i) % 7 == 0) {
                    Center(str.str(), line);
                    line += 2;
                    str.str(std::string());
                }
            }
        }
        OutputBottom("Do you want to try another date (Y/N) ? ", false);

        getc = ' ';
        while (getc != 'Y' && getc != 'N') {
            getc = toupper(_getch());
            std::cout << getc << '\b';
        }

    } while (getc == 'Y');

    Center("G O O D B Y E !", getsize().Y / 2);
    Sleep(1000);
    return 0;
}


void OutputBottom(const std::string& str, bool clear) {
    if (clear)
        clrscr();

    gotoxy(0, getsize().Y - 1);
    std::cout << str;
 
    if (clear)
        gotoxy(0, 0);
}

void Center(const std::string& str, short y) {
    gotoxy((getsize().X - str.size()) / 2, y);
    std::cout << str;
}

COORD getsize() {
    CONSOLE_SCREEN_BUFFER_INFO buffInfo;
    GetConsoleScreenBufferInfo(OUTHANDLE, &buffInfo);
    unsigned int consoleHeight = buffInfo.srWindow.Bottom - buffInfo.srWindow.Top + 1;
    unsigned int consoleWidth = buffInfo.srWindow.Right - buffInfo.srWindow.Left + 1;
    COORD consoleSize = { consoleWidth, consoleHeight };
    return consoleSize;
}

void gotoxy(short x, short y) {
    SetConsoleCursorPosition(OUTHANDLE, { x, y });
}

void clrscr() {
    DWORD dummy;
    FillConsoleOutputCharacter(OUTHANDLE, 32, 0xFFFFFF, {}, &dummy);
    gotoxy(0, 0);
}
