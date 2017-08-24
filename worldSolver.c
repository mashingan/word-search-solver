/*
 * compile with additional option -I. to include utility functions
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "util.h"

#define min(x, y) ((x) <= (y) ? (x) : (y))

/*
typedef struct PosGrid_ {
    int x, y, line, row;
} PosGrid;
*/

char **allocGrid(int rows, int cols) {
    char **result;
    result = malloc(rows * sizeof(char*));
    for (int i = 0; i < rows; i++) {
        result[i] = malloc(sizeof(char) * (cols+1));
        memset(result[i], 0, cols + 1);
        result[i][cols] = '\0';
    }
    return result;
}

char *reverseWord(char *word) {
    int length = strlen(word);
    char *result = malloc(sizeof(char) * (length + 1));
    result[length] = '\0';
    for (int i = 0; i < length; i++) {
        result[length-i-1] = word[i];
    }
    return result;
}

char **reverseGrid(char **grid, int rows, int columns) {
    if (grid == NULL)
        return NULL;

    char **result = allocGrid(rows, columns);
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
    int length = strlen(line);
    int lenword = strlen(word);
    /*
    printf("length: %d, lenword: %d\n", length, lenword);
    printf("line: %s, word: %s\n", line, word);
    */
    if (length < lenword)
        return 0;

    int position = 0;
    int result = 0;
    //printf("to check incline\n");
    while (position < length) {
        char* pos = strstr(line, word);
        //printf("pos: %s\n", pos);
        if (pos == NULL) {
            break;
        }
        //int lenpos = pos - line + lenword;
        int lenpos = pos - line + 1;
        position += lenpos;
        result++;
        for (int i = 0; i < lenpos; i++)
            line++;
    }
    //printf("%s of %s has %d\n", word, line, result);
    return result;
}

int checkLines(char** grid, char* word, int lines) {
    int result = 0;
    for (int i = 0; i < lines; i++) {
        char *line = grid[i];
        char *rev = reverseWord(line);
        //printf("checkInline for %s\n", line);
        result += (checkInline(line, word) + checkInline(rev, word));
        //printf("freeing rev\n");
        free(rev); // cycle alloc and free, inefficient, but still...
    }
    //printf("returning checkLines\n");
    return result;
}

char** normalizeVertical(char** grid, int rows, int columns) {
    /* AKA transpose */
    char **newgrid;
    newgrid = malloc(columns * sizeof(char*));
    for (int i = 0; i < columns; i++) {
        newgrid[i] = malloc(sizeof(char) * (rows+1));
        newgrid[i][rows] = '\0';
    }
    //printf("allocating memory is fine\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            newgrid[j][i] = grid[i][j];
        }
    }
    /*
    for (int i = 0; i < rows; i++)
        printf("%s\n", grid[i]);
    printf("copying value is fine\n");
    */
    return newgrid;
}

int countDiagonal(int rows, int columns) {
    int result = 0;
    if (rows <= columns)
        result = rows;
    else
        result = columns;
    return result;
}

char* tookDiagonal(char** grid, int rows, int columns) {
    int line = countDiagonal(rows, columns);
    char *result = malloc(line + 1);
    result[line] = '\0';
    for (int i = 0, j = 0; i < rows && j < columns && i < line;
            i++, j++)
        result[i] = grid[i][j];
    return result;
}

char** normalizeDiagonal(char** grid, int limit, int rows, int columns) {
    if (rows < limit || columns < limit)
        return NULL;

    char **result;
    int lenline = countDiagonal(rows, columns);
    int length = rows + columns + 1;
    int index = 0;
    //printf("grid length: %d\n", length);
    result = allocGrid(length, lenline+1);

    // ambil tengah 
    //printf("diagonal\n");
    for (int i = 0, j = 0; i < lenline; i++, j++) {
        //putchar(grid[i][j]);
        result[index][i] = grid[i][j];
    }
    //printf("\n");
    //printf("index: %d\n", index);
    index++;

    for (int i = 1; i < columns; i++) {
        int x, y;
        for (x=0, y=i; x < rows && y < columns;
                x++, y++)
        {
            //putchar(grid[x][y]);
            result[index][x] = grid[x][y];
        }
        result[index][x] = '\0';
        //printf("\n");
        //printf("index: %d, x: %d, y: %d\n", index, x, y);
        index++;
    }
    //printf("safely normalize columns\n");
    for (int i = 1; i < rows; i++) {
        int x, y;
        for (x=i, y=0; x < rows && y < columns;
                x++, y++)
        {
            //putchar(grid[x][y]);
            result[index][y] = grid[x][y];
        }
        result[index][x] = '\0';
        //printf("\n");
        //printf("index: %d, x: %d, y: %d\n", index, x, y);
        index++;
    }
    //printf("safely normalize rows\n");

    // ambil atas
    return result;
}

void printgrid(char** grid, int row) {
    for (int i = 0; i < row; i++)
        printf("    %s\n", grid[i]);
}

int calcWord(char **grid, char* word, int rows, int columns) {
    int result = 0;
    int diaglen = rows + columns + 1;
    char **newgrid = normalizeVertical(grid, rows, columns);
    char **newdiag = normalizeDiagonal(grid, strlen(word), rows, columns);
    int diagcol = min(rows, columns);
    char **revgrid = reverseGrid(grid, rows, columns);
    char **revdiag = normalizeDiagonal(revgrid, strlen(word), rows, columns);
    result =
        checkLines(grid, word, rows) +
        checkLines(newgrid, word, columns) +
        (newdiag==NULL? 0: checkLines(newdiag, word, rows + columns + 1)) +
        (revdiag==NULL? 0: checkLines(revdiag, word, diaglen));

    free(newgrid);
    free(revgrid);
    safefree(newdiag);
    safefree(revdiag);
    return result;
}

void testcase(char *grid[], char *sub, int lines) {
    printf("%s in checkLines grid is %d\n", sub,
            checkLines(grid, sub, lines));
    int col = strlen(grid[0]);
    char **newgrid = normalizeVertical(grid, lines, col);
    printf("%s in checkLines newgrid is %d\n", sub,
            checkLines(newgrid, sub, col));
    free(newgrid);
}

void testdiagonal(char **grid, int rows, int cols) {
    char *buf = tookDiagonal(grid, rows, cols);
    printf("diagonal is %s\n", buf);
    free(buf);
}

int main() {
    char* hello = "hellohellohello";
    char* subhe = "he";
    //printf("to check\n");
    printf("substring %s of %s is %d\n", subhe, hello,
            checkInline(hello, subhe));
    char *rev = reverseWord("hello world");
    printf("the reverse word: %s\n", rev);
    free(rev);
    char *grid[] = {
        "hello world 1",
        "hello world 2",
        "hello world 3"
    };
    /*
    char **grid;
    grid = malloc(3 * sizeof(char*));
    for (int i = 0; i < 3; i++) {
        grid[i] = malloc(12);
        strcpy(grid[i], "hello world");
    }
    */
    int columns = strlen(grid[0]);
    char **newgrid = normalizeVertical(grid, 3, columns);
    /*
    for (int i = 0; i < columns; i++) {
        printf("%d: %s\n", i, newgrid[i]);
    }
    printf("%s in checkLines grid is %d\n", subhe,
            checkLines(grid, subhe, 3));
    printf("%s in checkLines grid is %d\n", subhe,
            checkLines(newgrid, subhe, columns));
            */

    //free(grid);
    free(newgrid);

    char *example[] = {
        "catt",
        "aata",
        "tatc"
    };
    char *cat = "cat";
    /*
    printf("%s in checkLines grid is %d\n", cat,
            checkLines(example, cat, 3));
            */
    int catcol = strlen(example[0]);
    char **newcat = normalizeVertical(example, 3, catcol);
    /*
    printf("%s in checkLines newcat is %d\n", cat,
            checkLines(newcat, cat, catcol));
            */
    free(newcat);

    char *griddog[] = { "gogog", "ooooo", "godog", "ooooo", "gogog" };
    //testcase(griddog, "dog", 5);
    printgrid(grid, 3);
    printf("grid %s: %d\n", subhe, calcWord(grid, subhe, 3, 13));
    printgrid(example, 3);
    printf("example %s: %d\n", cat, calcWord(example, cat, 3, catcol));
    char *dog = "dog";
    printgrid(griddog, 5);
    printf("griddog %s: %d\n", dog, calcWord(griddog, dog, 5, 5));

    char *banana[] = { "bananana", "kalibrrr" };
    char *nana = "nana";
    printgrid(banana, 2);
    printf("banana %s: %d\n", nana, calcWord(banana, nana, 2, 8));

    /*
    char **catvert = normalizeVertical(example, 3, catcol);
    char **catdiag = normalizeDiagonal(example, strlen(cat), 3, catcol);
    char **catreve = reverseGrid(catdiag, 3+catcol+1, min(3, catcol));
    printf("horizontal checkLines: %d\n", checkLines(example, cat, 3));
    printgrid(example, 3);
    printf("vertical   checkLines: %d\n", checkLines(catvert, cat, 4));
    printgrid(catvert, 4);
    printf("diagonal   checkLines: %d\n", checkLines(catdiag, cat, 3+catcol+1));
    printgrid(catdiag, 3+catcol+1);
    printf("rev diagnl checkLines: %d\n", checkLines(catreve, cat, 3+catcol+1));
    printgrid(catreve, 3+catcol+1);
    free(catvert);
    free(catdiag);
    free(catreve);
    */

    /*
    char **dogdiag = normalizeDiagonal(griddog, strlen(dog), 5, 5);
    printgrid(dogdiag, 5 + 5 + 1);
    free(dogdiag);
    */
    /*
    printf("diagonal grid: %d\n", countDiagonal(3, columns));
    printf("diagonal example: %d\n", countDiagonal(3, catcol));
    printf("diagonal griddog: %d\n", countDiagonal(5, 5));
    printf("diagonal banana: %d\n", countDiagonal(2, 8));

    char *diag = tookDiagonal(grid, 3, columns);
    printf("diagonal: %s\n", diag);
    free(diag);
    testdiagonal(example, 3, catcol);
    testdiagonal(griddog, 5, 5);
    testdiagonal(banana, 2, 8);
    char **newdiagonal = normalizeDiagonal(grid, strlen(subhe), 3, columns);
    printgrid(newdiagonal, 3 + columns);
    printf("reverse grid\n");
    char **revdiagonal = reverseGrid(newdiagonal, 3 + columns, 3);
    printgrid(revdiagonal, 3 + columns);
    if (newdiagonal != NULL)
        free(newdiagonal);
    if (revdiagonal != NULL)
        free(revdiagonal);
        */
    return 0;
}
