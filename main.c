#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleep
#include <jack/jack.h>
#include <jack/midiport.h>


void cleanup()
{}

void jack_setup()
{}

int main(int argc, char *argv[])
{
  jack_setup();

  while(1){
    sleep(1);
  }

  cleanup();

  return 0;
}
