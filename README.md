man(1)                                     sncmpc man page                                     man(1)

NAME
       sncmpc - simple ncurses music player client

SYNOPSIS
       sncmpc [OPTION]...

DESCRIPTION
       sncmpc is a small, efficient and simple ncurses client for mpd.

OPTIONS
       -h, --host=HOST
              Connect to HOST instead of host defined in config.h

       -p, --port=PORT
              Connect to PORT instead of port defined in config.h

       -t, --timeout=TIMEOUT
              Use TIMEOUT ms as timeout instead of timeout defined in config.h

       -?, --help
              Display a help message and exit

       --usage
              Display a short usage message and exit

       -V, --version
              Display program version and exit

EXIT STATUS
       0      if OK,

       1      if connection to mpd could not be made,

       2      if arguments or environment variables have illegal values (such as $SNCMPC_PORT=-1).

ENVIRONMENT
       All  environment  variables  are overridden by respective command line options but do override
       the defaults specified in config.h. If environment variables contain illegal values  an  error
       is printed to stderr followed by the program exiting with status 2.

       SNCMPC_HOST
              Connect to specified host. See option --host.

       SNCMPC_PORT
              Connect with specified port. See option --port.

SEE ALSO
       mpc(1), mpd(1), ncmpcpp(1)

0.1.0                                        15 Nov 2018                                       man(1)
