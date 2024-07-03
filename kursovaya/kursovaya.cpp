// kursovaya.cpp: главный файл проекта.

#include "stdafx.h"   
#include <stdio.h>  
#include <conio.h>  
#include <clocale>  
#include <stdlib.h>  
#include <string.h>  
#include <windows.h> 

using namespace System; 
using namespace std; 
using namespace System::IO; 

#define ENTER 13   
#define ESC 27  
#define UP 72  
#define DOWN 80  
#define HOME 71  
#define END 79 
 
int NC; 
int menu(int); 
void max_ball(struct z*, int NC); 
void min_raznica(struct z*, int NC); 
void alf(struct z* studenty, int NC, struct sp **spisok); 
void search_number(struct z*, int NC); 
void question(struct z*, int NC); 
void diagramma(struct z*, int NC, struct sp **spisok);
void vstavka(struct z* studenty, char* stud, int NC, struct sp **spisok);
struct cheap { 
 char stud[30]; 
    char gorod[20]; 
    int diff; 
}; 
 
struct z { 
    long number; 
    char stud[30]; 
    char gorod[20]; 
    long ball; 
    long stip; 
    long costs; 
}; 
struct sp { 
    char stud[30]; 
    char gorod[20]; 
    long number; 
    long stip; 
	long ball; 
    struct sp* sled; 
    struct sp* pred; 
} *spisok; 
 
 
 
int menu(int n) 
{ 
    int y1 = 0, y2 = n - 1; 
    char c = 1;
	char questions[7][100] = { 
	"У студента из какого города самый высокий балл?                       ", 
	"Студент из какого города минимальную разницу стипендии и расходов?    ", 
	"Поиск студента по номеру                                              ",
	"Фамилии студентов в алфавитном порядке                                ", 
	"Совпадение прибыли в разных городах                                   ", 
	"Диаграмма прибыли у студента                                          ", 
	"Выход                                                                 " 
	};
    while (c != ESC) 
    { 
        switch (c) 
        { 
   case DOWN: y2 = y1; y1++; break; 
   case UP: y2 = y1; y1--; break; 
   case ENTER: return y1 + 1; 
   case HOME: if (y1 != 0) { y2 = y1; y1 = 0; }; break; 
   case END: if (y1 != 6) { y2 = y1; y1 = 6; }; break; 
        } 
        if (y1 > n - 1) { y2 = n - 1; y1 = 0; } 
        if (y1 < 0) { y2 = 0; y1 = n - 1; } 
        Console::ForegroundColor = ConsoleColor::White; 
  Console::BackgroundColor = ConsoleColor::Magenta; 
        Console::CursorLeft = 11; 
        Console::CursorTop = y1 + 5; 
        printf("%s", questions[y1]); 
  Console::ForegroundColor = ConsoleColor::White; 
  Console::BackgroundColor = ConsoleColor::DarkRed; 
        Console::CursorLeft = 11; 
        Console::CursorTop = y2 + 5; 
        printf("%s", questions[y2]); 
        c = _getch(); 
  } 
    exit(0); 
} 

int main() 
{ 
    int i, n; 
    FILE* in; 
    struct z* studenty;
	struct sp *spisok = 0;
	char questions[7][100] = { 
	"У студента из какого города самый высокий балл?                       ", 
	"Студент из какого города минимальную разницу стипендии и расходов?    ", 
	"Поиск студента по номеру                                              ",
	"Фамилии студентов в алфавитном порядке                                ", 
	"Совпадение прибыли в разных городах                                   ", 
	"Диаграмма прибыли у студента                                          ", 
	"Выход                                                                 " 
	}; 
	char Blankline[] = "                                                                       ";
    setlocale(LC_CTYPE, "Russian"); 
    Console::CursorVisible::set(false); Console::WindowWidth=120;
    Console::BufferHeight = Console::WindowHeight; 
    Console::BufferWidth = Console::WindowWidth; 
    if ((in = fopen("Student.dat", "r")) == NULL) 
    { 
        printf("Файл Student.dat не открыт!\n"); 
        _getch(); 
        exit(1); 
    } 
    fscanf(in, "%d", &NC); 
    studenty = (struct z*)malloc(NC * sizeof(struct z)); 
    for (i = 0; i < NC; i++) 
        fscanf(in, "%ld%s%s%ld%ld%ld", &studenty[i].number, studenty[i].stud, studenty[i].gorod, &studenty[i].ball, &studenty[i].stip, &studenty[i].costs); 
    printf("Номер по списку    Имя               Город         Балл    Стипендия Расходы в Граблях\n"); 
    for (i = 0; i < NC; i++) 
        printf("%-19ld%-18s%-14s%-8ld%-10ld%-24ld\n", studenty[i].number, studenty[i].stud, studenty[i].gorod, studenty[i].ball, studenty[i].stip, studenty[i].costs); 
    _getch(); 
    while (1) 
    { 
  Console::BackgroundColor = ConsoleColor::Black; 
        Console::Clear(); 
  Console::ForegroundColor = ConsoleColor::White; 
  Console::BackgroundColor = ConsoleColor::DarkRed; 
        Console::CursorLeft = 10; 
        Console::CursorTop = 4; 
        printf(Blankline); 
  for (i = 0; i < 7; i++) 
        { 
            Console::CursorLeft = 10; 
            Console::CursorTop = i + 5; 
            printf(" %s", questions[i]); 
        } 
        Console::CursorLeft = 10; 
        Console::CursorTop = 12; 
        printf(Blankline); 
        n = menu(7); 
   switch (n) { 
   case 1:max_ball(studenty, NC); break; 
   case 2:min_raznica(studenty, NC); break;
   case 3:search_number(studenty, NC); break;
   case 4:alf(studenty, NC, &spisok); break;
   case 5:question(studenty, NC); break; 
   case 6:diagramma(studenty, NC, &spisok); break; 
   case 7:exit(1); 
        } 
    } 
 return 0; 
 
 
} 
void max_ball(struct z* studenty, int NC) 
{ 
    int i = 0; 
    struct z best; 
    strcpy(best.gorod, studenty[0].gorod); 
    best.ball = studenty[0].ball; 
    for (i = 1; i < NC; i++) 
        if (studenty[i].ball > best.ball) 
        { 
            strcpy(best.gorod, studenty[i].gorod); 
            best.ball = studenty[i].ball; 
 
        } 
 Console::ForegroundColor = ConsoleColor::Black; 
 Console::BackgroundColor = ConsoleColor::Magenta; 
    Console::CursorLeft = 10; 
    Console::CursorTop = 16; 
    printf("Город с самым высокобалльным студентом: %s с баллом %ld", best.gorod, best.ball); 
    getch(); 
} 
void min_raznica(struct z* studenty, int NC) 
{ 
    int i = 0; 
    struct cheap best; 
    strcpy(best.stud, studenty[0].stud); 
    strcpy(best.gorod, studenty[0].gorod); 
    best.diff = 10000; 
    for (i = 1; i < NC; i++) 
    { 
        if (studenty[i].stip - studenty[i].costs < best.diff)
        { 
            strcpy(best.stud, studenty[i].stud); 
            strcpy(best.gorod, studenty[i].gorod); 
            best.diff = studenty[i].stip - studenty[i].costs; 
        } 
    } 
    Console::ForegroundColor = ConsoleColor::Black; 
    Console::BackgroundColor = ConsoleColor::Magenta; 
    Console::CursorLeft = 10; 
    Console::CursorTop = 16; 
	printf("Минимальная разница: у %s из города %s с разницей %d\n", best.stud, best.gorod, best.diff); 
    getch(); 
} 
void search_number(struct z* studenty, int NC) 
{ 
    int number_input, i, k = 0; 
    Console::Clear(); 
 Console::ForegroundColor = ConsoleColor::Black; 
 Console::BackgroundColor = ConsoleColor::Magenta; 
    Console::CursorLeft = 10; 
    Console::CursorTop = 16; 
    printf("\nВведите номер по списку:\n"); 
    scanf("%ld", &number_input); 
    for (i = 0; i < NC; i++) 
    { 
        if (studenty[i].number == number_input) 
        { 
            printf("\n%s %s\n", studenty[i].gorod, studenty[i].stud); 
            k++; 
        } 
    } 
    if (k == 0) 
        printf("Не нашлось студента заданного номера по списку:("); 
    getch(); 
} 

void vstavka(struct z* studenty, char* stud, int NC, struct sp **spisok) 
{ 
    int i; 
    struct sp* nov, * nt, * z = 0; 
    for (nt = *spisok; nt != 0 && strcmp(nt->stud, stud) < 0; z = nt, nt = nt->sled); 
    if (nt && strcmp(nt->stud, stud) == 0) return; 
    nov = (struct sp*)malloc(sizeof(struct sp)); 
    strcpy(nov->stud, stud); 
    for (i = 0; i < NC; i++) { 
        if (strcmp(studenty[i].stud, stud) == 0) { 
            (nov->ball = studenty[i].ball); 
            break; 
        } 
    } 
    nov->pred = z; 
    nov->sled = nt; 
    if (!z) *spisok = nov; 
    else z->sled = nov; 
    if (nt) nt->pred = nov; 
    return; 
}  

void alf(struct z* studenty, int NC, struct sp **spisok) 
{ 
    int i, n = 0; 
    struct sp* nt; 
    struct sp* z; 
 Console::ForegroundColor = ConsoleColor::Black; 
 Console::BackgroundColor = ConsoleColor::Magenta; 
    Console::Clear(); 
    if (!*spisok) { 
        for (i = 0; i < NC; i++) { 
            vstavka(studenty, studenty[i].stud, NC, spisok); 
        } 
    } 
    Console::Clear(); 
    printf("\n Алфавитный список студентов"); 
    printf("\t\t Алфавитный список студентов в обратном порядке"); 
    printf("\n ===============================\t"); 
    printf("===============================================\n"); 
    for (nt = *spisok; nt != 0; nt = nt->sled) { 
        printf("\n %-20s %ld", nt->stud, nt->ball); 
    } 
    for (nt = *spisok, z = 0; nt != 0; z = nt, nt = nt->sled); 
    for (nt = z; nt != 0; nt = nt->pred) 
    { 
        Console::CursorLeft = 48; 
        Console::CursorTop = 4 + n; 
        printf(" %-20s %ld", nt->stud, nt->ball); 
        n += 1; 
    } 
    getch(); 
    Console::Clear(); 
}


void question(struct z* studenty, int NC)
{
    int i, j, flag = 0;
    for (i = 0; i < NC; i++) {
        for (j = i + 1; j < NC; j++) {
            if ((studenty[i].stip == studenty[j].stip) && (strcmp(studenty[i].gorod, studenty[j].gorod)!=0)) {
				Console::ForegroundColor = ConsoleColor::White;//цвет символов
				Console::BackgroundColor = ConsoleColor::Magenta;//цвет фона
                Console::CursorLeft = 10;
                Console::CursorTop = 15;
				printf("Существует 2 студента из разных городов с одинаковой стипендией:");
                Console::CursorLeft = 10;
                Console::CursorTop = 16;
                printf("%s %ld %s и %s %ld %s", studenty[i].stud, studenty[i].stip, studenty[i].gorod, studenty[j].stud, studenty[j].stip, studenty[j].gorod);
                getch();
                flag = 1;
                break;
            }
        }
    }

    if (flag == 0) { //если пара студентов не найдена
		Console::ForegroundColor = ConsoleColor::Magenta;
        Console::BackgroundColor = ConsoleColor::White;
        Console::CursorLeft = 10;
        Console::CursorTop = 15;
        printf("В файле Student.dat не найдено студентовиз разных городов с одинаковой стипендией");
        getch();
        Console::Clear();
    }
}


void diagramma(struct z* studenty, int NC, struct sp **spisok) 
{ 
 struct sp *nt;
 int len,i,NColor;
 char str1[100];
 char str2[100];
 char str3[100];
 System::ConsoleColor Color;
 Console::BackgroundColor=ConsoleColor::Black;
 Console::Clear();
 if (!*spisok)
   for(i=0;i<NC;i++)
  vstavka(studenty,studenty[i].stud, NC, spisok);
 Color = ConsoleColor::Black;
 NColor=0;
 for (nt=*spisok,i=0;nt!=0;nt=nt->sled,i++)
 {
  sprintf(str1,"%s",studenty[i].stud);
  sprintf(str3,"%s",studenty[i].gorod);
  sprintf(str2,"%3.1f%%",(studenty[i].costs*100./studenty[i].stip));
  Console::ForegroundColor = ConsoleColor::Magenta;
  Console::BackgroundColor = ConsoleColor::Black;
  Console::CursorLeft=5; Console::CursorTop=i+1;
  printf("%s",str1);
  Console::CursorLeft=22;
  printf("%s",str3);
  Console::CursorLeft=34;
  printf("%s",str2);
  Console::BackgroundColor=++Color; NColor++;
  Console::CursorLeft=42;
  for(len=0; len < studenty[i].costs * 100./studenty[i].stip; len++) 
   printf(" "); 
  if(NColor==14)
   {Color=ConsoleColor::Black; NColor=0;} 
 } 
 getch();
 return ;

}
