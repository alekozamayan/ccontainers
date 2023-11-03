/*

 ANSI C Static Linear map implementation
 
 This is a reinterpretation of cMap as a statically allocated template map class.
 All of the methods function same as cMap, differing in that they must be defined for
 every derived types, like a C++ template class.
 
 There is 3 macro definitions included in this header file:
 
 - #define cStaticMap(KEY_TYPE, VALUE_TYPE, MAP_ALLOC)  :
   This is used to derive a map type with a 'typedef' statement.
   
 - #define cStaticMap_METHOD_DECLARATIONS(TYPENAME, KEY_TYPE, VALUE_TYPE, ...)
   This is used to declare cStaticMap methods for derived map type. It can be stated in
   a header or source file.
   
 - #define cStaticMap_METHOD_DEFINITIONS(TYPENAME, KEY_TYPE, VALUE_TYPE, ...)
   This is used to implement cStaticMap method definitions for derived map type. It should be
   stated in a source file.
   
 As an example, suppose we'd like to derive a class named 'IDMapType'. For it, we'll create
 one header (IDMapType.h) and one source (IDMapType.c) file.
 
 IDMapType.h :
 ------------------------------------------------------------------------------
 
 #ifndef ID_MAP_TYPE_H
 #define ID_MAP_TYPE_H
 
 #include <stdint.h>
 #include "cStaticMap.h"
 
 #define ID_MAP_KEY_TYPE   uint8_t
 #define ID_MAP_VALUE_TYPE uint32_t
 #define ID_MAP_ALLOC      20
 
 typedef cStaticMap(ID_MAP_KEY_TYPE, ID_MAP_VALUE_TYPE, ID_MAP_ALLOC) IDMapType;
 
 cStaticMap_METHOD_DECLARATIONS(IDMapType, ID_MAP_KEY_TYPE, ID_MAP_VALUE_TYPE)
 
 #endif
 
 -------------------------------------------------------------------------------
 
 
 IDMapType.c :
 ------------------------------------------------------------------------------
  
 #include "IDMapType.h"
 
 cStaticMap_METHOD_DEFINITIONS(IDMapType, ID_MAP_KEY_TYPE, ID_MAP_VALUE_TYPE)
 
 -------------------------------------------------------------------------------

 Authors: akozan
 
 Change Log:
 22.03.2019 first release
 ------------------------------------------------------------------------------------------------*/


#ifndef C_STATIC_MAP_H
#define C_STATIC_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/*This is the type definition macro of a template cStaticMap type*/
#define cStaticMap(KEY_TYPE, VALUE_TYPE, MAP_ALLOC) \
    struct {\
        size_t mapSize;\
        KEY_TYPE keyList[MAP_ALLOC];\
        VALUE_TYPE valueList[MAP_ALLOC];\
    }

    
/* Returns the number of elements in the map.
	\param me : cStaticMap instance pointer
	\return   : number of elements
size_t TYPENAME##_size(const TYPENAME* const me) */

/* Clears the map.
	\param me : cStaticMap instance pointer
	\return   : none.
void   TYPENAME##_clear(TYPENAME* const me) */

/* Returns the value index containing given key.
	\param me               : cStaticMap instance pointer
	\param key              : key value
    \param size_t valIdx    : index the value resides
    \result         : return value of function. 
                  result: 0 = Success, -1 = Failure
int    TYPENAME##_find(const TYPENAME* const me, const KEY_TYPE* const key, size_t* const valIdx) */
    
 
/* Adds new pair to the end of the map.
	\param me : cStaticMap instance pointer
	\param key: key to be added.
    \param value: value to be added.
	\result 		: result: 0 = Success, -1 = Failure
int	   TYPENAME##_insert(TYPENAME* const me, const KEY_TYPE* const key, const VALUE_TYPE* const value) */

/* Deletes the pair containing given key.
	\param me : cStaticMap instance pointer
	\param key : key value
	\result 		: result: 0 = Success, -1 = Failure
int   TYPENAME##_erase(TYPENAME* const me, const KEY_TYPE* const key) */
    

/*This macro is used to make function declarations 
 *of a concrete cStaticMap type.
 */
#define cStaticMap_METHOD_DECLARATIONS(TYPENAME, KEY_TYPE, VALUE_TYPE, ...)\
\
__VA_ARGS__ size_t TYPENAME##_size(const TYPENAME* const me);\
\
__VA_ARGS__ void   TYPENAME##_clear(TYPENAME* const me);\
\
__VA_ARGS__ int    TYPENAME##_find(const TYPENAME* const me, const KEY_TYPE* const key, size_t* const valIdx);\
\
__VA_ARGS__ int	   TYPENAME##_insert(TYPENAME* const me, const KEY_TYPE* const key, const VALUE_TYPE* const value);\
\
__VA_ARGS__ int    TYPENAME##_erase(TYPENAME* const me, const KEY_TYPE* const key);

 
/*This macro is used to make function definitions 
 *of a concrete cStaticMap type.
 */     
    
#define cStaticMap_METHOD_DEFINITIONS(TYPENAME, KEY_TYPE, VALUE_TYPE, ...)\
\
__VA_ARGS__ size_t TYPENAME##_size(const TYPENAME* const me)\
{\
    return me->mapSize;\
}\
\
__VA_ARGS__ void TYPENAME##_clear(TYPENAME* const me)\
{\
    me->mapSize = 0;\
}\
\
__VA_ARGS__ int  TYPENAME##_find(const TYPENAME* const me, const KEY_TYPE* const key, size_t* const valIdx)\
{\
    int result = -1;\
    size_t idx = 0;\
    for(; idx < me->mapSize; ++idx)\
    {\
        if(me->keyList[idx] == *key)\
        {\
            result = 0;\
            if(NULL != valIdx)\
            {\
                *valIdx = idx;\
            }\
            break;\
        }\
    }\
    return result;\
}\
\
__VA_ARGS__ int	   TYPENAME##_insert(TYPENAME* const me, const KEY_TYPE* const key, const VALUE_TYPE* const value)\
{\
    size_t valIdx;\
    int result = TYPENAME##_find(me, key, &valIdx);\
    if(0 == result)\
    {\
        me->valueList[valIdx] = *value;\
    }\
    else\
    {\
        if(me->mapSize < ((sizeof(me->keyList)) / (sizeof(me->keyList[0]))))\
        {\
            me->keyList[me->mapSize] = *key;\
            me->valueList[me->mapSize] = *value;\
            ++(me->mapSize);\
            result = 0;\
        }\
    }\
    return result;\
}\
\
__VA_ARGS__ int   TYPENAME##_erase(TYPENAME* const me, const KEY_TYPE* const key)\
{\
	int retVal = -1;\
    if((size_t)(0) < me->mapSize)\
    {\
        size_t valIdx;\
        if(0 == TYPENAME##_find(me, key, &valIdx))\
        {\
            for(++valIdx; valIdx < me->mapSize; ++valIdx)\
            {\
                me->keyList[valIdx - 1]   = me->keyList[valIdx];\
                me->valueList[valIdx - 1] = me->valueList[valIdx];\
            }\
            --(me->mapSize);\
            retVal = -1;\
        }\
    }\
	return retVal;\
}

#ifdef __cplusplus
}
#endif

#endif
