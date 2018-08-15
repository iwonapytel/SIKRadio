#ifndef ERR_H
#define ERR_H

void syserr(const char *fmt, int err, ...);

void fatal(const char *fmt, ...);

void syslog(const char *fmt, int err, ...);

#endif
