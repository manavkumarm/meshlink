bin_PROGRAMS = meshlinkapp chat chatpp manynodes channels channelsaio

AM_CPPFLAGS = -I../include -I../src

meshlinkapp_SOURCES = meshlinkapp.c
meshlinkapp_LDADD = ../src/libmeshlink.la ../catta/src/libcatta.la

chat_SOURCES = chat.c
chat_LDADD = ../src/libmeshlink.la ../catta/src/libcatta.la

chatpp_SOURCES = chatpp.cc
chatpp_LDADD = ../src/libmeshlink.la ../catta/src/libcatta.la

manynodes_SOURCES = manynodes.cc
manynodes_LDADD = ../src/libmeshlink.la ../catta/src/libcatta.la

channels_SOURCES = channels.c
channels_LDADD = ../src/libmeshlink.la ../catta/src/libcatta.la

channelsaio_SOURCES = channelsaio.c
channelsaio_LDADD = ../src/libmeshlink.la ../catta/src/libcatta.la
