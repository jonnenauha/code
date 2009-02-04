/* node.hpp -- class definition
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _NODE_HPP_
#define _NODE_HPP_

namespace Framework
{
  //=============================================================================
    // Defines a node in an N-ary tree
    class Node
    {
        //-------------------------------------------------------------------------
        public:
            Node (Node *p) 
                : parent_ (p)
            {}

            Node (const Node& rhs)
                : parent_ (rhs.parent_)
            { std::copy (rhs.child_.begin(), rhs.child_.end(), child_.begin()); }

            virtual Node* Clone () const { return new Node (*this); }

            virtual ~Node ()
            {
                try
                {
                    std::for_each (child_.begin(), child_.end(), deletefun<Node *>);
                }
                catch (...)
                {}
            }

            Node* Parent () { return parent_; }
            Node* Child (index_t i) { return child_[i]; }

            void Add (Node *n) { child_.push_back (n); }

            void Add (Node *n, string name) 
            { 
                child_.push_back (n); 
                child_name_.insert (make_pair (name, child_.size()-1));
            }

            /*Node& operator= (const Node& rhs)
            { 
                if (this == &rhs)
                    return *this;

                Node t (rhs);
                swap (*this, t);

                return *this;
            }*/


            //-------------------------------------------------------------------------
        protected:
            Node *                  parent_;
            vector <Node *>    child_;

            string                     parent_name_;
            map <string, index_t> child_name_;

        private:
    };

// TODO:
//    class NodeDepthFirstIterator
//    {
//        public:
//            typedef std::bidirectional_iterator_tag iterator_category;
//
//            typedef size_t        size_type;
//            typedef Node          value_type;
//            typedef value_type*   pointer;
//            typedef value_type&   reference;
//
//            this_type& operator ++ () throw() {}
//            this_type& operator -- () throw() {}
//            this_type operator ++ (int) throw() {}
//            this_type operator -- (int) throw() {}
//
//            value_type operator * () const throw() {}
//
//            bool operator == (const this_type& rhs) const throw() {}
//            bool operator < (const this_type& rhs) const throw() {}
//
//            bool operator == (const NodeDepthFirstIterator& rhs) const throw() {}
//            bool operator < (const NodeDepthFirstIterator& rhs) const throw() {}
//
//        private:
//            Node *node_;
//    };
}
#endif //_NODE_HPP_
