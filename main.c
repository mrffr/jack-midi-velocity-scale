#include <stdio.h>
#include <unistd.h> //usleep
#include <signal.h>
#include <jack/jack.h>
#include <jack/midiport.h>

jack_port_t *input_port = NULL;
jack_port_t *output_port = NULL;
jack_client_t *client = NULL;

//set velocity limits
//127 is the max value for midi velocity
int min_vel = 20;
int max_vel = 127;

int process(jack_nframes_t nframes, void* arg)
{
  void * midi_in_buf = jack_port_get_buffer(input_port, nframes);
  void * midi_out_buf = jack_port_get_buffer(output_port, nframes);

  jack_midi_clear_buffer(midi_out_buf);

  jack_nframes_t midi_event_n = jack_midi_get_event_count(midi_in_buf);
  jack_midi_event_t ev;

  for(jack_nframes_t i=0; i<midi_event_n; i++){
    jack_midi_event_get(&ev, midi_in_buf, i);

    //0x90 is Midi Note-On
    if((ev.buffer[0] & 0xf0) == 0x90){

      //linear scaling for now
      int old_val = ev.buffer[2];
      float v = ev.buffer[2]/127.0f;
      int new_val = min_vel + (max_vel - min_vel) * v;
      if(new_val > 127) new_val = 127;
      ev.buffer[2] = new_val;

      /*printf("%i %i old:%i new:%i\n",ev.buffer[0], ev.buffer[1],
	     old_val, ev.buffer[2]);
      */
    }

    jack_midi_event_write(midi_out_buf, ev.time, ev.buffer, ev.size);
  }

  return 0;
}

void cleanup()
{
  fprintf(stderr, "Closing program\n");
  jack_deactivate(client);
  jack_port_unregister(client, input_port);
  jack_port_unregister(client, output_port);
  jack_client_close(client);
  exit(0);
}

void jack_setup()
{
  client = jack_client_open("midi-velocity-scale", JackNullOption, 0);
  if(client == 0){
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

  jack_set_process_callback(client, &process, 0);

  if(jack_activate(client)){
    fprintf(stderr, "Cannot activate client\n");
    cleanup();
    exit(1);
  }
}

void usage(char *exename){
  fprintf(stderr, "usage: %s [-l] [-h]\n", exename);
  fprintf(stderr, "\t-l\t Minimum midi value.\n");
  fprintf(stderr, "\t-h\t Maximum midi value.\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
  //TODO handle command line args for min/max value
  int opt;
  while((opt = getopt(argc, argv, "l:h:")) != -1){
    switch(opt){
      case 'l': 
        {
          int min = atoi(optarg);
          if (min >= 0 && min <= 127){
            min_vel = min; 
          }
        }
        break;
      case 'h':
        {
          int max = atoi(optarg);
          if(max >= 0 && max <= 127){
            max_vel = max;
          }
        }
        break;
      default:
        usage(argv[0]);
        break;
    }
  }

  jack_setup();

  //cleanup connections after sigint and sigterm
  struct sigaction new_action;
  new_action.sa_handler = cleanup;
  sigemptyset(&new_action.sa_mask);
  sigaddset(&new_action.sa_mask, SIGINT);
  sigaddset(&new_action.sa_mask, SIGTERM);
  new_action.sa_flags = 0;

  sigaction(SIGINT, &new_action, NULL);
  sigaction(SIGTERM, &new_action, NULL);

  fprintf(stderr, "Starting\n");
  fprintf(stderr, "Min/Max midi velocity: %i %i\n", min_vel, max_vel);

  //main loop
  while(1){
    sleep(1);
  }

  cleanup();

  return 0;
}
