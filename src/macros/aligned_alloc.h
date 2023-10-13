#pragma once
#ifdef _MSC_VER
    #include <malloc.h>
#else
    #include <cstdlib>
#endif

#ifdef _MSC_VER
    #define PF_ALIGNED_ALLOC _aligned_malloc
#else
    #define PF_ALIGNED_ALLOC std::aligned_alloc
#endif

#ifdef _MSC_VER
    #define PF_ALIGNED_FREE _aligned_free
#else
    #define PF_ALIGNED_FREE free
#endif