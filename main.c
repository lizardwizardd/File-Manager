#include "header.h"
#include <winuser.h>

int elements;

// return a pressed key as a value from 0 to 4
int getKey() {

    while(1) {
        // this checks if the window is focused
        if(GetForegroundWindow() != GetConsoleWindow())
            continue;
        
        if (GetAsyncKeyState(VK_ESCAPE) & 0x07)
            return 0;
        if (GetAsyncKeyState(VK_UP) & 0x07)
            return 1;
        if (GetAsyncKeyState(VK_RIGHT) & 0x07)
            return 2;
        if (GetAsyncKeyState(VK_DOWN) & 0x07)
            return 3;
        if (GetAsyncKeyState(VK_LEFT) & 0x07)
            return 4;

        Sleep(200);
    }
}

void printColoredLine(int line, int color_text, int color_back, char names[][100], char dates[][30], long long sizes[])
{
    gotoxy(1, line + 2);
    textcolor(color_text);
    textbackground(color_back);

    if (sizes[line] == -1) {
        printf("%-26.26s %.24s  %14s\n", names[line], dates[line], "<DIR>");
        //printf("%-26.26s %.24s", names[line], dates[line]); textcolor(BROWN); textbackground(color_back); printf("  %14s\n", "<DIR>"); textcolor(LIGHTGRAY);
    }
    else {
        printf("%-26.26s %.24s  %11lld KB\n", names[line], dates[line], sizes[line]);
    }

    textcolor(LIGHTGRAY);
    textbackground(BLACK);
}

// appends str2/*.* to the end of str1
void changePath(char str1[], char str2[]) 
{
    int i = 0;
    int j = 0;
    int endfound = 0;

    while ((str1[i] != '\0') && (str1[i] != '*')) {
        i++;
    }
    while (str2[j] != '\0') {
        str1[i] = str2[j];
        i++;
        j++;
    }

    str1[i] = '\\';
    str1[i+1] = '*';
    str1[i+2] = '.';
    str1[i+3] = '*';
    str1[i+4] = '\0';
}

// change path to previous directory
void prevDir(char path[])
{
    int i = 0;

    while (path[i] != '\0')
        i++;

    i = i - 5;

    while (path[i] != '\\') 
        i--;

    path[i+1] = '*';
    path[i+2] = '.';
    path[i+3] = '*';
    path[i+4] = '\0';
}   

int getStringSize(char str[])
{
    int i = 0;

    while (str[i]!='\0')
    {
        i++;
    }

    return i;
}

// fills arrays with values from a specified directory
void getValues(char dir_path[], char names[][100], char dates[][30], long long sizes[])
{
    struct _finddata_t c_file;
    intptr_t hFile;
    int count = 0;
    int skips = 0;

    if ((hFile = _findfirst(dir_path, &c_file)) == -1L)
        strcpy(dates[0], ",,");
    else
    {
        do 
        {
            // skip first two files unless we're in C:
            if (dir_path[3] != '*' && skips < 2) {
                skips++;
                continue;
            }

            // fill names array
            strcpy(names[count], c_file.name);
            
            // fill dates array
            char buffer[30];
            ctime_s(buffer, _countof(buffer), &c_file.time_write);
            strcpy(dates[count], buffer);

            // fill sizes array
            if (c_file.size == 0) {
                sizes[count] = -1;
            }
            else {
                sizes[count] = (long long) c_file.size/8/1024;
            }

            count++;
        }   
        while (_findnext(hFile, &c_file) == 0);

        elements = count;
        _findclose(hFile);

    }
}

// print a table with all files and folders in in current directory
void printDirectory(char path[], char names[][100], char dates[][30], long long sizes[])
{
    printf(" Currrently browsing: %s\n\n", path);
    if (strcmp(dates[0], "*") == 0) {
        printf("Empty directory.\n\n");
    }
    else {
        for (int i = 0; i < elements; i++) {
            if (sizes[i] == -1) {
                printf(" %-26.26s %.24s  %14s\n", names[i], dates[i], "<DIR>");
                //printf(" %-26.26s %.24s", names[i], dates[i]); textcolor(YELLOW); printf("  %14s\n", "<DIR>"); textcolor(LIGHTGRAY);
            }
            else {
                printf(" %-26.26s %.24s  %11lld KB\n", names[i], dates[i], sizes[i]);
            }
        }
        printf("\n Files in directory: %d\n\n", elements);
    }
}

int main(void) {
    // initalization
    const int max_files = 100;
    char path[100] = "C:\\*.*";
    char names[max_files][100];
    char dates[max_files][30];
    long long sizes[max_files];
    
    int pos_in_menu = 0; // selected file
    int prev_pos = 0;    // previous selected file
    int key = -1;        // key pressed
    int depth = 0;       // how deep is current directory relatively to C:
    int pos_in_prev_dir; // position in previous directory

    hidecursor();
    setwindow(69, 40);

    // main body
    getValues(path, names, dates, sizes);
    printDirectory(path, names, dates, sizes);
    printColoredLine(pos_in_menu, BLACK, WHITE, names, dates, sizes);
    while (key != 0) 
    {
        key = -1;
        key = getKey();
        if (key == 1) {
            if (pos_in_menu > 0) {
                prev_pos = pos_in_menu;
                pos_in_menu--;

                printColoredLine(prev_pos, LIGHTGRAY, BLACK, names, dates, sizes);
                printColoredLine(pos_in_menu, BLACK, WHITE, names, dates, sizes);
            }
            else {
                //gotoxy(10, 38); printf("now %d\n", pos_in_menu);

                prev_pos = 0;
                pos_in_menu = elements - 1; 

                printColoredLine(prev_pos, LIGHTGRAY, BLACK, names, dates, sizes);
                printColoredLine(pos_in_menu, BLACK, WHITE, names, dates, sizes);
            }
        }

        if (key == 3) {
            if (pos_in_menu < elements - 1)
            {
                prev_pos = pos_in_menu;
                pos_in_menu++;

                printColoredLine(prev_pos, LIGHTGRAY, BLACK, names, dates, sizes);
                printColoredLine(pos_in_menu, BLACK, WHITE, names, dates, sizes);
            }
            else {
                //gotoxy(10, 38); printf("now %d\n", pos_in_menu);

                prev_pos = elements - 1;
                pos_in_menu = 0;

                printColoredLine(prev_pos, LIGHTGRAY, BLACK, names, dates, sizes);
                printColoredLine(pos_in_menu, BLACK, WHITE, names, dates, sizes);
            }

        }
        if (key == 2) {
            if (sizes[pos_in_menu] == -1) { // if it's a directory
                changePath(path, names[pos_in_menu]);
                getValues(path, names, dates, sizes);
                clrscr();

                pos_in_prev_dir = pos_in_menu;
                pos_in_menu = 0;
                prev_pos = 0;

                printDirectory(path, names, dates, sizes);
                printColoredLine(pos_in_menu, BLACK, WHITE, names, dates, sizes);
                
                depth++;
            }
        }
        if (key == 4) {
            if (depth > 0) {
                prevDir(path);
                getValues(path, names, dates, sizes);
                clrscr();

                pos_in_menu = pos_in_prev_dir;
                prev_pos = 0;

                printDirectory(path, names, dates, sizes);
                printColoredLine(pos_in_menu, BLACK, WHITE, names, dates, sizes);

                depth--;
            }
        }
        if (key == 0)
            return 1;
    }
    system("pause");
    return 1;
}