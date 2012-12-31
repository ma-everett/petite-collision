

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

  assert(verts);

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
  verts[ (numof - 2) ].x = 8.000f;
  verts[ (numof - 2) ].y = 4.5f;
  verts[ (numof - 2) ].z = 0.00f;
 
  verts[ (numof - 1) ].x = 9.00f;
  verts[ (numof - 1) ].y = 5.00f;
  verts[ (numof - 1) ].z = 0.00f;

  return verts;
}
  
#define NUMOFDYNAMICS 5
#define TOTALFRAMES 100
#define NUMOFVERTICES 500

int main(void) {
  
  size_t size = (1024 * 1024) * 5;
  void * memory = malloc(size); 
  pcol_session_t * session = pcol_createSession(size,memory,NUMOFDYNAMICS);
  
  vertex_t *verts = createVertices(NUMOFVERTICES);
 
  pcol_useDataBuffer( session, (void *)verts, NUMOFVERTICES, sizeof(vertex_t),0);
  
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
    hits = pcol_binaryCollisionCheckLimited(session,dyn,5);

    printf("[%03d] dyn.current = {%.2f,%.2f,%.2f\n",frames,
	   dyn->current.x,dyn->current.y,dyn->current.z);

    if (hits > 0) {
      printf("\t%d %s: x= %.2f, last good pos= %.2f\n",hits,(hits > 1) ? "collisions" : "collision",
	     dyn->future.x,dyn->current.x);
      
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
