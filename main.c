#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define SIZE_INPUT 1024

typedef struct Answer {
    struct Answer* next;
    int numRow;
    double averageSum;
} Answer;

void ClearAnswer(Answer *answer) {
    while (answer) {
        Answer* tmp = answer;
        answer = answer->next;
        free(tmp);
    }
}

void ResetAnswer(Answer *answer, double averageSum, int numRow) {
    // Clear Current List (from the second node onwards)
    if (answer->averageSum > averageSum)
    {
        Answer* current = answer->next;
        while (current != NULL) {
            Answer* temp = current;
            current = current->next;
            free(temp);
        }
        answer->next = NULL;
        answer->averageSum = averageSum;
        answer->numRow = numRow;
    }
    // Add New Node to List
    else if (abs(answer->averageSum - averageSum) < 1e-6)
    {
        Answer* current = answer;
        while (current->next) {
            current = current->next;
        }

        current->next = (Answer*)malloc(sizeof(Answer));
        current->next->numRow = numRow;
        current->next->averageSum = averageSum;
        current->next->next = NULL;
    }
}

Answer* FindMinLine(char *input) {
    Answer* answer = (Answer*) malloc(sizeof(Answer));
    answer->averageSum = -1.0;
    answer->numRow = 0;

    int num = 0, count = 0, numRow = 0, exitCounter = 0;
    double sum = 0.0;

    while (fgets(input, SIZE_INPUT, stdin))
    {
        if (input[0] == '\n')
        {
            exitCounter++;

            if (exitCounter == 2)
            {
                return answer;
            }
            continue;
        }
        else
        {
            exitCounter = 0;
        }

        count = 0;
        sum = 0.0;
        num = 0;

        for (int i = 0; input[i] != '\n'; i++) {
            if (!isdigit(input[i])) {
                sum += num;
                count++;
                num = 0;
            }
            else
            {
                num = num * 10 + (input[i] - '0');
            }
        }
        sum += num;
        count++;
        numRow++;

        if (count == 0) continue;
        else sum /= (double) count;

        // Init Answer
        if (answer->averageSum == -1)
        {
            answer->averageSum = sum;
            answer->numRow = numRow;
            answer->next = NULL;
        }
        else
        {
            ResetAnswer(answer, sum, numRow);
        }
    }

    return answer;
}

void GenerateOutput(Answer* answer) {
    if (answer == NULL) return;

    printf("%d", answer->numRow);
    answer = answer->next;

    while (answer != NULL) {
        printf(" %d", answer->numRow);
        answer = answer->next;
    }
    printf("\n");
}

int main() {
    char input[SIZE_INPUT];

    Answer* answer = FindMinLine(input);
    GenerateOutput(answer);
    ClearAnswer(answer);

    return EXIT_SUCCESS;
}
