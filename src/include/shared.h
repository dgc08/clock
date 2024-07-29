#ifndef SHARED_H_
#define SHARED_H_

#define help_text \
    "Clock - A terminal-based alarm clock and timer\n"                               \
    "\nUsage: %s {flags} {command} -- h.m.s"                                         \
    "\nCommands: "                                                                   \
    "\n      clock           Display the time."                                      \
    "\n                      No '-- h.m.s' needed."                                  \
    "\n      alarm           Play alarm at time 'h.m.s'"                             \
    "\n      timer           Play alarm in 'h.m.s'\n"                                \
    "\nFlags:"                                                                       \
    "\n     -a={filename}    Alternative alarm sound             Default=alarm.ogg"  \
    "\n     -b={program}     Alternative sound player            Default=ffplay"     \
    "\n     -n               Use newlines when "                                     \
    "\n                      redisplaying the remaining time     Default=off\n"      \
    "\n     -c               Use spaces when "                                       \
    "\n                      redisplaying the remaining time     Default=off\n"      \
    "\n     -d               Directly open the audio player"                         \
    "\n                      instead of starting a new process   Default=off\n"      \
    "\n"

#endif // SHARED_H_
