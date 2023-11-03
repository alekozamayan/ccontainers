#include <stdlib.h>
#include <string.h>
#include "cmap.h"

/*This macro defines the power value used in calculation of map allocation size, in terms of pair count.
If given 1, the insert and erase methods will reallocate the map whenever an element is
added or removed.
Otherwise, these methods will reallocate the map if required, with the nearest power of the
CMAP_ALLOC_POWER_SIZE.
The value 2 will make it work in the same allocation strategy with C++ std::vector container.
It provides lesser memory fragmentation.
NOTE: Do not define it as 0!
*/
#define CMAP_ALLOC_POWER_SIZE 2
#define CMAP_ALLOC_POWER_SIZE_RND ((size_t)(CMAP_ALLOC_POWER_SIZE))

/*Gives the pointer integer value of the array element at the specified index
  Why not to return directly the void pointer? That's because we need the
  pointer address value in integer to perform pointer arithmetics on void
  pointers.
*/
#define CMAP_CALC_KEY_IDX_PTR_VAL(pInstance, idx)    ((size_t)((pInstance)->pairArray) + (idx)*((pInstance)->elemSize))
#define CMAP_CALC_VAL_IDX_PTR_VAL(pInstance, idx)    (CMAP_CALC_KEY_IDX_PTR_VAL(pInstance, idx) + ((pInstance)->keySizeAligned))

/* This macro gets "size" and returns "align"ed size */
#define CMAP_ALIGN_SIZE(size, align)  ((size) % (align)) ? ((size) + ((align) - ((size) % (align)))) : (size)


int 	cMap_getAt(cMap* pInstance, const size_t idx, cPair* pPair)
{
    int retVal = -1;
    
    if(idx < pInstance->mapSize)
    {
        if(NULL != pPair)
        {
            pPair->first  = (void*)CMAP_CALC_KEY_IDX_PTR_VAL(pInstance, idx);
            pPair->second = (void*)CMAP_CALC_VAL_IDX_PTR_VAL(pInstance, idx);
            retVal = 0; 
        }
    }
    
    return retVal;
}

size_t 	cMap_size(const cMap* pInstance)
{
    return pInstance->mapSize;	
}

void 	cMap_clear(cMap* pInstance)
{
    if(NULL != pInstance->pairArray)
    {
        free(pInstance->pairArray);
        pInstance->pairArray = NULL;
    }
       
    pInstance->mapSize = (size_t)(0);
    pInstance->allocationSize = (size_t)(0);
}

int 	cMap_find(cMap* pInstance, const void* key, cPair* pPair)
{	
    int retVal = -1;   

    if(NULL != key)
    {
        if(NULL != pPair)
        {
            size_t idx;

            for(idx = 0; idx < pInstance->mapSize; ++idx)
            {
                if(0 == memcmp(key, (void*)CMAP_CALC_KEY_IDX_PTR_VAL(pInstance, idx), pInstance->keySize))
                {
                    pPair->first  = (void*)CMAP_CALC_KEY_IDX_PTR_VAL(pInstance, idx);
                    pPair->second = (void*)CMAP_CALC_VAL_IDX_PTR_VAL(pInstance, idx);
                    retVal = 0;
                    break;
                }		
            }	
        } 
    }

    return retVal;
}

int	cMap_insert(cMap* pInstance, const cPair* newPair)
{
    int result = -1;
    
    if(NULL != pInstance)
    {
        if(NULL != newPair)
        {
            cPair checkPair;
            
            if(0 == cMap_find(pInstance, newPair->first, &checkPair))
            {
                memcpy(checkPair.second, newPair->second, pInstance->valueSizeAligned);
            }
            else
            {
                void* newArrayPtr = pInstance->pairArray;
                        
                if((size_t)(0) == pInstance->mapSize)
                {
                    pInstance->allocationSize = CMAP_ALLOC_POWER_SIZE_RND;
                    newArrayPtr = (void*)malloc(pInstance->elemSize * pInstance->allocationSize);					
                }				
                else
                {
#if (1 < CMAP_ALLOC_POWER_SIZE)						
                    if(pInstance->mapSize == pInstance->allocationSize)
                    {
                        /*Allocate space with the nearest power*/					
                        pInstance->allocationSize *= CMAP_ALLOC_POWER_SIZE_RND;
                        newArrayPtr = (void*)realloc((void*)(pInstance->pairArray), (pInstance->elemSize * pInstance->allocationSize));
                    }                   	
#else
                    ++(pInstance->allocationSize);
                    newArrayPtr = (void*)realloc((void*)(pInstance->pairArray), (pInstance->elemSize * pInstance->allocationSize));
#endif	       					
                }

                if(NULL != newArrayPtr)
                {
                    pInstance->pairArray = newArrayPtr;			    

                    memcpy((void*)CMAP_CALC_KEY_IDX_PTR_VAL(pInstance, pInstance->mapSize), newPair->first, pInstance->keySizeAligned);
                    memcpy((void*)CMAP_CALC_VAL_IDX_PTR_VAL(pInstance, pInstance->mapSize), newPair->second,pInstance->valueSizeAligned);

                    ++pInstance->mapSize;

                    result = 0;
                }                
            }
        }
    }

    return result;
}

int 	cMap_erase(cMap* pInstance, const void* key)
{
    int result = -1;
    
    if((size_t)(0) < pInstance->mapSize)
    {
        cPair pairToDelete;

        if(0 == cMap_find(pInstance, key, &pairToDelete))
        {
            --(pInstance->mapSize);

            if((size_t)(0) < pInstance->mapSize)
            {			
                memmove((void*)(pairToDelete.first), (const void*)((size_t)(pairToDelete.first) + pInstance->elemSize), ((pInstance->mapSize * pInstance->elemSize) - ((size_t)(pairToDelete.first) - (size_t)pInstance->pairArray)));		
                
#if (1 < CMAP_ALLOC_POWER_SIZE)
                if(CMAP_ALLOC_POWER_SIZE_RND < pInstance->allocationSize)
                {				
                    if(pInstance->mapSize == (pInstance->allocationSize / CMAP_ALLOC_POWER_SIZE_RND))				
                    {				
                        pInstance->pairArray = (void*)realloc((void*)pInstance->pairArray, (pInstance->mapSize * pInstance->elemSize));
                        
                        pInstance->allocationSize /= CMAP_ALLOC_POWER_SIZE_RND;
                    }
                }
#else
                pInstance->pairArray = (void*)realloc((void*)pInstance->pairArray, (pInstance->mapSize * pInstance->elemSize));
                --(pInstance->allocationSize);
#endif	
                
            }
            else
            {
                free(pInstance->pairArray);
                pInstance->allocationSize = (size_t)(0);
            }
            
            result = 0;
        }
    }
    
    return result;
}

void concreteConstructCMap(cMap* instance, size_t keySize, size_t valueSize)
{
    if(NULL != instance)
    {
    	const size_t alignSize = sizeof(int);

    	instance->keySize   = keySize;
        instance->valueSize = valueSize;        
        instance->mapSize   = (size_t)(0);
        instance->allocationSize = (size_t)(0);
        instance->pairArray = NULL;
        
        instance->keySizeAligned   = CMAP_ALIGN_SIZE(instance->keySize, alignSize);
        instance->valueSizeAligned = CMAP_ALIGN_SIZE(instance->valueSize, alignSize);

        instance->elemSize = instance->keySizeAligned + instance->valueSizeAligned;
    } 
}
