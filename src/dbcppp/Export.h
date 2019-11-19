
#pragma once

#ifdef _WIN32
#   define DBCPPP_EXPORT  __declspec(dllexport)
#else
#   define DBCPPP_EXPORT
#endif
