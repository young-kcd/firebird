/********************************************************************
 * COPYRIGHT:
 * Copyright (c) 2001-2003, International Business Machines Corporation and
 * others. All Rights Reserved.
 ********************************************************************/

#ifndef RBBINODE_H
#define RBBINODE_H

#include "unicode/utypes.h"
#include "unicode/uobject.h"

//
//  class RBBINode
//
//                    Represents a node in the parse tree generated when reading
//                    a rule file.
//

U_NAMESPACE_BEGIN

class    UnicodeSet;
class    UVector;

class RBBINode : public UMemory {
    public:
        enum NodeType {
            setRef,
            uset,
            varRef,
            leafChar,
            lookAhead,
            tag,
            endMark,
            opStart,
            opCat,
            opOr,
            opStar,
            opPlus,
            opQuestion,
            opBreak,
            opReverse,
            opLParen
        };

        enum OpPrecedence {      
            precZero,
            precStart,
            precLParen,
            precOpOr,
            precOpCat
        };
            
        NodeType      fType;
        RBBINode      *fParent;
        RBBINode      *fLeftChild;
        RBBINode      *fRightChild;
        UnicodeSet    *fInputSet;           // For uset nodes only.
        OpPrecedence  fPrecedence;          // For binary ops only.
        
        UnicodeString fText;                // Text corresponding to this node.
                                            //   May be lazily evaluated when (if) needed
                                            //   for some node types.
        int           fFirstPos;            // Position in the rule source string of the
                                            //   first text associated with the node.
                                            //   If there's a left child, this will be the same
                                            //   as that child's left pos.
        int           fLastPos;             //  Last position in the rule source string
                                            //    of any text associated with this node.
                                            //    If there's a right child, this will be the same
                                            //    as that child's last postion.

        UBool         fNullable;            // See Aho.
        int32_t       fVal;                 // For leafChar nodes, the value.
                                            //   Values are the character category,
                                            //   corresponds to columns in the final
                                            //   state transition table.

        UBool         fLookAheadEnd;        // For endMark nodes, set TRUE if
                                            //   marking the end of a look-ahead rule.

        UVector       *fFirstPosSet;
        UVector       *fLastPosSet;         // TODO: rename fFirstPos & fLastPos to avoid confusion.
        UVector       *fFollowPos;


        RBBINode(NodeType t);
        RBBINode(const RBBINode &other);
        ~RBBINode();
        
        RBBINode    *cloneTree();
        RBBINode    *flattenVariables();
        void         flattenSets();
        void         findNodes(UVector *dest, RBBINode::NodeType kind, UErrorCode &status);

#ifdef RBBI_DEBUG
        void        printNode();
        void        printTree(UBool withHeading);
#else
        // Do-nothing inline functions for non-debug builds.  Can't make empty defines for member
        //   functions - they won't compile at the call sites.
        int         fakeField;
        #define printNode() fakeField=0;
        #define printTree(withHeading) fakeField=0;
#endif

    private:
        RBBINode &operator = (const RBBINode &other); // No defs.
        UBool operator == (const RBBINode &other);    // Private, so these functions won't accidently be used.

        int           fSerialNum;           //  Debugging aids.
        static int    gLastSerial;
};

#ifdef RBBI_DEBUG
U_CFUNC void 
RBBI_DEBUG_printUnicodeString(const UnicodeString &s, int minWidth=0);
#endif

U_NAMESPACE_END

#endif
