

/* main.c */
#include <stdlib.h>
#include "petitecollision.h"

typedef struct {

  float x,y,z;
  float r,g,b,a;
} vertex_t;


vertex_t * createVertices(int numof) {

  vertex_t *verts = (vertex_t *)malloc(sizeof(vertex_t) * numof);

  float n = 1.0 / (float)numof;
  int i = 0;
  for (; i < numof; i++) {

    vertex_t * v = &verts[i];
    v->x = n * (float)i;
    v->y = n * (float)i;
    v->z = n * (float)i;
    v->r = 0.88f;
    v->g = 0.88f;
    v->b = 0.88f;
    v->a = 0.88f;
  }

  return verts;
}
  
#define NUMOFDYNAMICS 5

int main(void) {
  
  size_t size5mb = (1024 * 1024) * 5;
  void * memory = malloc(size5mb); 
  pcol_session_t * session = pcol_createSession(size5mb,memory,NUMOFDYNAMICS);
  
  vertex_t *verts = createVertices(50);
 
  pcol_useDataBuffer( session, (void *)verts, 50, sizeof(float)*4,0);
  

  





  free(verts);
  pcol_destroySession(session);
  free(memory);
  return 0;
}
