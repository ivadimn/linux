#include "proc_status.h"

int main(int argc, char** argv) {

    if (argc != 2) {
		printf("Usage %s %s\n", argv[0], "<num_process>");
		exit(1);
	}
	int count_rec = open_status(argv[1]);
    
	return 0;
}