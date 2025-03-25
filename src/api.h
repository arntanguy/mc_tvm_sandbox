#pragma once

#if defined _WIN32 || defined __CYGWIN__
#  define TVMSandbox_DLLIMPORT __declspec(dllimport)
#  define TVMSandbox_DLLEXPORT __declspec(dllexport)
#  define TVMSandbox_DLLLOCAL
#else
// On Linux, for GCC >= 4, tag symbols using GCC extension.
#  if __GNUC__ >= 4
#    define TVMSandbox_DLLIMPORT __attribute__((visibility("default")))
#    define TVMSandbox_DLLEXPORT __attribute__((visibility("default")))
#    define TVMSandbox_DLLLOCAL __attribute__((visibility("hidden")))
#  else
// Otherwise (GCC < 4 or another compiler is used), export everything.
#    define TVMSandbox_DLLIMPORT
#    define TVMSandbox_DLLEXPORT
#    define TVMSandbox_DLLLOCAL
#  endif // __GNUC__ >= 4
#endif // defined _WIN32 || defined __CYGWIN__

#ifdef TVMSandbox_STATIC
// If one is using the library statically, get rid of
// extra information.
#  define TVMSandbox_DLLAPI
#  define TVMSandbox_LOCAL
#else
// Depending on whether one is building or using the
// library define DLLAPI to import or export.
#  ifdef TVMSandbox_EXPORTS
#    define TVMSandbox_DLLAPI TVMSandbox_DLLEXPORT
#  else
#    define TVMSandbox_DLLAPI TVMSandbox_DLLIMPORT
#  endif // TVMSandbox_EXPORTS
#  define TVMSandbox_LOCAL TVMSandbox_DLLLOCAL
#endif // TVMSandbox_STATIC