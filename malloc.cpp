// Name: Andrew Chou
// Class: CSS430
// ------------------------------------------malloc.cpp==-------------------------------------------------
// This malloc.cpp implement the first fit, best fit and free memory allocation implementation. This class
//    utilizes heap_top, heap_end pointer to keep track of the storage of the MCB object, and utilizes
//    sbrk() to request storage from the OS to place the process in.
//      cur_mcb->size is the total size of each free block we traverse through
//      size parameters is the process size that we wish to allocate in the MCB free block
//      sizeof(MCB) is the storage to store the data for "available" and "size" data member for MCB object. 
//
// -------------------------------------------------------------------------------------------------------


#include <unistd.h> // sbrk( )
#include <limits.h> // LONG_MAX

using namespace std;

static bool initialized = false;
static void *heap_top; // the beginning of the heap space
static void *heap_end; // the current boundary of the heap space, obtained from sbrk( 0 )

class MCB { // memory control block
public:
  int available; // true(1): this memory partition is available, false(0) unavailalbe.
  int size;      // MCB size + the user data size
};

void free_( void *dealloc_space ) {
  MCB *mcb;
  // locate this partition's mcb address from dealloc_space
  // Task 4: implement by yourself (just in one line).
  mcb= (MCB *)((unsigned long long int)dealloc_space - sizeof(MCB));
  mcb->available = 1;
  return;
}

void *malloc_f( long size ) {
  struct MCB *cur_mcb;          // current MCB
  void *new_space = NULL; // this is a pointer to a new memory space allocated for a user

  if( !initialized )   {
    // find the end of heap memory, upon an initialization
    heap_end = sbrk( 0 );
    heap_top = heap_end;
    initialized = true;
  }

  // append an MCB in front of a requested memroy space
  size = size + sizeof( MCB );

  // scan each mcb from the top to the bottom of the heap
  for(void* cur = heap_top; cur < heap_end; cur = (void *)((unsigned long long int)cur + cur_mcb->size)){
  //   let cur_mcb point to each mcb you are scanning
    cur_mcb = (MCB* ) cur;
  //   if cur_mcb->available and cur_mcb->size fits size, new_space points to this mcb
    // if the cur block is available and the cur_mcb->size (current avail block size) >= size
    if(cur_mcb->available == 1 && cur_mcb->size >= size){
  // Task 1: implement by yourself (up to 15 lines).
      // set available to unavailable (we are occupying the block)
      cur_mcb->available = 0;
      // return the pointer to new_space (to use the space)
      new_space = cur_mcb;
      break;
    }
  }

  // no space found yet
  if ( new_space == NULL ) {
    // get a space from OS
    sbrk(size);
    // old boundary now becomes new_space, i.e. initialize new_space with heap_end
    new_space = heap_end;
    // heap_end will go down by size
    // increment by size
    heap_end = (void *)((unsigned long long int) heap_end + size);
    // initialize cur_mcb with new_space and size.
    cur_mcb = (MCB* )new_space;
    // occupying that new space being requested from OS
    cur_mcb->available = 0;
    // the total size of free block is the same as size we want to allocate
    cur_mcb->size = size;
    // Task 2: implement by yourself (up to 5 lines).
  }

  // new space is after new MCB
  return (void *)( ( long long int )new_space + sizeof( MCB ) );
}

void *malloc_b( long size ) {
  struct MCB *cur_mcb;          // current MCB
  void *new_space = NULL; // this is a pointer to a new memory space allocated for a user
  // pointer to keep track of current best location
  MCB *best_space;

  if( !initialized )   {
    // find the end of heap memory, upon an initialization
    heap_end = sbrk( 0 );
    heap_top = heap_end;
    initialized = true;
  }

  // append an MCB in front of a requested memroy space
  size = size + sizeof( MCB );

  // scan each mcb from the top to the bottom of the heap
  for(void* cur = heap_top; cur < heap_end; cur = (void *)((unsigned long long int)cur + cur_mcb->size)){
  //   let cur_mcb point to each mcb you are scanning
    cur_mcb = (MCB *)cur;
    // storage to keep track of smallest remainder space (cur_mcb->size - size)
    // initialize this to be the max number
    long bestCounter = LONG_MAX;
  //   if cur_mcb->available and cur_mcb->size fits size and cur_mcb->size is the best size so far
    if(cur_mcb->available == 1 && cur_mcb->size >= size){
  //  temporarily memorize this best size so far and this best mcb so far
      // continuously comparing for the smallest until reaching the end of MCB stacks
      // if the remainder is smaller than counter,
      if((cur_mcb->size - size) < bestCounter){
        // update counter
        bestCounter = cur_mcb->size - size;
        // memorize current best location (with smallest wasted space = bestCounter)
        best_space = cur_mcb;
      }
    }
  }
  // at this point, best_space point to the part that hold smallest wasted space
  // After scan, check the best mcb so far. If it is not null
  if(best_space != NULL){
    // occupying the place
    best_space->available = 0;
    // new_space points to this best mcb so rar
    new_space = best_space;
  }
  // Task 3: implement by yourself (up to 20 lines).
  
  // no space found yet
  if ( new_space == NULL ) {
    // The same as Task 2
    // get a space from OS
    sbrk(size);
    // old boundary now becomes new_space, i.e. initialize new_space with heap_end
    new_space = heap_end;
    // heap_end will go down by size
    // increment by size
    heap_end = (void *)((unsigned long long int) heap_end + size);
    // initialize cur_mcb with new_space and size.
    cur_mcb = (MCB* )new_space;
    // occupying that new space being requested from OS
    cur_mcb->available = 0;
    // the total size of free block is the same as size we want to allocate
    cur_mcb->size = size;
    // Task 3': Just cut and past the logic from malloc_f to here.
  }

  // new space is after new MCB
  return (void *)( ( long long int )new_space + sizeof( MCB ) );
}
