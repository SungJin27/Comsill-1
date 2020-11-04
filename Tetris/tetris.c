#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;
	
	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		createRankList();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_RANK: rank(); break;
		case MENU_REC_PLAY: recommendedPlay(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;
	
	for (i = 0; i < VISIBLE_BLOCKS; i++) {
		nextBlock[i] = rand()%7;
	}

	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

//	move(9,WIDTH+10);
	DrawBox(9,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(16,WIDTH+10);
	printw("SCORE");
	DrawBox(17,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(18,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
		
			else printw(" ");
		}
	}

	for (i = 0; i < 4; i++) {
		move(10+i, WIDTH+13);
		for (j = 0; j < 4; j++) {
			if (block[nextBlock[2]][0][i][j] == 1) {
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}	
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate, char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c", tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();

	do{
		if(timed_out==0){
			alarm(1);
			timed_out=1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code
	int i, j;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) { 
			if (block[currentBlock][blockRotate][i][j]) {
				if (f[blockY + i][blockX + j] == 1) return 0; //블록을 놓으려고 하는 필드에 이미 블록이 쌓여져 있는지 여부 
				if (blockY + i >= HEIGHT) return 0; //블록을 나타내는 4×4 행렬의 각 요소의 실제 필드상의 y 좌표가 HEIGHT 보다 크거나 같은지 여부 
				if (blockX + j < 0 ) return 0; //블록을 나타내는 4×4 행렬의 각 요소의 실제 필드상의 x 좌표가 0보다 작 은지 여부 
				if (blockX + j >= WIDTH) return 0; // 블록을 나타내는 4×4 행렬의 각 요소의 실제 필드상의 x 좌표가 WIDTH보 다 크거나 같은지 여부 
			}	
		}
	}

	return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
	int i, j;
	int shadow;
	
	switch (command) {
		case KEY_UP:
			blockRotate = (blockRotate + 3) % 4;
			break;
		
		case KEY_DOWN:
			blockY--;
			break;

		case KEY_RIGHT:
			blockX--;
			break;

		case KEY_LEFT:
			blockX++;
			break;

		default:
			break;
	}
	
	for (i = 0; i < HEIGHT; i++) {
		if (!CheckToMove(f, currentBlock, blockRotate, blockY + i, blockX)) {
			shadow = blockY + i -1;
			break;
		}
	}

	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j]) {
				if (blockY + i >= 0) {
					move(blockY + i + 1, blockX + j + 1);
					printw(".");
				}
		
				if (shadow + i >= 0) {
					move(shadow + i + 1, blockX + j + 1);
					printw(".");
				}
			}

			if (block[currentBlock][recommendR][i][j]) {
				if (recommendY + i >= 0) {
					move(recommendY + i + 1, recommendX + j + 1);
					printw(".");
				}
			}
		}
	}

	switch (command) {
		case KEY_UP:
			blockRotate = (blockRotate + 1) % 4;
			break;

                case KEY_DOWN:
			blockY++;
			break;

                 case KEY_RIGHT:
			blockX++;
			break;

                 case KEY_LEFT:
			blockX--;
			break;
	
		default:
			break;
	}

	DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
	move(HEIGHT, WIDTH + 10);
}

void BlockDown(int sig) {
	//user code
	
	//강의자료 p26-27의 플로우차트를 참고한다.
	int i;
	int pos_y = blockY;

	if (CheckToMove(field, nextBlock[0], blockRotate, pos_y + 1, blockX) == 1) {
		blockY++;
		DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
	}

	else {
		if (blockY == -1) gameOver = 1;
		score = score + AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
		score = score + DeleteLine(field);
	
		for (i = 0; i < VISIBLE_BLOCKS -1; i++) {
			nextBlock[i] = nextBlock[i+1];

		}
		nextBlock[VISIBLE_BLOCKS-1] = rand() % 7;
		
		//recommend(recRoot);	
		modified_recommend(recRoot);
		
		Free_Tree(recRoot);
		recRoot = NULL;
		max = 0;
	
		blockRotate = 0;
		blockY = -1;
		blockX = WIDTH/2 - 2;

		DrawNextBlock(nextBlock);
		PrintScore(score);	
		DrawField();
	}

	
	timed_out = 0;	
}

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
	int i, j;
	int touched = 0;

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (block[currentBlock][blockRotate][i][j] == 1) {
				f[blockY + i][blockX + j] = 1;
				if (blockY + i == HEIGHT -1 || f[blockY + i + 1][blockX + j] == 1) touched++;
			}
		}
	}
	
	return touched * 10;
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code

	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
	int i, j;
	int m, n;
	int del_line = 0;
	int flag = 0;

	for (i = 0; i < HEIGHT; i++) {
		flag = 0;
		for (j = 0; j < WIDTH; j++) {
			if (f[i][j] == 0) break;
	
			if (j == WIDTH - 1)  flag = 1;

		}

		if (flag) {
		
			for (m = i-1; m >= 0; m--) { 
				for (n = j-1; n >= 0; n--) {
					f[m+1][n] = f[m][n];
				}
			}				
			
			del_line++;

		}		
			
	}
	
	return del_line * del_line * 100;
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
	int i, j;
	for (i = 0; i < HEIGHT; i++) {
		if (!CheckToMove(field, blockID, blockRotate, y + i, x)) {
			DrawBlock(y + i -1, x, blockID, blockRotate, '/');
			break;
		}
	}		
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate) {
	DrawBlock(y, x, blockID, blockRotate, ' ');

	DrawRecommend(recommendY, recommendX, blockID, recommendR);

	DrawShadow(y, x, blockID, blockRotate);
}

void createRankList(){
	// user code
	head = (Node*)malloc(sizeof(Node));
	head->link = NULL;
	
	int count = 0;
	FILE* fp = fopen("rank.txt", "r");
	if(fp == NULL) return;
	
	fscanf(fp, "%d\n", &(head->score)); 
	
	Node* temp;

	while(1) {
		Node* p;
		p = (Node*)malloc(sizeof(Node));
		
		if (fscanf(fp, "%s %d", p->name, &(p->score)) == EOF) break;
		
		if (head->link == NULL) { 
			head->link = p;
			p->link = NULL;
			temp = p;
		}
		
		else {
			temp->link = p;
			p->link = NULL;
			temp = temp->link;		
		}
		count++;
	}

	head->ranking = count;
	fclose(fp);	
}

void rank(){
	// user code
	clear();
	int x = -1, y = -1;
	int i;
	int count = 0;
	int flag = -1;
	char search[17];
	int delete;
	
	FILE* fp = fopen("rank.txt", "r");
	if (fp == NULL) return;
	
	Node* temp = head->link;
	Node* prev;

	for (count = 0; count < head->ranking; count++) {
		temp->ranking = count + 1;
		temp = temp->link;
	}
	
	printw("1. list ranks from X to Y\n");
	printw("2. list ranks by a specific name\n");
	printw("3. delete a specific rank\n");

	noecho();
	
	switch(wgetch(stdscr)) {
		case '1':
			temp = head->link;

			echo();
			printw("X: ");
			scanw("%d", &x);
			printw("Y: ");
			scanw("%d", &y);
			noecho();
			printw("	name	|	score	\n");
			printw("--------------------------------\n");
			
			echo();
			if (x > y && y != -1) {
				printw("search failure: no rank in the list\n");
				break;
			}

			if ( x == -1 && y == -1) {
				while (temp != NULL) {
					printw("%-16s| %d\n", temp->name, temp->score);
					temp = temp->link;
				}
				break;
			}
			
			else if (x != -1 && y != -1) {
				while (temp != NULL) {
					if (temp->ranking >= x && temp->ranking <= y) {
						printw("%-16s| %d\n", temp->name, temp->score);
					}
	
					if (temp->ranking > y) break;
					temp = temp->link;
				}
			
			}
		
			else if (x == -1 || y == -1) {
				if (x == -1) {
					while(temp != NULL) {
						if (temp->ranking > y) break;

						printw("%-16s| %d\n", temp->name, temp->score);
						temp = temp->link;
					}
				}
			
			
				else if (y == -1) {
					while (temp != NULL) {
						
						if (temp->ranking >= x ) {
						printw("%-16s| %d\n", temp->name, temp->score);
						}

						temp = temp->link;
						
						}
					}
						
			}
			noecho();

			break;

		case '2':
			temp = head->link;

			echo();
			printw("input the name: ");
			scanw("%s", search);
			
			printw("        name    |       score   \n");
			printw("--------------------------------\n");

			while(temp != NULL) {
				if (strcmp(temp->name, search) == 0) {
					printw("%-16s| %d\n", temp->name, temp->score);
					flag = 1;
				}
				
				temp = temp->link;	
			}			
		
			if (flag == -1) {
				printw("\nsearch failure: no name in the list\n");
			}
		
			noecho();

			break;

		case '3':
			temp = head->link;
			prev = head;

			echo();
			printw("input the rank: ");
	
			
			scanw("%d", &delete);

			while (temp != NULL) {
				if (temp->ranking == delete) {
					prev->link = temp->link;
					flag = 1;
					free(temp);

					break;
				}

				prev = temp;
				temp = temp->link;
			}
			
			if (flag == 1) {
				printw("\nresult: the rank deleted\n");
				(head->ranking)--;
			}

			else {
				printw("\nsearch failure: the rank not in the list\n");
			}

			writeRankFile();

			break;

		default:
			break;	
	}
	
	getch();
}

void writeRankFile(){
	// user code
	Node* curr = (Node*)malloc(sizeof(Node));
	curr = head->link;

	FILE* fp = fopen("rank.txt", "w");
	
	fprintf(fp, "%d\n", head->ranking);

	while(curr != NULL) {
		fprintf(fp, "%s %d\n", curr->name, curr->score);
		curr = curr->link;
	}

	fclose(fp);
}

void newRank(int score){
	// user code
	Node* pnew = (Node*)malloc(sizeof(Node));
	Node* temp = head;
	Node* prev;

	clear();	
	printw("username: ");

	echo();	
	scanw("%s", pnew->name);

	pnew->score = score;
	
	while(1) {
		if (temp == head) { 
			if (temp->link == NULL) {
				temp->link = pnew;
				pnew->link = NULL;
				break;
			}

			if (pnew->score > temp->link->score) {
				pnew->link = temp->link;
				temp->link = pnew;
				break;
			}
		}

		if (temp->link == NULL) {
			if (temp->score > pnew->score) {
				temp->link = pnew;
				pnew->link = NULL;
				break;
			}
			else {
				pnew->link = temp;
				prev->link = pnew;
				break;
			}
		}
		
		if (pnew->score > temp->link->score && pnew->score < temp->score) {
			pnew->link = temp->link;
			temp->link = pnew;
			break;
		}
		
		prev = temp;
		temp = temp->link;
	}
	
	(head->ranking)++;

	writeRankFile();
	
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
	DrawBlock(recommendY, recommendX, blockID, recommendR, 'R');
		
}

void Free_Tree (RecNode *root) {

	if (root == NULL) return;
	
	else {
		int i;
		for (i = 0; i < 40; i++) {
			Free_Tree(root->child[i]);
		}
		
		free(root);
	
		return;
	}
}

int recommend(RecNode* root) {
	//user code
		
//	int max = 0; //미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수
	int i, j, k;
	int a, b;
	int n;
	
	if (recRoot == NULL) {
		recRoot = (RecNode*)malloc(sizeof(RecNode));
		recRoot->level = 0;
		recRoot->acc = 0;
		recRoot->recBlockID = nextBlock[0];
		recRoot->recBlockX = blockX;
		recRoot->recBlockY = blockY;
		recRoot->recBlockRotate = blockRotate;
		
		for (a = 0; a < HEIGHT; a++) {
			for (b = 0; b < WIDTH; b++) {
				recRoot->recField[a][b] = field[a][b];
			}
		}
		root = recRoot;
		
	}

	for (i = 0; i < NUM_OF_ROTATE;  i++) {
		for (j = 0; j < WIDTH; j++) {
			int flag = 0;
			RecNode* tnew = (RecNode*)malloc(sizeof(RecNode));			

			//parent root의 정보를 가져온다
			root->child[i*10 + j] = tnew;
			tnew->parent = root;
			
			for (a = 0; a < 40; a++) {
				tnew->child[a] = NULL;
			}

			tnew->level = root->level + 1;
			tnew->acc = root->acc;
			tnew->recBlockID = nextBlock[tnew->level - 1];
	
			for (a = 0; a < HEIGHT; a++) {
				for (b = 0; b < WIDTH; b++) {
					tnew->recField[a][b] = root->recField[a][b];
				}
			}
				
			if (!CheckToMove(tnew->recField, tnew->recBlockID, i, -1, j)) {
				flag = 1;
			}
			
			for (k = 0; k < HEIGHT; k++) {
				if (!CheckToMove(tnew->recField, tnew->recBlockID, i, k, j)) {
					k = k-1;
					break;
				}
			}

			tnew->acc += AddBlockToField(tnew->recField, tnew->recBlockID, i, k, j);
			tnew->acc += DeleteLine(tnew->recField);

			tnew->recBlockRotate = i;
			tnew->recBlockY = k;
			tnew->recBlockX = j;

			if (tnew->level < VISIBLE_BLOCKS && flag == 0) recommend(tnew);
			
			if (tnew->level == VISIBLE_BLOCKS) {
			
				if (max < tnew->acc) {
					RecNode* temp = tnew;
					max = tnew->acc;
		
					while (1) {
						if (temp->level == 1) {
							recommendR = temp->recBlockRotate;
							recommendY = temp->recBlockY;
							recommendX = temp->recBlockX;
							break;
						}
						
						temp = temp->parent;
					}
				}
			}
			
		}
	}
	
}

void modified_recommend(RecNode* root) {
	int i, j, k;
        int a, b;
        int n;

        if (recRoot == NULL) {
                recRoot = (RecNode*)malloc(sizeof(RecNode));
                recRoot->level = 0;
                recRoot->acc = 0;
                recRoot->recBlockID = nextBlock[0];
                recRoot->recBlockX = blockX;
                recRoot->recBlockY = blockY;
                recRoot->recBlockRotate = blockRotate;

                for (a = 0; a < HEIGHT; a++) {
                        for (b = 0; b < WIDTH; b++) {
                                recRoot->recField[a][b] = field[a][b];
                        }
                }
                root = recRoot;

        }

        for (i = 0; i < NUM_OF_ROTATE;  i++) {
                for (j = 0; j < WIDTH; j++) {
                        int move_flag = 0;
			int acc_flag = 0;
                        RecNode* tnew = (RecNode*)malloc(sizeof(RecNode));

                        //parent root의 정보를 가져온다
                        root->child[i*10 + j] = tnew;
                        tnew->parent = root;

                        for (a = 0; a < 40; a++) {
                                tnew->child[a] = NULL;
                        }

                        tnew->level = root->level + 1;
                        tnew->acc = root->acc;
                        tnew->recBlockID = nextBlock[tnew->level - 1];

                        for (a = 0; a < HEIGHT; a++) {
                                for (b = 0; b < WIDTH; b++) {
                                       tnew->recField[a][b] = root->recField[a][b];
                                }
                        }

			//더이상 갈 수 없을 때
                        if (!CheckToMove(tnew->recField, tnew->recBlockID, i, -1, j)) {
                                move_flag = 1;
                        }

			//회전을 고려할 필요 없는 경우
			switch (tnew->recBlockID) {
				case 0:
					if (i >= 2) move_flag = 1;
					break;

				case 4:
					if (i >= 1) move_flag = 1;
					break;

				case 5:
					if (i >= 2) move_flag = 1;
					break;

				case 6:
					if (i >= 2) move_flag = 1;
					break;
	
				default:
					break;
			}			
		
		
		if(move_flag == 0) {
                        for (k = 0; k < HEIGHT; k++) {
                                if (!CheckToMove(tnew->recField, tnew->recBlockID, i, k, j)) {
                                        k = k-1;
                                        break;
                                }
                        }

                        tnew->acc += AddBlockToField(tnew->recField, tnew->recBlockID, i, k, j);
                        tnew->acc += DeleteLine(tnew->recField);
 			
                        tnew->recBlockRotate = i;
                        tnew->recBlockY = k;
                        tnew->recBlockX = j;
			
		
                        if (tnew->level < VISIBLE_BLOCKS) {
				if (max < tnew->acc) max = tnew->acc;
				
				//truning
				if (tnew->level == (VISIBLE_BLOCKS + 1)/2) {
					if (tnew->acc < max / 2) acc_flag = 1;
				}

				if (acc_flag == 0) modified_recommend(tnew);

			}

                        if (tnew->level == VISIBLE_BLOCKS) {

                                if (max < tnew->acc) {
                                        RecNode* temp = tnew;
                                        max = tnew->acc;

                                        while (1) {
                                                if (temp->level == 1) {
                                                        recommendR = temp->recBlockRotate;
                                                        recommendY = temp->recBlockY;
                                                        recommendX = temp->recBlockX;
                                                        break;
                                                }

                                                temp = temp->parent;
                                        }
                                }
                        }
		}

                }
        }
}


void recommendedPlay() {
	// user code
	
	int command;
	int i;
	time_t start, stop;
	double duration;
	long memory_size;

	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM, &act, &oact);
	InitTetris();

	start = time(NULL);

	modified_recommend(recRoot);
	Free_Tree(recRoot);
	recRoot = NULL;
	max = 0;

	blockRotate = recommendR;
	blockY = recommendY;
	blockX = recommendX;
	
	score = AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
	
	for (i = 0; i < VISIBLE_BLOCKS - 1; i++) {
		nextBlock[i] = nextBlock[i+1];
	}
	nextBlock[VISIBLE_BLOCKS -1] = rand() %7;

	blockRotate = 0;
	blockY = -1;
	blockX = WIDTH/2 -2;
	
	DrawNextBlock(nextBlock);
	PrintScore(score);
	DrawField();	

	modified_recommend(recRoot);
	memory_size = evalSize(recRoot);
	Free_Tree(recRoot);
	recRoot = NULL;
	max = 0;
	do {
		if (timed_out == 0) {
			alarm(1);
			timed_out = 1;
		}
		
		command = GetCommand();
		if (ProcessCommand(command) == QUIT) {
			alarm(0);
			DrawBox(HEIGHT/2 - 1, WIDTH/2 - 5, 1, 10);
			move(HEIGHT/2, WIDTH/2 - 4);
			printw("Good-bye!!");
			refresh();	
			getch();

			return;
		}

		blockRotate = recommendR;
		blockY = recommendY;
		blockX = recommendX;
	
	} while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2 - 1, WIDTH/2 - 5, 1 , 10);
	move(HEIGHT/2, WIDTH/2 - 4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);

	stop = time(NULL);
	duration = (double)difftime(stop, start);

	clear();
	move(0,0);
	
	printw("The total time : %lf seconds\n", duration);
	printw("The total memory : %ld bytes\n", memory_size); 
	getch();
}

long evalSize(RecNode* head) {
	if (head == NULL) return 0;
	
	else {
		int i;
		for (i = 0; i < 40; i++) {
			return sizeof(RecNode) + evalSize(head->child[i]);
		}

		return 0;
	}
}
