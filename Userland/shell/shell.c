#include <shell.h>
#include <clib.h>
#include <fanorona.h>
#include <command.h>
#include <scanf.h>
#include <fractal.h>
#include <c_syscall.h>
#define NULL 0

void * memset(void * destiny, int32_t c, uint64_t length);
extern char bss;
extern char endOfBinary;

/* command table entry */
typedef struct {
  uint8_t * command;
  uint8_t * descr;
  int64_t (*func_ptr)(uint64_t argc, uint8_t ** argv);
} CommDescr;

/* parsing operation */
static int64_t get_input(uint8_t * shell_buf, uint64_t size);
static CommDescr * parse_comm(const uint8_t * shell_buff, CommDescr ** comm_table);
static int64_t read_arg(uint8_t * argv, uint8_t ** shell_buff );
static int64_t parse_arg(uint8_t ** shell_buff, uint8_t * argv[MAX_ARGUMENT_SIZE]);

/* command management */
static CommDescr *  init_entry(uint8_t * command, uint8_t * description, int64_t (*func_ptr) (uint64_t, uint8_t**));
static void add_entry(uint8_t * command, uint8_t * description, int64_t (*func_ptr) (uint64_t, uint8_t**));

/* command table  */
static CommDescr * comm_table[COMM_TABLE_MAX_SIZE];
static uint64_t comm_table_size;

/* exit status boolean */
static uint8_t exit_val = 0;
static void comm_err(uint8_t * comm_str);

/* input error */
static void arg_err();



int64_t shell_main() {
  //Clean BSS
	memset(&bss, 0, &endOfBinary - &bss);

  init_shell();
  while(!shell());
}


static int64_t scan(uint64_t argc, uint8_t * argv[]) {
  int64_t i;
  double d;
  uint8_t buf[10];
  scanf("%s %d %f ", buf, &i, &d);
  printf("%s|%d|%d\n", buf, i, (int64_t)d);
  exit();
}

/* display available commands */
static int64_t help(uint64_t argc, uint8_t * argv[]) {
  printf("Available commands: \n");
  for (int i=0; i<comm_table_size;i++) {
    printf("\t%s - %s\n", comm_table[i]->command, comm_table[i]->descr);
  }
  exit();
}

static int64_t get_colors_from_argv(uint64_t argc, uint8_t * argv[],uint8_t * r, uint8_t * g, uint8_t * b) {
  int64_t ret = sscanf(argv[0], "%x", r);
  if (ret == 0) {
    return 1;
  }
  if (argc > 1) {
    ret = sscanf(argv[1], "%x", g);
    if (ret == 0) {
      return 1;
    }
    if (argc > 2) {
      ret = sscanf(argv[2], "%x", b);
      if (ret == 0) {
        return 1;
      }
    }
  }
  return 0;
}

static int64_t set_shell_color(uint64_t argc, uint8_t * argv[]) {
  int64_t r = 0, g = 0, b = 0, ret;
  if (argc == 0 || argc > 3) {
    exit();
  }
  if (get_colors_from_argv(argc, argv, &r,&g,&b)) {
    exit();
  }
  set_color((uint8_t) r, (uint8_t) g, (uint8_t) b);
  exit();
}

static int64_t set_shell__background_color(uint64_t argc, uint8_t * argv[]) {
  int64_t r = 0, g = 0, b = 0, ret;
  if (argc == 0 || argc > 3) {
    exit();
  }
  if (get_colors_from_argv(argc, argv, &r,&g,&b)) {
    exit();
  }

  set_back_color((uint8_t) r, (uint8_t) g, (uint8_t) b);
  // printf("%x %x %x", r, g, b);
  exit();
}

static int64_t clearscr(){
  clear_screen();
  exit();
}


/* add new entry to commad table */
static void add_entry(uint8_t * command, uint8_t * description, int64_t (*func_ptr) (uint64_t, uint8_t**)) {
  if (comm_table_size < COMM_TABLE_MAX_SIZE) {
    comm_table[comm_table_size] = init_entry(command, description, func_ptr);
    comm_table_size++;
  }
}

/* command table initialization*/
void init_shell(){
  /* init command table. */
  add_entry("hello-world", "simple debug hello, world",hello_world);
  add_entry("echo", "prints input, escapes with \"\"",echo);
  add_entry("fractal", "prints beautiful fractal", fractalMain);
  add_entry("time", "prints current time", curr_time);
  add_entry("fanorona", "play Fanorona(tm).", fanorona_main);
  add_entry("clear", "clear screen", clearscr);
  add_entry("help", "display available commands", help);
  add_entry("scan", "scanf debug", scan);
  add_entry("set-color", "set text color", set_shell_color);
  add_entry("set-back", "set background color", set_shell__background_color);
  add_entry("ps", "list current processes", _ps);
  add_entry("kill", "delete a process", _kill);
  add_entry("infiloop", "infinite loop process", infiloop);
  add_entry("waiter", "wait test process", waiter);
}

/* command entry initialization */
static CommDescr *  init_entry(uint8_t * command, uint8_t * description, int64_t (*func_ptr) (uint64_t, uint8_t**)){
  CommDescr * hw = malloc(sizeof(CommDescr));
  hw->command = command;
  hw->descr = description;
  hw->func_ptr = func_ptr;
  return hw;
}

/* command not found */
static void comm_err(uint8_t * comm_str) {
  printf("%s is an illegal command\n", comm_str);
}

/* arguments fail closing quote check. */
static void arg_err() {
  printf("Wrong syntax\n");
}

/* reads from input and executes a command.*/
uint8_t shell() {
  static uint8_t shell_buf[BUFSIZ];
  static uint8_t comm_str[BUFSIZ];
  uint8_t * shell_buf_ptr = shell_buf;
  uint8_t * argv[MAX_ARGUMENT_SIZE];
  uint64_t len;
  int64_t argc;
  uint8_t retval;
  CommDescr * comm;
  putchars("$ ", 2);

  get_input(shell_buf_ptr, BUFSIZ);

  read_comm(comm_str, &shell_buf_ptr);

  comm = parse_comm(comm_str, comm_table);

  if (comm == NULL) {
    comm_err(comm_str);
  }
  else {
    if ( (argc = parse_arg(&shell_buf_ptr, argv)) == -1) {
      arg_err();
    }
    else {
	  Args * args = malloc(sizeof(Args));
	  args->argc=argc;
	  args->argv=argv;
  	 fkexec(comm->func_ptr,comm->command,args);
  	 free(args);
    }
  }

  return exit_val;

}

/* parses first string from the shell_buff and looks for matching command.*/
CommDescr * parse_comm(const uint8_t * comm_str, CommDescr ** comm_table) {
  uint64_t i = 0;
  CommDescr * curr_comm;

  curr_comm = comm_table[i];
  while (i < comm_table_size && c_strcmp(comm_str, curr_comm->command) != 0) {
    curr_comm = comm_table[++i];

  }
  if (i >= comm_table_size) {
    curr_comm = NULL;
  }
  return curr_comm;
}

/* reads from shell_buff into comm_str until blank char*/
void read_comm(const uint8_t * comm_str, uint8_t ** shell_buf) {
  uint8_t * comm_str_ptr = comm_str;
  while(**shell_buf != '\0' && **shell_buf == ' ') {
    (*shell_buf)++;
  }
  while (**shell_buf != '\0' && **shell_buf != ' ') {
    *comm_str_ptr++ = *(*shell_buf)++;
  }
  *comm_str_ptr = '\0';
}

/* reads argument from shell_buff into argv*/
int64_t read_arg(uint8_t * argv, uint8_t ** shell_buff ) {
  static const uint8_t QUOTE = 0;
  static const uint8_t N_QUOTE = 1;

  uint8_t st = N_QUOTE;
  uint64_t i = 0;
  uint8_t end_b = 0;
  uint8_t tc;

  while(!end_b && i < BUFSIZ) {

    switch(**shell_buff) {
      case ' ':
        if (st == QUOTE) {
          argv[i++] = *(*shell_buff)++;
          break;
        }
      case '\n':
      case '\0':
        end_b = 1;
        break;
      case '"':
        if (st == QUOTE) {
          end_b = 1;
        }
        st = !st;
        (*shell_buff)++;
        break;
      case '\\':
          tc = *((*shell_buff) + 1);
          if (st == QUOTE && (tc == 'n' || tc == 't' || tc == 'b')){
            if ( tc == 'n'){
              argv[i++] = '\n';
            }
            else if (tc== 't') {
              argv[i++] = '\t';
            }
            else{
              argv[i++] = '\b';
            }
            (*shell_buff)++;
            (*shell_buff)++;
            break;
          }
      default:
        argv[i++] = *(*shell_buff)++;
        break;
    }
  }
  argv[i] = '\0';
  return st;
}

/* parses arguments from the shell buffer into argv string array. */
int64_t parse_arg(uint8_t ** shell_buff, uint8_t *argv[MAX_ARGUMENT_SIZE]) {
  uint64_t i = 0;

  while (i < MAX_ARGUMENT_SIZE) {
    while(**shell_buff != '\0' && **shell_buff == ' ') {
      (*shell_buff)++;
    }
    if (**shell_buff == '\0') {
      break;
    }
    argv[i] = malloc(BUFSIZ);
    if (read_arg(argv[i++], shell_buff) == 0) {
      return -1;
    };
  }
  return i;
}

/* gets input from the command line. */
int64_t get_input(uint8_t * shell_buf, uint64_t size) {
  uint64_t i = 0;
  uint8_t c;
  while ( (c = getchar()) != -1 && c != '\n' && i < size - 1) {
    shell_buf[i++] = c;
  }
  shell_buf[i++] = '\0';
  return i;
}
