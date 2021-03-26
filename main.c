#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleep
#include <jack/jack.h>
#include <jack/midiport.h>

jack_port_t *input_port = NULL;
jack_port_t *output_port = NULL;
jack_client_t *client = NULL;

void process()
{}

void cleanup()
{
  jack_deactivate(client);
  jack_port_unregister(client, input_port);
  jack_port_unregister(client, output_port);
  jack_client_close(client);
}

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
