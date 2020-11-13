#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LIST_BY_YEAR       0
#define LIST_BY_TYPE       1

#define TYPE_MAX           100
#define SETUP_MAX          100

typedef struct TEngine{
    struct TEngine * m_Next;
    struct TEngine * m_Prev;
    int              m_Year;
    char             m_Type  [ TYPE_MAX ];
    int              m_Setup [ SETUP_MAX ];
}TENGINE;

typedef struct TArchive
{
    struct TArchive * m_Next;
    struct TArchive * m_Prev;
    TENGINE         * m_Engines;
} TARCHIVE;


TENGINE* createEngine(const char* type,int year){
    
    TENGINE *res = (TENGINE *) malloc (sizeof(*res));
    res -> m_Next = NULL;
    res -> m_Prev = NULL;
    res -> m_Year = year;
    strncpy ( res -> m_Type, type, sizeof ( res -> m_Type ) );
    for ( int i = 0; i < SETUP_MAX; i ++ )
        res -> m_Setup[i] = 0;
    return res;
}
#endif /* __PROGTEST__ */
void swap(TENGINE *a, TENGINE *b,TARCHIVE* list)
{
    //a is the head
    if (a->m_Prev==NULL){
        b->m_Prev = NULL;
        list->m_Engines = b;
    } else {
        b->m_Prev = a->m_Prev;
        (a->m_Prev)->m_Next = b;
    }
    
    if (b->m_Next == NULL){
        a->m_Next = NULL;
    } else {
        a->m_Next = b->m_Next;
        (b->m_Next)->m_Prev = a;
    }
    
    b->m_Next = a;
    a->m_Prev = b;
    
}

void sortEngines(TARCHIVE* list,int listBy){
    //go to the head archive
    while(list->m_Prev!=NULL){
        list = list->m_Prev;
    }
    //bubblesort
    while(list!=NULL){
        int swapped;
        TENGINE *ptr1;
        
        /* Checking for empty list */
        if (list->m_Engines == NULL)
            return;
        
        do
        {
            swapped = 0;
            ptr1 = list->m_Engines;
            
            while (ptr1 != NULL)
            {
                if(ptr1->m_Next == NULL)break;
                if(listBy == LIST_BY_YEAR){
                    if (strcmp(ptr1->m_Type,ptr1->m_Next->m_Type)>0){
                        swap(ptr1, ptr1->m_Next, list);
                        swapped = 1;
                    }
                }
                if(listBy == LIST_BY_TYPE){
                    if (ptr1->m_Year > ptr1->m_Next->m_Year){
                        swap(ptr1, ptr1->m_Next, list);
                        swapped = 1;
                    }
                }
                ptr1 = ptr1->m_Next;
            }
        }
        while (swapped);
        
        list = list->m_Next;
    }
    
}

TARCHIVE* createArchive(){
    TARCHIVE* res = (TARCHIVE*)malloc(sizeof(TARCHIVE));
    res -> m_Next = NULL;
    res -> m_Prev = NULL;
    res -> m_Engines = NULL;
    
    return res;
}

void insertBefore(TARCHIVE* list){
    TARCHIVE* tmp;
    tmp = createArchive();
    
    tmp->m_Next = list;
    
    if (list->m_Prev == NULL){
        list->m_Prev = tmp;
        tmp->m_Prev = NULL;
    } else {
        tmp->m_Prev = (list->m_Prev);
        (list->m_Prev)->m_Next = tmp;
        list->m_Prev = tmp;
    }
    
}

void insertAfter(TARCHIVE* list){
    TARCHIVE* tmp;
    tmp = createArchive();
    
    tmp->m_Prev = list;
    if (list->m_Next == NULL){
        list->m_Next = tmp;
        tmp->m_Next = NULL;
    } else {
        tmp->m_Next = list->m_Next;
        (list->m_Next) -> m_Prev = tmp;
        list->m_Next = tmp;
    }
    
}

TARCHIVE* AddEngineByYear(TARCHIVE* list,TENGINE* engine){
    TENGINE* tmp;
    TARCHIVE* tmp_a;
    
    if (list ->m_Engines == NULL){
        list ->m_Engines = engine;
        return list;
    }
    
    tmp_a = list;
    //go to the head (i.e least string)
    while(tmp_a->m_Prev!=NULL){
        tmp_a = tmp_a->m_Prev;
    }
    list = tmp_a;
    
    //add before if less than head
    if (engine->m_Year < list ->m_Engines->m_Year){
        insertBefore(list);
        list = list->m_Prev;
        list->m_Engines = engine;
        while(list->m_Prev!=NULL){
            list = list->m_Prev;
        }
        return list;
    }
    if (engine->m_Year == list->m_Engines->m_Year){
        tmp = list->m_Engines;
        while(tmp->m_Next!=NULL){
            tmp = tmp->m_Next;
        }
        tmp->m_Next = engine;
        (tmp->m_Next)->m_Prev = tmp;
        while(list->m_Prev!=NULL){
            list = list->m_Prev;
        }
        return list;
    }
    
    while (list->m_Next!=NULL) {
        if (engine->m_Year == list->m_Engines->m_Year){
            tmp = list->m_Engines;
            while(tmp->m_Next!=NULL){
                tmp = tmp->m_Next;
            }
            tmp->m_Next = engine;
            (tmp->m_Next)->m_Prev = tmp;
            while(list->m_Prev!=NULL){
                list = list->m_Prev;
            }
            return list;
        }
        if (engine->m_Year == list->m_Next->m_Engines->m_Year){
            tmp = list->m_Next->m_Engines;
            while(tmp->m_Next!=NULL){
                tmp = tmp->m_Next;
            }
            tmp->m_Next = engine;
            (tmp->m_Next)->m_Prev = tmp;
            while(list->m_Prev!=NULL){
                list = list->m_Prev;
            }
            return list;
        }
        
        if (engine->m_Year < list ->m_Next->m_Engines->m_Year){
            insertAfter(list);
            list = list->m_Next;
            list->m_Engines = engine;
            while(list->m_Prev!=NULL){
                list = list->m_Prev;
            }
            return list;
        }
        
        list = list->m_Next;
    }
    
    insertAfter(list);
    list = list->m_Next;
    list->m_Engines = engine;
    
    while(list->m_Prev!=NULL){
        list = list->m_Prev;
    }
    return list;
}

TARCHIVE* AddEngineByType(TARCHIVE* list,TENGINE* engine){
    TENGINE* tmp;
    
    if (list ->m_Engines == NULL){
        list ->m_Engines = engine;
        
        return list;
    }
    
    
    //go to the head (i.e least string)
    while(list->m_Prev!=NULL){
        list = list->m_Prev;
    }
    
    //add before if less than head
    if (strcmp(engine->m_Type, list ->m_Engines->m_Type)<0){
        insertBefore(list);
        list = list->m_Prev;
        list->m_Engines = engine;
        while(list->m_Prev!=NULL){
            list = list->m_Prev;
        }
        return list;
    }
    if (strcmp(engine->m_Type, list ->m_Engines->m_Type)==0){
        tmp = list->m_Engines;
        while(tmp->m_Next!=NULL){
            tmp = tmp->m_Next;
        }
        tmp->m_Next = engine;
        (tmp->m_Next)->m_Prev = tmp;
        while(list->m_Prev!=NULL){
            list = list->m_Prev;
        }
        return list;
    }
    
    while (list->m_Next!=NULL) {
        if (strcmp(engine->m_Type, list ->m_Engines->m_Type)==0){
            tmp = list->m_Engines;
            while(tmp->m_Next!=NULL){
                tmp = tmp->m_Next;
            }
            tmp->m_Next = engine;
            (tmp->m_Next)->m_Prev = tmp;
            while(list->m_Prev!=NULL){
                list = list->m_Prev;
            }
            return list;
        }
        if (strcmp(engine->m_Type, list->m_Next ->m_Engines->m_Type)==0){
            tmp = list->m_Next->m_Engines;
            while(tmp->m_Next!=NULL){
                tmp = tmp->m_Next;
            }
            tmp->m_Next = engine;
            (tmp->m_Next)->m_Prev = tmp;
            while(list->m_Prev!=NULL){
                list = list->m_Prev;
            }
            return list;
        }
        
        if (strcmp(engine->m_Type, list ->m_Next->m_Engines->m_Type)<0){
            insertAfter(list);
            list = list->m_Next;
            list->m_Engines = engine;
            while(list->m_Prev!=NULL){
                list = list->m_Prev;
            }
            return list;
        }
        
        list = list->m_Next;
    }
    
    insertAfter(list);
    list = list->m_Next;
    list->m_Engines = engine;
    
    while(list->m_Prev!=NULL){
        list = list->m_Prev;
    }
    return list;
}

TARCHIVE* AddEngine(TARCHIVE* list,int listBy,TENGINE* engine){
    //create archive
    if(list == NULL){
        list = createArchive();
    }
    
    if (listBy == LIST_BY_YEAR){
        list = AddEngineByYear(list, engine);
        sortEngines(list,LIST_BY_YEAR);
    }
    if (listBy == LIST_BY_TYPE){
        list = AddEngineByType(list, engine);
        sortEngines(list,LIST_BY_TYPE);
    }
    
    return list;
}

void DelArchive(TARCHIVE* list){
    TARCHIVE* tmp_a;
    TENGINE* tmp_e;
    TENGINE* tmp;
    
    
    if(list == NULL) return;
    while(list->m_Prev!=NULL){
        list = list->m_Prev;
    }
    
    while(list!=NULL){
        tmp = list->m_Engines;
        while(tmp!=NULL){
            tmp_e = tmp;
            tmp = tmp->m_Next;
            free(tmp_e);
        }
        tmp_a = list;
        list = list -> m_Next;
        free(tmp_a);
    }
}

TARCHIVE* ReorderArchive(TARCHIVE* list,int listBy){
    TARCHIVE* newList = createArchive();
    TENGINE *tmp;
    TENGINE* tmp2;
    TARCHIVE* tmp_a = list;
    if(list == NULL) return NULL;
    //go to the head
    while(list->m_Prev!=NULL){
        list = list->m_Prev;
    }
    
    if(listBy == LIST_BY_TYPE){
        while(list!=NULL){
            tmp = list->m_Engines;
            while(tmp!=NULL){
                tmp2 = tmp->m_Next;
                tmp->m_Next = tmp->m_Prev = NULL;
                newList = AddEngine(newList,LIST_BY_TYPE,tmp);
                tmp = tmp2;
            }
            
            list = list -> m_Next;
        }
        
        sortEngines(newList,LIST_BY_TYPE);
        
    }
    if(listBy == LIST_BY_YEAR){
        while(list!=NULL){
            tmp = list->m_Engines;
            while(tmp!=NULL){
                tmp2 = tmp->m_Next;
                tmp->m_Next = tmp->m_Prev = NULL;
                newList = AddEngine(newList,LIST_BY_YEAR,tmp);
                tmp = tmp2;
            }
            
            list = list -> m_Next;
        }
        
        sortEngines(newList,LIST_BY_YEAR);
        
    }
    while(newList->m_Prev!=NULL){
        newList = newList->m_Prev;
    }
    //free old archive
    list = tmp_a;
    while(list->m_Prev!=NULL){
        list = list->m_Prev;
    }
    while(list!=NULL){
        tmp_a = list;
        list = list -> m_Next;
        free(tmp_a);
    }
    //
    
    return newList;
    
}
#ifndef __PROGTEST__

int                main                                    ()
{
    TARCHIVE * a;
    
    a = NULL;
    a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "TDI 1.9", 2010 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "TDI 1.9", 2005 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "TSI 1.2", 2010 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Prev == a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "TDI 2.0", 2005 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 2.0" )
            && a -> m_Engines -> m_Next -> m_Prev == a -> m_Engines
            && a -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Prev == a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "MPI 1.4", 2005 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Prev == a -> m_Engines
            && a -> m_Engines -> m_Next -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Next -> m_Type, "TDI 2.0" )
            && a -> m_Engines -> m_Next -> m_Next -> m_Prev == a -> m_Engines -> m_Next
            && a -> m_Engines -> m_Next -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Prev == a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
    a = ReorderArchive ( a, LIST_BY_TYPE );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Prev == a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Prev == a -> m_Next
            && a -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Next -> m_Engines -> m_Type, "TDI 2.0" )
            && a -> m_Next -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Prev == a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    DelArchive ( a );
    
    a = NULL;
    a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TDI 1.9", 2010 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TDI 1.9", 2005 ) );
    assert ( a);
    assert ( a -> m_Prev == NULL);
    assert ( a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Prev == a -> m_Engines
            && a -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TSI 1.2", 2010 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Prev == a -> m_Engines
            && a -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TDI 2.0", 2005 ) );
    assert ( a);
    assert ( a -> m_Prev == NULL);
    assert ( a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Prev == a -> m_Engines
            && a -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 2.0" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Prev == a -> m_Next
            && a -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "MPI 1.4", 2005 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Prev == a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Prev == a -> m_Next
            && a -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Next -> m_Engines -> m_Type, "TDI 2.0" )
            && a -> m_Next -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Prev == a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TDI 1.9", 2010 ) );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Prev == a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Prev == a -> m_Next -> m_Engines -> m_Next
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Prev == a -> m_Next
            && a -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Next -> m_Engines -> m_Type, "TDI 2.0" )
            && a -> m_Next -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Prev == a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
    a = ReorderArchive ( a, LIST_BY_YEAR );
    assert ( a
            && a -> m_Prev == NULL
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Prev == NULL
            && a -> m_Engines -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Prev == a -> m_Engines
            && a -> m_Engines -> m_Next -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Next -> m_Type, "TDI 2.0" )
            && a -> m_Engines -> m_Next -> m_Next -> m_Prev == a -> m_Engines -> m_Next
            && a -> m_Engines -> m_Next -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Prev == a
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Prev == NULL
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Prev == a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Prev == a -> m_Next -> m_Engines -> m_Next
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
    DelArchive ( a );
    
    return 0;
}
#endif /* __PROGTEST__ */
