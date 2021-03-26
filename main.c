#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleep
#include <jack/jack.h>
#include <jack/midiport.h>

jack_port_t *input_port = NULL;
jack_port_t *output_port = NULL;
jack_client_t *client = NULL;

int process()
{}

void cleanup()
{
  jack_deactivate(client);
  jack_port_unregister(client, input_port);
  jack_port_unregister(client, output_port);
  jack_client_close(client);
}

void jack_setup()
{
  if((client = jack_client_open("midi-velocity-scale", JackNullOption, 0)) == 0){
    fprintf(stderr, "Failed to start jack client\n");
    exit(1);
  }

  //midi ports
  input_port = jack_port_register(client, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);
  if(input_port == 0){
    jack_client_close(client);
  }

  output_port = jack_port_register(client, "midi_out", JACK_DEFAULT_MIDI_TYPE, JackPortIsOutput, 0);
  if(output_port == 0){
    jack_port_unregister(client, input_port);
    jack_client_close(client);
  }

  atexit(cleanup);

  jack_set_process_callback(client, &process, 0);

  if(jack_activate(client)){
    fprintf(stderr, "Cannot activate client\n");
    cleanup();
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  jack_setup();

  while(1){
    sleep(1);
  }

  cleanup();

  return 0;
}
