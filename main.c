#include <stdlib.h>
#include "screen.c"
#include "audio_manager.c"
#include <math.h>
#include <time.h>
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


/////////DRAW LINES ALGORITHM/////////////
void swap(int *a,int*b){
	int*temp;
	temp = a;
	a = b;
	b = temp;
}
int plot (int x,int y){
	DrawChar(x,y,219,rand()%15+1);
	return FALSE;
}
int sign(int x)
{
if(x>0)
 return 1;
else if(x<0)
 return -1;
else
 return 0;
}


void plotLine(int x1,int y1, int x2, int y2)
{
int x,y,dx,dy,swap,temp,s1,s2,p,i;

x=x1;
y=y1;
dx=abs(x2-x1);
dy=abs(y2-y1);
s1=sign(x2-x1);
s2=sign(y2-y1);
swap=0;
plot(x1,y1);
if(dy>dx)
 {
 temp=dx;
 dx=dy;
 dy=temp;
 swap=1;
 }
p=2*dy-dx;
for(i=0;i<dx;i++)
 {
 plot(x,y);
 while(p>=0)
  {
  p=p-2*dx;
  if(swap)
   x+=s1;
  else
   y+=s2;
  }
 p=p+2*dy;
 if(swap)
  y+=s2;
 else
  x+=s1;
 }
plot(x,y);
}	
///////////////////////////////////////////

void rotate(int point_x, int point_y, int center_x,int center_y, float angle, int *out_x,int *out_y){
            angle = (angle ) * (M_PI/180); // Convert to radians
            int rotatedX = cos(angle) * (float)(point_x - center_x) - sin(angle) * (float)(point_y-center_y) + center_x;
            int rotatedY = sin(angle) * (float)(point_x - center_x) + cos(angle) * (float)(point_y - center_y) + center_y;
			*out_x = rotatedX;
			*out_y = rotatedY;
}
void DrawPsychoDelic1(int f){
	plotLine(0,sin(f*0.1f)*15+14,29,cos(f*0.1f)*15+14);
	DrawChar(0,sin(f*0.1f)*15+14,'3',15);
	DrawChar(29,cos(f*0.1f)*15+14,'0',15);
}

void ShowFact(const char *Fact){
	int first_line_length;
	
	for(int i = 0; i < strlen(Fact);i++){
		if(Fact[i] == '\n'){
			first_line_length = i;
			break;
		}
	}
	Screen_DrawBox(0,0,30,30,0,0);
	Screen_PlotText(30/2-strlen("INTERESTING FACT")/2,7,"INTERESTING FACT",rand()%15+1);
	Screen_PlotText(30/2-first_line_length/2,15,Fact,rand()%15+1);
	Screen_DrawBuffer();
	Sleep(5000);
}
int DoesStringBeginWith(const char* string, const char *beginning)
{
   int truth = 0;
   int count = strlen(beginning);
   for(int i =0; i<count; i++)
     {
	if(string[i] == beginning[i])
	  {
	     truth++;
	  }
	
     }
   return truth == count;
}

char *Bye = "Thanks for watching my demo!";

int Goto = 0;
int Sound = 1;
int ShowFactAndQuit = 0;
int Fact2Show = 0;

int main(int argc,char**argv,char **envp)
{srand(time(NULL));
	for(int i = 0; i < 901; i++)ObjectsHeap[i] = 0;
     
	printf( "\nCommand-line arguments:\n" );
    for(int count = 0; count < argc; count++ ){
		printf( "  argv[%d]   %s\n", count, argv[count] );
		
        if(strcmp(argv[count],"-30")==0 || strcmp(argv[count],"-thirty")==0)while(1){printf("30!");}
		if(strcmp(argv[count],"-mute") ==0) Sound = 0;
		if(DoesStringBeginWith(argv[count],"--")){
			
			printf("GOT AN ARGUMENT %s with parameter %s!\n",argv[count],argv[count+1]);
			if(DoesStringBeginWith(argv[count],"--pallete-shift")){
				PALLETE_SHIFT = atoi(argv[count+1]);
			}
			if(DoesStringBeginWith(argv[count],"--thanks-message")){
				Bye = argv[count+1];
			}
			if(DoesStringBeginWith(argv[count],"--jump-to")){
				Goto = atoi(argv[count+1]);
			}
			if(DoesStringBeginWith(argv[count],"--show-fact")){
				ShowFactAndQuit = 1;
				Fact2Show = rand()%5;
			}
		}
		
	}
   

	
   Screen_Setup();
  if(Sound){
   AudioLibrary_init();
   
   AudioLibrary_LoadSound(0,"ost.sunvox");
   AudioLibrary_PlaySound(0,0,128);
   }SetConsoleTitle("30x30 by Kat Purpy");
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
char *InterestingFacts[5]={
	"30 (thirty) is the natural\nnumber following\n29 and preceding 31.",
	"The number of days in \nthe months April, June,\nSeptember and November",
	"Thirty can be prono\nunced as Thir-tea",
	"Adding up some subsets \nof its divisors(e.g., 5\n, 10 and 15) gives 30, \nhence 30 is a semiperfe\nct number",
	"Thirty is the sum of the\nfirst four squares,\nwhich makes it a\nsquare pyramidal number"
};
   Screen_DrawBox(0,0,30,30,0,0);
   if(ShowFactAndQuit){ShowFact(InterestingFacts[Fact2Show]);exit(0);};
   switch(Goto){
	   case 0:break;
	   case 1:goto BALLS;break;
	   case 2:goto LINES1;break;
	   case 3:goto TRIANGLE;break;
	   case 4:goto LINES2;break;
	   case 5:goto THANKS;break;
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
   ///FREE OBJECTS FOR NEXT SCENE
   //BALLS STUFF
   
   BALLS:;
   FreeObjects();
   for (int I = 0; I<15; I++){
	    Screen_PlotText(30/2-3,I,"30x30",2);
		Screen_DrawBuffer();

   Sleep(50);
   }
   Sleep(2000);
   
   
   
    char name[]="30x30";
   Object * Balls[10];
   for(int i = 0; i<strlen(name);i++){
	   Screen_PlotText(30/2-3,14,"30x30",2);
	   Balls[i] = CreateObject(30/2-3+i,14,name[i],i+2);
	   CreateBall(rand()%4-2,rand()%4-2,Balls[i]);
	  
	   DrawAllObjects();
	   Screen_DrawBuffer();
	   Sleep(50);
   }
   for(int f = 0; f<111; f++){
	    for(int i = 0; i<strlen(name)-1;i++)UpdateBall(Balls[i],f < 100);
		DrawAllObjects();
	   Screen_DrawBuffer();
	   Sleep(50);
   }
  ////////////////////////////
ShowFact(InterestingFacts[0]);
  
  LINES1:;
  FreeObjects();
  //////////SOME PSYCHODELIC LINES////////
	
   Object* a = CreateObject(15,16,'0',15);
   Object* b = CreateObject(15,15,'3',15);

   for(int f = 0; f < 75; f++){
	   rotate(a->x,a->y,15,15,22,&a->x,&a->y);
	   
	   DrawPsychoDelic1(cos(f*0.1f)*30+10);
	   DrawPsychoDelic1(cos(f*0.1f)*30+20);
	   DrawPsychoDelic1(cos(f*0.1f)*30);
	   
	   DrawAllObjects();
	 
	   Screen_DrawBuffer();
	   Sleep(50);
   }


  ShowFact(InterestingFacts[1]);
   TRIANGLE:;
  FreeObjects();   
LINES2:;  
Object* triangle[3] = {CreateObject(0,30,'0',15), CreateObject(15,5,'1',15),CreateObject(30,30,'2',15)};
   //if need to go to LINES2 then first init triangle then go to the lines2;
   if(Goto == 4) goto _LINES2;
   for(int f = 0; f < 100; f++){
		
		for(int i = 0; i<3; i++){rotate(triangle[i]->x,triangle[i]->y,15,15,45,&triangle[i]->x,&triangle[i]->y);
		
	   plotLine(triangle[i]->x,triangle[i]->y,triangle[(i+1)%2]->x,triangle[(i+1)%2]->y);
	   }
	   plotLine(triangle[0]->x,triangle[0]->y,triangle[2]->x,triangle[2]->y);
	   DrawAllObjects();
	
	   Screen_DrawBuffer();
	   Sleep(50);
   }
    ShowFact(InterestingFacts[2]);
   _LINES2:;
   for(int f = 200; f < 300; f++){
	   //rotate(a->x,a->y,15,15,22,&a->x,&a->y);
	   for(int i = 0; i<3; i++){rotate(triangle[i]->x,triangle[i]->y,15,15,45,&triangle[i]->x,&triangle[i]->y);
		
	   plotLine(triangle[i]->x,triangle[i]->y,triangle[(i+1)%2]->x,triangle[(i+1)%2]->y);
	   }
	   plotLine(triangle[0]->x,triangle[0]->y,triangle[2]->x,triangle[2]->y);
	   DrawPsychoDelic1(cos(f*0.1f)*30+10);
	   DrawPsychoDelic1(cos(f*0.1f)*30+20);
	   DrawPsychoDelic1(cos(f*0.1f)*30);
	   
	   DrawPsychoDelic1((sin(f*0.1f)*30+10)/2);
	   DrawPsychoDelic1((sin(f*0.1f)*30+20)/4);
	   DrawPsychoDelic1((cos(f*0.1f)*30)/8);
	   
	   DrawPsychoDelic1((cos(f*0.1f)*30+5));
	   DrawPsychoDelic1((cos(f*0.1f)*30+10));
	   DrawPsychoDelic1((sin(f*0.1f)*30));
	   
	   DrawAllObjects();
	 printf("30!30!30!30!30!30!30!");
	   Screen_DrawBuffer();
	   Sleep(50);
   }
   ///////STOP PSYCHODELIC
   
   Screen_Clear();
   ShowFact(InterestingFacts[3]);
   /////////////BALLS///////////////
   FINAL:;
   FreeObjects();
   Object * Balls_Final[800];
   char *Show = "30!";
   for(int i = 0; i < 799; i++){
	   Balls_Final[i] = CreateObject(i%30,i==0?0:i/30,Show[i ==0?0 :i%3],rand()%15+1);
	   CreateBall(rand()%4-2,rand()%4-2,Balls_Final[i]);
	   DrawAllObjects();
	   Screen_DrawBuffer();
	   
	   Sleep(5);
   }
   for(int f = 0; f < 60; f++){
	   for(int i = 0; i < 799; i++)UpdateBall(Balls_Final[i],0);
	   DrawAllObjects();
	   Screen_DrawBuffer();
	   Sleep(50);
   }
   THANKS:;
   FreeObjects();
   
   Object *Thank[28];
   for(int i = 0; i < strlen(Bye); i++){
	   Thank[i] = CreateObject(i%30,15,Bye[i ==0?0 :i%strlen(Bye)],rand()%15+1);
	   CreateBall(rand()%4-2,rand()%4-2,Thank[i]);
	   DrawAllObjects();
	   Screen_DrawBuffer();
	 if(Sound)   Beep(300, 50);
	   Sleep(50);
   }
   Sleep(5000);
   for(int f = 0; f < 70; f++){
	   for(int i = 0; i < strlen(Bye); i++)UpdateBall(Thank[i],0);
	    DrawAllObjects();
	   Screen_DrawBuffer();
	 if(Sound)   Beep(300-((300/70)*f)+100, 50);
	   Sleep(50);
   }
   FreeObjects();
   Screen_Clear();
   
   if(Sound)AudioLibrary_deinit();
}
