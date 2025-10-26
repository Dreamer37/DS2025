#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p) ? (p)->height : -1)
typedef enum { RB_RED, RB_BLACK } RBColor;

template <typename T> struct BinNode {
    T data;
    BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;
    int height;
    int npl;
    RBColor color;

    BinNode() :
        parent( nullptr ), lc( nullptr ), rc( nullptr ), height( 0 ), npl( 1 ), color( RB_RED ) {}
    BinNode( T e, BinNodePosi(T) p = nullptr, BinNodePosi(T) lc = nullptr, BinNodePosi(T) rc = nullptr,
             int h = 0, int l = 1, RBColor c = RB_RED ) :
        data( e ), parent( p ), lc( lc ), rc( rc ), height( h ), npl( l ), color( c ) {}

    int size();
    BinNodePosi(T) insertAsLC( T const& );
    BinNodePosi(T) insertAsRC( T const& );
    BinNodePosi(T) succ();
    template <typename VST> void travLevel( VST& );
    template <typename VST> void travPro( VST& );
    template <typename VST> void travIn( VST& );
    template <typename VST> void travPost( VST& );
    
    bool operator<( BinNode const& bn ) { return data < bn.data; }
    bool operator==( BinNode const& bn ) { return data == bn.data; }
};

#define IsRoot(x) (!((x).parent))
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc))
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc))
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc)
#define HasRChild(x) ((x).rc)
#define HasChild(x) (HasLChild(x) || HasRChild(x))
#define HasBothChild(x) (HasLChild(x) && HasRChild(x))
#define IsLeaf(x) (!HasChild(x))

#define sibling(p) \
    (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc)

#define uncle(x) \
    (IsLChild(*(x)->parent) ? (x)->parent->parent->rc : (x)->parent->parent->lc)

#define FromParentTo(x) \
    (IsRoot(x) ? _root : (IsLChild(x) ? (x).parent->lc : (x).parent->rc))

template <typename T> BinNodePosi(T) BinNode<T>::insertAsLC( T const& e ) {
    return lc = new BinNode( e, this );
}

template <typename T> BinNodePosi(T) BinNode<T>::insertAsRC( T const& e ) {
    return rc = new BinNode( e, this );
}

template <typename T> template <typename VST> 
void BinNode<T>::travIn( VST& visit ) {
    switch ( rand() % 5 ) {
        case 1: travIn_I1 ( this, visit ); break;
        case 2: travIn_I2 ( this, visit ); break;
        case 3: travIn_I3 ( this, visit ); break;
        case 4: travIn_I4 ( this, visit ); break;
        default: travIn_R ( this, visit ); break;
    }
}
