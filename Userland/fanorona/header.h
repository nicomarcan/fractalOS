#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SINGLE	    0
#define MULTI	    1
#define ERROR       11
#define ERR_COUNT   401
#define AMBIGUOUS	403
#define ERR_MULT    402
#define MEM_ERROR   666
#define PC          777
#define HUM         888

#define CHANGE_PLAYER_CHAR(c)	(c=='&'?'@':'O')

typedef struct Game * Game_p ;

typedef struct Move
{
	 int s[2];
	 int d[2];
	 char dir;
}Move;

typedef Move ** MoveSet;

typedef struct Moves
{
     int count ;
     Move ** moveset ;
} Moves ;

typedef struct Chip
{
     int row;
     int col;
     int owned;
     struct Player *  owner;
} Chip;

typedef struct Chip * Chip_p;
typedef Chip ** Chipset;
typedef struct Chip *** Board;

typedef struct Player
{
     char symbol;
     int chip_cant;
     int chipset_dim;
     int id;
     int control;
     Chipset chipset;
} Player;

typedef struct Backup{
    int rows;
    int cols;
    Board board;
    Player * pl[2];
} Backup;

/* FUNCIONES PARA EL CONTROL DEL JUEGO */

Game_p initGame(int rows,int cols,char p0_sy,char p1_sy,int p0_ctrl,int p1_ctrl);
/*Inicializa el juego. Devuelve NULL si ocurre un error de memoria.
*/
int undo(Game_p g);
/*Realiza una copia del ultimo backup disponible sobre el tablero actual,
**para deshacer las jugadas. Devuelve error si el undo no corresponde o si
**ocurre un error de memoria. Vuelve a calcular las jugadas validas para
**este nuevo tablero.
*/

int winState(Game_p g);
/*Retorna TRUE si algun jugador gano el juego. Esto sucede si la cantidad de
**fichas del contrincante es cero.
*/
void endGame(Game_p g);

int moveTo(Game_p g, Move * m, int pl_ctrl);
/*Funcion de interfaz para el movimiento. Devuelve -1 si no es valido,
**las fichas comidas si es valido o MEM_ERROR si ocurre un error de memoria.
*/

int endTurn(Game_p game);
/*Finaliza el turno y calcula las jugadas validas.
*/

/* OBTENCION DE DATOS DE LAS ESTRUCTURAS */
size_t getRows(Game_p g);
size_t getCols(Game_p g);
Moves * getMoves(Game_p g);
/*Devuelve una copia de los movimientos validos, o NULL si ocurre un eror de
**memoria.
*/
int getPlayerTurnSym(Game_p g);
int getPlayerTurnCtrl(Game_p g);
Backup * getBackup(Game_p g);
/*Devuelve una copia del campo backup de la estructura game, o NULL si ocurre
**un error de memoria.
*/
int getPlayerChipCant(Game_p game_p, int id);
int getMoveCount(Game_p g);

/* VALIDACION */
int validateBoard(size_t rows, size_t cols);

/* LIBERAR MEMORIA */
void freeMoves(Moves * m);
void freeBackup(Backup * backup, int rows, int cols);

/* GUARDADO Y RECUPERADO DE LA PARTIDA */
int saveGame (Game_p curr_game, char * file_name);
/*Crea un archivo de guardado segun el estandar establecido por la catedra.
*/
Game_p loadGame(char * file_name);
/*Carga un archivo de guardado en una estructura game. Si la carga es exitosa,
**devuelve un puntero a dicha estructura, en caso contrario devuelve NULL.
*/
