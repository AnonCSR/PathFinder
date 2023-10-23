#pragma once

#if (defined(__GNUC__) && (__GNUC__ >= 3))  \
  || (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 800)) \
  || defined(__clang__)
#    define PF_likely(x) __builtin_expect(x, 1)
#    define PF_unlikely(x) __builtin_expect(x, 0)
#else
#    define PF_likely(x) (x)
#    define PF_unlikely(x) (x)
#endif