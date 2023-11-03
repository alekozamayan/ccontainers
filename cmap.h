/*
 ANSI C Linear map implementation
 
 It is created as an alternative to C++ STL <map> container class. Elements are allocated in linear
 manner instead of tree, to make all located in the same possible cache region.
 Some template methods in C++ <map> which utilize Key and Value types are alternatively implemented
 using void* arguments and cMap.keySize, cMap.valueSize elements.
 
 NOTE: Since cMap allocates elements in heap, it should be deallocated by using "clear" method at
 the end of the scope, no matter if cMap is created on stack. Since this is a struct implementation,
 the responsibility of destruction of the object is on the user. 

 Authors: akozan
 
 Change Log:
 22.03.2019 first release
 ------------------------------------------------------------------------------------------------*/


#ifndef CMAP_H
#define CMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>


/*cPair type, used to contain key-value bindings
  in the map.*/
typedef struct {
	void* first;
	void* second;
} cPair;	

typedef struct cMapType cMap;

/*cMap type. 
 NOTE: the implementation is not 
 concealed to make it able to allocate on
 stack (not the pairArray, only the struct itself). 
 These members are intended to be private.
 DO NOT MODIFY THE VALUES DIRECTLY!*/
struct cMapType{
	 /*size of the key type in bytes*/
     size_t keySize;
     /*aligned keySize in bytes*/
     size_t keySizeAligned;
	 /*size of the value type in bytes*/
     size_t valueSize;
     /*aligned valueSize in bytes*/
     size_t valueSizeAligned;
     /*size of a map element in bytes*/
     size_t elemSize;
     /*number of the elements*/
     size_t mapSize;
     /*map allocation size, in terms of elements*/
     size_t allocationSize;
     /*dynamic array of the recorded pair elements*/
     void* pairArray;
};

/* Returns the pair at the index "idx".
	\param instance : cMap instance pointer
	\param idx 		: index value.
    \param pPair    : the pair at the index "idx"
	\return 		: result: 0 = Success, -1 = Failure*/
int 	cMap_getAt(cMap* pInstance, const size_t idx, cPair* pPair);

/* Returns the number of elements in the map.
	\param instance : cMap instance pointer
	\return 		: number of elements*/
size_t 	cMap_size(const cMap* pInstance);

/* Clears the map.
	\param instance : cMap instance pointer
	\return 		: none.*/
void 	cMap_clear(cMap* pInstance);

/* Returns the pair containing given key.
	\param instance : cMap instance pointer
	\param key 		: pointer of the key.
    \param pPair    : the pair containing given key
	\return 		: result: 0 = Success, -1 = Failure*/
int 	cMap_find(cMap* pInstance, const void* key, cPair* pPair);

/* Adds new pair to the end of the map.
	\param instance : cMap instance pointer
	\param newPair	: pointer of the pair to be added.
	\return 		: result: 0 = Success, -1 = Failure*/
int		cMap_insert(cMap* pInstance, const cPair* newPair);

/* Deletes the pair containing given key.
	\param instance : cMap instance pointer
	\param key 		: pointer of the key.
	\return 		: result: 0 = Success, -1 = Failure*/
int 	cMap_erase(cMap* pInstance, const void* key);


/*---------------------------------------------------------------------------*/
/*This function constructs an allocated cMap object. Need to call after
  the creation of object.
  \param instance 	: allocated cMap pointer to be constructed
  \param keySize 	: size of the key type	
  \param valueSize 	: size of the value type	
  \return		  	: none*/
void concreteConstructCMap(cMap* instance, size_t keySize, size_t valueSize);

/*This is a macro wrapper for "concreteConstructCMap" function, provides creation using
typenames. (C++ template logic)*/
#define constructCMap(instance, TYPE1, TYPE2)  concreteConstructCMap(instance, sizeof(TYPE1), sizeof(TYPE2))
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
