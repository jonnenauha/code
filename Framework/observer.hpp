/* observer.hpp -- class definition
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _OBSERVER_HPP_
#define _OBSERVER_HPP_

namespace Framework
{
    //=============================================================================
    // Sends notices
    struct IObservable 
    {
        virtual void Notify () = 0;
        virtual void Dispose () = 0;
    };

    //=============================================================================
    // Receives notices
    template <typename Observable>
    struct IObserver
    {
        virtual void NoticeChange (Observable subject, SubjectChange change) = 0;
        virtual void NoticeDispose (Observable subject) = 0;
    };

    //=============================================================================
    class SubjectManager
    {
        //-------------------------------------------------------------------------
        public:
            SubjectManager () {}

            virtual ~SubjectManager ()
            {
                try
                {}
                catch (...)
                {}
            }

            void RegisterSubject (Entity &ent)
            {
                entity_map_.insert (make_pair (ent.Id(), &ent));
            }

            void RegisterSubject (Component &com)
            {
                component_map_.insert (make_pair (com.Id(), &com));
            }

            /*void Connect (Subject &subject, IObserver<Subject> &observer)
            {
            }*/

            //-------------------------------------------------------------------------
        private:
            entity_map_t    entity_map_;
            component_map_t component_map_;

            SubjectManager (const SubjectManager& rhs);
            SubjectManager& operator= (const SubjectManager& rhs);
    };

    /*
    //=============================================================================
    // List of multiple notifiables
    template <T>
        class NoticeList : 
            public INotifiable <T>
    {
        public:
            void Add (T t) { list_.push_back (t); }

            void Notice (T t) 
            { 
                std::for_each 
                    (list_.begin(), list_.end(), bind2nd (notice_, t));
            }

            void Dispose (T t)
            {
                std::for_each 
                    (list_.begin(), list_.end(), binder2nd (dispose_, t));
            }

        private:
            list <INotifiable <T> > list_;

            static void notice_ (INotifiable <T> &n, T t) { n.Notice (t); }
            static void dispose_ (INotifiable <T> &n, T t) { n.Dispose (t); }

            NoticeList& operator= (const NoticeList& rhs);
            NoticeList (const NoticeList& rhs);
    };

    class ObservableNode;

    //=============================================================================
    // Propagate changes in observables to notifiables
    class NotificationManager : 
        public INotifiable <ObservableNode *>
    {
        private:
            typedef map <ObservableNode *, NoticeList<ObservableNode *> *> NoticeMap;

        public:
            void Notice (ObservableNode *n) { notices_[n].Notice (n); }
            void Dispose (ObservableNode *n) { notices_[n].Dispose (n); }

            void DeliverAll (NodeChangeScheduler schedule)
            {
                NoticeMap::iterator nit (notices_.begin());
                for (; nit != notices_.end(); ++nit);
            }

            void Subscribe (ObservableNode *n, INotifiable <ObservableNode *> s) 
            { 
                s-> SetProtocol (n);

                if (!notices_.count (n))
                    notices_.insert (make_pair (n, new NoticeList <ObservableNode *>())); 
                
                notices_[n].Add (s);
            }
            
            ObservableNode* MakeNode (ObservableNode *p, INotifiable <ObservableNode *> s)
            { 
                ObservableNode *n (MakeNode (p, s));
                Subscribe (n, s);
                return n;
            }

        private:
            NoticeMap   notices_;

            NotificationManager (const NotificationManager& rhs);
            NotificationManager& operator= (const NotificationManager& rhs);
    };

    //=============================================================================
    // Method of resolving a set of changes into one change
    class NodeChangeScheduler
    { 
        public:
            NodeChangeScheduler (int p) :
                accum_ (p, NULL);

            virtual void operator() (const NodeChange &n)
            {
                if (result.protocol != n.protocol)
                    throw new std::logic_error;

                accum_.data = n.data; // simply takes the last change
            }

            virtual NodeChange GetResult ()
            { 
                return accum_; // simply return internal ptr
            }

            virtual void ClearResult ()
            {
                // no nothing
            }

        private:
            NodeChange accum_;
            
            NodeChangeScheduler& operator= (const NodeChangeScheduler& rhs);
            NodeChangeScheduler (const NodeChangeScheduler& rhs);
    }

    //=============================================================================
    // List of multiple changes to a node
    class ChangeList 
    {
        public:
            void Add (NodeChange n) { list_.push_back (n); }

            NodeChange Accumulate (NodeChangeScheduler &sched) 
            { 
                std::for_each 
                    (list_.begin(), list_.end(), sched);

                return sched.GetResult();
            }

        private:
            list <NodeChange> list_;

            ChangeList& operator= (const ChangeList& rhs);
            ChangeList (const ChangeList& rhs);
    };

    //=============================================================================
    // Object which listens to observable nodes
    class NodeObserver :
        public INotifiable <ObservableNode *>
    {
        public:
            NodeObserver (int p) : protocol_ (p) {}

            virtual void Notice (ObservableNode *n) { changes_.Add (n); }
            virtual void Dispose (ObservableNode *n) { throw new std::runtime_error; }
            virtual void SetProtocol (ObservableNode *n) { n->change_->protocol = protocol_; }
            virtual void GetResult (NodeChangeScheduler &s) { return s.GetResult(); }

        private:
            ChangeList  changes_;
            int         protocol_;

            NodeObserver& operator= (const NodeObserver& rhs);
            NodeObserver (const NodeObserver& rhs);
    };
    */

}
#endif //_OBSERVER_HPP_
