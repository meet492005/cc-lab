#include <stdio.h>
#include <string.h>

int main() {
    int states, symbols;
    int startState, acceptCount;
    int acceptStates[10];
    int transition[10][10];
    char inputSymbols[10];
    char str[100];

    printf("Number of input symbols: ");
    scanf("%d", &symbols);

    printf("Input symbols: ");
    for (int i = 0; i < symbols; i++) {
        scanf(" %c", &inputSymbols[i]);
    }

    printf("Enter number of states: ");
    scanf("%d", &states);

    printf("Initial state: ");
    scanf("%d", &startState);

    printf("Number of accepting states: ");
    scanf("%d", &acceptCount);

    printf("Accepting states: ");
    for (int i = 0; i < acceptCount; i++) {
        scanf("%d", &acceptStates[i]);
    }

    printf("Enter Transition Table:\n");
    for (int i = 1; i <= states; i++) {
        for (int j = 0; j < symbols; j++) {
            printf("%d to %c -> ", i, inputSymbols[j]);
            scanf("%d", &transition[i][j]);
        }
    }

    printf("Input string: ");
    scanf("%s", str);

    int currentState = startState;

    for (int i = 0; i < strlen(str); i++) {
        int found = 0;
        for (int j = 0; j < symbols; j++) {
            if (str[i] == inputSymbols[j]) {
                currentState = transition[currentState][j];
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("Invalid String\n");
            return 0;
        }
    }

    for (int i = 0; i < acceptCount; i++) {
        if (currentState == acceptStates[i]) {
            printf("Valid String\n");
            return 0;
        }
    }

    printf("Invalid String\n");
    return 0;
}
