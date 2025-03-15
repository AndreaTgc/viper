#ifndef guard_utils_h
#define guard_utils_h

#if ALLOW_FORCE_INLINING == 1
#ifdef _MSC_VER
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE __attribute__((always_inline))
#endif
#else
#define FORCE_INLINE
#endif // ALLOW_FORCE_INLINING

#endif // guard_utils_h
