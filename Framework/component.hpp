/* class.hpp -- class definition
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _COMPONENT_HPP_
#define _COMPONENT_HPP_

namespace Framework
{
    //=============================================================================
    // TODO: friends are bad m'kay
    class Component :
        public Object, public Subject
    {
        friend class ComponentSystem;

        //-------------------------------------------------------------------------
        public:
            component_id_t Id () const { return component_id_; }
            component_system_id_t SystemId () const { return component_system_id_; }

            void Change (string field) 
            { 
                Subject::change_.SetBit (component_field_map_[field]); 
            } 

            //-------------------------------------------------------------------------
        protected:
            Component (component_id_t c, component_system_id_t s, component_field_map_t &m) :
                component_id_ (c),
                component_system_id_ (s),
                component_field_map_ (m),
                component_field_mask_ (0)
            {
                object_id_ = TypeId::COMPONENT_OBJECT_TYPE;
            }

            component_id_t          component_id_;
            component_system_id_t   component_system_id_;
            component_field_map_t   &component_field_map_;
            change_mask_t           component_field_mask_;

        private:
            Component (const Component& rhs);
            Component& operator= (const Component& rhs);
    };

    //=============================================================================
    class ComponentSystem
    {
        //-------------------------------------------------------------------------
        public:
            ComponentSystem 
                (const string &n, const component_field_list_t &fields) : 
                    name_ (n), field_list_ (fields) 
            {
                for (int i=0; i < field_list_.size(); ++i)
                    field_map_.insert (make_pair (field_list_[i], i));
            }

            string                  GetName () const { return name_; }
            component_field_list_t  GetFields () const { return field_list_; }
            component_field_map_t   GetFieldMap () const { return field_map_; }
            size_t                  GetFieldNum () const { return field_list_.size(); }
            component_system_id_t   GetID () const { return id_; }
            void                    SetID (component_system_id_t id) { id_ = id; }

            Component *GetComponent () 
            { 
                component_.insert (make_pair 
                        (next_free_component_id_, new Component 
                         (next_free_component_id_, id_, field_map_)));

                ++ next_free_component_id_;
            }

            Component *GetComponent (component_id_t id) { return component_[id]; }

            //-------------------------------------------------------------------------
        private:
            string                  name_;
            component_map_t         component_;
            component_field_list_t  field_list_;
            component_field_map_t   field_map_;
            component_id_t          next_free_component_id_;
            component_system_id_t   id_;

            ComponentSystem (const ComponentSystem& rhs);
            ComponentSystem& operator= (const ComponentSystem& rhs);
    };

}

#endif //_COMPONENT_HPP_
