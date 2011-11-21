#include <iostream>
#include <SDL.h>
#include <time.h>

bool init_SDL();
bool create_win_SDL();
void refresh_SDL();
void draw_damier();
void draw_pions_plat();
void draw_pion(int x, int y, int color);
void event_SDL(SDL_Event event);
void random_plateau();
void tour();
void chargement(int state);

using namespace std;

SDL_Surface *screen;
SDL_Surface *chargement_surf;
SDL_Rect position_charg;
SDL_Surface *recta, *fond;
SDL_Surface *blanc, *noir;
SDL_Surface *pion_j, *pion_n, *pion_r;
SDL_Rect position;

int pion[8][8];
int continuer;

int x;
int y;

bool clic;
int player,oldplayer;

int main ( int argc, char** argv )
{
    bool depla=false;
    bool first[2] = {true,true};
    player=0;
    int color=0;

    int xtab;
    int ytab;

    continuer=1;
    clic=false;

    SDL_Event event;

    /*pion[7][2]=2;
    pion[5][3]=1;
    pion[4][5]=2;
    pion[6][7]=3;*/

    // initialisation SDL
    if(init_SDL())
    {
        return 0;
    }

    // creation fenetre
    if(create_win_SDL())
    {
        return 0;
    }

    chargement(0);
    fond = SDL_LoadBMP("image/1/fond.bmp");
    chargement(10);
    recta = SDL_LoadBMP("image/1/cadre.bmp");
    chargement(20);
    blanc = SDL_LoadBMP("image/1/blanc.bmp");
    chargement(30);
    noir = SDL_LoadBMP("image/1/noir.bmp");
    chargement(40);
    pion_j = SDL_LoadBMP("image/1/pion_j.bmp");
    chargement(50);
    SDL_SetColorKey(pion_j, SDL_SRCCOLORKEY, SDL_MapRGB(pion_j->format, 255, 255, 255));
    chargement(60);
    pion_n = SDL_LoadBMP("image/1/pion_n.bmp");
    chargement(70);
    SDL_SetColorKey(pion_n, SDL_SRCCOLORKEY, SDL_MapRGB(pion_n->format, 24, 255, 0));
    chargement(80);
    pion_r = SDL_LoadBMP("image/1/pion_r.bmp");
    chargement(90);
    SDL_SetColorKey(pion_r, SDL_SRCCOLORKEY, SDL_MapRGB(pion_r->format, 24, 255, 0));
    random_plateau();
    chargement(100);




    //boucle jeu
    while(continuer)
    {
        oldplayer=player;
        while(player==oldplayer && continuer)
        {
            event_SDL(event);

            if(first[player] && clic)
            {
                x=(x-50)/62;
                y=(y-50)/62;
                if(pion[x][y]==1)
                {
                    pion[x][y]=0;
                    first[player]=false;
                    tour();
                }
                clic=false;
            }


            SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 238, 143, 15));
            position.x=0;
            position.y=0;
            SDL_BlitSurface(fond,NULL,screen,&position);

            position.x=25;
            position.y=25;
            //SDL_FillRect(recta,NULL,SDL_MapRGB(recta->format,192,121,41));
            SDL_BlitSurface(recta,NULL,screen,&position);

            draw_damier();
            draw_pions_plat();

            if(depla)
            {
                draw_pion(x-21,y-21,color);
            }

            if(clic && !depla)
            {
                xtab=(x-50)/62;
                ytab=(y-50)/62;
                color=pion[xtab][ytab];
                pion[xtab][ytab]=0;
                draw_pion(x-21,y-21,color);
                depla=true;
            }

            if(depla && !clic)
            {
                x=(x-50)/62;
                y=(y-50)/62;

                pion[x][y]=color;

                depla=false;
                tour();
            }

            refresh_SDL();
        }
    }

    SDL_FreeSurface(recta);
    SDL_FreeSurface(noir);
    SDL_FreeSurface(blanc);
    SDL_Quit();
    return 0;
}

bool init_SDL()
{
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        cout << "Impossible d'initialiser la SDL : " << SDL_GetError() << endl;
        return 1;
    }
    return 0;
}

bool create_win_SDL()
{
    screen = SDL_SetVideoMode(600, 600, 16,SDL_HWSURFACE|SDL_DOUBLEBUF);
    if (!screen)
    {
        cout << "Impossible d'initialiser la fenetre SDL :" << SDL_GetError() << endl;
        return 1;
    }
    return 0;
}

void refresh_SDL()
{
    SDL_Flip(screen);
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
}

void draw_damier()
{
    int altern_color=1;

        for(int j=0;j<8;j++)
        {
            for(int i=0;i<8;i++)
            {
                position.x=50+(62*i);
                position.y=50+(62*j);
                if(altern_color%2)
                {
                    //SDL_FillRect(blanc,NULL,SDL_MapRGB(recta->format,255,255,255));
                    SDL_BlitSurface(blanc,NULL,screen,&position);
                    altern_color=0;
                }
                else
                {
                    //SDL_FillRect(noir,NULL,SDL_MapRGB(recta->format,0,0,0));
                    SDL_BlitSurface(noir,NULL,screen,&position);
                    altern_color=1;
                }
            }
            altern_color--;
        }
}

void draw_pions_plat()
{
    for(int j=0;j<8;j++)
    {
        for(int i=0;i<8;i++)
        {
            position.x=60+(62*i);
            position.y=60+(62*j);

            switch(pion[i][j])
            {
                case 1:
                    //SDL_FillRect(pion_j,NULL,SDL_MapRGB(pion_j->format,0,255,255));
                    SDL_BlitSurface(pion_j,NULL,screen,&position);
                    break;

                case 2:
                    //SDL_FillRect(pion_n,NULL,SDL_MapRGB(pion_n->format,255,255,255));
                    SDL_BlitSurface(pion_n,NULL,screen,&position);
                    break;
                case 3:
                    //SDL_FillRect(pion_r,NULL,SDL_MapRGB(pion_r->format,255,0,0));
                    SDL_BlitSurface(pion_r,NULL,screen,&position);
                    break;
            }
        }
    }
}

void event_SDL(SDL_Event event)
{
    SDL_PollEvent(&event);
    switch(event.type)
    {
        case SDL_QUIT:
            continuer=0;
            break;
        case SDL_MOUSEBUTTONDOWN:
            clic=true;
            break;
        case SDL_MOUSEBUTTONUP:
            clic=false;
            break;
        case SDL_MOUSEMOTION:
            x = event.motion.x;
            y = event.motion.y;
            break;
        case SDL_KEYDOWN:
            random_plateau();
            break;
    }
}


// TODO : reorganiser
void random_plateau()
{
    srand(time(NULL));

    int jaune = 34;
    int rouge = 20;
    int noir = 10;

    int result;

    for(int j=0;j<8;j++)
    {
        for(int i=0;i<8;i++)
        {
            result=(rand()%(6-1) +1);

            cout << "result : " << result << endl;

            if(result==1 || result==5 ||jaune>0 || result==6)
            {
                cout << "here" << endl;
                jaune--;
                cout << "j :"<< jaune << endl;
                pion[i][j]=1;
            }
            else {result=3;}

            if(result==3)
            {
                if(rouge>0)
                {
                    rouge--;
                    cout << "r :"<< rouge << endl;
                    pion[i][j]=3;
                }
                else
                {
                        result--;
                }
            }
            if(result==2)
            {
                if(noir>0)
                {
                    noir--;
                    cout << "n :"<< noir << endl;
                    pion[i][j]=2;
                }
                else
                {
                    result--;
                }
            }
            if(result==1)
            {
                    pion[i][j]=1;
            }
        }
    }
}

void tour()
{
    player++;
    if(player>1)
    {
        player=0;
    }
}

void draw_pion(int x, int y, int color)
{
    position.x=x;
    position.y=y;
    switch(color)
    {
        case 1:
            SDL_BlitSurface(pion_j,NULL,screen,&position);
            break;
        case 2:
            SDL_BlitSurface(pion_n,NULL,screen,&position);
            break;
        case 3:
            SDL_BlitSurface(pion_r,NULL,screen,&position);
            break;
    }
}

void chargement(int state)
{
    int size = 600/100;
    position_charg.x=0;
    position_charg.y=200;
    chargement_surf = SDL_CreateRGBSurface(SDL_HWSURFACE, size*state, 100, 32, 0, 0, 0, 0);
    SDL_FillRect(chargement_surf, NULL, SDL_MapRGB(screen->format, 255, 0, 0));
    SDL_BlitSurface(chargement_surf, NULL, screen, &position_charg);
    refresh_SDL();
}
