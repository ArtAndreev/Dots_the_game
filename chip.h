#ifndef CHIP_H
#define CHIP_H


struct Chip
{
  Chip();//points заполняется в зависимости от shape
  Chip(int shape, int color);

  int shape;
  int color;
  int points;
};

#endif // CHIP_H