

#include "petitecollision.h"
#include <stdio.h>

void *_palloc(void * ptr,size_t size,char *file, int line) {

  if (ptr != 0) {
    printf("%s:%d free memory\n",file,line);
    return 0;
  }
  
  printf("%s:%d alloc'd=%dbytes\n",file,line,size);
  return malloc(size);
}

#define MALLOC(s) _palloc(0,(s),__FILE__,__LINE__)
#define FREE(p) _palloc((p),0,__FILE__,__LINE__)


pcol_session_t * pcol_createSession(size_t memorysize, void * memory,int numofdynamics) {
  
  pcol_session_t * session = (pcol_session_t *)memory;
  session->size = memorysize;

  /* FIXME: this is just a cheat for the moment */
  session->dynamics = (pcol_dynamicArray_t *)MALLOC(sizeof(pcol_dynamicArray_t));
  session->dynamics->numof = numofdynamics;
  session->dynamics->used = 0;
  session->dynamics->points = (pcol_dynamic_t *)MALLOC(sizeof(pcol_dynamic_t) * numofdynamics);
  int i = 0;
  for (; i < session->dynamics->numof; i++) {

    pcol_dynamic_t *p = &session->dynamics->points[i];
    p->x = p->y = p->z = 0.0f;
    p->radius = -1.0f; /* not in use */
  }

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
    if (p->radius >= 0.00f) {
      
      p->radius = 0.00f;
      return p;
    }
    
  }

  return 0;
}

void pcol_beridDynamic(pcol_dynamic_t * dynamic) {

  dynamic->radius = -1.0f;
}
