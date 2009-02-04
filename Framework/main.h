/* main.h -- exported function header
 *
 *			Ryan McDougall -- 2009
 */

#ifndef _MAIN_H_
#define _MAIN_H_

#include <iostream>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <vector>
#include <list>
#include <map>

#include <boost/ref.hpp>
#include <boost/signals.hpp>


namespace Framework
{
    using std::string;
    using std::vector;
    using std::list;
    using std::map;
    using std::make_pair;

    namespace TypeId
    {
        enum 
        {
            ENTITY_OBJECT_TYPE = 1,
            COMPONENT_OBJECT_TYPE
        };
    }

    class Object;
    class Subject;
    class SubjectManager;
    class Entity;
    class EntityManager;
    class Component;
    class ComponentSystem;
    class ComponentManager;
    struct SubjectChange;

    typedef unsigned int index_t;
    typedef unsigned int object_type_id_t;
    typedef unsigned int entity_id_t;
    typedef unsigned int archetype_id_t;
    typedef unsigned int component_id_t;
    typedef unsigned int component_mask_t;
    typedef unsigned int component_system_id_t;
    typedef unsigned int component_field_id_t;
    typedef unsigned int change_mask_t;
    
    typedef vector <Entity *>                               entity_list_t;

    typedef map <entity_id_t, Entity *>                     entity_map_t;
    typedef map <component_id_t, Component *>               component_map_t;
    typedef map <component_system_id_t, ComponentSystem *>  component_system_map_t;
    typedef map <archetype_id_t, component_mask_t>          component_mask_map_t;

    typedef vector <string>                         component_field_list_t;
    typedef map <string, component_field_id_t>      component_field_map_t;
    typedef map <string, component_system_id_t>     component_system_id_map_t;
    typedef map <string, archetype_id_t>            archetype_id_map_t;


    template <typename T> void deletefun (T obj) { delete obj; }
    template <typename T> void deletepairfun (T obj) { delete obj.first; delete obj.second; }
}

#endif //_MAIN_H_
