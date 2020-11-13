#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <limits.h>

typedef struct {
    double rank;
    char* str;
}Phrase;

//struct comparator
int compare(const void *p1, const void *p2)
{
    const Phrase *elem1 = (const Phrase *)p1;
    const Phrase *elem2 = (const Phrase *)p2;
    
    if (elem1->rank < elem2->rank)
        return 1;
    else if (elem1->rank > elem2->rank)
        return -1;
    else
        return 0;
}

int main(){
    int num_of_phrases = 10;
    Phrase* phrases = (Phrase*) malloc(sizeof(Phrase)*num_of_phrases);
    size_t buf_size = 32;
    size_t num_of_chars;
    char* buffer = (char*)malloc(buf_size*sizeof(char));
    int i = 0;
    int counter = 0;
    printf("Frequent searches:\n");
    
    while((num_of_chars = getline(&buffer,&buf_size,stdin))!=0 && buffer[0]!='\n'){
        //realloc if needed
        if (i >= num_of_phrases){
            num_of_phrases *= 1.5;
            phrases = (Phrase*)realloc(phrases,num_of_phrases*sizeof(Phrase));
        }
        
        phrases[i].str = (char*)malloc(buf_size*sizeof(char));
        
        //realloc if needed
        if (num_of_chars >= buf_size) {
            buf_size *= 1.5;
            buffer = (char*)realloc(buffer,buf_size*sizeof(char));
            phrases[i].str = (char*)realloc(phrases[i].str,buf_size*sizeof(char));
        }
        
        
        if(sscanf(buffer," %lf : %[^\n]",&phrases[i].rank,phrases[i].str)!=2){
            for(int j = 0; j <= i; j++){
                free(phrases[j].str);
            }
            free(phrases);
            free(buffer);
            printf("Invalid input.\n");
            return 0;
        }
        i++;
    }
    
    //if no phrases
    if (!i) {
        free(phrases);
        free(buffer);
        printf("Invalid input.\n");
        return 0;
    }
    
    qsort(phrases,i,sizeof(Phrase),compare);
    printf("Searches:\n");
    while(getline(&buffer,&buf_size,stdin)!=EOF){
        sscanf(buffer,"%[^\n]",buffer);
        for (int j = 0; j < i; j++){
            if (strcasestr(phrases[j].str,buffer)!=NULL) {
                counter++;
            }
        }
        printf("Found: %d\n",counter);
        for (int j = 0; j < i; j++){
            if (strcasestr(phrases[j].str,buffer)!=NULL) {
                printf("> %s\n",phrases[j].str);
            }
        }
        
        counter = 0;
    }
    
    for(int j = 0; j < i; j++){
        free(phrases[j].str);
    }
    free(phrases);
    free(buffer);
    return 0;
}
