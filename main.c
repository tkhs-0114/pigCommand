#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

void Error(char* str){
	fprintf(stderr, "エラー: %s\n", str);
	endwin();
	exit(1);
}

int main(int argc, char* argv[])
{
	if(argc < 2) exit(1);
	FILE *file = fopen(argv[1], "r");
	if(file == NULL) Error("ファイルが存在していません");

	int h,w;
	char PIG[9] = " ~@^o.  \0";

	initscr();curs_set(0);//timeout(0);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	getmaxyx(stdscr, h, w);
	char lines[h][256];
	bool flag_continue = true;

	while(flag_continue){
		int Numline = 0;
		for(int i=0; i<h; i++){
			if(fgets(lines[i], 256, file) == NULL){
				flag_continue = false;
				break;
			}
			Numline = i+1;
		}
		timeout(0);
		int now_pig = 0;
		bool lines_end[Numline];
		for(int j=1; j<w-10; j++){

			for(int i=1; i<=Numline; i++){
				if(j==1)lines_end[i] = false;//初期化
				if(lines[i-1][j-1] == '\0')lines_end[i] = true;
				now_pig = j;
				attrset(COLOR_PAIR(1));
				if(!lines_end[i])
					mvaddch(i, j, lines[i-1][j-1]);
				attrset(COLOR_PAIR(2));
				mvaddstr(i, j+1, PIG);
				refresh();
			}
			if(getch() != -1) break;
			usleep(50000);
		}
		timeout(-1);
		char inp = getch();
		for(int j=now_pig; j>=1; j--){
			for(int i=1; i<=Numline; i++){
				mvaddstr(i, j+1, PIG);
				refresh();
			}
			usleep(10000);
		}
		if(inp == 'q') break;
	}

	endwin();
	fclose(file);
	return (0);
}
