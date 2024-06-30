#include "proc_status.h"
#include "str_dict.h"


static dict_t* status = NULL;

static void _parse_line(char* line) {
    char *pp = NULL;
    char key[KEY_LEN];
    pp = strchr(line, ':');
    int key_len = pp - line;
    strncpy(key, line, key_len);
    key[key_len] = '\0';
    pp++;
    while(*pp == ' ' || *pp == '\t')
        pp++;
    put(&status, (alpha*)key, (alpha*)pp);
}


int open_status(char* pid) {
    char file_name[PATH_LEN];
    char line[BUFF_LEN];
    char* pline;
    int count_lines = 0;
    size_t len = 0;

    if (status != NULL) {
        destroy_dict(status);
    }

    snprintf(file_name, PATH_LEN - 1, "/proc/%s/status", pid);

    FILE *fd = fopen(file_name, "r");
    
	if (fd == NULL) {
		return -1;
	}

	status = create_dict(256, 0.72, 2.0);

    do  {
        pline = fgets(line, BUFF_LEN, fd);
        len = strlen(line);
        line[len - 1] = '\0';
        _parse_line(line);
        count_lines++;
    } while (pline != NULL);
    
    if (ferror(fd)) {
        fclose(fd);
        destroy_dict(status);
        return -1;    
    }
    fclose(fd);   
    print_dict(status);
    return count_lines;
}

void close_status() {
    destroy_dict(status);
    status = NULL;
}