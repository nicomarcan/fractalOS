/* Autores:
*
*  Tomás Raies
*  M. Alejo Saqués
*
*  Contenido:
*
*  Funciones para la presentación de datos al usuario
*  para el juego Faronona
*
*/

#include <c_string.h>
#include <c_syscall.h>
#include <clib.h>
#include <printf.h>
#include <fanorona.h>


#define FALSE 0
#define TRUE  1
#define DRAW  2
#define START 10
#define FIN   12

#define QUIT  100
#define UNDO  101
#define SAVE  102
#define MOVE  103
#define SPACE 104
#define SAVE_PATH 105

#define SR    106
#define SC    107
#define M1    108
#define DR    109
#define DC    110
#define M2    111
#define DIR   112
#define M3    113
#define STR   114
#define M4    115

#define MAX_STRING_LENGTH 100

#define HUMPC 200
#define PCHUM 201
#define HUMHUM 202
#define PCPC  203

#define ROWS  303
#define COLS  304
#define LOAD  305
#define BLOCK 10

typedef struct Command
{
     int option;
     int errtype;
     char * save_path;
     Move * move;
}Command;


int evalQuit(char c, int * n);
int evalUndo(char c, int * n);
// int evalSave(char c, int * n, int substate, char * save_path);
int
evalMove(char c,int substate,int * sr,int * sc,int * dr, int * dc, char * dir);
Command * inputCommand(void);

static char minorChangeSlash(char slash);
void printMove(Move * cm);
int printMoves(Game_p g);
int printBoard(const Game_p g);
void printSymbol(Chip * chip, char c);
char * getPath (size_t max);

void playGame(int p_ctrl1, int p_ctrl2, Game_p loaded_game);

int evalQuit(char c, int * n)
{
     char s[] = "uit";
     int length = 3;

     if(c == s[*n]){
	  (*n)++;
     }
     else{
	  return ERROR;
     }

     if (*n == length){
	  return FIN;
     }
     else{
	  return QUIT;
     }
}

int evalUndo(char c, int * n)
{
     char s[] = "ndo";
     int length = 3;

     if(c == s[*n]){
	  (*n)++;
     }
     else{
	  return ERROR;
     }

     if (*n == length){
	  return FIN;
     }
     else{
	  return UNDO;
     }
}

// int evalSave(char c, int * n, int substate,char * save_path)
// {
//      char s[] = "ave";
//      int length = 3;
//
//      switch(substate){
//      case START:
//
// 	  if(c == s[*n]){
// 	       (*n)++;
// 	  }
// 	  else{
//
// 	       return ERROR;
// 	  }
// 	  if (*n == length){
//
// 	       *n = 0;
// 	       return SPACE;
// 	  }
// 	  else{
// 	       return START;
// 	  }
// 	  break;
//      case SPACE:
//
// 	  if (!isblank(c)){
// 	       ungetc(c);
// 	       return SAVE_PATH;
// 	  }
// 	  return SPACE;
//
// 	  break;
//      case SAVE_PATH:
// 	  if (*n >= MAX_STRING_LENGTH || c == '/'){
// 	       return ERROR;
// 	  }
// 	  if (!isblank(c)){
// 	       save_path[*n] = c;
// 	       (*n)++;
// 	       return SAVE_PATH;
// 	  }
// 	  if (isblank(c)){
// 	       save_path[*n] = '\0';
// 	       return FIN;
// 	  }
//
// 	  break;
//      }
// }

int
evalMove(char c,int substate,int * sr, int * sc, int * dr, int * dc,char * dir)
{
	switch(substate){
	 case START :
	 if (c == '[')
     {
		return SR ;
	 }
	 else if (c_isblank(c))
     {
		return START ;
     }
     else{
         return ERROR;
     }
	 break ;
     case SR:
	  if (c_isdigit(c)){
	       *sr = *sr * 10 + c - '0';
           if (*sr > 19){
               return ERROR;
           }
	       return SR;
	  }
	  else if (c == ','){
	       return SC;
	  }
	  else
	       return ERROR;
	  break;
     case SC:
	  if (c_isdigit(c)){
	       *sc = *sc * 10 + c - '0';
           if (*sc > 19){
               return ERROR;
           }
	       return SC;
	  }
	  else if (c == ']'){
	       return M2;
	  }
	  else{
	       return ERROR;
	  }
	  break;
     case M2:
	  if (c == '['){
	       return DR;
	  }
	  else{
	       return ERROR;
	  }
	  break;
     case DR:
	  if (c_isdigit(c)){
	       *dr = *dr * 10 + c -'0';
           if (*dr > 19){
               return ERROR;
           }
	       return DR;
	  }
	  else if (c==','){
	       return DC;
	  }
	  else{
	       return ERROR;
	  }
	  break;
     case DC:
	  if (c_isdigit(c)){
	       *dc = *dc * 10 + c - '0';
           if (*dc > 19){
               return ERROR;
           }
	       return DC;
	  }
	  else if(c==']'){
	       return M3;
	  }
	  else{
	       return ERROR;
	  }
	  break;
     case M3:
	  if (c == '['){
	       return DIR;
	  }
	  else if (c_isblank(c)){
	       return FIN;
	  }
	  else{
	       return ERROR;
	  }
	  break;
     case DIR:
	  if (c == 'w' || c == 'a'){
	       *dir = c;
	       return M4;
	  }
	  else{
	       return ERROR;
	  }
	  break;
     case M4:
	  if (c == ']'){
	       return FIN;
	  }
	  else{
	       return ERROR;
	  }
	  break;
     }
}
Command * inputCommand(void){
     char c;
     char dir = '\0', save_path[MAX_STRING_LENGTH];
     int errflag = FALSE;
     int errtype;
     int n = 0;
     int state = START, substate = START, option = START;
     int sr,sc,dr,dc;
     Command * command;
     sr=sc=dr=dc=0;
     command = malloc(sizeof(*command));

     while( (c=c_tolower(getchar())) != '\n' && c != EOF ){
	  switch(state){
	  case QUIT:
	       substate = evalQuit(c,&n);
	       if (substate == ERROR){
		    errflag = TRUE;
		    errtype = QUIT;
                state = FIN;
	       }
	       else if(substate == FIN){
		    state = FIN;
		    option = QUIT;
	       }
	       break;
	  case UNDO:
	       substate = evalUndo(c,&n);
	       if (substate == ERROR){
		    errflag = TRUE;
		    errtype = UNDO;
                state = FIN;
	       }
	       else if (substate == FIN){
		    state = FIN;
		    option = UNDO;
	       }
	       break;
	  case SAVE:

	      //  substate = evalSave(c,&n,substate,save_path);
	      //  if (substate == ERROR){
		    // errflag = TRUE;
		    // errtype = SAVE;
        //         state = FIN;
	      //  }
	      //  else if (substate == FIN){
		    // state = FIN;
		    // option = SAVE;
	      //  }
	       break;
	  case MOVE:
	       substate = evalMove(c,substate,&sr,&sc,&dr,&dc,&dir);
	       if (substate == ERROR){
		    errflag = TRUE;
		    errtype = MOVE;
                state = FIN;
	       }
	       else if (substate == FIN){
		    state = FIN;
		    option = MOVE;
	       }
	       break;
	  case START:
	       if (c_isalpha(c)){
		    switch(c){
		    case 'q':
			 state = QUIT;
			 break;
		    case 'u':
			 state = UNDO;
			 break;
		    case 's':
			 state = SAVE;
			 break;
		    case 'm':
			 state = MOVE;
			 break;
		    default:
			 errflag = TRUE;
			 errtype = START;
		    }
	       }
	       else if (!c_isblank(c)){
		    errflag = TRUE;
		    errtype = START;
	       }
	       break;
	  case FIN:
	       if (!c_isblank(c)){
		    errflag = TRUE;
		    errtype = FIN;
	       }
	  }
     }

     if (errflag == TRUE){
	  command->option = ERROR;
	  command->errtype = errtype;
	  return command;
     }
     else if (state != FIN){
	  if (substate == SAVE_PATH){
	       save_path[n]='\0';
	       command->option = state;
	  }
       else if (substate == M2 || substate == M3){
            command->option = state;
       }
	  else{
	       command->option = ERROR;
	       command->errtype = state;

	  }
     }
     else{
	  command->option = option;
     }

     switch(command->option){
     case SAVE:
	  command->save_path = malloc(strlen(save_path)+1);
	  if (command->save_path == NULL){
	       command->option = ERROR;
	       command->errtype = SAVE;
	       return command;
	  }
	  c_strcpy(command->save_path, save_path);
	  break;
     case MOVE:
	  command->move = malloc( sizeof( *(command->move)));
	  if (command->move == NULL){
	       command->option = ERROR;
	       command->errtype = MOVE;
	       return command;
	  }
	  command->move->s[0] = sr;
	  command->move->s[1] = sc;
	  command->move->d[0] = dr;
	  command->move->d[1] = dc;
	  command->move->dir = dir;
	  break;
     }
     return command;
}

void displayError(int errtype)
{
     switch(errtype){
     case SAVE:
	  printf("Guardado fallo.\n");
	  break;
     case UNDO:
	  printf("No puede deshacer la jugada ahora.\n");
	  break;
     case MOVE:
	  printf("Movimiento invalidо.\n");
	  break;
     default:
	  printf("Entrada invalida.\n");
	  break;
     }
}
/** NUEVO AHORA LLAMA A UNDO, NO UNDO MANAGER**/
int undoGame(Game_p game)
{
     int ans;
     ans = undo(game);
     if (ans == ERR_MULT){
	  printf("No puede deshacer la jugada en modo multijugador.\n");
     }
     else if (ans == ERR_COUNT){
	  printf("No hay jugada para deshacer.\n");
     }
     else if (ans == MEM_ERROR) {
         return ans;
     }
     else{
	  printf("Se deshizo el ultimo turno.\n");
	  printBoard(game);
	  ans = printMoves(game);
     }
     return ans;
}



int binaryGetAnswer( void)
{
     int ans;
     int state = START;
     int c;

     while( (c = getchar()) != '\n' && c != EOF){
	  switch (state){
	  case START:
	       if (c_isalpha(c)){
		    switch(c_tolower(c)){
		    case 'y':
			 ans = TRUE;
			 state = FIN;
			 break;
		    case 'n':
			 ans = FALSE;
			 state = FIN;
			 break;
		    default:
			 ans = ERROR;
			 state = FIN;
			 break;
		    }
	       }
	       else if (!c_isblank(c)){
		    ans = ERROR;
		    state = FIN;
		    break;
	       }
	       break;
	  case FIN:
	       if (!c_isblank(c)){
		    ans = ERROR;
	       }
	       break;
	  }
     }
     return ans;
}


int quitGame(Game_p game)
{
     int f, v;
     char * save_path;
    //  printf("Quiere guardar antes de salir?[y/n]\n");
    //  while ( (f = binaryGetAnswer()) == ERROR ){
	  // printf("Respuesta erronea. Quiere guardar?\n");
    //  }
    //  while (f == TRUE){
    //      printf("Donde lo quiere guardar?.\n");
    //      save_path = getPath(MAX_STRING_LENGTH);
    //      if (save_path == NULL){
    //          printf("El nombre de archivo es demasiado largo o incluye \
    //                 caracteres prohibidos. Intente otra vez\n");
    //      }
    //      else{
    //          f = saveGame(game, save_path);
    //          if (f == TRUE){
    //              printf("No se pudo guardar. Intente nuevamente.\n");
    //          }
    //      }
    //  }
     printf("Juego terminado.\n");
     return TRUE;
}


char solveAmbiguous(void)
{
        int state = START, c;
        char dir;

        printf("Para que direccion quiere comer?. ([a] ; [w]) \n");
        while ( (c = c_tolower(getchar())) != '\n' && c != EOF){
            switch (state){
                case START:
                if ( c == 'a' || c == 'w'){
                    dir = c;
                    state = FIN;
                }
                else if (!c_isblank(c)){
                    dir = '\0';
                    state = FIN;
                }
                break;
                case FIN:
                if (!c_isblank(c)){
                    dir = '\0';
                }
                break;
            }
        }
        return dir;
}
int manageMove(Game_p g, Move * move, int pl_ctrl)
{
     int eaten;
     int move_count;
     int err_flag;

     eaten = moveTo(g,move,pl_ctrl);
     while(eaten == AMBIGUOUS){
         move->dir = solveAmbiguous();
         printf("%c \n", move->dir);
         eaten = moveTo(g,move,pl_ctrl);
     }
     if (eaten == -1){
	  printf("Movimiento invalido. Revise el tablero. \n");
	  return 1;
     }
     if (eaten == MEM_ERROR){
         return MEM_ERROR;
     }
     err_flag = printBoard(g);
     if (err_flag == MEM_ERROR)
     {
         return MEM_ERROR;
     }
     printf("Se comieron %d fichas.\n", eaten);
     move_count = getMoveCount(g);
     if (move_count == 0){
	  err_flag = endTurn(g);
      if (err_flag == MEM_ERROR)
      {
          return MEM_ERROR;
      }
	  printf("Turno del Jugador %c.\n",CHANGE_PLAYER_CHAR(getPlayerTurnSym(g)));
	  err_flag = printBoard(g);
      if (err_flag == MEM_ERROR)
      {
          return MEM_ERROR;
      }
     }
     err_flag = printMoves(g);
     if (err_flag == MEM_ERROR)
     {
         return MEM_ERROR;
     }
     return FALSE;
}

int manageCommand(const Command * comm,Game_p game)
{
     int quit_flag = FALSE;
     int game_state;
     switch(comm->option){
     case ERROR:
	  displayError(comm->errtype);
	  break;
     case SAVE:
	  // saveGame(game, comm->save_path);
	  break;
     case UNDO:
	  undoGame(game);
	  break;
     case QUIT:
	  quit_flag = quitGame(game);
	  break;
     case MOVE:
	  game_state = manageMove(game, comm->move,HUM);
      if (game_state == -2){
          quit_flag = DRAW;
      }
      if (game_state == MEM_ERROR){
          quit_flag = MEM_ERROR;
      }
	  break;
     }
     return quit_flag;
}

void freeCommand( Command * comm)
{
     if (comm->option == SAVE){
	  free(comm->save_path);
     }
     if (comm->option == MOVE){
	  free(comm->move);
     }
     free(comm);
     return;
}



int printMoves(Game_p g)
{
  Moves * moves;
  int i;
  moves = getMoves(g);
  if (moves == NULL){
      return MEM_ERROR;
  }
  for (i=0;i<moves->count;i++){
      if (i%3 == 0){
          putchar('\n');
      }
      printMove(moves->moveset[i]);
      free(moves->moveset[i]);
  }
  putchar('\n');
  free(moves->moveset);
  free(moves);
  return FALSE;
}

void printMove(Move * cm)
{
    /******NUEVO CAMBIE ESTO FIJATE SI LO PREFERIS ASI */
    if (cm->dir == 0)
	   printf("m[%d,%d][%d,%d]\t\t",cm->s[0],cm->s[1],cm->d[0],cm->d[1]);
    else
       printf("m[%d,%d][%d,%d][%c]\t\t",cm->s[0],cm->s[1],cm->d[0],cm->d[1],cm->dir);
    return;
}

int printBoard(const Game_p g)
{
     int i,j;
     char d,c;
     int rows, cols;
     Backup * b ;

     rows = getRows(g);
     cols = getCols(g);
     b = getBackup(g);
     if (b == NULL)
     {
         return MEM_ERROR;
     }
     d = '\\';
     printf("  ");
     for(j=0;j<cols;j++){
	      printf(" %d  ", j);
     };
     printf("\n");
     for(i=0;i<rows-1;i++){
	  c = d;
	  printf("%d ",i);
	  for(j=0;j<cols-1;j++){
	       printSymbol(b->board[i][j], '-');
	  };

	  printSymbol(b->board[i][j], '\n');
	  printf("  ");
	  for (j=0;j<cols-1;j++){
	       printf(" | %c",c);
	       c = minorChangeSlash(c);
	  }
	  printf(" | \n");

	  d = minorChangeSlash(d);
     };
     printf("%d ", i );
     for(j=0;j<cols-1;j++){
	  printSymbol(b->board[i][j], '-');
     };
     printSymbol(b->board[i][j],'\n');
     freeBackup(b, rows, cols);

}

void printSymbol(Chip * chip, char c)
{
     if (chip->owned == 0)
	 {
	  printf(" %c %c",' ',c);
     }
     else if (chip->owner == NULL)
	 {
	  printf(" %c %c",'*',c );
     }
     else
	 {
	  printf(" %c %c", CHANGE_PLAYER_CHAR(chip->owner->symbol),c);
     }
}

static char minorChangeSlash(char slash)
{
     if(slash == '/'){
	  return '\\';
     }
     else if(slash == '\\'){
	  return '/';
     }
     else{
	  return slash;
     }
}
int getInputMenu()
{
     int ans, c, state = START;

     while ( (c=getchar())!= '\n' && c != EOF){
	  switch(state){
	  case START:
	       if (c_isdigit(c)){
		    switch(c){
		    case '1' :
			 ans = HUMPC;
			 state = FIN;
			 break;
		    case '2':
			 ans = HUMHUM;
			 state = FIN;
			 break;
		    case '3':
			 ans = LOAD;
			 state = FIN;
			 break;
		    case '4':
			 ans = QUIT;
			 state = FIN;
			 break;
		    case '5':         ///para debuggear, acordarse de quitar
			 ans = PCPC;
			 state = FIN;
			 break;
		    default:
			 ans = ERROR;
			 state = FIN;
			 break;
		    }
	       }
	       break;
	  case FIN:
	       if (!c_isblank(c)){
		    ans = ERROR;
	       }
	       break;
	  }
     }
     return ans;
}

char * getPath (size_t max)
{
	char * vec, *aux=NULL ;
    int state = START;
	int c ;
	int i=0;

    vec = malloc(BLOCK);
	while (((c=getchar())!=EOF)&&(c!='\n'))
	{
        switch(state){
            case START:
            if (c == '/'){
                state = FIN;
                free(vec);
                vec = NULL;
            }
            else if (!c_isblank(c)){

                vec[i] = c;
                state = STR;
                i++;
            }
            break;
            case STR:
            if (i==max || c == '/')
    		{
    			free(vec);
    			vec = NULL;
                state = FIN;
    		}
            else if (c_isblank(c)){
                state = FIN;
            }
            else{
                if ((i%BLOCK)==0)
        		{
        			aux=realloc(vec,i+BLOCK) ;
        			if (aux==NULL)
        			{
        				free(vec);
        				vec = NULL;
                        state = FIN;
                        break; /*Pongo dos breaks en este caso. */
        			}
        			else
        			{
        				vec=aux ;
        			}
        		}
        		vec[i]=c;
                i++;
            }
            break;
            case FIN:
            if (!c_isblank(c)){
                free(vec);
                vec = NULL;
            }
		}
	}
    if (vec != NULL){
        vec[i++]=0 ;
    	vec=realloc(vec,i) ;
    }
	return vec ;
}

int mainMenu()
{
    int o;
    char * load_file;
    Game_p load_game;
    printf("INTRODUZCA UNA DE LAS SIGUIENTES OPCIONES\n");
    printf("\t1-JUGAR UN JUGADOR \n");
    printf("\t2-JUGAR MULTIJUGADOR \n");
    printf("\t3-CARGAR PARTIDA \n");
    printf("\t4-SALIR\n");

    while ( (o = getInputMenu()) == ERROR)
	{
		printf("ENTRADA INCORRECTA\nELIJA UN NUMERO DEL MENU\n");
    }
    switch(o)
	{
		case HUMPC:
		{
			playGame(HUM,PC,NULL);
			break;
		}
		case HUMHUM:
		{
			playGame(HUM,HUM,NULL);
			break;
		}
		case PCPC:
		{
			playGame(PC,PC,NULL);
			break;
		}
		case LOAD:
		{
			// printf("INTRODUZCA EL NOMBRE DEL ARCHIVO:") ;
			// load_file=getPath(MAX_STRING_LENGTH) ;
			// load_game=loadGame(load_file) ;
			// if  (load_game == NULL)
			// {
			// 	printf("EL ARCHIVO NO EXISTE O ESTÁ CORRUPTO\n") ;
			// }
			// else
			// {
			// 	playGame(0,0,load_game);
			// }
			break;
		}
		case QUIT:
		{
			printf("ADIOS\n");
			return TRUE;
		}
    }
    return FALSE;
}

int getBoardDim(int * rows, int * cols)
{
     int state = START, c, ans = TRUE;

     *rows = 0;
     *cols = 0;

     while ( (c=getchar()) != '\n' && c != EOF){
	  switch (state){
	  case START:
	       if (c_isdigit(c)){
		    *rows = c - '0';
		    state = ROWS;
	       }
	       else if (!c_isblank(c)){
		    ans = ERROR;
		    state = FIN;
	       }
	       break;
	  case ROWS:
	       if (c_isdigit(c)){
		    *rows = *rows * 10 + c - '0';
	       }
	       else if (c_isblank(c)){
		    state = M1;
	       }
	       else if (c_tolower(c) == 'x'){
		    state = M2;
	       }
	       else{
		    ans = ERROR;
		    state = FIN;
	       }
	       break;
	  case M1:
	       if(c_tolower(c) == 'x'){
		    state = M2;
	       }
	       else if(!c_isblank(c)){
		    ans = ERROR;
		    state = FIN;
	       }
	       break;
	  case M2:
	       if (c_isdigit(c)){
		    *cols = c -'0';
		    state = COLS;
	       }
	       else if (!c_isblank(c)){
		    ans = ERROR;
		    state = FIN;
	       }
	       break;
	  case COLS:
	       if (c_isdigit(c)){
		    *cols = *cols * 10 + c - '0';
	       }
	       else if (c_isblank(c)){
		    state = FIN;
	       }
	       else{
		    ans = ERROR;
		    state = FIN;
	       }
	       break;

	  case FIN:
	       if (!c_isblank(c)){
		    ans = ERROR;
	       }
	       break;
	  }
     }
     if (ans != ERROR && *rows != 0 && *cols != 0){
	  return validateBoard(*rows, *cols);
     }
     return ERROR;
}


void playGame(int p_ctrl1, int p_ctrl2, Game_p loaded_game)
{
     int turn =0;
     Game_p game_p;
     Command * comm;
     int rows,cols;
     int quit_flag = FALSE;
     int err_flag;
     char sym;

     if (loaded_game==NULL)
     {
          printf("Ingrese la dimension del tablero [ROWS]X[COLS].\n");

          while (getBoardDim(&rows,&cols) == ERROR){
              printf("Dimension invalida. Deben ser impares entre 3 y 19.\n");
          }

          game_p = initGame(rows,cols,'&','#',p_ctrl1,p_ctrl2);

     }
     else
     {
          game_p=loaded_game ;
     }
     /**NUEVO TODO ESTE IF Y EL DE ABAJO *****/
     if (game_p == NULL){
         quit_flag = MEM_ERROR;
     }
     else
     {
         err_flag = printBoard(game_p);
         if (err_flag == MEM_ERROR)
         {
             quit_flag = MEM_ERROR;
         }
         else{
             err_flag = printMoves(game_p);
             if (err_flag == MEM_ERROR)
             {
                 quit_flag = MEM_ERROR;
             }
         }
     }
     while (quit_flag == FALSE && winState(game_p) == FALSE){
	  turn++;
      sym = CHANGE_PLAYER_CHAR(getPlayerTurnSym(game_p));
	  printf("Jugador %c: ", sym );
	  if (getPlayerTurnCtrl(game_p) == PC){
	       putchar('\n');
           /** nuEVO DECIA GAME STATE BORRA ESA VARIABLE**/
	       quit_flag = manageMove(game_p, NULL, PC);
	  }
	  else{
	       comm = inputCommand();
	       quit_flag = manageCommand(comm, game_p);
	       freeCommand(comm);
	  }
      if (getMoveCount(game_p) == 0){
          quit_flag = DRAW;
      }
     }
     if (quit_flag == MEM_ERROR){
         /** NUEVO CAMBIE LO QUE IMPRIME ESTO Y EL IF ES IMPORTANTE********/
         printf("Lo sentimos, ocurrio un error de memoria.\n");
         if (game_p == NULL){
             return;
         }
     }
     if (getPlayerChipCant(game_p,0) == 0){
         sym = CHANGE_PLAYER_CHAR(getPlayerSym(game_p,1));
         printf("Gano el jugador %c en %d turnos.\n", sym ,turn);
     }
     else if (getPlayerChipCant(game_p,1) == 0){
         sym = CHANGE_PLAYER_CHAR(getPlayerSym(game_p,0));
         printf("Gano el jugador %c en %d turnos.\n", sym ,turn);
     }
     else if (quit_flag == DRAW){
         printf("Se llego a una situacion de empate.\n");
     }
     endGame(game_p);
     return;
}



int fanorona_main(void)
{
     int quit_flag = FALSE;
     srand(time(0));
	 printf("\n\n") ;
	 printf("8888888888\n");
	 printf("888\n");
	 printf("888\n");
	 printf("8888888  8888b.  888d888  .d88b.  88888b.   .d88b.  88888b.   8888b.\n");
	 printf("888         .88b 888P.   d88..88b 888 .88b d88..88b 888 .88b     .88b\n");
	 printf("888     .d888888 888     888  888 888  888 888  888 888  888 .d888888\n");
	 printf("888     888  888 888     Y88..88P 888  888 Y88..88P 888  888 888  888\n");
	 printf("888     .Y888888 888      .Y88P.  888  888  .Y88P.  888  888 .Y888888\n");
	 printf("\n\n") ;
    while (quit_flag == FALSE)
	{
		quit_flag = mainMenu();
    }
    exit();

}
