#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

#ifndef VERSION
#  define VERSION *version unknown*
#endif
#define VERSION_STR STR(VERSION)
