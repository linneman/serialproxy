bin_PROGRAMS=serialproxy
serialproxy_SOURCES= \
	serialproxy.c \
	serialproxy.h \
	proxy.c \
	proxy.h \
	control.c \
	control.h \
	strutils.c \
	strutils.h \
	sp_config.c \
	sp_config.h \
	sp_log.c \
	sp_log.h \
	revision.c \
	revision.h \
	common.h
serialproxy_LDFLAGS = -lpthread $(libintercom_LIBS)
serialproxy_CPPFLAGS = $(libintercom_CFLAGS)
