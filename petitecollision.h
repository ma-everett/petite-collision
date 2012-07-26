
#ifndef PETITE_COLLISION_H
#define PETITE_COLLISION_H 1

/* 0. take a ptr to a polygonal soup structure. 
 * 1. per dynamic entity (player for instance) check for local space collisions
 * 2. take all polygons in local space and convert to point cloud
 * 3. do fast collision detection against point cloud. 
 * 4. as local area changes (or polygon soup changes) rebuild point cloud
 * --- aim for speed and very low memory usage. Should be fixed from start. 
 * --- there will be no external requirements
 * features I need: fast, small memory requirements,dynamic and simple
 * I should be able to add user data to collisions, points and entities also
 * a hinting system for tuning highly collision likely areas. 
 * A reduce system for removing non-viable points would be nice but not important.
 * A taskable query system. Decent debug and memory useage -> dot syntax outputer code
 */

#include <stdlib.h>

typedef struct {

  void * data;
  size_t start;
  size_t interval;
  int numof;

} pcol_buffer_t;

typedef struct {

  float x,y,z; 
} pcol_point_t;


typedef struct {
  
  pcol_point_t future; /* possible position */
  pcol_point_t current; /* good position */

  float radius; /* if < 0.0 then not in use */
  float probeRadius;

} pcol_dynamic_t;

typedef struct {

  pcol_dynamic_t *points;
  int numof;

} pcol_dynamicArray_t;

typedef struct {

  pcol_buffer_t buffer;

  pcol_dynamicArray_t * dynamics;

  size_t size;
} pcol_session_t;

pcol_session_t * pcol_createSession(size_t memorysize, void * memory, int numofdynamics);
void pcol_destroySession(pcol_session_t * session);

void pcol_useDataBuffer(pcol_session_t * session, 
			void * data, int numof, size_t interval,size_t start);


pcol_dynamic_t * pcol_receiveDynamic(pcol_session_t * session);
void pcol_beridDynamic(pcol_dynamic_t * dynamic);

/* binary collision checks */

int pcol_binaryCollisionCheck(pcol_session_t * session, pcol_dynamic_t * dynamic);




#endif
