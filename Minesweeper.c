#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>

struct User
{
    char Name[50];
    int score;
}Users[20];

int nofusers=0;
int FlagToDelayGeneration=0;
void StartGame(char**ShownArray,int**HiddenArrray,int r,int c,int NumberOfMoves,int t);
int  OpenCell(char**ShownArray,int**HiddenArrray,int X,int Y,int r, int c);
void Reopen(char**ShownArray,int**HiddenArrray,int X,int Y,int r, int c);
void GenerateRandom2DArray(int**HiddenArray,int X,int Y,int r,int c);
void PrintSArray(char**ShownArray,int r,int s);
int CheckForFlags(char**ShownArray,int X,int Y,int r,int c);
void FlagCell(char**ShownArray,int X,int Y,int r,int c);
void Question(char**ShownArray,int X,int Y,int r, int c);
void Unmark(char**ShownArray,int X,int Y,int r, int c);
void Lose(char**ShownArray,int**HiddenArray,int r, int c);
void Win(char**ShownArray,int**HiddenArray,int r, int c,int NumberOfMoves,int t);
void save(char**ShownArray,int**HiddenArray,int r, int c,int NumberOfMoves,int Realtime);
void read_fusers();

int main()
{
    char Choice;
    int Rows,i,j,tt,Colmuns,NumberOfMoves;
    struct User temp;
    read_fusers(); // Reading LeaderBoards Informations
    while(1)
    {

        printf("Start New game(1)\n");
        printf("\nLoad Game(2)\n");
        printf("\nLeaderBoards(3)\n");
        printf("\nExit(4)\n");
        printf("\nEnter your choice : ");
        fflush(stdin); scanf("%c",&Choice);

        switch(Choice)
        {
            case '1' : //Starting New Game
            {
                /*Initializing Number of moves and time elapsed by zero*/
                    FlagToDelayGeneration=0;
                    NumberOfMoves=0;
                    tt=0;

                    printf("\nEnter Dimensions of the program : ");
                    scanf("%d %d",&Rows,&Colmuns);

                    /**Allocating Memories for both 2D Array**/
                    int **HiddenArray=(int**)malloc(Rows*sizeof(int*));
                    for (i=0; i<Rows; i++) HiddenArray[i] =(int*)malloc(Colmuns*sizeof(int));

                    char **ShownArray=(char**)malloc(Rows*sizeof(char*));
                    for (i=0; i<Rows; i++) ShownArray[i]=(char *)malloc(Colmuns*sizeof(char));
                    for(i=0;i<Rows;i++) //Initializing Shown array by X
                        for(j=0;j<Colmuns;j++) ShownArray[i][j]='X';
                    StartGame(ShownArray,HiddenArray,Rows,Colmuns,NumberOfMoves,tt);
            }
            break;

            case '2' : //Loading Previous Game
                {
                    char temp[40];
                    printf("\nEnter your User Name : ");
                    fflush(stdin); scanf("%[^\n]s",temp);
                    strcat(temp,".txt");
                    FILE*fm;
                    fm=fopen(temp,"r"); //Opening User file to read from it
                    if(fm==NULL)
                        printf("Save file not found\n");
                    else
                    {
                    fscanf(fm,"%d,%d,%d,%d\n",&Rows,&Colmuns,&tt,&NumberOfMoves);

                    /**Allocating Memories for both 2D Array**/

                    int **HiddenArray=(int**)malloc(Rows*sizeof(int*));
                    for (i=0; i<Rows; i++) HiddenArray[i] =(int*)malloc(Colmuns*sizeof(int));

                    char **ShownArray=(char**)malloc(Rows*sizeof(char*));
                    for (i=0; i<Rows; i++) ShownArray[i]=(char *)malloc(Colmuns*sizeof(char));

                    /**Reading The 2D Arrays**/
                    for(i=0;i<Rows;i++)
                    {
                        for(j=0;j<Colmuns;j++)
                        {
                            fscanf(fm,"%d,",&HiddenArray[i][j]);
                            fscanf(fm,"%c,",&ShownArray[i][j]);
                        }
                        fscanf(fm,"\n");
                    }

                    fclose(fm);
                    remove(temp); //Removing Save File
                    for(i=0;i<Rows;i++) //Checking if the user has opened a cell or not
                        for(j=0;j<Colmuns;j++)
                            if(ShownArray[i][j]!='X' && ShownArray[i][j]!='F' && ShownArray[i][j]!='?')
                                {FlagToDelayGeneration=1; break;}
                    StartGame(ShownArray,HiddenArray,Rows,Colmuns,NumberOfMoves,tt);
                    }
                }
            break;

            case '3' : //LeaderBoards
                {
                    for(i=0;i<nofusers;i++)
                    {
                    for(i=0;i<nofusers;i++)
                    {
                    if(Users[i].score<Users[i+1].score)
                    {

                        temp=Users[i];
                        Users[i]=Users[i+1];
                        Users[i+1]=temp;
                    }}}
                    printf("N\tName\t\tScore\n\n");
                    for(i=0;i<nofusers;i++) printf("%d\t%s\t\t%d\n",(i+1),Users[i].Name,Users[i].score);
                    printf("\n");

                }
            break;

            case '4' :
            exit(0);
            break;

            default :
            printf("\nInvalid Input\n\n");
            break;

        }
    }
    return 0;
}

                            /**Direct Functions**/

void StartGame(char**ShownArray,int**HiddenArray,int Rows,int Colmuns,int NumberOfMoves,int tt)
{
    int i,j,X,Y,z=0,Lost,NumberOfOpenedCells,Realtime,Temptime,Rmin,Rsec,NumberOfflags=0;
    char Choice;
    char temp[Rows][Colmuns];
    Temptime=(clock()/CLOCKS_PER_SEC);
    for(i=0;i<Rows;i++)
            for(j=0;j<Colmuns;j++) temp[i][j]=ShownArray[i][j];

    while(1)
    {
        system("cls");
        int fm=0;
        PrintSArray(ShownArray,Rows,Colmuns);
        /*Checking if a move was taken*/
        for(i=0;i<Rows;i++)
            for(j=0;j<Colmuns;j++) if(temp[i][j]!=ShownArray[i][j]) {fm=1; break;}
            if(fm==1) NumberOfMoves++;
            printf("\nNumber Of Moves = %d",NumberOfMoves);
        /*Checking Number of cells flagged*/
        NumberOfflags=0; /**Edited Line**/
        for(i=0;i<Rows;i++)
            for(j=0;j<Colmuns;j++) if(ShownArray[i][j]=='F') NumberOfflags++;
            printf("\nNumber of Cells Flagged = %d",NumberOfflags);
        /*Updating Time*/
        Realtime=tt+(clock()/CLOCKS_PER_SEC)-Temptime;
        Rmin=Realtime/60;
        Rsec=Realtime%60;
        printf("\nTime = %d:%d",Rmin,Rsec);
        for(i=0;i<Rows;i++)
            for(j=0;j<Colmuns;j++) temp[i][j]=ShownArray[i][j];
        printf("\n\nActions :\n\nopen(1)\n\nFlag(2)\n\nQuestion(3)\n\nUnmark(4)\n\nSave(5)\n\nQuit(6)\n\n");
        printf("Enter your action : ");
        fflush(stdin); scanf("%c",&Choice);

        switch(Choice)
        {

            case '1' :
                {
                    printf("Enter (X,Y) Co-ordinates : ");
                    fflush(stdin); scanf("%d %d",&X,&Y);
                    if(ShownArray[Y-1][X-1]!='X' && ShownArray[Y-1][X-1]!='?' && ShownArray[Y-1][X-1]!='F' &&CheckForFlags(ShownArray,X-1,Y-1,Rows,Colmuns)!=0)
                    {
                    Reopen(ShownArray,HiddenArray,X-1,Y-1,Rows,Colmuns);
                    }
                    else
                        {
                        OpenCell(ShownArray,HiddenArray,X-1,Y-1,Rows,Colmuns);
                        }

                }
            break;

            case '2' :
                {
                printf("Enter (X,Y) Co-ordinates : ");
                scanf("%d %d",&X,&Y);
                FlagCell(ShownArray,X-1,Y-1,Rows,Colmuns);
                }
            break;

            case '3' :
                {
                printf("Enter (X,Y) Co-ordinates : ");
                scanf("%d %d",&X,&Y);
                Question(ShownArray,X-1,Y-1,Rows,Colmuns);
                }
            break;

            case '4' :
                {
                printf("Enter (X,Y) Co-ordinates : ");
                scanf("%d %d",&X,&Y);
                Unmark(ShownArray,X-1,Y-1,Rows,Colmuns);
                }
            break;

            case '5' :
                {
                save(ShownArray,HiddenArray,Rows,Colmuns,NumberOfMoves,Realtime);

                }
                break;
            case '6' :
                {
                 printf("\nWarning All your Progress will be lost if you didn't save , are you sure you wanna quit?");
                 printf("\nPress 1 to confirm or any other key to go back : "); fflush(stdin); scanf("%c",&Choice);
                 if(Choice=='1') exit(0);
                }
                break;
            case 'S' :
                {
                printf("\n");
                for(i=(Rows-1);i>-1;i--) //printing
                {
                    printf("%d |\t",(i+1));
                    for(j=0;j<Colmuns;j++)
                    {
                        printf("%d\t",HiddenArray[i][j]); if(HiddenArray[i][j]==9) z++;
                    }
                    printf("\n\n");
                }
                printf("\t");
                for(j=0;j<(Colmuns*5);j++) printf("-");
                printf("\n");
                for(j=1;j<(Colmuns+1);j++)printf("\t%d",j);
                }
                fflush(stdin); getchar();
            break;

            default : printf("\nInvalid input\n");
        }
        Lost=0;
        for(i=0;i<Rows;i++)
        for(j=0;j<Colmuns;j++) if(ShownArray[i][j]=='*') {Lost=1; break;} //Checking IF Player lost
        if(Lost==1) break;
        else
        {
            NumberOfOpenedCells=0;
            for(i=0;i<Rows;i++)
            for(j=0;j<Colmuns;j++) if(ShownArray[i][j]!='X' && ShownArray[i][j]!='F' && ShownArray[i][j]!='?') NumberOfOpenedCells++;
        }
        if(NumberOfOpenedCells==((Rows*Colmuns)-(1+((Rows*Colmuns)/10)))) {Lost=-1; break;}
    }
    if(Lost==1) Lose(ShownArray,HiddenArray,Rows,Colmuns);
    else if(Lost==-1) Win(ShownArray,HiddenArray,Rows,Colmuns,NumberOfMoves,Realtime);

}

int OpenCell(char** ShownArray,int** HiddenArray,int X,int Y,int r,int c)
{
    int i,j;

    /**Taking the first opening cell to generate the code**/
    if(FlagToDelayGeneration==0)
    {


        GenerateRandom2DArray(HiddenArray,X,Y,r,c);
        for(i=0;i<r;i++)
            for(j=0;j<c;j++)
            {
                if(Y==i && X==j) ShownArray[i][j]=HiddenArray[Y][X]+'0';
            }

        FlagToDelayGeneration++;


        if(ShownArray[Y][X]=='0')
            {
                ShownArray[Y][X]=' ';
                if(Y!=(r-1) && X!=0)      OpenCell(ShownArray,HiddenArray,X-1,Y+1,r,c);
                if(X!=0)                  OpenCell(ShownArray,HiddenArray,X-1,Y,r,c);
                if(Y!=0 && X!=0)          OpenCell(ShownArray,HiddenArray,X-1,Y-1,r,c);

                if(Y!=(r-1))              OpenCell(ShownArray,HiddenArray,X,Y+1,r,c);
                if(Y!=0)                  OpenCell(ShownArray,HiddenArray,X,Y-1,r,c);

                if(Y!=(r-1) && X!=(c-1))  OpenCell(ShownArray,HiddenArray,X+1,Y+1,r,c);
                if(X!=(c-1))              OpenCell(ShownArray,HiddenArray,X+1,Y,r,c);
                if(Y!=0 && X!=(c-1))      OpenCell(ShownArray,HiddenArray,X+1,Y-1,r,c);
            }


    return 0;
    }
    else
    {
        if(HiddenArray[Y][X]==9)
        {

            ShownArray[Y][X]='*';


        }
        else if(ShownArray[Y][X]!=' ') ShownArray[Y][X]=HiddenArray[Y][X]+'0';



    }
        if(ShownArray[Y][X]=='0')
            {
                ShownArray[Y][X]=' ';
                if(Y!=(r-1) && X!=0)      OpenCell(ShownArray,HiddenArray,X-1,Y+1,r,c);
                if(X!=0)                  OpenCell(ShownArray,HiddenArray,X-1,Y,r,c);
                if(Y!=0 && X!=0)          OpenCell(ShownArray,HiddenArray,X-1,Y-1,r,c);

                if(Y!=(r-1))              OpenCell(ShownArray,HiddenArray,X,Y+1,r,c);
                if(Y!=0)                  OpenCell(ShownArray,HiddenArray,X,Y-1,r,c);

                if(Y!=(r-1) && X!=(c-1))  OpenCell(ShownArray,HiddenArray,X+1,Y+1,r,c);
                if(X!=(c-1))              OpenCell(ShownArray,HiddenArray,X+1,Y,r,c);
                if(Y!=0 && X!=(c-1))      OpenCell(ShownArray,HiddenArray,X+1,Y-1,r,c);
            }



    return 0;
    }

void FlagCell(char**ShownArray,int X,int Y,int r,int c)
{
    if(ShownArray[Y][X]=='X')
    {
        ShownArray[Y][X]='F';
    }
    else printf("\nThis Cell Can't be flagged\n");

}

void Question(char**ShownArray,int X,int Y,int r, int c)
{
    if(ShownArray[Y][X]=='X')
    {
        ShownArray[Y][X]='?';
    }
    else printf("\nThis Cell Can't be Questioned\n");
}

void Unmark(char**ShownArray,int X,int Y,int r, int c)
{
    if(ShownArray[Y][X]=='F' || ShownArray[Y][X]=='?')
    {
        ShownArray[Y][X]='X';
    }
    else printf("\nThis Cell is already unmarked\n");
}

                            /**Indirect Primary Functions**/

void PrintSArray(char**ShownArray,int r,int c)
{
    int i,j;

    for(i=(r-1);i>-1;i--) //printing
    {
        printf("%d |",(i+1));
        for(j=0;j<c;j++)
        {
            printf("\t%c",ShownArray[i][j]);
        }
        printf("\n\n");
    }

    printf("\t");

    for(j=0;j<(c*4);j++) printf("-");

    printf("\n");

    for(j=1;j<(c+1);j++)printf("\t%d",j);

    printf("\n");
}

void GenerateRandom2DArray(int**HiddenArray,int X,int Y,int r,int c)
{
    int sum=0,n;
    int i,RandomX,RandomY;

    /*I Represents Number Of Mines*/
    n=1+((r*c)/10);
    for(i=0;i<n;i++)
    {

        RandomX=rand()%c; //Random Column
        RandomY=rand()%r;  //Random Row

        /*(9) Represents Number Of Mines*/
        if(RandomX!=X && RandomY!=Y && HiddenArray[RandomY][RandomX]!=9) HiddenArray[RandomY][RandomX]=9;
        else i--;
    }



    for(Y=0;Y<r;Y++)
        for(X=0;X<c;X++)
            if(HiddenArray[Y][X]!=9)
            {

                if(Y!=0 && X!=(c-1))      if(HiddenArray[Y-1][X+1]==9) sum++;
                if(X!=(c-1))              if(HiddenArray[Y][X+1]==9)   sum++;
                if(Y!=(r-1) && X!=(c-1))  if(HiddenArray[Y+1][X+1]==9) sum++;

                if(Y!=(r-1))              if(HiddenArray[Y+1][X]==9)   sum++;
                if(Y!=0)                  if(HiddenArray[Y-1][X]==9)   sum++;

                if(Y!=0 && X!=0)          if(HiddenArray[Y-1][X-1]==9) sum++;
                if(X!=0)                  if(HiddenArray[Y][X-1]==9)   sum++;
                if(Y!=(r-1) && X!=0)      if(HiddenArray[Y+1][X-1]==9) sum++;

                HiddenArray[Y][X]=sum;
                sum=0;

            }

}

 int CheckForFlags(char**ShownArray,int X,int Y,int r,int c)
{
            int sum=0;
            if(Y!=0 && X!=(c-1))    if(ShownArray[Y-1][X+1]=='F') sum++;
            if(X!=(c-1))            if(ShownArray[Y][X+1]=='F')   sum++;
            if(Y!=(r-1) && X!=(c-1))if(ShownArray[Y+1][X+1]=='F') sum++;

            if(Y!=(r-1))            if(ShownArray[Y+1][X]=='F')   sum++;
            if(Y!=0)                if(ShownArray[Y-1][X]=='F')   sum++;

            if(Y!=0 && X!=0)        if(ShownArray[Y-1][X-1]=='F') sum++;
            if(X!=0)                if(ShownArray[Y][X-1]=='F')   sum++;
            if(Y!=(r-1) && X!=0)    if(ShownArray[Y+1][X-1]=='F') sum++;

            return sum;
}

void Reopen(char**ShownArray,int**HiddenArray,int X,int Y,int r, int c)
{
         if(HiddenArray[Y][X]==CheckForFlags(ShownArray,X,Y,r,c))
         {
            if(Y!=0 && X!=(c-1))    if(ShownArray[Y-1][X+1]!='F') OpenCell(ShownArray,HiddenArray,X+1,Y-1,r,c);
            if(X!=(c-1))            if(ShownArray[Y][X+1]!='F')   OpenCell(ShownArray,HiddenArray,X+1,Y,r,c);
            if(Y!=(r-1) && X!=(c-1))if(ShownArray[Y+1][X+1]!='F') OpenCell(ShownArray,HiddenArray,X+1,Y+1,r,c);

            if(Y!=(r-1))            if(ShownArray[Y+1][X]!='F')   OpenCell(ShownArray,HiddenArray,X,Y+1,r,c);
            if(Y!=0)                if(ShownArray[Y-1][X]!='F')   OpenCell(ShownArray,HiddenArray,X,Y-1,r,c);

            if(Y!=0 && X!=0)        if(ShownArray[Y-1][X-1]!='F') OpenCell(ShownArray,HiddenArray,X-1,Y-1,r,c);
            if(X!=0)                if(ShownArray[Y][X-1]!='F')   OpenCell(ShownArray,HiddenArray,X-1,Y,r,c);;
            if(Y!=(r-1) && X!=0)    if(ShownArray[Y+1][X-1]!='F') OpenCell(ShownArray,HiddenArray,X-1,Y+1,r,c);
         }

}

                            /**Win and Lose Functions**/

void Lose(char**ShownArray,int**HiddenArray,int r, int c)
{
    int i,j;
    printf("\n\n\t\t\t !!  YOU LOST :(  !!\n\n");
    for(i=0;i<r;i++)
        for(j=0;j<c;j++) if(ShownArray[i][j]=='*') ShownArray[i][j]='!';

    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
    {
        if(ShownArray[i][j]=='!') ShownArray[i][j]='M';
        else if(ShownArray[i][j]=='F' && HiddenArray[i][j]!=9) ShownArray[i][j]='_';
        else if(HiddenArray[i][j]==9) ShownArray[i][j]='*';
        else if(HiddenArray[i][j]==0)  ShownArray[i][j]=' ';
        else ShownArray[i][j]=HiddenArray[i][j]+'0';
    }
    PrintSArray(ShownArray,r,c);
}

void Win(char**ShownArray,int**HiddenArray,int r, int c,int NumberOfMoves,int t)
{
    int i,j,score,flag=0;
    char temp[20];
    printf("\n\n\t\t\t !!  YOU WON :D  !!\n\n");
    for(i=0;i<r;i++)
        for(j=0;j<c;j++)
    {
        if(HiddenArray[i][j]==9) ShownArray[i][j]='F';
        else if(HiddenArray[i][j]==0)  ShownArray[i][j]=' ';
        else ShownArray[i][j]=HiddenArray[i][j]+'0';
    }
    PrintSArray(ShownArray,r,c);
    if(t==0) t=1;
    score=(pow((r*c),4)/(t*NumberOfMoves));
    printf("Your Score = %d",score);
    printf("\nEnter Your User Name : ");
    fflush(stdin); scanf("%[^\n]s",temp);
    for(i=0;i<nofusers;i++)
    {
        if(strcmp(temp,Users[i].Name)==0) {flag=1; break;}
    }
    if(flag==1)
    {
        if(score>Users[i].score) Users[i].score=score;
    }
    else
    {
        strcpy(Users[nofusers].Name,temp);
        Users[nofusers].score=score;
        nofusers++;

    }
    FILE*fptr;
    fptr=fopen("LeaderBoards.txt","w");
    for(i=0;i<nofusers;i++)
    {
        fprintf(fptr,"%s,%d\n",Users[i].Name,Users[i].score);
    }
    fclose(fptr);

}

                          /**Reading LeaderBoards Function**/
void read_fusers()
{
    FILE*fptr;
    fptr=fopen("LeaderBoards.txt","r");
    if(fptr!=NULL)
    {
        while(getc(fptr)!=EOF)
        {
            long offset = ftell(fptr)-sizeof(char);
            fseek(fptr, offset, SEEK_SET);
            fscanf(fptr,"%[^,],%d\n",Users[nofusers].Name,&Users[nofusers].score);
            nofusers++;
        }
    }
    fclose(fptr);
}

                          /**Saving Function**/

void save(char**ShownArray,int**HiddenArray,int r, int c,int NumberOfMoves,int Realtime)
{
    char temp[40];
    int i,j;
    printf("\nEnter your User Name : ");
    fflush(stdin); scanf("%[^\n]s",temp);
    FILE*fm;
    strcat(temp,".txt");
    fm=fopen(temp,"w");
    fprintf(fm,"%d,%d,%d,%d\n",r,c,Realtime,NumberOfMoves);
    for(i=0;i<r;i++)
    {
        for(j=0;j<c;j++)
        {
            fprintf(fm,"%d,",HiddenArray[i][j]);
            fprintf(fm,"%c,",ShownArray[i][j]);
        }
        fprintf(fm,"\n");
    }
    fclose(fm);
    printf("\nData Saved\n\n");

}
