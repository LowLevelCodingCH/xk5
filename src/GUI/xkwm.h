#ifndef XKWM_H_
#define XKWM_H_

#define SPRITE_WIDTH  12
#define SPRITE_HEIGHT 12

typedef struct {
  int color;
} pixel;

typedef struct {
  pixel r[SPRITE_WIDTH];
} row;

typedef struct {
  row spr[SPRITE_HEIGHT];
} Sprite;

void draw_sprite(int x, int y, Sprite spr, int only) {
  if(only == 1) {
    for(int i = 0; i < 320*200; i++)
      *(char*)(0xa0000 + i) = 15;
  }

  int px = x;
  int py = y;
  for(int i = 0; i < SPRITE_HEIGHT; i++) {
    px = x;
    row r = spr.spr[i];
    for(int j = 0; j < SPRITE_WIDTH; j++) {
      pixel p = r.r[j];
      if(p.color != 15)draw_pixel(px, py, p.color);
      px++;
    }
    py++;
  }
}

Sprite create_sprite(pixel tex[SPRITE_WIDTH][SPRITE_HEIGHT]) {
  Sprite spr = {0};

  int x=0;
  int y=0;

  int index = 0;

  for(int i = 0; i < SPRITE_WIDTH; i++) {
    for(int j = 0; j < SPRITE_HEIGHT; j++) {
      x++;
      spr.spr[y].r[x].color = tex[y][x].color;
    }
    x = 0;
    y++;
  }
  kprintln(int_to_string(__LINE__));

  return spr;
}

pixel cursor_texture[SPRITE_WIDTH][SPRITE_HEIGHT] = {
  {15, 00, 00, 15, 15, 15, 15, 15, 15, 15, 15, 15},
  {15, 00, 63, 00, 15, 15, 15, 15, 15, 15, 15, 15},
  {15, 00, 63, 63, 00, 15, 15, 15, 15, 15, 15, 15},
  {15, 00, 63, 63, 63, 00, 15, 15, 15, 15, 15, 15},
  {15, 00, 63, 63, 63, 63, 00, 15, 15, 15, 15, 15},
  {15, 00, 63, 63, 63, 63, 63, 00, 15, 15, 15, 15},
  {15, 00, 63, 63, 63, 63, 63, 00, 15, 15, 15, 15},
  {15, 00, 63, 63, 63, 63, 00, 15, 15, 15, 15, 15},
  {15, 00, 63, 00, 00, 63, 00, 15, 15, 15, 15, 15},
  {15, 00, 00, 15, 15, 00, 63, 00, 15, 15, 15, 15},
  {15, 15, 15, 15, 15, 00, 63, 00, 15, 15, 15, 15},
  {15, 15, 15, 15, 15, 15, 00, 15, 15, 15, 15, 15}
};

typedef struct {
  unsigned int w,h,y,x;
} rect_t;

#define MAX_WINDOWS_CNT 3
#define MAX_ELEMENT_CNT 5

typedef enum {
  text,
  color,
  button,
} element_kind_t;

typedef struct {
  unsigned int x;
  unsigned int y;
  element_kind_t kind;
  char exists; // false
} element_t;

typedef struct {
  unsigned int x;
  unsigned int y;
  unsigned int w;
  unsigned int h;
  int id;
  char exists;
  char* title;
  element_t elements[MAX_ELEMENT_CNT];
} window_t;

pixel cbtn[SPRITE_WIDTH][SPRITE_HEIGHT] = {
  {04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04},
  {04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04},
  {04, 04, 00, 04, 04, 04, 04, 04, 04, 00, 04, 04},
  {04, 04, 04, 00, 04, 04, 04, 04, 00, 04, 04, 04},
  {04, 04, 04, 04, 00, 04, 04, 00, 04, 04, 04, 04},
  {04, 04, 04, 04, 04, 00, 00, 04, 04, 04, 04, 04},
  {04, 04, 04, 04, 04, 00, 00, 04, 04, 04, 04, 04},
  {04, 04, 04, 04, 00, 04, 04, 00, 04, 04, 04, 04},
  {04, 04, 04, 00, 04, 04, 04, 04, 00, 04, 04, 04},
  {04, 04, 00, 04, 04, 04, 04, 04, 04, 00, 04, 04},
  {04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04},
  {04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04, 04}
};

void draw_rect(rect_t *rect, unsigned char color) {
  for(int i = 0; i < rect->h; i++) {
    for(int j = 0; j < rect->w; j++) {
      draw_pixel(j+rect->x, i+rect->y, color);
    }
  }
}

void draw_window(window_t* window) {
  Sprite close_btn = create_sprite(cbtn);

  // Draw content to the field
  for(int i = 0; i <   window->h; i++) {
    for(int j = 0; j < window->w; j++) {
      draw_pixel(j+window->x, i+window->y, 7);
    }
  }

  draw_sprite(window->x, window->y, close_btn, 0);

  draw_text(window->title, window->x + SPRITE_WIDTH, window->y, 0);

  // ELEMENTS
  //
  for(int i = 0; i < MAX_ELEMENT_CNT; i++) {
    // DRAW_ELEMENT(ELEMENT);
  }
}

typedef window_t* WHND;

int free_window_spot = 0;

window_t windows[MAX_WINDOWS_CNT];

WHND add_window(window_t window) {
  if(free_window_spot != -1) windows[free_window_spot] = window;
  if(free_window_spot < MAX_WINDOWS_CNT) free_window_spot++;
  else {free_window_spot = -1;}
  return &windows[free_window_spot];
}

WHND find_window(unsigned int x, unsigned int y) {
	for (size_t i = MAX_WINDOWS_CNT-1; i != -1; i--) {
		if (x >= windows[i].x && x < windows[i].x + windows[i].w &&
			y >= windows[i].y && y < windows[i].y + windows[i].h) {

			return &windows[i];
		}
	}

	return NULL;
}

void nullify_window(WHND wptr) {
  free_window_spot--;
  wptr->exists = 0;
  wptr->x = 1000;
  wptr->y = 1000;
  wptr->w = 0;
  wptr->h = 0;
  scpy(wptr->title, "");
  wptr = NULL;
}

window_t create_xkwm_window(int width, int height, int x, int y, char *title){
  window_t window;
  window.w = width;
  window.h = height;
  window.x = x;
  window.y = y;
  window.exists = 1;
  window.title = title;
  return window;
} // XKWM

void gmode(void) {
  window_t window0 = create_xkwm_window(110, 50, 20, 8, "test #0");
  window_t window1 = create_xkwm_window(40, 80, 100, 30, "test #1");

  WHND win0 = add_window(window0);
  WHND win1 = add_window(window1);

  Sprite cur = create_sprite(cursor_texture);

  int x = 12;
  int y = 12;
  char cgr = 0x00;
  lidfk();
  for(int i = 0; i < 320*200; i++)
    *(char*)(0xa0000 + i) = 15;
  for(;;){
    draw_sprite(x, y, cur, 1);
    // other graphics
    for(int i = 0; i < MAX_WINDOWS_CNT; i++) {
      if(windows[i].exists != 0) {
        draw_window(&windows[i]);
      }
    }
    //...
    draw_sprite(x, y, cur, 0);

    cgr = read_char();

    if(cgr == 10) {
      WHND wptr = find_window(x, y);
      if(x >= wptr->x + 0 && x <= wptr->x + SPRITE_WIDTH) {
        if(y >= wptr->y + 0 && y <= wptr->y + SPRITE_HEIGHT) {
          nullify_window(wptr);
          wptr = NULL;
        }
      }
    }
    if(cgr == 'w' && y > 0)  {
      y--;
    }
    if(cgr == 's' && y < 200){
      y++;
    }
    if(cgr == 'a' && x > 0)  {
      x--;
    }
    if(cgr == 'd' && x < 320){
      x++;
    }
  }
}

#endif // XKWM_H_
