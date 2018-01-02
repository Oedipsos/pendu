
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "hanging_def.h"
#include "dico.h"

#define clrscr() puts("\033[2J\033[;H")

typedef struct 
{
    char  letters[28];
    char  check[64];
    char  word[64];
    int   wordlen;
} Str;

typedef struct
{
    int W;
    int L;
    int G;
} Stats;  

char 	readc(void)			//Reads char(upper) and cleans buffer
{
    char c = 0;
    
    c = getchar();
    if(c == '\n')
        return c;
    
    c = toupper(c);
    
    while(getchar() != '\n');
    return c;
}

void    show_hang(int state)			//Prints hanged man's state (8 in total)
{
    printf("\n");

    for(int i = 0; i < HEIGHT; i++)
        printf("\t%s\n", hang[state][i]);
  
    printf("\n");
}

int	check_str(Str *t, char c)
{
    int k = 0;
    
    for(int j = 0; j <= t->wordlen; j++) {
            
        if(t->word[j] == c) {
            t->check[j] = t->word[j];
            k++;
        }
    }
    return k;
}                

void	show_help(void)
{
    printf("\n\n");
    printf("\t[====================================]\n"
           "\t|                                    |\n"
           "\t|    ?   : Displays this help panel  |\n"
           "\t|    #   : Displays statistics       |\n"
           "\t| CTRL C : Quit without saving       |\n"
           "\t|                                    |\n"   
           "\t[====================================]\n\n"
           "Press ENTER to continue... ");
    
    readc();
}

int 	set_letters(Str *t, char c)		//stocks new letter in string & displays the already-chosen letters with 's' option
{
    if(c <= ' ')
        return 0;
    
    for(int i = 0; i < 27; i++) {
        if(t->letters[i] == c)
            return 1;
        
        if(t->letters[i] == 0) {
            t->letters[i] = c;
            t->letters[i+1] = 0;
            return 0;
        }
    }
}

void	show_letters(Str t) 
{
    printf("Already used :  ");
    for(int i = 0; t.letters[i] != 0; i++)
        printf("\033[9m%c\033[0m  ", t.letters[i]);
    putchar('\n');
     
}

/*     [===============]
       |     STATS     |
       [===============] 
*/
   
void	show_stats(Stats s)
{
    float avgW;
    char  tmp[10];
    
    if(s.G != 0)
        avgW = (float)s.W / (float)s.G * 100.;
    else
        avgW = 0.;
    
    sprintf(tmp, "%2.2f%%", avgW);
    
    printf("\n\n");
    printf("\t[========================================]\n"
           "\t|                                        |\n"
           "\t|           Games played : %3d           |\n"
           "\t|   WIN                           LOSS   |\n"
           "\t|   %3d                            %3d   |\n"
           "\t|           Win ratio : %-10s       |\n"
           "\t|                                        |\n"
           "\t[========================================]\n\n"
           "Press ENTER to continue... "
           , s.G, s.W, s.L, tmp);
    
    readc();
}

int	save_stats(const char *fn, Stats s, char i)
{
    FILE *file;
    
    if(i == 1) 
        printf("Save file doesn't exist, do you want to create it (y/N)? ");
    else
        printf("Do you want to save your results for this session (y/N)? ");
    
    if(readc() != 'N') { 
        
        if((file = fopen(fn, "w")) == NULL) {
            fprintf(stderr, "Error while opening \"%s\" !\n", fn);
            return -6;
        } else {
            fprintf(file, "%d %d %d", s.W, s.L, s.G);
            fclose(file);
            printf("\nGame successfully saved !\n\n");
        }
    
    } else
        printf("\nGame not saved...\n\n");
}

int	load_stats(const char *fn, Stats *s)
{
    FILE *file;
    
    if((file = fopen(fn, "r")) != NULL) {
        fscanf(file, "%d %d %d", &s->W, &s->L, &s->G);
        fclose(file);
        return 0;
    } else
        return 1;
}





int	main(void)
{
    char 	buf[64], l = 0;
    Stats	stat = {0, 0, 0};
    l = load_stats(".stats", &stat);
    
    while(1) {
        
        //Game init
        Str t = { 0 };
        unsigned char tmp, k, f = 1, i = 7;
        
        if( (t.wordlen = load_dico(t.word, sizeof(t.word), "dico")) < 0)
            exit(-t.wordlen);

        memset(&t.check, '.', t.wordlen);
        
        while(i > 0) {
            
            int k = 0;
            clrscr();
            show_hang(i);

            if(f) {
                
                printf("\nWelcome to the Hangman game !\n\n"
                       "You have to find a random word by selecting letters that are part of that word.\n"
                       "You have 7 tries to resolve it !\t('?' to display help)\n\n");
                
                tmp = t.word[0];
                check_str(&t, tmp);					//Show 1st letter
                set_letters(&t, tmp);
                printf("\n\n");
                f--;
            
            } else {
                printf("\n%u guess(es) left !\t('?' to display help)\n\n\n", i);
                show_letters(t);
                printf("\n\n\n");    
            }
            puts(t.check);
            printf("\nEnter your letter : ");
            tmp = readc();
                        
            switch(tmp) {
                case '?' : 
                    clrscr();
                    show_help();
                    break;
                case '#' :
                    clrscr();
                    show_stats(stat);
                    break;
                default :
                    set_letters(&t, tmp);
                    if(!(k = check_str(&t, tmp)))
                        i--;
            }
            
            if(!strcmp(t.check, t.word))
                break;
        }
        
        clrscr();
        
        if(!i) {
            show_hang(0);
            printf("\nYou've lost the game :(\n"
                   "The right word was : ");
            puts(t.word);
            stat.L++;
        } else {
            show_hang(8);
            printf("\nCongratulations :D\n"
                   "You've found the word : ");
            puts(t.word);
            stat.W++;
        }
        
        stat.G++;
        printf("\n\nDo you want to play again (y/N)? ");
        if(readc() != 'Y') break;
    }
    save_stats(".stats", stat, l);
}
