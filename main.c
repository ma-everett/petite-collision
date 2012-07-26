

/* main.c */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
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
    v->x = (n * (float)i) + 0.01f;
    v->y = (n * (float)i) + 0.01f;
    v->z = (n * (float)i) + 0.01f;
    v->r = 0.88f;
    v->g = 0.88f;
    v->b = 0.88f;
    v->a = 0.88f;
  }

  verts[ (numof - 1) ].x = 9.00f;
  verts[ (numof - 1) ].y = 5.00f;
  verts[ (numof - 1) ].z = 0.00f;

  return verts;
}
  
#define NUMOFDYNAMICS 5
#define TOTALFRAMES 100

int main(void) {
  
  size_t size5mb = (1024 * 1024) * 5;
  void * memory = malloc(size5mb); 
  pcol_session_t * session = pcol_createSession(size5mb,memory,NUMOFDYNAMICS);
  
  vertex_t *verts = createVertices(5);
 
  pcol_useDataBuffer( session, (void *)verts, 5, sizeof(vertex_t),0);
  
  /* grab a dynamic to use */
  pcol_dynamic_t * dyn = pcol_receiveDynamic(session);
  assert(dyn);


  dyn->current.x = 0.0f;
  dyn->current.y = 5.0f;
  dyn->current.z = 0.0f;
  dyn->future = dyn->current;

  dyn->radius = 3.0f;
  dyn->probeRadius = 20.0f; /* the local area to check, ideally this is dependant on speed */

  int frames = 0;
  int hits = 0;
  for (;frames < TOTALFRAMES; frames++) {

    /* update the dynamics position : */
    dyn->future.x += 0.1f;
    /* check for collisions */
    hits = pcol_binaryCollisionCheck(session,dyn);
    if (hits > 0) {
      printf("collision: x= %.2f, last good pos= %.2f\n",dyn->future.x,dyn->current.x);
      dyn->future = dyn->current;
    }       


    dyn->current = dyn->future;
  }

  printf("end %.2f %.2f %.2f\n",dyn->current.x,dyn->current.y,dyn->current.z);

  pcol_beridDynamic(dyn);


  free(verts);
  pcol_destroySession(session);
  free(memory);
  return 0;
}
