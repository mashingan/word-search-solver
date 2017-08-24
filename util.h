#ifndef min
#define min(x, y) ((x) <= (y) ? (x) : (y))
#endif

#define MAXBUFFER 1024

int countLines(FILE *f);

int countLinesFromFile(char* filename) {
    FILE *fp = fopen(filename, "r");
    int result = countLines(fp);
    fclose(fp);
    return result;
}

int countLines(FILE *f) {
    int ch;
    int result = 0;
    while ((ch = fgetc(f))  != EOF)
        if (ch == '\n') result++;
    fseek(f, 0, SEEK_SET);
    return result;
}

char* readString(FILE *f) {
    int ch;
    char buf[MAXBUFFER];
    memset(buf, 0, MAXBUFFER);
    int index = 0;
    char *result;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n') {
            break;
        }
        buf[index] = ch;
        index++;
    }
    result = malloc(index+1 * sizeof(char));
    strcpy(result, buf);
    result[index] = '\0';
    /*
    while ((ch = fgetc(f)) != EOF) {
        length++;
        if (ch == '\n') break;
    }
    char *result = malloc(length+1 * sizeof(char));
    result[length] = '\0';
    fseek(f, -length, SEEK_CUR);
    for (int i = 0; i < length; i++)
        result[i] = fgetc(f);
    fseek(f, 1, SEEK_CUR);
    */
    return result;
}

int readInt(FILE *f) {
    int result;
    int ch;
    char buffer[10];
    int i = 0;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n') {
            buffer[i] = '\0';
            result = atoi(buffer);
            break;
        }
        buffer[i] = ch;
        i++;
    }
    return result;
}

void safefree(void* p) {
    if (!p)
        free(p);
}
