#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>
#define CTRL_PORT 30730
#define UI_PORT 10730
#define BSIZE 65536 //bytes
#define RTIME 250


int main() {
  return 0;
}
/*
struct arguments {
  int ctrl_port, ui_port, bsize, rtime;
};

static int parse_opt (int key, char *arg, struct argp_state *state) {
   struct arguments *arguments = state->input;

   switch (key) {
     case 'd': {
       //printf(atoi(arg));
       break;
     }
     case 'C': {
       printf("2137");
       //arguments->ctrl_port = atoi(arg);
       break;
     }
     case 'U': {
       arguments->ui_port = atoi(arg);
       break;
     }
     case 'b': {
       arguments->bsize = atoi(arg);
       break;
     }
     case 'R': {
       arguments->rtime = atoi(arg);
       break;
     }
   }
   return 0;
}

int main (int argc, char **argv) {
   struct argp_option options[] = {
     { "discover_addr", 'd', "NUM", OPTION_ARG_OPTIONAL, "DISCOVER_ADDR"},
     { "ctrl_port", 'C', "NUM", OPTION_ARG_OPTIONAL, "CTRL_PORT"},
     { "ui_port", 'U', "NUM", OPTION_ARG_OPTIONAL, "UI_PORT"},
     { "bsize", 'b', "NUM", OPTION_ARG_OPTIONAL, "BSIZE"},
     { "rtime", 'R', "NUM", OPTION_ARG_OPTIONAL, "RTIME"}
   };
   struct arguments arguments;
   arguments.ctrl_port = CTRL_PORT;
   arguments.ui_port = UI_PORT;
   arguments.bsize = BSIZE;
   arguments.rtime = RTIME;

   struct argp argp = { options, parse_opt, 0, 0 };
   argp_parse(&argp, argc, argv, 0, 0, &arguments);
   printf("%d %d %d %d", arguments.ctrl_port, arguments.ui_port, arguments.bsize, arguments.rtime);
}
*/
