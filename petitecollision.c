

#include "petitecollision.h"
#include <stdio.h>

/* TODO: this should be in the client not in the library! */
void *_palloc(void * ptr,size_t size,char *file, int line) {

  if (ptr != 0) {
    printf("%s:%d free %d\n",file,line,(int)ptr);
    return 0;
  }
  
  void *m = malloc(size);
  printf("%s:%d malloc %d %dbytes\n",file,line,size,(int)m);
  return m;
}

#define MALLOC(s) _palloc(0,(s),__FILE__,__LINE__)
#define FREE(p) _palloc((p),0,__FILE__,__LINE__)


pcol_session_t * pcol_createSession(size_t memorysize, void * memory,int numofdynamics) {
  
  pcol_session_t * session = (pcol_session_t *)memory;
  session->size = memorysize;

  /* FIXME: this is just a cheat for the moment */
  session->dynamics = (pcol_dynamicArray_t *)MALLOC(sizeof(pcol_dynamicArray_t));
  session->dynamics->numof = numofdynamics;
  session->dynamics->points = (pcol_dynamic_t *)MALLOC(sizeof(pcol_dynamic_t) * numofdynamics);

  int i = 0;
  for (; i < session->dynamics->numof; i++) {

    pcol_dynamic_t *p = &session->dynamics->points[i];
    p->current.x = p->current.y = p->current.z = 0.0f;
    p->future = p->current;
    p->probeRadius = 0.0f;
    p->radius = -1.0f; /* not in use */
  }

  printf("%s session created\n",__FUNCTION__);

  return session;
}


void pcol_destroySession(pcol_session_t * session) {

  FREE(session->dynamics->points);
  FREE(session->dynamics);
}


void pcol_useDataBuffer(pcol_session_t * session, 
			void * data, int numof, size_t interval, size_t start) {

  session->buffer.data = data;
  session->buffer.numof = numof;
  session->buffer.interval = interval;
  session->buffer.start = start;
}

pcol_dynamic_t * pcol_receiveDynamic(pcol_session_t * session) {

  pcol_dynamic_t * p = 0;
  
  int i = 0;
  for (; i < session->dynamics->numof; i++) {

    p = &session->dynamics->points[i];
    if (p->radius < 0.00f) {
      
      p->radius = 0.00f;
      return p;
    }
    
  }

  printf("%s dynamic not found\n",__FUNCTION__);

  return 0;
}

void pcol_beridDynamic(pcol_dynamic_t * dynamic) {

  dynamic->radius = -1.0f;
}

int intersect_RaySphere(pcol_point_t *origin, pcol_point_t *future,
			pcol_point_t *centre, float radius) {

  /* based on RaySphereInteresect from `Real-Time Rendering` pg.299 */

  return 0;
}

int within_sphere(pcol_point_t *origin, pcol_point_t *target, float radius) {
  
  float x = origin->x - target->x;
  float y = origin->y - target->y;
  float z = origin->z - target->z;
  
  /*
  printf("%s\n\torigin=%.2f,%.2f,%.2f\n\ttarget=%.2f,%.2f,%.2f\n",__FUNCTION__,
	 origin->x,origin->y,origin->z,
	 target->x,target->y,target->z);
  */

  return ((x * x) + (y * y) + (z * z) <= (radius * radius));
}
 

/* this is for development really don't use in production! */
int pcol_binaryCollisionCheck(pcol_session_t * session, pcol_dynamic_t * dynamic) {

  /* go through all the points and look for collisions : */
  int i = 0;
  int hits = 0;
  pcol_point_t * p = (pcol_point_t *)(session->buffer.data + session->buffer.start);

  //printf("ptr=%d, p=%d, interval=%d\n",(int)session->buffer.data,(int)p,session->buffer.interval);
  //printf("\tx=%.2f,y=%.2f,z=%.2f\n",p->x,p->y,p->z);

  for (; i < session->buffer.numof; i++) {

    hits += within_sphere(&dynamic->future,p,dynamic->radius);

    p = (pcol_point_t *)((void *)p + session->buffer.interval);
    
  }

  return hits;
}

int pcol_binaryCollisionCheckLimited(pcol_session_t *session, pcol_dynamic_t *dynamic,int limit) {

  int i=0;
  int hits = 0;
  pcol_point_t * p = (pcol_point_t *)(session->buffer.data + session->buffer.start);

  for (; i < session->buffer.numof; i++) {

    hits += within_sphere(&dynamic->future,p,dynamic->radius);

    p = (pcol_point_t *)((void *)p + session->buffer.interval);
    
    if (hits >= limit)
      break;
  }

  return hits;
}

