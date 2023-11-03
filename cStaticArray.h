 /*

 ANSI C Static Array implementation
 
 This is a reinterpretation of cVector as a statically allocated template array class.
 All of the methods function same as cVector, differing in that they must be defined for
 every derived types, like a C++ template class.
 
 There is 3 macro definitions included in this header file:
 
 - #define cStaticArray(VALUE_TYPE, ARRAY_ALLOC)  :
   This is used to derive an array type with a 'typedef' statement.
   
 - #define cStaticArray_METHOD_DECLARATIONS(TYPENAME, VALUE_TYPE, ...)
   This is used to declare cStaticArray methods for derived array type. It can be stated in
   a header or source file.
   
 - #define cStaticArray_METHOD_DEFINITIONS(TYPENAME, VALUE_TYPE, ...)
   This is used to implement cStaticArray method definitions for derived map type. It should be
   stated in a source file.
   
 As an example, suppose we'd like to derive a class named 'IDArrayType'. For it, we'll create
 one header (IDArrayType.h) and one source (IDArrayType.c) file.
 
 IDArrayType.h :
 ------------------------------------------------------------------------------
 
 #ifndef ID_ARRAY_TYPE_H
 #define ID_ARRAY_TYPE_H
 
 #include <stdint.h>
 #include "cStaticArray.h"
 
 #define ID_ARRAY_VALUE_TYPE uint32_t
 #define ID_ARRAY_ALLOC      20
 
 typedef cStaticArray(ID_ARRAY_VALUE_TYPE, ID_ARRAY_ALLOC) IDArrayType;
 
 cStaticArray_METHOD_DECLARATIONS(IDArrayType, ID_ARRAY_VALUE_TYPE)
 
 #endif
 
 -------------------------------------------------------------------------------
 
 
 IDArrayType.c :
 ------------------------------------------------------------------------------
  
 #include "IDArrayType.h"
 
 cStaticArray_METHOD_DEFINITIONS(IDArrayType, ID_ARRAY_VALUE_TYPE)
 
 -------------------------------------------------------------------------------

 Authors: akozan
 
 Change Log:
 22.03.2019 first release
 ------------------------------------------------------------------------------------------------*/


#ifndef C_STATIC_ARRAY_H
#define C_STATIC_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/*This is the type definition macro of a template cStaticArray type*/
#define cStaticArray(VALUE_TYPE, ARRAY_ALLOC)\
    struct {\
        size_t arraySize;\
        VALUE_TYPE valueList[ARRAY_ALLOC];\
    }

/* Returns the number of elements in the array.
	\param me : cStaticArray instance pointer
	\return   : number of elements
size_t TYPENAME_size(const TYPENAME* me) */
    

/* Clears the array.
	\param me : cStaticArray instance pointer
	\return   : none.
void TYPENAME_clear(TYPENAME* me) */  
    
    
/* Returns the idx of given element.
	\param me : cStaticArray instance pointer
	\param elem 	: value of element.
	\retVal 		: the index of the element. if not found, returns the size of vector
size_t TYPENAME_find(TYPENAME* me, const VALUE_TYPE* elem) */

/* Adds new element to the index "idx".
	\param me : cStaticArray instance pointer
	\param newElem	: element to be added.
    \param idx  : the insertion index 
	\retVal		: result: 0 = Success, -1 = Failure
int TYPENAME_insert(TYPENAME* me, const VALUE_TYPE* newElem, const size_t idx) */

/* Deletes the element at the index "idx".
	\param me : cStaticArray instance pointer
	\param idx 		: index value.
	\retVal 		: result: 0 = Success, -1 = Failure
int TYPENAME_eraseAt(TYPENAME* me, const size_t idx) */
   

/* Deletes the element given.
	\param instance : cStaticArray instance pointer
	\param elem 	: element value.
	\retVal 		: result: 0 = Success, -1 = Failure
int TYPENAME_erase(TYPENAME* me, const VALUE_TYPE* elem) */

/* Adds new element to the start of the array.
	\param me : cStaticArray instance pointer
	\param newElem	: element to be added.
	\retVal 		: result: 0 = Success, -1 = Failure
int TYPENAME_pushf(TYPENAME* me, const VALUE_TYPE* newElem)*/     

/* Clears the element at the start of the vector.
	\param me : cStaticArray instance pointer	
	\retVal   : result: 0 = Success, -1 = Failure
int TYPENAME_popf(TYPENAME* me) */
    
/* Adds new element to the end of the vector.
	\param me : cStaticArray instance pointer
	\param newElem	: element to be added.
	\retVal 		: result: 0 = Success, -1 = Failure
int TYPENAME_pushb(TYPENAME* me, const VALUE_TYPE* newElem) */

/* Clears the element at the end of the vector.
	\param me : cStaticArray instance pointer
	\retVal   : result: 0 = Success, -1 = Failure
int TYPENAME_popb(TYPENAME* me) */
   
    
/*This macro is used to make function declarations 
 *of a concrete cStaticArray type.
 */
#define cStaticArray_METHOD_DECLARATIONS(TYPENAME, VALUE_TYPE, ...)\
\
__VA_ARGS__ size_t TYPENAME##_size(const TYPENAME* const me);\
\
__VA_ARGS__ void TYPENAME##_clear(TYPENAME* const me);\
\
__VA_ARGS__ size_t TYPENAME##_find(const TYPENAME* const me, const VALUE_TYPE* const elem);\
\
__VA_ARGS__ int TYPENAME##_insert(TYPENAME* const me, const VALUE_TYPE* const newElem, const size_t idx);\
\
__VA_ARGS__ int TYPENAME##_eraseAt(TYPENAME* const me, const size_t idx);\
\
__VA_ARGS__ int TYPENAME##_erase(TYPENAME* const me, const VALUE_TYPE* const elem);\
\
__VA_ARGS__ int TYPENAME##_pushf(TYPENAME* const me, const VALUE_TYPE* const newElem);\
\
__VA_ARGS__ int TYPENAME##_popf(TYPENAME* const me);\
\
__VA_ARGS__ int TYPENAME##_pushb(TYPENAME* const me, const VALUE_TYPE* const newElem);\
\
__VA_ARGS__ int TYPENAME##_popb(TYPENAME* const me);

    
/*This macro is used to make function definitions 
 *of a concrete cStaticArray type.
 */    

#define cStaticArray_METHOD_DEFINITIONS(TYPENAME, VALUE_TYPE, ...)\
\
__VA_ARGS__ size_t TYPENAME##_size(const TYPENAME* me)\
{\
    return me->arraySize;\
}\
\
__VA_ARGS__ void TYPENAME##_clear(TYPENAME* const me)\
{\
    me->arraySize = 0;\
}\
\
__VA_ARGS__ size_t TYPENAME##_find(const TYPENAME* const me, const VALUE_TYPE* const elem)\
{\
    size_t retVal = 0;\
    for(; retVal < me->arraySize; ++retVal)\
    {\
        if(me->valueList[retVal] == *elem)\
        {\
            break;\
        }\
    }\
    return retVal;\
}\
\
__VA_ARGS__ int TYPENAME##_insert(TYPENAME* const me, const VALUE_TYPE* const newElem, const size_t idx)\
{\
    int retVal = -1;\
    if(me->arraySize < ((sizeof(me->valueList)) / (sizeof(me->valueList[0]))))\
    {\
        if(idx <= me->arraySize)\
        {\
        	size_t revIdx = me->arraySize;\
        	for(;revIdx > idx; --revIdx)\
			{\
			   me->valueList[revIdx] = me->valueList[revIdx - 1];\
			}\
            me->valueList[idx] = *newElem;\
            ++(me->arraySize);\
            retVal = 0;\
        }\
    }\
    return retVal;\
}\
\
__VA_ARGS__ int TYPENAME##_eraseAt(TYPENAME* const me, const size_t idx)\
{\
    int retVal = -1;\
    if(me->arraySize > 0)\
    {\
        if(idx < me->arraySize)\
        {\
            size_t valIdx = idx + 1;\
            for(; valIdx < me->arraySize; ++valIdx)\
            {\
               me->valueList[valIdx - 1] = me->valueList[valIdx];\
            }\
            --(me->arraySize);\
            retVal = 0;\
        }\
    }\
    return retVal;\
}\
\
__VA_ARGS__ int TYPENAME##_erase(TYPENAME* const me, const VALUE_TYPE* const elem)\
{\
    size_t index = TYPENAME##_find(me, elem);\
    return TYPENAME##_eraseAt(me, index);\
}\
\
__VA_ARGS__ int TYPENAME##_pushf(TYPENAME* const me, const VALUE_TYPE* const newElem)\
{\
    return TYPENAME##_insert(me, newElem, (size_t)0);\
}\
\
__VA_ARGS__ int TYPENAME##_popf(TYPENAME* const me)\
{\
    return TYPENAME##_eraseAt(me, (size_t)0);\
}\
\
__VA_ARGS__ int TYPENAME##_pushb(TYPENAME* const me, const VALUE_TYPE* const newElem)\
{\
    return TYPENAME##_insert(me, newElem, me->arraySize);\
}\
\
__VA_ARGS__ int TYPENAME##_popb(TYPENAME* const me)\
{\
    return TYPENAME##_eraseAt(me, (me->arraySize - 1));\
}
    

#ifdef __cplusplus
}
#endif

#endif
