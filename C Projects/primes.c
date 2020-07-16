
#include <stdio.h>

int main()
{
    int n = 15;
    
    int i=2;
    int res[n];

    for(int i=0;i<n;i++){
        res[i] = 0;
    }

    int flag;
    res[0] = 1;
    while(i<=n){
        flag = 0;
        for(int j=2;j<i;j++){
            printf("j is %d   and i is %d \n", j, i);
            if(i%j == 0){
                printf(" %d not prime\n", i);
                flag = 1;
                break;
            }
        }

        if(flag == 0){
            printf("adding new prime to list %d \n", i);
            res[i-1] = i;
        }
        i++;
    }
    printf("primes are : \n");
    for(int i=0;i<n;i++){
        if(res[i] != 0)
            printf("%d , ", res[i]);
    }
    printf("\n");

    return 0;
}
