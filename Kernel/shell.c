#include <shell.h>
#include <clib.h>
#include <kb_driver.h>
#include <utils.h>
#define NULL 0



/* command table entry */
typedef struct {
  uint8_t * command;
  int64_t (*func_ptr)(uint64_t argc, uint8_t ** argv);
} CommDescr;

int64_t get_input(uint8_t * shell_buf, uint64_t size);
CommDescr *  init_entry(uint8_t * command, int64_t (*func_ptr) (uint64_t, uint8_t**));
void add_entry(uint8_t * command, int64_t (*func_ptr) (uint64_t, uint8_t**));
CommDescr * parse_comm(const uint8_t * shell_buff, CommDescr ** comm_table);
int64_t read_arg(uint8_t * argv, uint8_t ** shell_buff );
int64_t parse_arg(uint8_t ** shell_buff, uint8_t * argv[MAX_ARGUMENT_SIZE]);

static CommDescr * comm_table[COMM_TABLE_MAX_SIZE];
static uint64_t comm_table_size;
static uint8_t exit_val = 0;

int64_t hello_world(uint64_t argc, uint8_t * argv[]) {
  putchars("hello, world",12);
}

int64_t exit(uint64_t argc, uint8_t * argv[]) {
  exit_val = 1;
}

int64_t echo(uint64_t argc, uint8_t * argv[]) {
  for (int i=0; i < argc; i++) {
    ncPrint(argv[i]);
  }
}

void add_entry(uint8_t * command, int64_t (*func_ptr) (uint64_t, uint8_t**)) {
  if (comm_table_size < COMM_TABLE_MAX_SIZE) {
    comm_table[comm_table_size] = init_entry(command, func_ptr);
    comm_table_size++;
  }
}

void init_shell(){
  /* init command table. */
  add_entry("hello-world", hello_world);
  add_entry("echo", echo);
  add_entry("exit", exit);
}

CommDescr *  init_entry(uint8_t * command, int64_t (*func_ptr) (uint64_t, uint8_t**)){
  CommDescr * hw = malloc(sizeof(CommDescr));
  hw->command = command;
  hw->func_ptr = func_ptr;
  return hw;
}
void comm_err(uint8_t * comm_str) {
  ncPrint(comm_str);
  ncPrint(" no es un comando valido.");
}
void arg_err() {
  ncPrint("Error de sintaxis.");
}

uint8_t shell() {
  static uint8_t shell_buf[BUFSIZ];
  static uint8_t comm_str[BUFSIZ];
  uint8_t * shell_buf_ptr = shell_buf;
  uint8_t * argv[MAX_ARGUMENT_SIZE];
  uint64_t len;
  int64_t argc;
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
      comm->func_ptr(argc, argv);
    }
  }

  putchar('\n');
  return exit_val;
  // CommDescr * comm_descr = parse_comm(&shell_buff, comm_table);
  // parse_arg(&shell_buff, comm_descr->argc, argv);

}

CommDescr * parse_comm(const uint8_t * comm_str, CommDescr ** comm_table) {
  uint64_t i = 0;
  CommDescr * curr_comm;
  static uint8_t shell_str [BUFSIZ];
  curr_comm = comm_table[i];

  while (i < comm_table_size && c_strcmp(comm_str, curr_comm->command) != 0) {
    curr_comm = comm_table[++i];
  }
  if (i >= comm_table_size) {
    curr_comm = NULL;
  }
  return curr_comm;
}

/* reads from shell_buff into shell_str until blank char*/
void read_comm(const uint8_t * comm_str, uint8_t ** shell_buf) {
  uint8_t * comm_str_ptr = comm_str;
  while(**shell_buf != '\0' && **shell_buf == ' ') {
    *shell_buf++;
  }
  while (**shell_buf != '\0' && **shell_buf != ' ') {
    *comm_str_ptr++ = *(*shell_buf)++;
  }
  *comm_str_ptr = '\0';
}

int64_t read_arg(uint8_t * argv, uint8_t ** shell_buff ) {
  static const uint8_t QUOTE = 0;
  static const uint8_t N_QUOTE = 1;

  uint8_t st = N_QUOTE;
  uint64_t i = 0;
  uint8_t end_b = 0;

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
          if (st == QUOTE && *( (*shell_buff) + 1) == 'n'){
            argv[i++] = '\n';
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

int64_t get_input(uint8_t * shell_buf, uint64_t size) {
  uint64_t i = 0;
  uint8_t c;
  while ( (c = getchar()) != -1 && c != '\n' && i < size - 1) {
    shell_buf[i++] = c;
  }
  shell_buf[i++] = '\0';
  return i;
}
