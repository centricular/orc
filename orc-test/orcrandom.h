
#ifndef _ORC_RANDOM_H_
#define _ORC_RANDOM_H_

#include <orc/orcutils.h>

ORC_BEGIN_DECLS

typedef struct _OrcRandomContext OrcRandomContext;
struct _OrcRandomContext {
  unsigned int x;
};

ORC_EXPORT void orc_random_init (OrcRandomContext *context, int seed);
ORC_EXPORT void orc_random_bits (OrcRandomContext *context, void *data, int n_bytes);
ORC_EXPORT void orc_random_floats (OrcRandomContext *context, float *data, int n);
ORC_EXPORT unsigned int orc_random (OrcRandomContext *context);

ORC_END_DECLS

#endif

