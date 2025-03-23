#include "raylib.h"
#include "stdint.h"
#include <math.h>

#define CELL_SIZE 16 
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 50
#define STEP_TIME 0.2
#define FONT_SIZE 25

typedef int32_t i32;
typedef int64_t i64;
typedef double f64;
typedef float f32;

typedef struct v2_s {
	i32 x, y;
} v2;

f32 timer = 0.;

bool cells[SCREEN_WIDTH][SCREEN_HEIGHT];
bool next_gen[SCREEN_WIDTH][SCREEN_HEIGHT];

bool paused = false;

void render() {

	for (i32 x = 0; x < SCREEN_WIDTH; ++x) {
		for (i32 y = 0; y < SCREEN_HEIGHT; ++y) {
			if (cells[x][y])
				DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, BLACK);
			else
				DrawRectangleLines(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE, LIGHTGRAY);
		}
	}

	DrawText(TextFormat("Paused: %s", paused ? "true": "false"), 10, 10, FONT_SIZE, BLACK);
}

bool in_bounds(v2 pos) {
	return !(pos.x < 0 || pos.x >= SCREEN_WIDTH || pos.y < 0 || pos.y >= SCREEN_HEIGHT);
}

bool is_live(v2 pos) {
	if (!in_bounds(pos))
		return false;
	
	return cells[pos.x][pos.y];
}

i32 count_neighbours(v2 pos) {
	i32 n = 0;

	for (i32 i = -1; i <= 1; ++i) {
		for (i32 j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0)
				continue;

			if (is_live((v2) { pos.x + i, pos.y + j }))
				n++;
		}
	}

	return n;
}

void set_cell(v2 pos, bool val) {
	if (in_bounds(pos)) {
		cells[pos.x][pos.y] = val;
	}
}

void advance_gen() {
	for (i32 x = 0; x < SCREEN_WIDTH; ++x) {
		for (i32 y = 0; y < SCREEN_HEIGHT; ++y) {
			cells[x][y] = next_gen[x][y];
		}
	}
}

void handle_mouse() {
	if (!paused)
		return;

	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
		Vector2 pos = GetMousePosition();

		i32 x = ((i32) floor(pos.x)) / CELL_SIZE;
		i32 y = ((i32) floor(pos.y)) / CELL_SIZE;
		
		set_cell((v2) {x, y}, !is_live((v2) {x, y}));
	}
}

void update() {
	handle_mouse();

	if (IsKeyPressed(KEY_SPACE))
		paused = !paused;

	if (paused)
		return;

	timer += GetFrameTime();

	if (timer < STEP_TIME)
		return;

	timer = 0;

	for (i32 x = 0; x < SCREEN_WIDTH; ++x) {
		for (i32 y = 0; y < SCREEN_HEIGHT; ++y) {
			v2 pos = { x, y };
			i32 n = count_neighbours(pos);
			bool live = is_live(pos);

			// Rule 1
			if (live && n < 2)
				next_gen[x][y] = false;

			// Rule 2
			else if (live && (n == 2 || n == 3))
				next_gen[x][y] = true;

			// Rule 3
			else if (live && n > 3)
				next_gen[x][y] = false;

			// Rule 4
			else if (!live && n == 3)
				next_gen[x][y] = true;
			
			else
				next_gen[x][y] = cells[x][y];
		}
	}

	advance_gen();
}

i32 main() {
	InitWindow(SCREEN_WIDTH * CELL_SIZE, SCREEN_HEIGHT * CELL_SIZE, "Conway's Game of Life");

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		update();
		render();
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
