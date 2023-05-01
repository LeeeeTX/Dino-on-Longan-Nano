#include "lcd/lcd.h"
#include <string.h>
#include "utils.h"
#include "img.h"
#include <time.h>

int mode = 0;   // 0 for easy and 1 for hard
int scores[4] = {0,0,0,0};   // record the scores

void Inp_init(void)
{
    gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
}

void Adc_init(void) 
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_0|GPIO_PIN_1);
    RCU_CFG0|=(0b10<<14)|(1<<28);
    rcu_periph_clock_enable(RCU_ADC0);
    ADC_CTL1(ADC0)|=ADC_CTL1_ADCON;
}

void IO_init(void)
{
    Inp_init(); // inport init
    Adc_init(); // A/D init
    Lcd_Init(); // LCD init
}

void Start_menu();
void Start();
void Setting();
void Rank(int score);
void Start_menu();

// u8 buf[WIDTH * HEIGHT * 2];
int main(void)
{
    srand((unsigned)now());
    IO_init();         // init OLED
    // YOUR CODE HERE
    LCD_Clear(BLACK);
    BACK_COLOR=BLACK;
    Start_menu();
    // Just for test: input the latest score
    // Rank(3452);

    return 0;
}

/* void Start_menu()
{
    LCD_ShowString(64, 16, (u8 *)("Dino!"), WHITE);
    LCD_ShowString(28, 40, (u8 *)("Start"), WHITE);
    LCD_ShowString(88, 40, (u8 *)("Setting"), WHITE);
    while(1)
    {
        if (Get_Button(0))
        {
            LCD_DrawLine(28, 58, 68, 58,WHITE);
            delay_1ms(500);
            LCD_Clear(BLACK);
            // BACK_COLOR=BLACK;
            Start();
        }
        if (Get_Button(1))
        {
            LCD_DrawLine(88, 58, 145, 58,WHITE);
            delay_1ms(500);
            LCD_Clear(BLACK);
            // BACK_COLOR=BLACK;
            Setting();
        }
    }
} */

typedef enum {
    PTER,
    CACT,
    GND,
    TREX,
} spec;

typedef struct {
    int x, y, w, h;
    spec what;
    u8 frame;
} thing;

unsigned char* dispatch(thing t){
    if(t.what == GND && t.frame == 0)return g1;
    if(t.what == GND && t.frame == 1)return g2;

    if(t.what == PTER && t.frame == 0)return pter1;
    if(t.what == PTER && t.frame == 1)return pter2;

    if(t.what == CACT && t.frame == 0)return cactus1;
    if(t.what == CACT && t.frame == 1)return cactus2;

    if(t.what == TREX && t.frame == 0)return trex1;
    if(t.what == TREX && t.frame == 1)return trex2;
    if(t.what == TREX && t.frame == 2)return trex3;
    if(t.what == TREX && t.frame == 3)return trex4;
    if(t.what == TREX && t.frame == 4)return trex5;

}

u8 draw_thing(thing t, u8 flipped){
    if(t.x >= WIDTH || t.x + t.w - 1 < 0) return 0;
    if(t.y >= HEIGHT || t.y + t.h - 1 < 0) return 0;
    LCD_ShowPic(t.x, t.y, t.w, t.h, dispatch(t), flipped);
    return 1;
}

#define OBSS_SZ 2
#define DAY (30 * 1000) // unit ms
#define TREX_UPD 150 // unit ms
#define GROUND_UPD 50 // unit ms
#define OBSS_UPD 500 // unit ms
                      
#define GROUND (HEIGHT * 4 / 5)
#define SKY (HEIGHT * 4 / 5 - 15) 
#define VELOCITY 8
#define GRAVITY 1
#define on_tick(T) if((uint64_t)(duration / (T)) != (uint64_t)(last_duration / (T)))

bool collide(thing left, thing right){
    double a = (left.x + left.w / 2.0 - right.x - right.w / 2.0); 
    double b = (left.y + ((left.frame >= 3) ? (left.h * 5 / 6) : (left.h / 6)) - right.y - right.h / 2.0); 
    double dis2 = a * a + b * b;
    return right.what == PTER ? dis2 < 200 : dis2 < 160;
}

void Start()
{
        LCD_ShowString(64, 32, (u8 *)(mode == 0 ? "Easy" : "Hard"), WHITE);
        delay_1ms(700);
        LCD_Clear(BACK_COLOR);

        double start_time = now();
        double last_duration = 0;
        int v_y = 0;
        u8 flipped = 0, dashed = 0, jumping = 0;
        u8 speed = (mode == 0 ? 4 : 6);

        thing trex, obss[OBSS_SZ], gs[2];

        trex.what = TREX, trex.frame = rand() % 2;
        trex.w = trex.h = 20;
        trex.x = 5, trex.y = GROUND - trex.h;

        for(int i = 0; i < OBSS_SZ; ++i){
            obss[i].what = rand() % 2 == 0 ? PTER : CACT;
            obss[i].x = ((i == 0) ? WIDTH : obss[i-1].x) + (WIDTH / OBSS_SZ) + rand() % 40;
            obss[i].h = 20;
            obss[i].w = obss[i].what == PTER ? 20 : 12;
            obss[i].y = obss[i].what == PTER ? SKY - obss[i].h : GROUND - obss[i].h;
            obss[i].frame = rand() % 2;
        }

        for(int i = 0; i < 2; ++i){
            gs[i].what = GND;
            gs[i].w = 160, gs[i].h = 10; 
            gs[i].x = gs[i].w * i, gs[i].y = GROUND - 3;
            gs[i].frame = rand() % 2;
        }

        u16 score = 0, dead = 0;
        int frame = 0;
        while(1){
            delay_schedule(); // This might help to make the FPS consistent if the delay is big enough
            double cur_time = now(); 
            double duration = cur_time - start_time;
            // LCD_Clear(BACK_COLOR);

            // Events:
            on_tick(DAY){
                flipped ^= 1;
                BACK_COLOR = flipped ? WHITE : BLACK;
                LCD_Clear(BACK_COLOR);
            }
            on_tick(TREX_UPD){
                if(dashed){
                    trex.frame = trex.frame == 3 ? 4 : 3;
                } else if(!jumping)
                    trex.frame ^= 1;
            }
            on_tick(GROUND_UPD){
                for(int i = 0; i < 2; ++i)
                    gs[i].x -= speed;
                for(int i = 0; i < 2; ++i){
                    if(gs[i].x + gs[i].w < 0){
                        gs[i].x = gs[i^1].x + gs[i^1].w;
                        gs[i].frame = rand() % 2;
                    }
                }
                for(int i = 0; i < OBSS_SZ; ++i) {
                    obss[i].x -= speed;
                    if(obss[i].x + obss[i].w + speed >= 0)
                        LCD_Fill(obss[i].x + obss[i].w >= 0 ? obss[i].x + obss[i].w : 0, 
                                 obss[i].y, obss[i].x + obss[i].w + speed, obss[i].y + obss[i].h, BACK_COLOR);
                }
                for(int i = 0; i < OBSS_SZ; ++i)
                    if(obss[i].x + obss[i].w < 0){
                        score += 1;
                        obss[i].what = rand() % 2 == 0 ? PTER : CACT;
                        obss[i].w = obss[i].what == PTER ? 20 : 12;
                        obss[i].y = obss[i].what == PTER ? SKY - obss[i].h : GROUND - obss[i].h;
                        obss[i].x = obss[(i-1+OBSS_SZ) % OBSS_SZ].x + (WIDTH / OBSS_SZ) + rand() % 40;
                        obss[i].frame = rand() % 2;
                    }
            }
            on_tick(OBSS_UPD){
                for(int i = 0; i < OBSS_SZ; ++i){
                    obss[i].frame ^= (obss[i].what == PTER);
                }
            }

            // Renders
            for(int i = 0; i < 2; ++i) {
                draw_thing(gs[i], flipped);
            }
            for(int i = 0; i < OBSS_SZ; ++i) {
                draw_thing(obss[i], flipped);
                if(collide(trex, obss[i]))dead = 1;
            }
            if(dead) break;
            u8 dashing = 0;
            if (jumping) {
                if(v_y > 0)
                    LCD_Fill(trex.x, trex.y + trex.h - v_y, trex.x + trex.w , trex.y + trex.h, BACK_COLOR);
                else if(v_y < 0)
                    LCD_Fill(trex.x, trex.y,       trex.x + trex.w , trex.y - v_y, BACK_COLOR);
                trex.y -= v_y;
                v_y -= GRAVITY;
                if (mode ==  0 && frame == 1)
                    v_y += GRAVITY, frame = 0;
                else
                    frame = 1;
                if(trex.y >= GROUND - trex.h) {
                  trex.y = GROUND - trex.h;
                  jumping = 0;
                }
            } else {
                if (Get_Button(0)) {
                    dashing = 0, jumping = 1;
                    if (mode == 0) v_y = 5;
                    else v_y = VELOCITY - 1;
                    trex.frame = 2;
                    trex.w = 20;
                } else 
                    if(Get_Button(1)){
                    dashing = 1;
                    trex.w = 27;
                    trex.frame = rand() % 2 == 1 ? 3 : 4;
                } else {
                    dashing = 0;
                    trex.w = 20;
                    if(trex.frame >= 2) trex.frame = rand() % 2;
                } 
            }
            if(dashed && !dashing) {
                LCD_Fill(trex.x + trex.w , trex.y, trex.x + trex.w + 20, trex.y + trex.h, BACK_COLOR);
            } 
            dashed = dashing;
            draw_thing(trex, flipped);
            u16 FORE_COLOR = flipped ? BLACK : WHITE;
            LCD_ShowString(70, 0, (u8 *)("Score: "), FORE_COLOR);
            LCD_ShowNum(120, 0, score, 4, FORE_COLOR);

            last_duration = duration;
            delay_wait(30);
        }
        delay_wait(1000);
        BACK_COLOR = BLACK;
        Rank(score);
        LCD_Clear(BLACK);
        LCD_ShowString(64, 16, (u8 *)("Dino!"), WHITE);
        LCD_ShowString(28, 40, (u8 *)("Start"), WHITE);
        LCD_ShowString(88, 40, (u8 *)("Setting"), WHITE);
}

void Render_Easy();
void Render_Hard();
void Render_Setting();

void Setting()
{
    Render_Setting();
    /* LCD_ShowString(54, 16, (u8 *)("Setting"), WHITE);
    LCD_ShowString(28, 40, (u8 *)("Easy"), WHITE);
    LCD_ShowString(98, 40, (u8 *)("Hard"), WHITE); */
    while(1)
    {
        if (Get_Button(0))
        {
            mode = 0;
            BACK_COLOR = BLACK;
            Render_Easy();

        }
        if (Get_Button(1))
        {
            BACK_COLOR=BLACK;
            mode = 1;
            Render_Hard();
        }
    }
} 

void Rank(int score)
{

    LCD_Clear(BLACK);
    // LCD_ShowString(64,  0, (u8 *)("Rank"), WHITE);
    if (score <= scores[3]) Display_try_again();
    // {
        // LCD_ShowString(64,  0, (u8 *)("Rank"), WHITE);
        // LCD_ShowString(0, 26, (u8 *)("You are not in rank"), WHITE);
        // LCD_ShowString(42, 42, (u8 *)("Try again!"), WHITE);
    // }
    else
    {
        scores[3] = score;
        for (int i = 3; i > 0; i--)
        {
            if (scores[i] > scores[i - 1])
            {
                int temp = scores[i - 1];
                scores[i - 1] = scores[i];
                scores[i] = temp;
            }
            else break;
        }
        Display_rank(scores[0], scores[1], scores[2], scores[3]);
        // LCD_ShowString(32, 16, (u8 *)("#1: "), WHITE);
        // LCD_ShowNum(48, 16, scores[0], 10, WHITE);
        // LCD_ShowString(32, 32, (u8 *)("#2: "), WHITE);
        // LCD_ShowNum(48, 32, scores[1], 10, WHITE);
        // LCD_ShowString(32, 48, (u8 *)("#3: "), WHITE);
        // LCD_ShowNum(48, 48, scores[2], 10, WHITE);
        // LCD_ShowString(32, 64, (u8 *)("#4: "), WHITE);
        // LCD_ShowNum(48, 64, scores[3], 10, WHITE);
    }
    while(!Get_Button(0) && !Get_Button(1));
    LCD_Clear(BLACK);
    BACK_COLOR=BLACK;
    delay_1ms(300);
    Start_menu();
}
