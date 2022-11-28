#include "libs.h"
#include "header.h"

#define MAX_FILES_ON_SCREEN 38

int elements; // Amount of files and directories in the current folder

// Return a pressed key as a value from 0 to 4
// Arrow up = 1, Arrow down = 3, Arrow right = 2, Arrow left = 4, ESC = 0
int getKey_Browse() 
{
    while(1) {
        // Check if the window is focused
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
        if (GetAsyncKeyState(VK_SPACE) & 0x07)
            return 5;

        Sleep(180);
    }
}

int getKey_Sort() 
{
    while(1) {
        // Check if the window is focused
        if(GetForegroundWindow() != GetConsoleWindow())
            continue;
        
        if (GetAsyncKeyState(0x31) & 0x07)
            return 1;
        if (GetAsyncKeyState(0x32) & 0x07)
            return 2;
        if (GetAsyncKeyState(0x33) & 0x07)
            return 3;
        if (GetAsyncKeyState(0x34) & 0x07)
            return 4;
        if (GetAsyncKeyState(0x35) & 0x07)
            return 5;
        if (GetAsyncKeyState(0x36) & 0x07)
            return 6;
        if (GetAsyncKeyState(0x30) & 0x07)
            return 0;

        Sleep(150);
    }
}

// Print a table with all files and folders in the current directory
void printDirectory(struct _finddata_t* files)
{
    int count = 0; 
    if (elements == 0) {
        gotoxy(0, 0);
        printf("Empty directory.\n\n");
    }
    else {
        gotoxy(0, 0);
        for (int i = 0; i < elements; i++) {
            if (count > MAX_FILES_ON_SCREEN)
                break;
            else
            {
                if (files[i].size == 0) {
                printf(" %-26.26s  %14s\n", files[i].name, "<DIR>");
            }
                else {
                printf(" %-26.26s  %11lu KB\n", files[i].name, files[i].size / 1024);
            }
            }
        }
        gotoxy(0, 39);
        printf("Files in directory: %d", elements);
    }
}

// Change background color and text color of a given line 
void printColoredLine(int line, int color_text, int color_back, struct _finddata_t* file)
{
    gotoxy(0, line);
    textcolor(color_text);
    textbackground(color_back);

    if (file[line].size == 0)
        printf(" %-26.26s  %14s\n", file[line].name, "<DIR>");
    else
        printf(" %-26.26s  %11lu KB\n", file[line].name, file[line].size / 1024);

    // Set colors back to default
    textcolor(LIGHTGRAY);
    textbackground(BLACK);
}

// Append str2/*.* to the end of str1
void changePath(char str1[], char str2[]) 
{
    int i = 0;
    int j = 0;
    int endfound = 0;

    while ((str1[i] != '\0') && (str1[i] != '*')) 
    {
        i++;
    }

    while (str2[j] != '\0') 
    {
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

// Change path to previous directory
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

int getFileCount(char path[])
{
    struct _finddata_t c_file;
    intptr_t hFile;

    int count = 0;
    if ((hFile = _findfirst(path, &c_file)) == -1L)
        count = 0;
    else
    {
        do
            count++;
        while (_findnext(hFile, &c_file) == 0);

        if (path[3] != '*')
        count = count - 2;
    }
    return count;
}

struct _finddata_t* getValues(char path[])
{
    struct _finddata_t c_file;
    intptr_t hFile;
    
    // Initaialize an array with the size of the amount of files 
    elements = getFileCount(path);
    struct _finddata_t *files_in_dir = (struct _finddata_t *)malloc(sizeof(struct _finddata_t) * elements);

    int i = 0;
    int skips = 0;
    if ((hFile = _findfirst(path, &c_file)) == -1L)
        files_in_dir[0].size = -1;
    else
        do
        {
            // Skip first two files ("." and "..") unless in root directory
            if (path[3] != '*' && skips < 2) {
                skips++;
                continue;
            }

            files_in_dir[i] = c_file;
            i++;
        } 
        while(_findnext(hFile, &c_file) == 0);
    _findclose(hFile);
    return files_in_dir;
}


int main(void)
{
    struct _finddata_t* files;
    char path[100] = "C:\\*.*";

    int pos_in_menu = 0; // selected file
    int prev_pos = 0;    // previous selected file
    int key = -1;        // key pressed
    int depth = 0;       // how deep is current directory relatively to C:
    int pos_in_prev_dir; // position in previous directory

    hidecursor();
    setwindow(44, 40);

    files = getValues(path);
    sort_bubble(files, elements);
    printDirectory(files);
    printColoredLine(pos_in_menu, BLACK, WHITE, files);

    while (key != 0) 
    {
        key = -1;
        key = getKey_Browse();
        switch(key)
        {
            case 1:    // Arrow up is pressed
                if (elements != 0)
                {
                    if (pos_in_menu > 0)            // If not on the first line
                    {
                        prev_pos = pos_in_menu;
                        pos_in_menu--;

                        printColoredLine(prev_pos, LIGHTGRAY, BLACK, files);
                        printColoredLine(pos_in_menu, BLACK, WHITE, files);
                    }
                    else                            // If on the first line
                    {
                        prev_pos = 0;
                        if (elements > MAX_FILES_ON_SCREEN)
                            pos_in_menu = MAX_FILES_ON_SCREEN - 1;
                        else
                            pos_in_menu = elements - 1;

                        printColoredLine(prev_pos, LIGHTGRAY, BLACK, files);
                        printColoredLine(pos_in_menu, BLACK, WHITE, files);
                    }
                }
                break;

            case 3:    // Arrow down is pressed
                if (elements != 0)
                {   // If on the last line (case 1)
                    if ((pos_in_menu == elements - 1) && elements < MAX_FILES_ON_SCREEN) 
                    {
                        prev_pos = elements - 1;
                        pos_in_menu = 0;

                        printColoredLine(prev_pos, LIGHTGRAY, BLACK, files);
                        printColoredLine(pos_in_menu, BLACK, WHITE, files);
                    }
                    else
                    {   // If on the last line (case 2)
                        if (pos_in_menu == MAX_FILES_ON_SCREEN - 1) 
                        {
                            prev_pos = MAX_FILES_ON_SCREEN - 1;
                            pos_in_menu = 0;
                            
                            printColoredLine(prev_pos, LIGHTGRAY, BLACK, files);
                            printColoredLine(pos_in_menu, BLACK, WHITE, files);
                        }
                        else                            // If on the last line
                        {
                            prev_pos = pos_in_menu;
                            pos_in_menu++;

                            printColoredLine(prev_pos, LIGHTGRAY, BLACK, files);
                            printColoredLine(pos_in_menu, BLACK, WHITE, files);
                        }
                    }
                }
                break;

            case 2:    // Arrow right is pressed
                if (files[pos_in_menu].size == 0) // If a directory is selected
                { 
                    changePath(path, files[pos_in_menu].name);
                    files = getValues(path);
                    clrscr();

                    pos_in_prev_dir = pos_in_menu;
                    pos_in_menu = 0;
                    prev_pos = 0;

                    printDirectory(files);
                if (elements != 0)
                    printColoredLine(pos_in_menu, BLACK, WHITE, files);

                depth++; }
                break;

            case 4:    // Arrow left is pressed 
                if (depth > 0) {
                    prevDir(path);
                    files = getValues(path);
                    clrscr();

                    pos_in_menu = pos_in_prev_dir;
                    prev_pos = 0;

                    printDirectory(files);
                if (elements != 0)
                    printColoredLine(pos_in_menu, BLACK, WHITE, files);

                depth--;}
                break;

            case 5:
                if (elements > 0) {
                    clrscr();
                    gotoxy(0,0);
                    printf
                        (
                        "Press a key to choose a sort method\n\n"
                        "1 - Bubble sort\n"
                        "2 - Selection sort\n"
                        "3 - Insertion sort\n"
                        "4 - Merge sort\n"
                        "5 - Quick sort\n"
                        "6 - Shell sort\n\n"
                        "0 - Cancel"
                        );
                    int key2 = -1;
                    while (key2 != 0)
                    {
                        key2 = getKey_Sort();
                        clrscr();
                        switch (key2)
                        {
                            case 0:
                                break;
                            case 1:
                                sort_bubble(files, elements);
                                break;
                            case 2:
                                sort_selection(files, elements);
                                break;
                            case 3:
                                sort_insertion(files, elements);
                                break;
                            case 4:
                                sort_merge(files, 0, elements - 1);
                                break;
                            case 5:
                                sort_quick(files, elements);
                                break;
                            case 6:
                                sort_shell(files, elements);
                                break;
                        }
                        printDirectory(files);
                        printColoredLine(0, BLACK, WHITE, files);
                        pos_in_menu = 0;
                        prev_pos = 0;
                        key2 = 0;
                    } 
                }
                break;

            case 0:
                system("pause");
                return 1;
        }
    }
    system("pause");
    return 1;
}
