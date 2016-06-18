/* Autores:
*
*  Tomás Raies
*  M. Alejo Saqués
*
*  Contenido:
*
*  Funciones para el procesamiento de los diferentes aspectos que hacen
*  al juego Faronona
*
*/

#include <clib.h>
#include <lib.h>
#include <fanorona.h>

#define TRUE 1
#define FALSE 0
#define MEM_BLOCK 100000
#define BLOCK 25
#define N {-1,0}
#define S {1,0}
#define E {0,1}
#define W {0,-1}
#define NW {-1,-1}
#define NE {-1,1}
#define SW {1,-1}
#define SE {1,1}
#define STRONG 8
#define WEAK 4

#define NOT_OUT_OF_BOARD(a,b,rows,cols)  (a>=0)&&(a<rows)&&(b>=0)&&(b<cols)

typedef struct TurnPath{
     Chipset chipset;
     int chip_cant;
}TurnPath;

typedef struct Game
{
     char game_type;
     int rows;
     int cols;
     int move_no;
     int curr_dir[2];
     Board board;
     Player * pl_turn;
     Player * pl[2];
     TurnPath * turn_path;
     Moves * v_moves;
     Chip * curr_chip;
     Backup * backup;
     int undo_flag;
} Game;


    /* La estructura Game reúne toda la información básica del juego, que será
    *  utilizada por las funciones del backend para realizar los cálculos per-
    *  tinentes.
    *
    *  ACLARACIONES:
    *  En el campo game_type se guarda el tipo de partida (0-- SINGLEPLAYER ,
    *  1-- MULTIPLAYER). En pl_turn y pl se almacenan el jugador actual y el
    *  conjunto de jugadores respectivamente, ambos referenciando a structs
    *  Player (ver header.h).
    */


/* FUNCIONES DE INICIALIZACION */
static Board initBoard(size_t r, size_t c, Player *  pl[2]);
static void
    initChip(Chip * new_chip, Player * owner, size_t row, size_t col);
static Player *
    initPlayer(size_t rows, size_t cols, int id, char sym, int ctrl);
static int initValidMoves (Game * g);
static void initTurnPath(Game * p);
static Backup * initBackup(Game_p g);
/*Inicializa un backup de una estructura game. Devuelve un puntero al backup
**creado o NULL si hubo un error.
*/



/* FUNCIONES DE REALOCACION DE MEMORIA */
static void freeBoard(Board board, int n, int m, int cols);
static void freeChipset(Player *  player);
static void freeGame(Game * game);

static void freePlayer(Player * p);

/***** FUNCIONES DE MOVIMIENTO *****/

static int moveChip(Move * m, Board board, Player * pl[2], int rows, int cols);
/*Realiza un movimiento en el tablero. Devuelve la cantidad de chips comidas.
*/
static int eatChips(Chip *,int, int, Board , Player * , int , int );
/*Intercambia las posiciones de dos chips en el tablero.
*/
static void swapCoordinates(Chip * chip_s, Chip * chip_d, Board board);
/*Devuelve la siguiente Chip segun el incremento. Devuelve NULL si no hay.
*/
static void loseChip(Player *  player, Chip *  chip);
/*Libera un chip de la posesion de un jugador.
*/
static Chip * getIncChip(Chip *,int ,int ,Board, int , int );
/*Devuelve la siguiente Chip segun el incremento. Devuelve NULL si no hay.
*/
static int findMove(Game * g,const Move * m);
/*Busca en las jugadas validas el movimiento que ingreso el usuario.  Si dicho
**movimiento es ambiguo devuleve AMBIGUOUS.
*/
static int equalMove(const Move * m1, const Move * m2);
/*Compara dos movimientos y devuelve si son iguales.
*/

/* FUNCIONES MENORES (AUXILIARES) */
static Player * minorChangePlayer(Player * player,Player * pl[2]);

/**** FUNCIONES DE GESTION DE TURNO ****/

static int addToTurnPath(TurnPath * turn_path, Chip * chip);
/*Almacena las posiciones pasadas dentro de un mismo turno.
*/
static void freeTurnPath(Game_p g);
static void minorChangePlayerTurn( Game * g);

/* FUNCIONES PARA VERIFICACION DE LOS MOVIMIENTOS POSIBLES */

static int validMoves (Game * g);
/*Calcula todos los movimientos posibles y los asigna a la estructura game.
**Devuelve FALSE si se ejecuta correctamente o MEM_ERROR si ocurre un error.
*/
static Move * assignMove (Move m, const char direction);
/*Asigna un movimiento. Devuelve NULL si hay un error de memoria.
*/
static int mandatoryMoves (Game * g);
/*Calcula los movimientos obligatorios que puede realizar un jugador al comienzo
**de su turno.Devuelve MEM_ERROR si ocurre un error de memoria.
*/

static int validChipMove(Chip * chip, Game * g, int c, int * c_moves,
    int (*f)(Game *,Move,int *,int,int *));
/*Calcula los movimientos validos para un chip en particular, teniendo en cuenta
**las direcciones posibles. Devuelve MEM_ERROR si encuentra un error de memoria.
**Devuleve TRUE si tuvo que cortar el ciclo (ver softCapture).
*/

static int paikaMove(Game * g, Move m, int dir[2], int c, int * c_moves);
/*Asigna el movimiento paika si es posible. Devuelve TRUE si
**se encuentra una jugada de captura posible, FALSE si no y MEM_ERROR si ocurre
**un error de memoria.
*/

static int isRival(Game * g, int temp[2]);
/*Evalua si un chip en una posicion esta poseido por un rival.
*/
static int softCapture(Game * g, Move move, int dir[2]);
/*Evalua si una captura es posible. Devuleve TRUE o FALSE
*/
static int
    captureMove(Game * g, Move m, int dir[2], int careful, int * careful_move );
/*Asigna un movimiento de captura si es posible. Evalua ambas direcciones.
**Devuelve MEM_ERROR si hay un error de memoria, o FALSE si no.
*/
static int evalAttackDir( int pos[2], int inc[2], Game_p g);
/*Evalua si un ataque enemigo es posible en una direccion determinada.
*/
static int inDanger (Chip * curr_chip , Game_p g);
/*Evalua si un chip esta en peligro de ser comido por el otro jugador, en
**cualquier direccion posible.
*/
static int posType (size_t row, size_t col);
/*Devuelve si una posicion es fuerte o debil.
*/

/****** FUNCIONES PARA EL MANEJO DEL TABLERO *******/

Board allocateBoard(int rows, int cols);
/*Reserva espacio de memoria para una estructura Board, con la cantidad de
**columnas y filas indicada. Devuelve un puntero a esa estructura o NULL si
**hubo un error.
*/
void setCurrChip(Move * m, Game * g, int eaten);
/*Recibe un movimiento ejecutado y setea el chip actual de la estructura game
**que recibe como el chip que se movio.
*/

/***** FUNCIONES DE COPIA ******/
static void cpyMove(Move * m1, const Move * m2);
static int cpyMoves(Moves * m1, const Moves * m2);
/*Realiza copia de una estructura de Moves a otra. MEM_ERROR si ocurre un error
**de memoria.
*/
static void cpyChip( Chip * chip_d, Chip * chip_s,  Player ** pl);
static void
    cpyBoard(Board board_d, Board board_s,  int rows, int cols, Player ** pl);
/*Realiza una copia de un tablero a otro del mismo tamanio.
*/






int validateBoard(size_t rows, size_t cols)
{
    if(rows>1 && cols>1 && rows<21 && cols<21 && rows % 2 == 1 && cols % 2 == 1)
	{
		return 1;
    }
    else
	{
		return ERROR;
    }
}

Game * initGame(int rows,int cols,char p0_sy,char p1_sy,int p0_ctrl,int p1_ctrl)
{
     Game * new_game;
     int err_flag;

     new_game = malloc(sizeof(*new_game));
     if (new_game == NULL) {
		      return NULL;
     }
     new_game->rows = rows;
     new_game->cols = cols;
     new_game->pl[0] = initPlayer(rows,cols,0,p0_sy,p0_ctrl);
     new_game->pl[1] = initPlayer(rows,cols,1,p1_sy,p1_ctrl);

     if (new_game->pl[0] == NULL || new_game->pl[1] == NULL)
	 {
		 freeGame(new_game);
		 return NULL;
     }
     new_game->board = initBoard(rows,cols,new_game->pl);
     if (new_game->board == NULL)
	 {
		 freeGame(new_game);
		 return NULL;
     }
	 initTurnPath(new_game);
	 if (new_game->turn_path == NULL)
	 {
		freeGame(new_game);
	 	return NULL;
	 }

	 err_flag = initValidMoves(new_game);
     if (err_flag == MEM_ERROR)
	 {
         return NULL;
     }
     if (new_game->v_moves == NULL)
	 {
		 freeGame(new_game);
		 return NULL;
     }

     if (p0_ctrl == HUM &&  p1_ctrl == PC)
	 {
		 new_game->game_type = 0;
		 new_game->backup = initBackup(new_game);
		 if (new_game->backup == NULL){
             freeGame(new_game);
             return NULL;
         }
		 new_game->undo_flag = FALSE;
     }
     else
	 {
		new_game->game_type = 1;
     }
     return new_game;
}
static int initValidMoves (Game * g)
{
	Moves * v_moves;
    int err_flag;
	g->v_moves = malloc(sizeof( *g->v_moves));
	if (g->v_moves == NULL)
	{
		return;
	}
	err_flag = validMoves(g);
    if (err_flag == MEM_ERROR)
	{
        return MEM_ERROR;
    }
	return FALSE;
}
static void initTurnPath(Game * g)
{
	g->turn_path = malloc(sizeof(*g->turn_path));
	if (g->turn_path == NULL)
	{
		return;
	}
	g->pl_turn =g->pl[0];
	return;
}


static void initChip(Chip * new_chip,Player * owner, size_t row, size_t col)
{
     int n;
     new_chip->owner = owner;
     new_chip->owned = 0;
     new_chip->row = row;
     new_chip->col = col;
     if (owner != NULL)
	 {
		 n = owner->chip_cant;
		 owner->chipset[n] = new_chip;
		 (owner->chip_cant)++;
		 new_chip->owned = 1;
     }
     return;
}

Board initBoard(size_t rows, size_t cols, Player *  pl[2])
{
     Board board;
     int i,j;
     Player * player;
     player = pl[0];
     board = allocateBoard(rows,cols);
     if(board == NULL)
	 {
	  return NULL;
     }
     for(i=0;i<rows;i++)
	 {
		 if ( (rows-1) / 2 == i )
		 {
			   player = pl[1];
			   for(j=0;j< cols/2 ;j++)
			    {
					initChip(board[i][j],player,i,j);
					player = minorChangePlayer(player,pl);
			    }
				initChip(board[i][j], NULL,i,j);
			    for(j=j+1;j<cols;j++)
				{
					initChip(board[i][j],player,i,j);
				    player = minorChangePlayer(player,pl);
				}
		  }
		  else
		  {
			   if( (rows-1)/2 < i )
			   {
					player = pl[0];
			   }
			   else
			   {
					player = pl[1];
			   }
			   for(j=0;j<cols;j++)
			   {
					initChip(board[i][j],player,i,j);
			   }
		  }
      }
     return board;
}

static Player *
initPlayer(size_t rows, size_t cols, int id, char symbol,int control)
{
     Player * new_player;
     int dim;
     new_player = malloc(sizeof(*new_player));
     if(new_player == NULL)
	 {
		return NULL;
     }
     new_player->control = control;
     dim = (rows/2)*cols+cols/2;
     new_player->chipset = malloc(sizeof(*new_player->chipset)*(dim));
     new_player->chipset_dim = dim;
     if (new_player->chipset == NULL)
	 {
        free(new_player);
        return NULL;
     }
     new_player->id = id;
     new_player->symbol = symbol;
     new_player->chip_cant = 0;
     return new_player;
}


static void freeGame(Game * g){
    freeBoard(g->board,g->rows,g->cols,g->cols);
    freePlayer(g->pl[0]);
    freePlayer(g->pl[1]);
    freeTurnPath(g);
    free(g->turn_path);
    freeMoves(g->v_moves);
    free(g->v_moves);
    if (g->game_type == 0)
	{
        freeBackup(g->backup, g->rows, g->cols);
    }
    free(g);
    return;
}
void freeBackup(Backup * backup, int rows, int cols){
    if (backup == NULL)
	{
        return;
    }
    freeBoard(backup->board,rows, cols,cols );
    freePlayer(backup->pl[0]);
    freePlayer(backup->pl[1]);
    free(backup);
    return;
}
static void freeBoard(Board board, int n, int m, int cols)
{
     int i, j;
     if (board == NULL){
         return;
     }
     for (i=0;i<n-1;i++){
	  for (j = 0; j < cols; j++){
	       free(board[i][j]);
	  }
	  free(board[i]);
     }
     for(j=0; j < m ;j++){
	  free(board[i][j]);
     }
     free(board[i]);
     free(board);
     return;
}
static void freeChipset(Player *  player)
{
     free(player->chipset);
     player->chip_cant = 0;
     return;
}


static void freeTurnPath(Game_p g)
{
     int i;

     if (g->turn_path == NULL){
         return;
     }
     for ( i=0;i < g->turn_path->chip_cant; i++){
	  g->turn_path->chipset[i]->owned = 0;
     }
     g->turn_path->chip_cant = 0;
     free(g->turn_path->chipset);
     g->turn_path->chipset = NULL;
	 g->curr_chip = NULL;
     g->move_no = 0;
     return;
}

void freeMoves(Moves * m)
{
  int i;
  if (m == NULL){
      return;
  }
  for (i=0;i<m->count;i++){
    free(m->moveset[i]);
  }
  free(m->moveset);
  m->moveset = NULL;
  m->count = 0;
  return;
}

void endGame(Game_p g)
{
     freeGame(g);
     return;
}

static Player * minorChangePlayer(Player * p,Player * pl[2])
{
     if ( pl[0]->id == p->id )
	  return pl[1];
     else if ( pl[1]->id == p->id)
	  return pl[0];
	  return NULL;
}

static void freePlayer(Player * p){
      if (p == NULL){
          return;
      }
      free(p->chipset);
      free(p);
      return;
}


static int moveChip(Move * m, Board board, Player * pl[2], int rows, int cols)
{
     int inc_row,inc_col, eaten = 0;
     Player * pl_o = NULL;
     Chip * chip_player, * chip_empty;
     Chip * chip_pos, * chip_pre;
     char dir;


     chip_player = board[m->s[0]][m->s[1]];
     chip_empty  = board[m->d[0]][m->d[1]];
     if (chip_player->owner->id == pl[0]->id )
     {
	  pl_o = pl[1];
     }
     else if (chip_player->owner->id == pl[1]->id)
     {
	  pl_o = pl[0];
     }



     swapCoordinates(chip_player,chip_empty, board);
     inc_row = chip_player->row - chip_empty->row;
     inc_col = chip_player->col - chip_empty->col;
     chip_pos = getIncChip(chip_player,inc_row, inc_col,board, rows, cols);
     chip_pre = getIncChip(chip_empty,-inc_row,-inc_col,board, rows, cols);

     if ( chip_pos != NULL && chip_pos->owner == pl_o)
     {
         if (m->dir == 0 || m->dir == 'a')
         {
            eaten = eatChips(chip_pos,inc_row,inc_col,board, pl_o, rows, cols);
         }
     }
     else if(chip_pre != NULL && chip_pre->owner == pl_o)
     {
         if (m->dir == 0 || m->dir == 'w')
         {
	        eaten = eatChips(chip_pre,-inc_row,-inc_col,board,pl_o, rows, cols);
         }
     }
     return eaten;
}

static void swapCoordinates(Chip * chip_s, Chip * chip_d, Board board)
{
     int aux_row, aux_col;
     board[chip_s->row][chip_s->col] = chip_d;
     board[chip_d->row][chip_d->col] = chip_s;
     aux_row = chip_s->row;
     aux_col = chip_s->col;
     chip_s->row = chip_d->row;
     chip_s->col = chip_d->col;
     chip_d->row = aux_row;
     chip_d->col = aux_col;
     chip_d->owned = 1;
}

static Chip *
getIncChip(Chip * c, int inc_row, int inc_col,Board board, int rows, int cols)
{
     int p_row, p_col;
     p_row = c->row + inc_row;
     p_col = c->col + inc_col;
     if (NOT_OUT_OF_BOARD(p_row,p_col,rows,cols)){
	  return board[p_row][p_col];
     }
     return NULL;
}

static int
eatChips(Chip * curr_chip,int inc_row,int inc_col,Board board,Player * pl_o,int rows, int cols)
{
     int  n = 0 ;
     void * aux;
     while ( curr_chip != NULL && curr_chip->owner == pl_o){
	  loseChip( pl_o, curr_chip);

	  curr_chip = getIncChip(curr_chip,inc_row,inc_col,board, rows, cols);
	  n++;
     }
     return n;
}

static void loseChip(Player *  player, Chip *  chip)
{
     int i=0, dim;
     /* este while no controla que i sea menor a chip_cant para ahorrar una
     ** comparacion innecesaria (el chip esta si o si en el chipset)
     */
     while ( player->chipset[i] != chip){
	  i++;
     }
     dim = --(player->chip_cant);
     player->chipset[i] = player->chipset[dim];
     chip->owned = 0;
     chip->owner = NULL;
     return;
}

int endTurn(Game * game)
{
     int err_flag;
     freeTurnPath(game);
     minorChangePlayerTurn(game);
	 game->curr_chip = NULL;
	 game->curr_dir[0] = 0;
	 game->curr_dir[1] = 0;
     err_flag = validMoves(game);
     if (err_flag == MEM_ERROR){
         return MEM_ERROR;
     }
     return FALSE ;
}


static int addToTurnPath(TurnPath * turn_path, Chip * chip)
{
     int n, count;
     void * aux;

     n = turn_path->chip_cant;
     if ( n % MEM_BLOCK == 0)
     {
        count = n + MEM_BLOCK;
        turn_path->chipset = malloc(sizeof(*(turn_path->chipset))*count);
        // if ( aux == NULL)
        // {
        //    return MEM_ERROR;
        // }
     }
     turn_path->chipset[n] = chip;
     turn_path->chip_cant++;
     chip->owned = 1;
     return 0;
}



static void minorChangePlayerTurn( Game * g)
{
     if ( g->pl_turn == g->pl[0]){
	 g->pl_turn = g->pl[1];
     }
     else{
	  g->pl_turn = g->pl[0];
     }
     return;
}

static int validMoves (Game * g)
{
     int flag = FALSE;
     int aux = 0;
     Move ** moveset;

     g->v_moves->count = 0;

     if (g->move_no!=0 && g->curr_chip == NULL){
	  return flag;
     }
     if (g->move_no!=0){
          flag = validChipMove(g->curr_chip, g, 0, &aux, captureMove);
     }
     else{
         flag = mandatoryMoves(g);
     }
     if (flag != MEM_ERROR){
         moveset = g->v_moves->moveset;
         moveset = malloc(sizeof(*moveset)*g->v_moves->count);
     }
     return flag;
}

static int mandatoryMoves (Game * g)
{
     int i ;
     int valid;
     int flag = FALSE;
     int careful_moves = 0;
     int careful;
     Chip * chip;

     for (i=0 ;i< g->pl_turn->chip_cant && flag == FALSE;i++){
         careful = TRUE;
         if (inDanger(g->pl_turn->chipset[i],g)){
             careful = FALSE;
         }
         chip = g->pl_turn->chipset[i];
         flag = validChipMove(chip, g, careful ,&careful_moves, paikaMove);
     }
     if (flag == TRUE){
         freeMoves(g->v_moves);
         for (i = i-1 ;i< g->pl_turn->chip_cant && flag != MEM_ERROR;i++){
             chip = g->pl_turn->chipset[i];
             flag = validChipMove(chip,g,careful, &careful_moves, captureMove);
         }
     }
     if (careful_moves == 0){
         freeMoves(g->v_moves);
     }
     return flag;
}

static int
validChipMove(Chip * chip, Game * g, int c,int * c_moves,
                int (*f)(Game *,Move,int *,int,int *))
{
    int j;
    int dirs[][2] ={N,S,E,W,NW,NE,SW,SE};
    int pos_type;
    int ans = FALSE;
    Move move;
    Chip * dest_chip;

    move.s[0] = chip->row;
    move.s[1] = chip->col;

    pos_type = posType(move.s[0],move.s[1]);
    for (j=0;j < pos_type && ans == FALSE ;j++){

         move.d[0]=move.s[0]+dirs[j][0] ;
         move.d[1]=move.s[1]+dirs[j][1] ;

         if (NOT_OUT_OF_BOARD(move.d[0],move.d[1],g->rows,g->cols)){

            dest_chip = g->board[move.d[0]][move.d[1]];
            if (dest_chip->owned == 0)
            {
                ans = f(g, move,dirs[j], c,c_moves);
            }
            ;
         }
    }
    return ans;
}

static int
    paikaMove(Game * g, Move m, int dir[2],int careful, int * careful_moves)
{
    int ans = FALSE;
    Chip * dest_chip;
    Move **  aux;
    int count;
    Move * move;
    Move ** moveset;
    moveset = g->v_moves->moveset;
    count = g->v_moves->count;
    dest_chip = g->board[m.d[0]][m.d[1]];

    if ( softCapture(g,m,dir) == FALSE)
    {

        if (careful == FALSE || !inDanger(dest_chip,g))
        {
            (*careful_moves)++;
        }
        if (( count % BLOCK)==0)
        {
            g->v_moves->moveset = malloc(sizeof(*moveset) * (count + BLOCK));
            // if (aux==NULL)
            // {
            //    return MEM_ERROR;
            // }
            //  = aux;
        }
        move = assignMove(m,0) ;
        if ( move == NULL)
        {
            return MEM_ERROR;
        }
        else
        {
            g->v_moves->moveset[count] = move;
            (g->v_moves->count)++;
            ans = FALSE;
        }
    }
    else{
       ans = TRUE;
    }
    return ans;
}

static int inDanger (Chip * curr_chip , Game_p g)
{
	int pos[2] , pos_type , i ;
	int dirs[][2]={ N , S , E , W , NE , NW , SW , SE };
	pos[0]=curr_chip->row ;
	pos[1]=curr_chip->col ;
	pos_type=posType(pos[0],pos[1]) ;
	for (i=0 ; i<pos_type ; i++ )
	{
		if ( evalAttackDir(pos, dirs[i],g))
		{
			return TRUE;
		}
	}
	return FALSE ;
}

static int evalAttackDir( int pos[2], int inc[2], Game_p g)
{
	int p1[2], p2[2];
	Chip * c1, *c2;

	p1[0]= pos[0]+inc[0];
	p1[1]= pos[1]+inc[1];
	p2[0]= p1[0]+inc[0];
	p2[1]= p1[1]+inc[1];

	if (NOT_OUT_OF_BOARD(p1[0],p1[1],g->rows,g->cols))
    {
        if (NOT_OUT_OF_BOARD(p2[0],p2[1],g->rows,g->cols))
        {
    		c1 = g->board[p1[0]][p1[1]];
    		c2 = g->board[p2[0]][p2[1]];
    		if (c1->owned == FALSE)
            {
    			if (c2->owner != NULL && c2->owner->id != g->pl_turn->id)
                {
    				return TRUE;
    			}
    		}
    		else if (c1->owner != NULL && c1->owner->id != g->pl_turn->id)
            {
    			if (c2->owned == FALSE)
                {
    				return TRUE;
    			}
    		}
        }
	}
	return FALSE ;
}

static int
captureMove(Game * g, Move m, int dir[2], int careful, int * careful_move )
{
    int temp_pos1[2], temp_pos2[2];
    Move  ** aux;
    Move * move;
    Move ** moveset;
    int count;

	temp_pos1[0] = m.d[0]+dir[0];
    temp_pos1[1] = m.d[1]+dir[1];
	temp_pos2[0] = m.s[0]-dir[0];
    temp_pos2[1] = m.s[1]-dir[1];

    if (!(g->curr_dir[0] == dir[0] && g->curr_dir[1] == dir[1]))
    {
        //ans = hardCapture(g, m, dir, careful_move);
        if (isRival(g, temp_pos1) == TRUE)
        {
            count = g->v_moves->count;
            moveset = g->v_moves->moveset;
            if (count % BLOCK == 0)
            {
                 g->v_moves->moveset = malloc(sizeof(*moveset) * (count+BLOCK));
                //  if (aux == NULL)
                //  {
                //      return MEM_ERROR;
                //  }
                //   = aux;
              }
            move = assignMove(m,'a') ;
            if ( move == NULL)
            {
             return MEM_ERROR;
            }
            g->v_moves->moveset[count] = move;
            (g->v_moves->count)++;
            (*careful_move)++;
        }
        if (isRival(g, temp_pos2) == TRUE)
        {
            count = g->v_moves->count;
            moveset = g->v_moves->moveset;
            if (count % BLOCK == 0)
            {
                g->v_moves->moveset  = malloc(sizeof(*moveset) * (count+BLOCK));
                // if (aux == NULL)
                // {
                //    return MEM_ERROR;
                // }
                // = aux;
            }
            move = assignMove(m,'w') ;
            if ( move == NULL)
            {
                return MEM_ERROR;
            }
            g->v_moves->moveset[count] = move;
            (g->v_moves->count)++;
            (*careful_move)++;
    	}

    }
    return FALSE;
}

static int posType (size_t row, size_t col)
{
	return ((row+col)%2==0?STRONG:WEAK) ;
}


static int softCapture(Game * g, Move m, int dir[2])
{
    int temp_pos1[2], temp_pos2[2];
    int ans = FALSE;

	temp_pos1[0] = m.d[0]+dir[0];
    temp_pos1[1] = m.d[1]+dir[1];
	temp_pos2[0] = m.s[0]-dir[0];
    temp_pos2[1] = m.s[1]-dir[1];

	if (isRival(g, temp_pos1) == TRUE || isRival(g, temp_pos2) == TRUE){
		return TRUE;
	}
   	return FALSE;
}

static int isRival(Game * g, int temp[2])
{
	Player * chip_owner;

	if (NOT_OUT_OF_BOARD(temp[0],temp[1],g->rows,g->cols)){
     chip_owner = g->board[temp[0]][temp[1]]->owner;
     if(chip_owner != NULL && chip_owner->id != g->pl_turn->id){
         return TRUE;
     }
    }
	return FALSE;
}

static Move * bot (Game * g)
{
     return (g->v_moves->moveset)[rand()%(g->v_moves->count)] ;
}




static Move *
assignMove (Move m, const char direction)
{
     Move * to_be_assigned ;
     to_be_assigned= malloc(sizeof( *to_be_assigned));
     if (to_be_assigned==NULL)
     {
	  return NULL;
     }

     to_be_assigned->s[0]=m.s[0];
     to_be_assigned->s[1]=m.s[1];
     to_be_assigned->d[0]=m.d[0];
     to_be_assigned->d[1]=m.d[1];
     to_be_assigned->dir =direction ;
     return to_be_assigned;
}

int getPlayerTurnSym(Game * g)
{
     return g->pl_turn->symbol;
}

int getPlayerTurnCtrl(Game * g)
{
     return g->pl_turn->control;
}

size_t getRows(Game * g)
{
     return g->rows;
}
size_t getCols(Game * g)
{
     return g->cols;
}

int getMoveCount(Game_p g)
{
    return g->v_moves->count;
}
Moves * getMoves(Game * g)
{
     int err_flag;
     Moves * moves;
     moves = malloc(sizeof (*moves));
     if (moves == NULL){
         return NULL;
     }
     err_flag = cpyMoves(moves,g->v_moves);
     if (err_flag == MEM_ERROR){
         free(moves);
         return NULL;
     }
     return moves;
}

Backup * getBackup(Game * g)
{
    Backup * backup;
    backup = initBackup(g);
    return backup;

}
static int cpyMoves(Moves * m1, const Moves * m2)
{
    int err_flag, i;
    m1->count = 0;
    m1->moveset = malloc(sizeof(*(m1->moveset))*m2->count);
    if (m1->moveset == NULL){
        return MEM_ERROR;
    }
    for(i=0;i<m2->count;i++){
        m1->moveset[i] = malloc(sizeof(**(m1->moveset)));
        if (m1->moveset[i] == NULL){
            freeMoves(m1);
            return MEM_ERROR;
        }
        cpyMove(m1->moveset[i], m2->moveset[i]);
        (m1->count)++;
    }
    return FALSE;
}

static void cpyMove(Move * m1, const Move * m2)
{
    m1->s[0] = m2->s[0];
    m1->s[1] = m2->s[1];
    m1->d[0] = m2->d[0];
    m1->d[1] = m2->d[1];
    m1->dir = m2->dir;
    return;
}


int moveTo(Game * g, Move * m, int pl_ctrl)
{
     int eaten, v, err_flag;
     Move * cm = m;
     Board backup_board;

     if (pl_ctrl == HUM){
            v = findMove(g,cm);
            if (v == AMBIGUOUS){
             return AMBIGUOUS;
            }
            if (v == 0){
              return -1;
            }

            if (g->game_type == 0 && g->move_no == 0){
                backup_board = g->backup->board;
                cpyBoard(backup_board,g->board,g->rows,g->cols,g->backup->pl);
                g->undo_flag = TRUE;
            }
     }
     else if (pl_ctrl == PC){
         cm = bot(g);
     }

     eaten = moveChip(cm,g->board,g->pl, g->rows, g->cols);
     err_flag = addToTurnPath(g->turn_path, g->board[cm->s[0]][cm->s[1]]);
     if (err_flag == MEM_ERROR){
         return MEM_ERROR;
     }
     setCurrChip(cm, g, eaten);
     freeMoves(g->v_moves);
     err_flag = validMoves(g);
     if (err_flag == MEM_ERROR){
         return MEM_ERROR;
     }
     return eaten;
}

void setCurrChip(Move * m, Game * g, int eaten)
{
	 int dir[2];

     if (eaten){
	  g->curr_chip = g->board[m->d[0]][m->d[1]];
	  g->curr_dir[0] = m->d[0] - m->s[0];
	  g->curr_dir[1] = m->d[1] - m->s[1];
     }
     (g->move_no)++;
     return;
}

int findMove(Game * g,const Move * m)
{
     int i, flag = FALSE;
     for (i=0; i < g->v_moves->count && flag == FALSE;i++)
     {
	  if (equalMove(m, g->v_moves->moveset[i] ))
      {
          flag = TRUE;
	  }
     }
     if(i< g->v_moves->count && m->dir == 0 )
     {
        if (equalMove(m,g->v_moves->moveset[i]))
        {
            flag = AMBIGUOUS;
        }
     }
     return flag;
}

int equalMove(const Move * m1, const Move * m2)
{
     return m1->s[0] == m2->s[0] && m1->s[1] == m2->s[1] && m1->d[0] == m2->d[0]\
        && m1->d[1] == m2->d[1] && (m1->dir == m2->dir || m1->dir == 0);
}


int winState(Game * g)
{
     return g->pl[0]->chip_cant == 0 || g->pl[1]->chip_cant == 0;
}


int getPlayerChipCant(Game * game_p, int id)
{
     return game_p->pl[id]->chip_cant;
}

char getPlayerSym(Game * game_p, int id)
{
     return game_p->pl[id]->symbol;
}

// int saveGame (Game * curr_game, char * file_name)
// {
//      FILE * dest=NULL ;
// 	 int params[4] ; /* vector de parametros (0- tipo de juego 1-turno 2-fils 3-cols */
//      int i , j ;
// 	 char zero='0' ;
//      Player * curr_player ;
//      params[0]=curr_game->game_type ;
// 	 params[1]=((curr_game->pl_turn)==(curr_game->pl[0])?1:2) ;
//      params[2]=curr_game->rows ;
//      params[3]=curr_game->cols ;
//      dest=fopen(file_name,"wb") ;
//      if (dest==NULL)
//      {
//           free(file_name) ;
//           return MEM_ERROR ; /* error de memoria */
//      }
// 	 fwrite(params,sizeof(params),1,dest) ;
//      for (i=0;i<params[2];i++)
//      {
//           for(j=0;j<params[3];j++)
//           {
//                curr_player=((curr_game->board)[i][j])->owner ;
//                if (curr_player!=NULL)
//                {
//                fwrite(&(curr_player->symbol),sizeof(char),1,dest) ;
//                }
//                else
//                {
//                fwrite(&zero,sizeof(char),1,dest) ;
//                }
//           }
//      }
//      fclose(dest) ;
//      return 0 ;
//
// }
//
// Game_p loadGame(char * file_name)
// {
//     Game_p load_game=NULL ;
//     FILE * saved_file;
//     int i,j,n,control=0,count=0;
// 	char c;
//     int params[4] ;
// 	Player * owner;
//     Chip * new_chip;
//     saved_file=fopen(file_name,"r") ;
//     if (saved_file==NULL)
//     {
// 		return NULL ; /* EL ARCHIVO NO EXISTE */
//     }
//     control=fread(params,sizeof(params),1,saved_file) ;
// 	if (control!=1)
// 	{
// 		return NULL ; /* EL ARCHIVO ESTA CORRUPTO */
// 	}
// 	if (validateBoard(params[2],params[3])==ERROR)
// 	{
// 		return NULL ;
// 	}
// 	if ( params[1]<1 || params[1]>2 )
// 	{
// 		return NULL ;
// 	}
// 	switch (params[0])
// 	{
// 		case 0:
// 		{
// 			load_game=initGame(params[2],params[3],'&','#', HUM,PC ) ;
// 			break ;
// 		}
// 		case 1:
// 		{
// 			load_game=initGame(params[2],params[3],'&','#', HUM,HUM ) ;
// 			break ;
// 		}
// 		default :
// 		{
// 			return NULL ;
// 			break ;
// 		}
//     }
//     if (load_game == NULL)
//     {
//         return NULL;
//     }
//     load_game->pl_turn=(params[1]==1?load_game->pl[0]:load_game->pl[1]) ;
// 	load_game->pl[0]->chip_cant = 0;
//     load_game->pl[1]->chip_cant = 0;
//     for (i=0;(i<params[2])&&(!feof(saved_file));i++)
//     {
//           for(j=0;j<params[3]&&(!feof(saved_file));j++)
// 			{
// 				fread(&c,sizeof(c),1,saved_file);
// 				switch(c)
// 				{
// 					case '&' :
// 					{
// 						new_chip = load_game->board[i][j];
// 						owner = load_game->pl[0];
// 						new_chip->owned = 1;
// 						new_chip->owner = owner;
// 						n = owner->chip_cant;
// 						owner->chipset[n] = new_chip;
// 						owner->chip_cant++;
// 						break ;
// 					}
// 					case '#' :
// 					{
// 						new_chip = load_game->board[i][j];
// 						owner = load_game->pl[1];
// 						new_chip->owned = 1;
// 						new_chip->owner = owner;
// 						n = owner->chip_cant;
// 						owner->chipset[n] = new_chip;
// 						owner->chip_cant++;
// 						break ;
// 					}
// 					case '0' :
// 					{
// 						load_game->board[i][j]->owned= 0;
// 						load_game->board[i][j]->owner= NULL;
// 						break ;
// 					}
// 					default :
// 					{
// 						freeGame(load_game);
// 						return NULL ;
// 						break ;
// 					}
// 				}
// 				count++ ;
// 			}
//     }
// 	if(count!=(params[2]*params[3]))
// 	{
// 		freeGame(load_game);
// 		return NULL ;
// 	}
//     load_game->backup = initBackup(load_game);
//     validMoves(load_game);
//     fclose(saved_file) ;
//     return load_game ;
// }

void cpyBoard(Board board_d, Board board_s,  int rows, int cols, Player ** pl)
{
     int i, j;

     pl[0]->chip_cant = 0;
     pl[1]->chip_cant = 0;
     for (i=0; i<rows;i++){
          for (j = 0; j<cols;j++){
               cpyChip(board_d[i][j], board_s[i][j], pl);
          }
     }
     return;
}

void cpyChip( Chip * chip_d, Chip * chip_s,  Player ** pl)
{
	Player * player;
	chip_d->row = chip_s->row;
	chip_d->col = chip_s->col;
    chip_d->owned = chip_s->owned;
	if (chip_s->owner == NULL)
	{
		chip_d->owner = NULL;
		return;
	}
	if (chip_s->owner->id == pl[0]->id)
	{
		player = pl[0];
	}
	else if (chip_s->owner->id == pl[1]->id)
	{
		player = pl[1];
	}
	chip_d->owner = player;
	player->chipset[player->chip_cant] = chip_d;
	(player->chip_cant)++;
	return;
}

Backup * initBackup(Game_p g)
{
    Backup * backup;
    Player * pl[2];
    Board board;
    int max_chips;

    backup = malloc(sizeof(*backup));
    if (backup == NULL)
	{
        g->backup = NULL;
        return;
    }
    pl[0] = initPlayer(g->rows,g->cols,0,g->pl[0]->symbol,HUM);
    pl[1] = initPlayer(g->rows,g->cols,1,g->pl[1]->symbol,PC);

    board = allocateBoard(g->rows,g->cols);
    if (pl[0] == NULL || pl[1] == NULL || board == NULL)
	{
        freeBoard(board,g->rows,g->cols,g->cols);
        freePlayer(pl[0]);
        freePlayer(pl[1]);
        free(backup);
        g->backup = NULL;
        return;
    }
   	cpyBoard(board,g->board,g->rows, g->cols,pl);
    backup->pl[0] = pl[0];
    backup->pl[1] = pl[1];
    backup->board = board;
    return backup;
}

Board allocateBoard(int rows, int cols)
{
     Board new_board;
     int i,j;

     new_board = malloc(sizeof(*new_board)*rows);
     if (new_board == NULL)
     {
         return NULL;
     }
     for(i=0;i<rows;i++){
	  new_board[i] = malloc(sizeof(**new_board)*cols);

      if (new_board[i] == NULL)
      {
          freeBoard(new_board, i, 0, cols);
          return NULL;
      }
	  for(j=0;j<cols;j++){
	       new_board[i][j] = malloc(sizeof(***new_board));
           if (new_board[i][j] == NULL)
           {
               freeBoard(new_board, i, j, cols);
               return NULL;
           }
	  }

     }
     return new_board;
}

int undo(Game * g)
{
     int err_flag;

     if (g->game_type == 1)
	 {
	  return ERR_MULT;
     }
     else if (g->undo_flag == FALSE)
	 {
	  return ERR_COUNT;
     }

	 g->pl[0]->chip_cant = 0;
     g->pl[1]->chip_cant = 0;
     freeTurnPath(g);
     cpyBoard(g->board,g->backup->board,g->rows,g->cols, g->pl);
     g->undo_flag = FALSE;
     err_flag = validMoves(g);
     if (err_flag == MEM_ERROR)
	 {
         return MEM_ERROR;
     }
     return FALSE;
}
