
#ifndef DBCPPP_EXPORT_H
#define DBCPPP_EXPORT_H

#ifdef DBCPPP_STATIC_DEFINE
#  define DBCPPP_EXPORT
#  define DBCPPP_NO_EXPORT
#else
#  ifndef DBCPPP_EXPORT
#    ifdef dbcppp_EXPORTS
        /* We are building this library */
#      define DBCPPP_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define DBCPPP_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef DBCPPP_NO_EXPORT
#    define DBCPPP_NO_EXPORT 
#  endif
#endif

#ifndef DBCPPP_DEPRECATED
#  define DBCPPP_DEPRECATED __declspec(deprecated)
#endif

#ifndef DBCPPP_DEPRECATED_EXPORT
#  define DBCPPP_DEPRECATED_EXPORT DBCPPP_EXPORT DBCPPP_DEPRECATED
#endif

#ifndef DBCPPP_DEPRECATED_NO_EXPORT
#  define DBCPPP_DEPRECATED_NO_EXPORT DBCPPP_NO_EXPORT DBCPPP_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef DBCPPP_NO_DEPRECATED
#    define DBCPPP_NO_DEPRECATED
#  endif
#endif

#endif /* DBCPPP_EXPORT_H */
