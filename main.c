#include <stdlib.h>
#include "screen.c"
#include "audio_manager.c"

///GENERIC IMPORTANT STUFF
typedef struct Object
{
   int x,y,character,color;
   char *data;
}Object;

Object *ObjectsHeap[901];

Object* CreateObject(int x,int y,int character,int color)
{
   int ID = 0;
   while(ObjectsHeap[ID] != 0)
	ID++;
   
   ObjectsHeap[ID] = malloc(sizeof(Object));
   ObjectsHeap[ID]->x = x;
   ObjectsHeap[ID]->y = y;
   ObjectsHeap[ID]->character = character;
   ObjectsHeap[ID]->color = color;
   return ObjectsHeap[ID];
}
void DrawChar(int x,int y,int character,int color)
{if(x > SCREEN_HEIGHT || y > SCREEN_WIDTH || x < 0 || y <0)return;
   Screen_drawParams sdp;
   sdp.x = x;
     sdp.y = y;
     sdp.Character = character;
   sdp.color_flags = color;
   Screen_PlotChar(&sdp);
}

void DrawAllObjects()
{
   int ID = -1;
   while(ObjectsHeap[ID] != 0)
     {
		 ID++;
	if(ObjectsHeap[ID] == 0) break;
	DrawChar(ObjectsHeap[ID]->x, ObjectsHeap[ID]->y ,ObjectsHeap[ID]->character ,ObjectsHeap[ID]->color);
	}
}

void FreeObjects(){
	for(int i = 0; i < 901; i++)
     {
	 if(ObjectsHeap[i] != 0)free(ObjectsHeap[i]);
     }
	 for(int i = 0; i < 901; i++)
     {
	ObjectsHeap[i] = 0;
     }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///BALLS STUFF
void CreateBall(int dx,int dy, Object* ball){
	ball->data = malloc(sizeof(char)*16);
	sprintf(ball->data,"%i,%i",dx,dy);
}
void UpdateBall(Object* ball,int bounds){
	int dx,dy;
	sscanf(ball->data,"%i,%i",&dx,&dy);
	if(bounds){
	if(ball->x+dx >= 30 || ball->x <= 0) dx = -dx;
	if(ball->y+dy >= 30 || ball->y <= 0) dy = -dy;
	}ball->x += dx;
	ball->y += dy;
	sprintf(ball->data,"%i,%i",dx,dy);
	
}
/////////////////
int main()
{
   Screen_Setup();
   AudioLibrary_init();

   char PurpyLogo[26][25]=
{
{" B                   B "},
{" BB                 BB "},
{" BPB               BPB "},
{" BPPB             BPPB "},
{" BPPPB           BPPPB "},
{" BPRPPB         BPPRPB "},
{" BPRRPPB       BPPRRPB "},
{" BPPPBBBBBBBBBBBBBPPPB "},
{" BPPBPPPPPPPPPPPPPBPPB "},
{" BPBPPPPPPPPPPPPPPPBPB "},
{" BBPPPBBPPPPPPPBBPPPBB "},
{" BPPPBPPBPPPPPBPPBPPPB "},
{" BPPBPPPPBPPPBPPPPBPPB "},
{"BBPPPPPPPPPPPPPPPPPPPBB"},
{"BPPPPPPPPPPPPPPPPPPPPPB"},
{"BPPPPPPPPPPPPPPPPPPPPPB"},
{"BPPPPPPPPPRRRPPPPPPPPPB"},
{"BBPPPPPPPPRRRPPPPPPPPBB"},
{" BPPBBPPPPPBPPPPPBBPPB "},
{" BPPPPBBPPPBPPPBBPPPPB "},
{"  BPPPPPBBPBPBBPPPPPB  "},
{"   BPPPPPPBBBPPPPPPB   "},
{"    BPPPPPPPPPPPPPB    "},
{"     BBBPPPPPPPBBB     "},
{"        BBBBBBB        "}
};
   
   
   
   for(int i = 0; i < 901; i++)
     {
	ObjectsHeap[i] = 0;
     }
   
   //payloads stuff
   
   for(int x = 0; x<25; x++)for(int y=0;y<23;y++){
	 CreateObject(y+3,x+2,219,
	   PurpyLogo[x][y] == 'B' ? 0 :
	   PurpyLogo[x][y] == 'R' ? 12 :
	   PurpyLogo[x][y] == 'P' ? 13 :
	   203
	   );

   }
    Screen_DrawBox(0,0,30,30,203,219);
	for(int i = 0; i< 25*23;i++)if(i%2==0)ObjectsHeap[i]->x -= 30; else ObjectsHeap[i]->x += 30;
   Sleep(1000);
   int I = 0;
   for(int I = 0; I < 30; I++){
     Screen_DrawBox(0,0,30,30,203,219);
	for(int i = 0; i< 25*23;i++) if(i%2==0)ObjectsHeap[i]->x++; else ObjectsHeap[i]->x--;
   
   DrawAllObjects();
   
    Screen_DrawBuffer();

   Sleep(50);
   }
   ///////////Show Kat Purpy presents
   Screen_DrawBox(0,0,30,30,203,219);
   DrawAllObjects();
   Screen_PlotText(30/2-18/2,30/2+4,"Kat Purpy presents",2);
   Screen_DrawBuffer();
   Sleep(5000);
   ///////////////////////////
   //////////Hide Kat Purpy
   for(int I = 0; I < 30; I++){
     Screen_DrawBox(0,0,30,30,203,219);
	for(int i = 0; i< 25*23;i++) if(i%2==0)ObjectsHeap[i]->y++; else ObjectsHeap[i]->y--;
   
   DrawAllObjects();
   
    Screen_DrawBuffer();

   Sleep(50);
   }
   for (int I = 0; I<15; I++){
	    Screen_PlotText(30/2-10/2,I,"900 pixels",2);
		Screen_DrawBuffer();

   Sleep(50);
   }
   Sleep(2000);
   ///FREE OBJECTS FOR NEXT SCENE
   FreeObjects();
   char name[]="900 pixels";
   Object * Balls[10];
   for(int i = 0; i<strlen(name);i++){
	   Screen_PlotText(10,14,"900 pixels",2);
	   Balls[i] = CreateObject(10+i,14,name[i],i+2);
	   CreateBall(rand()%4-2,rand()%4-2,Balls[i]);
	  
	   DrawAllObjects();
	   Screen_DrawBuffer();
	   Sleep(50);
   }
   for(int f = 0; f<150; f++){
	    for(int i = 0; i<strlen(name);i++)UpdateBall(Balls[i],f < 100);
		DrawAllObjects();
	   Screen_DrawBuffer();
	   Sleep(50);
   }
   ///Balls stuff
   
   
   
   
   
   AudioLibrary_deinit();
}
