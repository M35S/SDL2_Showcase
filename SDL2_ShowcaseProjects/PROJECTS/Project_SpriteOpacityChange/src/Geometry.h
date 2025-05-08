#pragma once

struct Point
{
	float x = 0.0f;
	float y = 0.0f;
};

struct Line
{
	Point firstPoint, secondPoint;
};

struct Rect
{
	Point coord;
	float width = 0.0f;
	float height = 0.0f;
};

struct Colour
{
	char red = 0;
	char green = 255;
	char blue = 0;
	char alpha = 120;
};

struct CollisionObject
{
	Rect box;
	bool isGrounded;
	bool isSolid;
	bool isVisible;
	bool isActive;
	Colour colour;
};