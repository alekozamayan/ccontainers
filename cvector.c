#include <stdlib.h>
#include <string.h>
#include "cvector.h"

/*This macro defines the power value used in calculation of vector allocation size, in terms of pair count.
If given 1, the insert and erase methods will reallocate the vector whenever an element is
added or removed.
Otherwise, these methods will reallocate the vector if required, with the nearest power of the
CVECTOR_ALLOC_POWER_SIZE.
The value 2 will make it work in the same allocation strategy with C++ std::vector container.
It provides lesser memory fragmentation.
NOTE: Do not define it as 0!*/
#define CVECTOR_ALLOC_POWER_SIZE                2
#define CVECTOR_ALLOC_POWER_SIZE_RND            ((size_t)(CVECTOR_ALLOC_POWER_SIZE))

/*Gives the pointer integer value of the array element at the specified index
  Why not to return directly the void pointer? That's because we need the
  pointer address value in integer to perform pointer arithmetics on void
  pointers.*/
#define CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx)    ((size_t)((pInstance)->array) + (idx)*((pInstance)->elemSizeAligned))

/* This macro gets "size" and returns "align"ed size */
#define CVECTOR_ALIGN_SIZE(size, align)  ((size) % (align)) ? ((size) + ((align) - ((size) % (align)))) : (size)


void* 	cVector_getAt(cVector* pInstance, const size_t idx)
{
    return (idx < pInstance->vectSize) ? (void*)CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx) : NULL;
}

size_t 	cVector_size(const cVector* pInstance)
{
    return pInstance->vectSize;	
}

void 	cVector_clear(cVector* pInstance)
{
    if(NULL != pInstance->array)
    {
        free(pInstance->array);
        pInstance->array = NULL;
    }
    pInstance->vectSize = (size_t)(0);
    pInstance->allocSize = (size_t)(0);
}


size_t 	cVector_find(cVector* pInstance, const void* elem)
{
    size_t idx = pInstance->vectSize;
    
    if(NULL != elem)
    {
	    for(idx = 0; idx < pInstance->vectSize; ++idx)
	    {
	        if(0 == memcmp(elem, (void*)CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx), pInstance->elemSize))
	        {                
		        break;
	        }
	    }	
    }
    
    return idx;
}


int		cVector_insert(cVector* pInstance, const void* newElem, const size_t idx)
{
    int result = -1;
    
    if(NULL != pInstance)
    {
        if(NULL != newElem)
        {
            if(idx <= pInstance->vectSize)
            {
                void* newArrayPtr = pInstance->array;
                
                if((size_t)(0) == pInstance->vectSize)
                {
                    pInstance->allocSize = CVECTOR_ALLOC_POWER_SIZE_RND;
                    newArrayPtr = (void*)malloc((pInstance->allocSize * pInstance->elemSizeAligned));
                }
                else
                {
#if (1 < CVECTOR_ALLOC_POWER_SIZE)						
                    if(pInstance->vectSize == pInstance->allocSize)
                    {
                        /*Allocate space with the nearest power*/					
                        pInstance->allocSize *= CVECTOR_ALLOC_POWER_SIZE_RND;
                        newArrayPtr = (void*)realloc(pInstance->array, (pInstance->allocSize * pInstance->elemSizeAligned));
                    }                   	
#else
                    ++(pInstance->allocSize);
                    newArrayPtr = (void*)realloc(pInstance->array, (pInstance->allocSize * pInstance->elemSizeAligned));
#endif
                }
                
                if(NULL != newArrayPtr)
                {
                    pInstance->array = newArrayPtr;                                        

                    if(idx < pInstance->vectSize)
                    {
                        memmove((void*)CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx+1), (const void*)CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx), ((pInstance->vectSize * pInstance->elemSizeAligned) - (CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx) - (size_t)pInstance->array)));
                    }                    

                    memcpy((void*)CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx), newElem, pInstance->elemSizeAligned);
                   
                    ++pInstance->vectSize;

                    result = 0;
                }
            }
        }
    }

    return result;
}


int 	cVector_eraseAt(cVector* pInstance, const size_t idx)
{
    int returnVal = -1;
    
    if(idx < pInstance->vectSize)
    {       
        --(pInstance->vectSize);
        
        if((size_t)(0) < pInstance->vectSize)
        {
            memmove((void*)CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx), (const void*)CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx+1), ((pInstance->vectSize * pInstance->elemSizeAligned) - (CVECTOR_CALC_IDX_PTR_VAL(pInstance, idx) - (size_t)pInstance->array)));
			
#if (1 < CVECTOR_ALLOC_POWER_SIZE)
			if(CVECTOR_ALLOC_POWER_SIZE_RND < pInstance->allocSize)
            {				
				if(pInstance->vectSize == (pInstance->allocSize / CVECTOR_ALLOC_POWER_SIZE_RND))				
				{				
					pInstance->array = (void*)realloc(pInstance->array, (pInstance->vectSize * pInstance->elemSizeAligned));
					
					pInstance->allocSize /= CVECTOR_ALLOC_POWER_SIZE_RND;
				}
			}
#else
			pInstance->array = (void*)realloc(pInstance->array, (pInstance->vectSize * pInstance->elemSizeAligned));
			--(pInstance->allocSize);
#endif		
	    }
	    else
	    {
			free(pInstance->array);
			pInstance->allocSize = (size_t)(0);
	    }
        
        returnVal = 0;
    }
    
    return returnVal;
}

int     cVector_erase(cVector* pInstance, const void* elem)
{
    return cVector_eraseAt(pInstance, cVector_find(pInstance, elem));
}

void concreteConstructCVector(cVector* instance, size_t elemSize)
{
    if(NULL != instance)
    {
        instance->elemSize  = elemSize;
        instance->vectSize  = (size_t)(0);
        instance->allocSize = (size_t)(0);
        instance->array = NULL;

        instance->elemSizeAligned = CVECTOR_ALIGN_SIZE(instance->elemSize, (sizeof(int)));
    } 
}

