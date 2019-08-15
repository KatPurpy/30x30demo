#include <stdlib.h>
#include "screen.c"
#include "audio_manager.c"

typedef struct Object
{
   int x,y,character,color;
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
   printf("CREATED OBJECT NUM %i\n",ID);
   Sleep(10);
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
   Screen_DrawBuffer();
}


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
	 CreateObject(y,x,219,
	   PurpyLogo[x][y] == 'B' ? 0 :
	   PurpyLogo[x][y] == 'R' ? 12 :
	   PurpyLogo[x][y] == 'P' ? 13 :
	   203
	   );

   }
   
//   for(int i = 0; i < 901; i++)
  //   {
//	ObjectsHeap[i] = 0;
  //   }
	// CreateObject(2,2,'O',12);
  // 

   
   while(1){
     Screen_DrawBox(0,0,30,30,203,219);
	for(int i = 0; i< 25*23-25*22;i++) if(i%2!=0)ObjectsHeap[i]->x++; else ObjectsHeap[i]->x--;
   DrawAllObjects();
   Sleep(50);
   }
   
   AudioLibrary_deinit();
}
