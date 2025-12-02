#include <iostream>
using namespace std;

#define N_CHAR ( 0x80 - 0x20 )
struct HuffChar {
    char ch; int weight;
    HuffChar ( char c = '^', int w = 0 ) : ch(c), weight(w) {}
    bool operator< ( HuffChar const &hc) { return weight > hc.weight; }
    bool operator== ( HuffChar const &hc) { return weight == hc.weight; }
};

#include "BinNode.h"
#define HuffTree BinNode<HuffChar>

#include "List.h"
typedef List<HuffTree*> HuffForest;

#include "Bitmap.h"
typedef Bitmap HuffCode;

#include "HuffmanImpl.h"
typedef HashTable<char, char*> HuffTable;

int main ( int argc, char *argv[] ) {
    int* freq = statistics ( argc, argv );
    HuffForest* forest = initForest ( freq ); release (freq);
    HuffTree* tree = generateTree ( forest ); release (forest);
    HuffTable* table = generateTable ( tree );
    for ( int i = 2; i < argc; i++ ) {
        Bitmap* codeString = new Bitmap;
        int n = ecode ( table, codeString, argv[i] );
        decode ( tree, codeString, n );
        release (codeString);
    }
    release (table); release (tree); return 0;
}

int* statistics ( char* sample_s ) {
    int* freq = new int[N_CHAR];
    memset ( freq, 0, N_CHAR * sizeof(int) );
    FILE* fp = fopen ( sample_s, "r" );
    for ( char ch; 0 < fscanf ( fp, '%c', &ch ); ) {
        if ( ch >= 0x20 ) freq[ch - 0x20]++;
    }
    fclose (fp); return freq;
}   

HuffForest* initForest ( int* freq ) {
    HuffForest* forest = new HuffForest;
    for ( int i = 0; i < N_CHAR; i++ ) {
        forest->insertAsLast ( new HuffTree );
        forest->last()->data->insertAsRoot ( HuffChar ( char(i + 0x20), freq[i] ) );
    }
    return forest;
}

HuffTree* minHChar ( HuffForest* forest ) {
    ListNodepi( HuffTree* ) p = forest->first();
    ListNodepi( HuffTree* ) minChar = p;
    int minWeight = p->data->root()->data.weight;
    while ( forest->valid ( p = p->succ ) ) {
        if ( p->data->root()->data.weight < minWeight ) {
            minWeight = p->data->root()->data.weight; minChar = p;
        }
    }
    return forest->remove ( minChar );
}

HuffTree* generateTree ( HuffForest* forest ) {
    while ( forest->size() > 1 ) {
        HuffTree* min1 = minHChar ( forest );
        HuffTree* min2 = minHChar ( forest );
        HuffTree* S = new HuffTree;
        S->insertAsRoot ( HuffChar ( '^', min1->root()->data.weight + min2->root()->data.weight ) );
        S->root()->insertAsLC ( min1->root() ); S->root()->insertAsRC ( min2->root() );
        forest->insertAsLast ( S );
    }
    return forest->first()->data;
}

static void generateCT(Bitmap *code, int length, HuffTable table, BinNodePosi(HuffChar) v)
{
    if ( IsLeaf( *v ) ) {
        table->put ( v->data.ch, code->bits2string ( length ) ); return;
    }
    if ( HasLChild( *v ) ) {
        code->clear ( length ); generateCT ( code, length + 1, table, v->lc );
    }
    if ( HasRChild( *v ) ) {
        code->set ( length ); generateCT ( code, length + 1, table, v->rc );
    }
}

HuffTable* generateTable ( HuffTree* tree ) {
    HuffTable* table = new HuffTable; Bitmap* code = new Bitmap;
    generateCT ( code, 0, table, tree->root() ); release code; return table;
}

int ecode ( HuffTable* table, Bitmap* codeString, char* s ) {
    int n = 0;
    for ( size_t m = strlen(s), i = 0; i < m; i++ ) {
        char** pCharCode = table->get ( s[i] );
        if ( !pCharCode ) pCharCode = table->get ( s[i] + 'A' - 'a' );
        if ( !pCharCode ) pCharCode = table->get ( ' ' );
        printf ( "%s", *pCharCode );
        for (size_t m = strlen(*pCharCode), j = 0; j < m; j++ ) {
            '1' == *(*pCharCode + j) ? codeString->set(n++) : codeString->clear(n++);
        }
    }
    printf ( "\n" ); return n;
}

void decode ( HuffTree* tree, Bitmap* code, int n ) {
    BinNodePosi(HuffChar) x = tree->root();
    for ( int i = 0; i < n; i++ ) {
        x = code->test(i) ? x->rc : x->lc;
        if ( IsLeaf( *x ) ) {
            printf ( "%c", x->data.ch ); x = tree->root();
        }
    }
}