#ifndef ERR_H
#define ERR_H

void syserr(const char *fmt, ...);

void fatal(const char *fmt, ...);

void syslogger(const char *fmt, ...);

#endif
