
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include "constantes.h" //Variaveis constantes globais
#include "player.h"     //Informa�oes do player
#include "Monsters.h"   //Informa�oes dos mostros
#include "arrays.h"     //Matrizes importantes
#include "Horda.h"

int init_fail (ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer); //Fun�ao falha na inicializa�ao
void destroy_al(ALLEGRO_DISPLAY *janela,ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer);
void init_system(Sistema &torre); //Carrega informa�oes das torres
void draw_tower(ALLEGRO_BITMAP *imagem, int pos_x, int pos_y); //desenha a torre
void coor_matrix(ALLEGRO_FONT *fonte); //Desenha a matriz para fins de debug

//Fun�oes dos montros
void horda(int z, int n);
void createmonster(int z, int n);
void movemonster(int z, int n);
void erasemonster(int z, int n);
void drawmonster(int z, int n, ALLEGRO_BITMAP *imagem);


int main(int argc, char const *argv[]) {

    int i = 0;
    int h = 0;
    int pos_x = 0;
    int pos_y = 0;
    int n_mostros = 10;
    bool nova_horda = true;
    bool GameOver = false;

    Sistema torre;

    //Declara�ao vair�veis allegro
    ALLEGRO_DISPLAY *janela = NULL;	            //Vari�vel para a janela
    ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;   //  ''     para eventos
    ALLEGRO_BITMAP *imagem = NULL;              //  ''     para imagem
    ALLEGRO_TIMER *timer = NULL;                //  ''     para o tempo (fps)
    ALLEGRO_FONT *fonte = NULL;                 //  ''     para fonte

    //Inicializa o allegro, mouse e add-ons
    al_init();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    //Atribui atributos �s vari�veis allegro
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    fila_eventos = al_create_event_queue();
    imagem = al_load_bitmap("virus.png");
    timer = al_create_timer(1.0 / fps);
    fonte = al_load_font("arial.ttf", 12, 0);    //Fonte DejaVu

    //Inicializa o mouse e tempo
    al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
    al_start_timer(timer);
    init_fail(janela, fonte, fila_eventos, imagem, timer); //Fun�ao de teste

    //Regista os eventos da janela, mouse e timer na vari�vel de eventos (fila_eventos)
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));

    init_system(torre);
    horda(h, n_mostros);


    al_clear_to_color(al_map_rgb(235, 235, 235));   //Limpa a tela
    al_flip_display();                              //Atualiza a tela

    //Loop principal
    while (!GameOver)
    {
        al_clear_to_color(al_map_rgb(235, 235, 235)); //Limpa a tela
        coor_matrix(fonte);
        ALLEGRO_EVENT evento;                         //Variavel para eventos
        al_wait_for_event(fila_eventos, &evento);
        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        /*
        else if(evento.type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			pos_x = evento.mouse.x;
			pos_y = evento.mouse.y;
		}
        */
        else if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
            h++;
            horda(h, n_mostros);
        }


        for(int x = 0; x < n_mostros; x++) {
            if (Monstro[x].stillalive == true){
                movemonster(h, x);
                drawmonster(h, x, imagem);

                if (Monstro[x].xlocation > LARGURA_TELA){
                    erasemonster(h, x);

                    //nova_horda = false;
                }
            }
        //nova_horda = false;
        }

        if(evento.type == ALLEGRO_EVENT_TIMER){ //Evento de renderiza�ao
            al_draw_textf(fonte, al_map_rgb(0, 0, 0), LARGURA_TELA/4, 50, ALLEGRO_ALIGN_CENTRE, "Taxa de Frames: %i", i);
            i++;
            al_flip_display();
        }
    }

    destroy_al(janela, fonte, fila_eventos, imagem, timer); //Destroi as vari�veis allegro

    return 0;
}

void init_system(Sistema &torre) {
    torre.x = 400;
    torre.y = 400;
    torre.lives = 10;
    torre.boundx = 405;
    torre.boundy = 390;
    torre.score = 0;
}

void draw_tower(ALLEGRO_BITMAP *imagem, int pos_x, int pos_y){
    al_draw_bitmap(imagem, pos_x, pos_y, 0);
    }

void coor_matrix(ALLEGRO_FONT *fonte){
    int i = 0;
    int j = 0;
    for (i=0;  i<A; i++) {
        for(j=0; j<B; j++){
            al_draw_line(0 + (LARGURA_TELA / B) * j, 0, 0 + (LARGURA_TELA / B) * j, ALTURA_TELA, al_map_rgb(255, 0, 90  ), 1); //Linhas verticais
            al_draw_line( 0,  0 + (ALTURA_TELA / A) * i, LARGURA_TELA, (ALTURA_TELA / A) * i, al_map_rgb(0, 255, 0), 1);       //Linhas horizontais
            al_draw_textf(fonte, al_map_rgb(0, 0, 0),(LARGURA_TELA / B)*j, (ALTURA_TELA / A)*i, ALLEGRO_ALIGN_LEFT  , "%c%i",letras[j], i+1);
        }
    }
}

void destroy_al(ALLEGRO_DISPLAY *janela,ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer){
        al_destroy_display(janela);
        al_destroy_font(fonte);
        al_destroy_event_queue(fila_eventos);
        al_destroy_bitmap(imagem);
        al_destroy_timer(timer);
    }

int init_fail(ALLEGRO_DISPLAY *janela, ALLEGRO_FONT *fonte, ALLEGRO_EVENT_QUEUE *fila_eventos, ALLEGRO_BITMAP *imagem, ALLEGRO_TIMER *timer){
    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return -1;
    }

    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return -1;
    }

    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    if (!imagem)
    {
        fprintf(stderr, "Falha ao carregar imagem.\n");
        al_destroy_bitmap(imagem);
        al_destroy_display(janela);
        return -1;
    }

    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao inicializar o fila de eventos.\n");
        al_destroy_display(janela);
        return -1;
    }

    if(!fonte){
        fprintf(stderr, "Falha ao inicializar a fonte.\n");
        al_destroy_font(fonte);
        al_destroy_display(janela);
        return -1;
    }

    if(!timer){
        fprintf(stderr, "Falha ao inicializar o timer.\n");
        al_destroy_timer(timer);
        al_destroy_display(janela);
        return -1;
    }
}

void createmonster(int z,int n){
    Horda[z].Monstro[n].xlocation = 45;
    Horda[z].Monstro[n].ylocation = 0 - ((n - 1) * 50);
    Horda[z].Monstro[n].health = 5;
    Horda[z].Monstro[n].speed = 5;
    Horda[z].Monstro[n].stillalive = true;
}

void horda(int z,int n){
    for(int x = 0; x < n; x++){
            createmonster(z, x);
        }
    }

void movemonster(int z,int n){

    if ((Horda[z].Monstro[n].xlocation < 250) && (Monstro[n].ylocation < 225))
        Horda[z].Monstro[n].ylocation += Monstro[n].speed;

    if ((Horda[z].Monstro[n].xlocation < 250) && (Monstro[n].ylocation == 225))
        Horda[z].Monstro[n].xlocation+= Monstro[n].speed;

    if ((Horda[z].Monstro[n].xlocation == 250) && (Monstro[n].ylocation > 125))
        Horda[z].Monstro[n].ylocation+= Monstro[n].speed;

    if ((Horda[z].Monstro[n].xlocation < 425) && (Monstro[n].ylocation == 125))
        Horda[z].Monstro[n].xlocation+= Monstro[n].speed;

    if ((Horda[z].Monstro[n].xlocation == 425) && (Monstro[n].ylocation < 325))
        Horda[z].Monstro[n].ylocation+= Monstro[n].speed;

    if ((Horda[z].Monstro[n].xlocation < LARGURA_TELA) && (Monstro[n].ylocation == 325))
        Horda[z].Monstro[n].xlocation+= Monstro[n].speed;
}

void erasemonster(int z,int n){
    Horda[z].Monstro[n].stillalive = false;
    Horda[z].Monstro[n].xlocation = 0;
    Horda[z].Monstro[n].ylocation = 0;
    al_draw_filled_circle(Horda[z].Monstro[n].xlocation, Horda[z].Monstro[n].ylocation, 18, al_map_rgb(255, 255, 0));
}

void drawmonster(int z,int n, ALLEGRO_BITMAP *imagem){
    al_draw_bitmap(imagem, Horda[z].Monstro[n].xlocation, Horda[z].Monstro[n].ylocation, 0);
    al_draw_filled_circle(Horda[z].Monstro[n].xlocation, Horda[z].Monstro[n].ylocation, 18, al_map_rgb(0, 0, 255));
}
