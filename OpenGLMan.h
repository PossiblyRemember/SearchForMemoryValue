#pragma once
#define NDEBUG
#include <GL/freeglut.h>
#include <iostream>
#include <thread>


void renderBitmapString(float x, float y, void* font, const char* string);

void initOpenGLWin();

void display();

int counter();

void reshape(int width, int height);

void changeText();

void debug();

int mcp();