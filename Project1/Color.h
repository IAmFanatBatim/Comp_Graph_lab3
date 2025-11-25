#pragma once
#ifndef COLOR_H
#define COLOR_H
//Структура для задания цвета
typedef struct tagCOLOR
{
	unsigned char red;		// Компонента красного цвета
	unsigned char green;	// Компонента зелёного цвета
	unsigned char blue;		// Компонента синего цвета
	unsigned char alpha;	// Прозрачность (альфа канал)

	//Конструкторы для создания экземпляра цвета по умолчанию (непрозрачный черный) или с зазаданными компонентами RGB-ALPHA
	tagCOLOR() : red(0), green(0), blue(0), alpha(255) {
	}
	tagCOLOR(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : red(r), green(g), blue(b), alpha(a) {
	}
} Color;

#endif