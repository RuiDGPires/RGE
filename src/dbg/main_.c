#include "../CrimVm/src/vm.h"
#include "../CrimVm/src/util.h"

#include <termios.h>
#include <unistd.h>

struct termios saved_attributes;

#define MAX_LINES 600 
#define MAX_LINE_SIZE 100

static u32 *mapping;

void reset_input_mode (void)
{
  tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode (void)
{
  struct termios tattr;

/* Make sure stdin is a terminal. */
  if (!isatty (STDIN_FILENO))
    {
      fprintf (stderr, "Not a terminal.\n");
      exit (EXIT_FAILURE);
    }

/* Save the terminal attributes so we can restore them later. */
  tcgetattr (STDIN_FILENO, &saved_attributes);
  atexit (reset_input_mode);

/* Set the funny terminal modes. */
  tcgetattr (STDIN_FILENO, &tattr);
  tattr.c_lflag &= ~(ICANON | ECHO);	/* Clear ICANON and ECHO. */
  tattr.c_cc[VMIN] = 1;
  tattr.c_cc[VTIME] = 0;
  tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}

static char file_buff[MAX_LINES][MAX_LINE_SIZE];

#define BLUE "\e[1;36m"
#define YELLOW "\e[1;33m"
#define BOLD "\e[1;37m"
#define GREEN "\e[1;32m"
#define reset "\e[0m"

static int regs[9];

static void print_info(Vm vm){
	if (mapping[vm->pc] == -1) return;
	system("clear");

	printf("\n %s->%s[%u] ", YELLOW, BOLD, mapping[vm->pc]);
	printf("%s%s", file_buff[mapping[vm->pc] -1], reset);
	printf("\n");

	printf(" REGS \n");
	printf("--------\n");
	for (u32 i = 1; i <= 9; i++){
		printf("[%d]: %s0x%x\n%s", i, vm->regs[i] != regs[i-1] ? GREEN : "", vm->regs[i], reset);
		regs[i-1] = vm->regs[i];
	}	

	printf("\n STACK \n");
	printf("--------\n");
	for (u32 i = VM_MEM_SIZE-1; i >= vm->regs[SP]; i--){
		printf("[ 0x%x ]", vm->mem[i]);
		if (i == vm->regs[FP])
			printf(" %sFP%s", BLUE, reset);
		if (i == vm->regs[SP])
			printf(" %sSP%s", YELLOW, reset);

		printf("\n");
	}

	fflush(stdout);
}

static void run_binary(char *file_path){
	Vm_ vm;
	vm_init(&vm);
	vm_load(&vm, file_path);

	// Pass the control to the VM
	set_input_mode();
	do {
		print_info(&vm);
		vm.driver.step(&vm);
		getchar();
		
	}while(!vm.driver.is_at_end);
	reset_input_mode();
	
	if (vm.regs[SP] == VM_MEM_SIZE)
		printf("Program ended with return code: 0\n");
	else if (vm.regs[SP] < VM_MEM_SIZE-1)
		printf("Program ended with unpoped stack\n");
	else
		printf("Program ended with return code: %u\n", vm.mem[vm.regs[SP]]);
	vm_destroy(&vm);
}

void compile_and_run(char name[]){
	char tmpfile[] = "/tmp/casXXXXXX";
	if (mkstemp(tmpfile) < 0) THROW_ERROR("Error creating temporary file");
	assemble(name, tmpfile);
	mapping = get_mapping();
	run_binary(tmpfile);
	remove(tmpfile);
}


#define PROGRAM_ENTRY_
int main(int argc, char *argv[]){
	ASSERT(argc == 2, "Invalid number of arguments");
	FILE *file = fopen(argv[1], "r");
	
	ASSERT(file != NULL, "Unable to open file: %s", argv[1]);

	int line = 0;	
	while(fgets(file_buff[line], MAX_LINE_SIZE, file)) line++;
	fclose(file);
	compile_and_run(argv[1]);
	return 0;
}
