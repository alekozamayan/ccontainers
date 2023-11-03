 /*
 ANSI C Linear vector implementation
 
 It is created as an alternative to C++ STL <vector> container class.
 Some template methods in C++ <vector> which utilize Value type are alternatively implemented
 using void* arguments and cVector.elemSize element.
 
 NOTE: Since cVector allocates elements in heap, it should be deallocated by using "clear" method at
 the end of the scope, no matter if cVector is created on stack. Since this is a struct implementation
 and won't be destructed automatically while returning from the scope,
 the responsibility of destruction of the object is on the user. 

 Authors: akozan
 
 Change Log:
 22.03.2019 first release
 ------------------------------------------------------------------------------------------------*/


#ifndef CVECTOR_H
#define CVECTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef struct cVectorType cVector;

/*cVector type. 
 NOTE: the implementation is not 
 concealed to make it able to allocate on
 stack (not the array, only the struct itself). 
 These members are intended to be private.
 DO NOT MODIFY THE VALUES DIRECTLY!*/
struct cVectorType{
	 /*size of the element type in bytes*/
     size_t elemSize;
     /*aligned elemSize in bytes*/
     size_t elemSizeAligned;
	 /*number of the elements*/
     size_t vectSize;
	 /*vector allocation size, in terms of elements*/
     size_t allocSize;
	 /*dynamic array of the recorded elements*/
     void* array;
};

/* Returns the element at the index "idx".
	\param instance : cVector instance pointer
	\param idx 		: index value.
	\return 		: the element at the index "idx". if the index is invalid, returns NULL*/
void* 	cVector_getAt(cVector* pInstance, const size_t idx);

/* Returns the number of elements in the vector.
	\param instance : cVector instance pointer
	\return 		: number of elements*/
size_t 	cVector_size(const cVector* pInstance);

/* Clears the vector.
	\param instance : cVector instance pointer
	\return 		: none.*/
void 	cVector_clear(cVector* pInstance);

/* Returns the idx of given element.
	\param instance : cMap instance pointer
	\param elem 	: pointer of the element.
	\return 		: the index of the element. if not found, returns the size of vector*/
size_t 	cVector_find(cVector* pInstance, const void* elem);

/* Adds new element to the index "idx".
	\param instance : cVector instance pointer
	\param newElem	: pointer of the element to be added.
	\return 		: result: 0 = Success, -1 = Failure*/
int		cVector_insert(cVector* pInstance, const void* newElem, const size_t idx);


/* Deletes the element at the index "idx".
	\param instance : cVector instance pointer
	\param idx 		: index value.
	\return 		: result: 0 = Success, -1 = Failure*/
int 	cVector_eraseAt(cVector* pInstance, const size_t idx);

/* Deletes the element given.
	\param instance : cVector instance pointer
	\param elem 	: pointer of the element.
	\return 		: result: 0 = Success, -1 = Failure*/
int     cVector_erase(cVector* pInstance, const void* elem);   

/* Macro Extensions */
/*---------------------------------------------------------------------------*/
/* Adds new element to the start of the vector.
	\param instance : cVector instance pointer
	\param newElem	: pointer of the element to be added.
	\return 		: result: 0 = Success, -1 = Failure*/
#define cVector_pushf(pInstance, newElem)\
        cVector_insert(pInstance, newElem, (size_t)0)

/* Clears the element at the start of the vector.
	\param instance : cVector instance pointer	
	\return 		: result: 0 = Success, -1 = Failure*/
#define cVector_popf(pInstance)\
        cVector_eraseAt(pInstance, (size_t)0)
    
/* Adds new element to the end of the vector.
	\param instance : cVector instance pointer
	\param newElem	: pointer of the element to be added.
	\return 		: result: 0 = Success, -1 = Failure*/
#define cVector_pushb(pInstance, newElem)\
        cVector_insert(pInstance, newElem, (pInstance)->vectSize)

/* Clears the element at the end of the vector.
	\param instance : cVector instance pointer	
	\return 		: result: 0 = Success, -1 = Failure*/
#define cVector_popb(pInstance)\
        cVector_eraseAt(pInstance, ((pInstance)->vectSize - 1))
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*This function constructs an allocated cVector object. Need to call after
  the creation of object.
  \param instance 	: allocated cVector pointer to be constructed
  \param elemSize 	: size of the element type	
  \return		  	: none*/
void concreteConstructCVector(cVector* instance, size_t elemSize);

/*This is a macro wrapper for "concreteConstructCVector" function, provides creation
using typenames. (C++ template logic)*/
#define constructCVector(instance, TYPE)  concreteConstructCVector(instance, sizeof(TYPE))
/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
