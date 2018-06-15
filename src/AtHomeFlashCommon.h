#ifndef ATHOMEFLASHCOMMON_H
# define ATHOMEFLASHCOMMON_H
# if !defined(__AVR__) && (defined(ESP8266) && !defined(ARDUINO))
#  define STRCMP    strcmp
#  define SNPRINTF  snprintf
#  define SSCANF    sscanf
#  ifndef PROGMEM
#   define PROGMEM
#  endif /* PROGMEM */
#  ifdef ARDUINO
#   include <WString.h>
#   ifdef __cplusplus
#    define FH(str)   reinterpret_cast<const __FlashStringHelper *>(str)
#   else
#    define FH(str)  ((const __FlashStringHelper *)(str))
#   endif /* __cplusplus */
#  else
#   define FH(str) str
#   define PGM_P const char *
#  endif /* ARDUINO */
# else
#  ifdef __AVR__
#   include <avr/pgmspace.h>
#   define STRCMP    strcmp_P
#   define SNPRINTF  snprintf_P
#   define SSCANF    sscanf_P
#  else
#   define STRCMP   strcmp
#   define SNPRINTF snprintf
#   define SSCANF   sscanf
#  endif
#  ifdef ARDUINO
#   include <WString.h>
#   ifdef __cplusplus
#    define FH(str)   reinterpret_cast<const __FlashStringHelper *>(str)
#   else
#    define FH(str)  ((const __FlashStringHelper *)(str))
#   endif /* __cplusplus */
#  endif /* ARDUINO */
# endif /* __AVR__ */
#endif /* ATHOMEFLASHCOMMON_H */
