#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
int m;
char name[15];
scanf("%s", &name);
printf("Hello, %s", name);
printf("Hellolease, enter the size of array:");
scanf("%d", &m);


for (int i = 0; i < 1000; ++i){
    int *A = malloc(m*sizeof(int));
    if (NULL == A){
    printf("OS cannot give you free memory. Exitting...");
    exit(1);
    }
    for (int k = 0; k < m; ++k)
        A[k]=k;
    free(A);
}
printf("Successfully created!\n");
system("pause");
return 0;
}