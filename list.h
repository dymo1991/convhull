template <class AnyObject> class List
{
    private:
        struct Elem {                                   /* Element of list */
            AnyObject Data;
            Elem* Next;
            };
        Elem *pHead;
        Elem *pCurr;
    public:
        List();
        ~List();

        void Clear();                                   /* Clear the list */
        int IsEmpty();                                  /* Is list empty */
        void Add(AnyObject NodeData);                   /* Add element */
        int Remove(AnyObject* NodeData);                /* Remove first element */
        int FindAndRemoveAll(AnyObject NodeData);       /* Remove any element */
        int GetNext(AnyObject* NodeData);               /* Get next element */
        int GetFirst(AnyObject* NodeData);              /* Get first element */
        void Init(List<AnyObject>* list);
};

template <class AnyObject> List<AnyObject>::List()
{
    pHead = 0;
    pCurr = 0;
};

template <class AnyObject> int List<AnyObject>::IsEmpty ()
{
    if(pHead==0)
        return 1;
    else
        return 0;
};


template <class AnyObject> void List<AnyObject>::Add(AnyObject NodeData)
{
    Elem *pTmp;


    pTmp = new (Elem);
    pTmp->Data= NodeData;
    pTmp->Next=pHead;
    pHead = pTmp;
};

template <class AnyObject> int List<AnyObject>::Remove(AnyObject* NodeData)
{
    Elem *pTmp;

    if (!pHead)
        return 0;
    else
    {
        *NodeData=pHead->Data;
        pTmp = pHead;
        pHead = pHead->Next;
        delete ( pTmp);
        pCurr = 0;
        return 1;
    }
};

template <class AnyObject> int List<AnyObject>::GetNext(AnyObject* NodeData)
{
    if ( pCurr )
    {
        *NodeData=pCurr->Data;
        pCurr=pCurr->Next;
        return 1;
    }
    else
        return 0;

};

template <class AnyObject> int List<AnyObject>::GetFirst(AnyObject* NodeData)
{
    if ( pHead )
    {
        *NodeData=pHead->Data;
        pCurr=pHead->Next;
        return 1;
    }
    else
        return 0;
};

template <class AnyObject> int List<AnyObject>::FindAndRemoveAll(AnyObject NodeData)
{
    Elem        *pTmp, *pTmp2;
    AnyObject   pObjTmp;
    int Res = 0;

    while ( pHead && ( pHead->Data == NodeData ) )
    {
        Remove( &pObjTmp );
        Res++;
    };

    pTmp = pHead;
    if ( !pTmp )
        return Res;
    while (pTmp && pTmp->Next)
    {
        if ( pTmp->Next->Data == NodeData )
        {
            pTmp2=pTmp->Next;
            pTmp->Next = pTmp->Next->Next;
            Res++;
            delete ( pTmp2 );
        }
        else
            pTmp = pTmp->Next;
    };
    return Res;
};

template <class AnyObject> void List<AnyObject>::Clear()
{
    Elem *pTmp;

    while (pHead)
    {
        pTmp = pHead;
        pHead = pHead->Next;
        delete ( pTmp );
    }
    pCurr = 0;
};

template <class AnyObject> List<AnyObject>::~List()
{
    Clear();
};

template <class AnyObject> void List<AnyObject>::Init(List<AnyObject>* list)
{
    AnyObject Any;

    if ( list->GetFirst(&Any) )
    {
        do {
            Add(Any);
        } while(list->GetNext(&Any));
    };
};