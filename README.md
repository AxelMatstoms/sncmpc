man {#man align="center"}
===

[NAME](#NAME)\
[SYNOPSIS](#SYNOPSIS)\
[DESCRIPTION](#DESCRIPTION)\
[OPTIONS](#OPTIONS)\
[EXIT STATUS](#EXIT%20STATUS)\
[ENVIRONMENT](#ENVIRONMENT)\
[SEE ALSO](#SEE%20ALSO)\

------------------------------------------------------------------------

NAME []{#NAME}
--------------

sncmpc − simple ncurses music player client

SYNOPSIS []{#SYNOPSIS}
----------------------

+-------------+-------------+-------------+-------------+-------------+
|             | **sncmpc**  |             | \[*OPTION*\ |             |
|             |             |             | ]\...       |             |
+-------------+-------------+-------------+-------------+-------------+

DESCRIPTION []{#DESCRIPTION}
----------------------------

sncmpc is a small, efficient and simple ncurses client for mpd.

OPTIONS []{#OPTIONS}
--------------------

**−h**, **−−host**=*HOST*

Connect to *HOST* instead of host defined in config.h

**−p**, **−−port**=*PORT*

Connect to *PORT* instead of port defined in config.h

**−t**, **−−timeout**=*TIMEOUT*

Use *TIMEOUT* ms as timeout instead of timeout defined in config.h

**−?**, **−−help**

Display a help message and exit

**−−usage**

Display a short usage message and exit

**−V**, **−−version**

Display program version and exit

EXIT STATUS []{#EXIT STATUS}
----------------------------

+-----------------+-----------------+-----------------+-----------------+
|                 | 0               |                 | if OK,          |
+-----------------+-----------------+-----------------+-----------------+
|                 | 1               |                 | if connection   |
|                 |                 |                 | to mpd could    |
|                 |                 |                 | not be made,    |
+-----------------+-----------------+-----------------+-----------------+
|                 | 2               |                 | if arguments or |
|                 |                 |                 | environment     |
|                 |                 |                 | variables have  |
|                 |                 |                 | illegal values  |
|                 |                 |                 | (such as        |
|                 |                 |                 | \$SNCMPC\_PORT= |
|                 |                 |                 | -1).            |
+-----------------+-----------------+-----------------+-----------------+

ENVIRONMENT []{#ENVIRONMENT}
----------------------------

All environment variables are overridden by respective command line
options but do override the defaults specified in config.h. If
environment variables contain illegal values an error is printed to
stderr followed by the program exiting with status 2. **\
SNCMPC\_HOST**

Connect to specified host. See option *−−host*.

**SNCMPC\_PORT**

Connect with specified port. See option *−−port*.

SEE ALSO []{#SEE ALSO}
----------------------

mpc(1), mpd(1), ncmpcpp(1)

------------------------------------------------------------------------
