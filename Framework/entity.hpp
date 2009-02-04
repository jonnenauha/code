/* node.hpp -- class definition
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _ENTITY_HPP_
#define _ENTITY_HPP_

namespace Framework
{
    //=============================================================================
    class Entity : 
        public Object, public Subject
    {
        friend class EntityManager;

        //-------------------------------------------------------------------------
        public:
            void Change (string component) 
            {
                Subject::change_.SetBit (compsys_id_map_[component]);
            }

            bool HasComponent (component_system_id_t id) const 
            { 
                return ((component_mask_ << id) & 1); 
            }

            Component *GetComponent (component_system_id_t id)
            {
                return component_map_[id];
            }

            entity_id_t Id () const { return entity_id_; }
            string Name () const { return entity_name_; }

            archetype_id_t  ArchetypeId () const { return archetype_id_; }
            string     ArchetypeName () const { return archetype_name_; }

            //-------------------------------------------------------------------------
        protected:
            Entity (entity_id_t e, archetype_id_t t, component_mask_t m, 
                    string n, string a, component_system_id_map_t &cm) :
                entity_id_ (e), 
                archetype_id_ (t), 
                component_mask_ (m),
                entity_name_ (n), 
                archetype_name_ (a),
                compsys_id_map_ (cm)
            {
                object_id_ = TypeId::ENTITY_OBJECT_TYPE;
            }

            entity_id_t         entity_id_;
            archetype_id_t      archetype_id_;
            component_mask_t    component_mask_;

            string entity_name_;
            string archetype_name_;

            component_map_t             component_map_;
            component_system_id_map_t   &compsys_id_map_;
    };


    //=============================================================================
    class EntityManager
    {
        //-------------------------------------------------------------------------
        public:
            EntityManager () : 
                next_free_compsys_id_ (0),
                next_free_entity_id_ (0) {}

            void RegisterEntityArchetype (string name, component_mask_t mask)
            {
                archetype_id_map_.insert 
                    (make_pair (name, next_free_architype_id_));

                component_mask_map_.insert 
                    (make_pair (next_free_architype_id_, mask));

                ++ next_free_architype_id_;
            }

            component_system_id_t RegisterComponentSystem (ComponentSystem &sys)
            {
                component_system_id_map_.insert 
                    (make_pair (sys.GetName(), next_free_compsys_id_));

                component_system_map_.insert 
                    (make_pair (next_free_compsys_id_, &sys));

                ++ next_free_compsys_id_;

                return GetComponentSystemId (sys.GetName());
            }

            ComponentSystem &GetComponentSystem (component_system_id_t id)
            {
                return *(component_system_map_[id]);
            }

            component_system_id_t GetComponentSystemId (string name)
            {
                return component_system_id_map_[name];
            }
            
            Entity *GetEntity (string name, string archetype)
            {
                archetype_id_t archetypeid (archetype_id_map_[archetype]);
                component_mask_t mask (component_mask_map_[archetypeid]); 

                Entity *ent (new Entity 
                        (next_free_entity_id_++, 
                         archetypeid, mask, name, archetype, 
                         component_system_id_map_));

                for (index_t i = 0; i < sizeof (component_mask_t); ++i)
                    if (ent->HasComponent (i))
                        ent->component_map_.insert 
                            (make_pair (i, component_system_map_[i]->GetComponent ()));

                entity_list_.push_back (ent);
                return ent;
            }

            Component *GetComponent (component_system_id_t sysid, component_id_t id) 
            { 
                return GetComponentSystem (sysid).GetComponent (id); 
            }


            //-------------------------------------------------------------------------
        protected:
            archetype_id_t              next_free_architype_id_;
            entity_id_t                 next_free_entity_id_;
            entity_list_t               entity_list_;
            archetype_id_map_t          archetype_id_map_;
            component_mask_map_t        component_mask_map_;
            component_system_id_map_t   component_system_id_map_;
            component_system_map_t      component_system_map_;
            component_system_id_t       next_free_compsys_id_;

        private:
            EntityManager (const EntityManager& rhs);
            EntityManager& operator= (const EntityManager& rhs);
    };

}
#endif // _ENTITY_HPP_
