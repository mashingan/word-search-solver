#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "util.h"


char **allocGrid(int rows, int cols) {
    char **result;
    result = malloc(rows * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        result[i] = malloc(sizeof(char) * (cols+1));
        if (result[i] == NULL) {
            free(result);
            return NULL;
        }
        memset(result[i], '\0', cols + 1);
        result[i][cols] = '\0';
    }
    return result;
}

void purgegrid(char** grid, int rows) {
    if (grid == NULL) return;

    for (int i = 0; i < rows; i++) {
        free(grid[i]);
    }
    free(grid);
}

char *reverseWord(char *word) {
    if (word == NULL) return NULL;

    int length = strlen(word);
    char *result = malloc(sizeof(char) * (length + 1));
    for (int i = 0; i < length; i++) {
        result[length-i-1] = word[i];
    }
    result[length] = '\0';
    return result;
}

char **reverseGrid(char **grid, int rows, int columns) {
    if (grid == NULL)
        return NULL;

    char **result = allocGrid(rows, columns);
    if (result == NULL) return NULL;
    for (int i = 0; i < rows; i++) {
        int len = strlen(grid[i]);
        char *rev = reverseWord(grid[i]);
        //strcpy_s(result[i], len+1, rev);
        strcpy(result[i], rev);
        result[i][len] = '\0';
        free(rev);
    }
    return result;
}

int checkInline(char* line, char* word) {
    if (!line) return 0;

    int length = strlen(line);
    int lenword = strlen(word);
    if (length < lenword)
        return 0;

    int position = 0;
    int result = 0;
    while (position < length) {
        char* pos = strstr(line, word);

        if (pos == NULL) {
            break;
        }

        int lenpos = pos - line + 1;
        position += lenpos;
        result++;
        for (int i = 0; i < lenpos; i++)
            line++;
    }

    return result;
}

int checkLines(char** grid, char* word, int lines) {
    if (grid == NULL) return 0;
    int result = 0;
    for (int i = 0; i < lines; i++) {
        char *line = grid[i];
        char *rev = reverseWord(line);
        //printf("checkLines => line %s, rev %s\n", line, rev);
        result += (checkInline(line, word) + checkInline(rev, word));
        free(rev);
    }
    return result;
}

char** normalizeVertical(char** grid, int limit, int rows, int columns) {
    /* AKA transpose */
    if (columns < limit) return NULL;
    char **newgrid = allocGrid(columns, rows+1);
    if (newgrid == NULL) return NULL;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            newgrid[j][i] = grid[i][j];
        }
    }

    return newgrid;
}

void appendGrid(char***, char*, int);

char** normalizeDiagonal(char** grid, int limit, int rows, int columns) {
    if (grid == NULL || rows < limit || columns < limit)
        return NULL;

    char **result;
    int lenline = min(rows, columns);
    int length = rows + columns - 1;
    int index = 0;

    char buffer[101];
    result = malloc(length * sizeof(char*));
    if (result == NULL) return NULL;

    // ambil tengah 
    result[index] = malloc(lenline+1);
    for (int i = 0, j = 0; i < lenline; i++, j++) {
        buffer[i] = grid[i][j];
    }
    buffer[lenline] = '\0';
    strcpy(result[index], buffer);
    result[index][lenline] = '\0';
    index++;

    // row atas
    for (int i = 1; i < columns; i++) {
        memset(buffer, 0, lenline+1);
        int x, y;
        for (x=0, y=i; x < rows && y < columns; x++, y++)
        {
            buffer[x] = grid[x][y];
        }

        if (rows <= columns) {
            buffer[x] = '\0';
        } else {
            buffer[y] = '\0';
        }
        int idxlen = strlen(buffer);
        result[index] = malloc(idxlen+1);
        strcpy(result[index], buffer);

        result[index][idxlen] = '\0';
        index++;
    }

    // kolum bawah
    for (int i = 1; i < rows; i++) {
        memset(buffer, 0, lenline+1);
        int x, y;
        for (x=i, y=0; x < rows && y < columns; x++, y++)
        {
            buffer[y] = grid[x][y];
        }
        if (rows <= columns) {
            buffer[x] = '\0';
        } else {
            buffer[y] = '\0';
        }
        int idxlen = strlen(buffer);
        result[index] = malloc(idxlen+1);
        strcpy(result[index], buffer);

        result[index][idxlen] = '\0';
        index++;
    }
    assert(index == length);

    return result;
}

void printgrid(char** grid, int row) {
    if (!grid) return;
    for (int i = 0; i < row; i++)
        printf("    %s\n", grid[i]);
}

int solveWord(char **grid, char* word, int rows, int columns) {
    int result = 0;
    int len = strlen(word);
    int diaglen = rows + columns - 1;
    char **newgrid = normalizeVertical(grid, len, rows, columns);
    char **newdiag = normalizeDiagonal(grid, len, rows, columns);
    int diagcol = min(rows, columns);
    char **revgrid = reverseGrid(grid, rows, columns);
    char **revdiag = normalizeDiagonal(revgrid, len, rows, columns);
    result =
        checkLines(grid, word, rows) +
        (newgrid==NULL? 0: checkLines(newgrid, word, columns)) +
        (newdiag==NULL? 0: checkLines(newdiag, word, diaglen)) +
        (revdiag==NULL? 0: checkLines(revdiag, word, diaglen));

    purgegrid(newgrid, columns);
    purgegrid(newdiag, diaglen);
    purgegrid(revgrid, rows);
    purgegrid(revdiag, diaglen);
    return result;
}

void testcase(char **grid, char *word, int rows, int cols, int testnum) {
    printf("Case %d: %d\n", testnum, solveWord(grid, word, rows, cols));
}

void appendGrid(char*** grid, char *word, int pos) {
    int len = strlen(word);
    strcpy((*grid)[pos], word);
    (*grid)[pos][len] = '\0';
}

char *cpystring(char *word) {
    int len = strlen(word);
    char *newword = malloc(len + 1);
    if (newword == NULL) {
        perror("cpystring():");
        exit(EXIT_FAILURE);
    }
    strcpy(newword, word);
    newword[len] = '\0';
    return newword;
}

int main(int argc, char *argv[]) {
    char *filename;
    if (argc <= 1) {
        filename = cpystring("input.in.example");
    } else {
        filename = cpystring(argv[1]);
    }
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("cannot read %s\n", filename);
        perror("fopen()");
        return EXIT_FAILURE;
    }
    free(filename);

    int casetimes = readInt(fp);
    for (int i = 0; i < casetimes; i++) {
        int rows = readInt(fp);
        int cols = readInt(fp);
        char **grid = allocGrid(rows, cols+1);
        if (grid == NULL) {
            printf("allocation failure\n");
            perror("allocGrid()");
            return EXIT_FAILURE;
        }

        for (int row = 0; row < rows; row++) {
            char *line = readString(fp);
            appendGrid(&grid, line, row);
            free(line);
        }
        char *word = readString(fp);
        printf("Case %d: %d\n", i+1, solveWord(grid, word, rows, cols));
        free(word);
        purgegrid(grid, rows);
    }
    fclose(fp);

    return 0;
}
