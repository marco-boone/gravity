#include "raylib.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "stdio.h"
#define INIT_SPACE_RANGE 200.0
#define SCREEN_W 1600 
#define SCREEN_H 900
#define NUM_PLANETS 10

struct Planet {
    float mass; 
    Vector2 pos; 
    Vector2 force; 
    Vector2 vel;
    Vector2 acc;
    float size; 
    Color color; 
};

Vector2 g_force(struct Planet *pa, struct Planet *pb)
{
    Vector2 r_vec = {pb->pos.x - pa->pos.x, pb->pos.y-pa->pos.y};
    float r = sqrtf(powf(r_vec.x,2)+powf(r_vec.y,2));
    if(r < 20) r = 20; 
    float g = (((pa->mass)*(pb->mass))/powf(r,2)) * 1000;
    Vector2 force = {g*(r_vec.x/r), g*(r_vec.y/r)};
    return force; 
}

float rand_init_pos()
{
    return ((float)rand()/RAND_MAX)*2*INIT_SPACE_RANGE-INIT_SPACE_RANGE; 
}
float rand_color()
{
    return ((float)rand()/RAND_MAX)*255; 
}

void init_planets(struct Planet *p)
{    
    for(int i = 0; i < NUM_PLANETS; i++)
    {
        p[i].mass  = ((float)rand()/RAND_MAX)*200+100; 
        p[i].pos   = (Vector2){(float)SCREEN_W/2 + rand_init_pos(), (float)SCREEN_H/2 + rand_init_pos()};
        p[i].force = (Vector2){0,0};
        p[i].acc   = (Vector2){0,0};
        p[i].vel   = (Vector2){0,0};
	p[i].size  = p[i].mass/10; 
	p[i].color = (Color){rand_color(), rand_color(), rand_color(), 255};
    }
}

void update_planets(struct Planet *p, float dt)
{
    for(int i = 0; i < NUM_PLANETS; i++)
    {
	p[i].force.x = 0;
	p[i].force.y = 0; 

	for(int j = 0; j < NUM_PLANETS; j++)
	{
	    if(i==j)continue; 
	    Vector2 force = g_force(&p[i],&p[j]); 
	    p[i].force.x += force.x;
	    p[i].force.y += force.y; 
	}

	p[i].acc.x = p[i].force.x / p[i].mass;
        p[i].acc.y = p[i].force.y / p[i].mass;

	p[i].vel.x += p[i].acc.x * dt;
	p[i].vel.y += p[i].acc.y * dt; 

	p[i].pos.x += p[i].vel.x * dt; 
	p[i].pos.y += p[i].vel.y * dt;

    }
    
}

int main()
{
    srand(time(NULL)); 
    float dt =  0;
    struct Planet p[NUM_PLANETS]; 
    init_planets(&p[0]); 
    
    InitWindow(SCREEN_W, SCREEN_H, "gravity_sim");
    SetTargetFPS(60); 

    while(!WindowShouldClose())
    {
	dt = GetFrameTime(); 
	update_planets(&p[0], dt); 

	BeginDrawing(); 
	    ClearBackground(BLACK);
	    for(int i = 0; i < NUM_PLANETS; i++)
	    {
	        DrawCircleV(p[i].pos, p[i].size, p[i].color);
	    }
	EndDrawing(); 
    }

    CloseWindow(); 
    return 0; 
}
