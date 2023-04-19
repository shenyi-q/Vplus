#ifndef GLOBAL_H
#define GLOBAL_H

#define TRUE 1
#define FALSE 0

typedef struct 
{
  //! True when stepper motor is running.
  unsigned char running:1;
  //! True when uart has received a string (ended with '/r').
  unsigned char cmd:1;
  //! Dummy bits to fill up a byte.
  unsigned char dummy:6;
}GLOBAL_FLAGS;

#endif
