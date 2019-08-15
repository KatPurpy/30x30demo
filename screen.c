#pragma once
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define HANDLE__ GetStdHandle(STD_OUTPUT_HANDLE)
#define SCREEN_SET_BUFFER_SIZE_COMMAND "cls && mode con: cols=30 lines=30"

#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 30

typedef struct
{
    int x, y, color_flags;
    char Character;
} Screen_drawParams;


Screen_drawParams Screen_Buffer_old[SCREEN_WIDTH][SCREEN_HEIGHT];
Screen_drawParams Screen_Buffer_new[SCREEN_WIDTH][SCREEN_HEIGHT];
int Screen_Buffer_affected_points[SCREEN_WIDTH][SCREEN_HEIGHT];

void Screen_Setup()
{
    system(SCREEN_SET_BUFFER_SIZE_COMMAND);
}

void Screen_Change(Screen_drawParams *SDP, int x, int y, int color_flags, char Character)
{
    SDP->x = x;
    SDP->y = y;
    SDP->color_flags = color_flags;
    SDP->Character = Character;
}

void Screen_SetColor(int color_flags)
{
    SetConsoleTextAttribute(HANDLE__, color_flags);
}
void Screen_SetCurPos(int x, int y)
{
    COORD *Cord;
    Cord = malloc(sizeof(COORD));
    Cord->X = x;
    Cord->Y = y;
    SetConsoleCursorPosition(HANDLE__, *Cord);
    free(Cord);
}
//Plots char into the buffer
void Screen_PlotChar(Screen_drawParams *Params)
{
   if(Params->x > SCREEN_HEIGHT || Params->y > SCREEN_WIDTH || Params->x < 0 || Params->y <0){
	   printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
	   return;
   }
    Screen_Buffer_new[Params->x][Params->y] = *Params;
}
void Screen_PlotCharDirectly(Screen_drawParams *Params){
if(Params->Character==-1)return;
Screen_SetColor(Params->color_flags);
    Screen_SetCurPos(Params->x, Params->y);
    putchar(Params->Character);
}
//////////////////////////////////////////////////////////////
static int Screen_CalculateCheckSum(Screen_drawParams param){
    //Stolen and upgraded from this article https://habr.com/ru/post/168195/
    
    int prime = 31;
    int result = 1;
    result = prime * result + param.x;
    result = prime * result + param.y;
    result = prime * result + param.Character;
    result = prime * result + param.color_flags;
    return result;
}
void Screen_DrawBuffer(){
    Screen_drawParams sdp;
    sdp.Character=219;
    sdp.color_flags=0;
    for(int x = 0; x < SCREEN_WIDTH; x++)
    for(int y = 0; y < SCREEN_HEIGHT-1;y++){
       sdp.x=x;
       sdp.y=y;
        

        if(Screen_CalculateCheckSum(Screen_Buffer_old[x][y]) != Screen_CalculateCheckSum(Screen_Buffer_new[x][y])){
        Screen_PlotCharDirectly(&sdp); 
        Screen_Buffer_affected_points[x][y] = 1;
        Screen_Buffer_old[x][y] = Screen_Buffer_new[x][y];
        Screen_PlotCharDirectly(&Screen_Buffer_old[x][y]);
      
        }else{
            Screen_Buffer_affected_points[x][y] = 0;
        }
        
       //  Screen_Buffer_new[x][y] = nil;
    Screen_Buffer_new[x][y].Character=-1;
    
    }
    
}

/////////////////////////////////////////////////////////////////////////////






void Screen_DrawBox(int x1, int y1, int x2, int y2, int color_flags, char Character)
{

    Screen_drawParams A;
    A.Character = Character;
    A.color_flags = color_flags;
    for (int y = y1; y < y2; y++)
    {
        for(int x=x1;x<x2;x++){
            A.x = x;
            A.y = y;
            Screen_PlotChar(&A);
        }
    }

}
void Screen_PlotText(int x,int y,const char* Text,int color_flags)
{
   Screen_drawParams sdp;
   sdp.y = y;
   sdp.color_flags = color_flags;
   int offset = 0;
   for(int i = 0; i < strlen(Text);i++)
     {
	sdp.x = x+i-offset;
	sdp.Character = Text[i];
	Screen_PlotChar(&sdp);
     if(Text[i]=='\n'){
	   sdp.y++;
	   offset = i+1;
	}
     }
   
}
/////////////////////////////////////////////////////////////////

void Screen_Clear()
{
    system("cls");
}
