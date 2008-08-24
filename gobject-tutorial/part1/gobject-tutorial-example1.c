/* Insert GPL header. */

#include <glib-object.h>
#include "gobject-tutorial-example1.h"

/* Implementation of virtual functions. */
void	some_object_method1_impl (SomeObject *self, gint a)
{
	self->m_a = a;
	g_print ("Method1: %i\n", self->m_a);
}

void	some_object_method2_impl (SomeObject *self, gchar* b)
{
	self->m_b = b;
	g_print ("Method2: %s\n", self->m_b);
}

/* Public methods. */
void	some_object_method1 (SomeObject *self, gint a)
{
	SOME_OBJECT_GET_CLASS (self)->method1 (self, a);
}

void	some_object_method2 (SomeObject *self, gchar* b)
{
	SOME_OBJECT_GET_CLASS (self)->method2 (self, b);
}

void	some_object_method3 (SomeObject *self, gfloat c)
{
	self->m_c = c;
	g_print ("Method3: %f\n", self->m_c);
}

/* This is called when the class object is created. */
void	some_object_class_init		(gpointer g_class, gpointer class_data)
{
	SomeObjectClass	*this_class	= SOME_OBJECT_CLASS (g_class);
	
	/* fill in the class struct members */
	this_class->method1 = &some_object_method1_impl;
	this_class->method2 = &some_object_method2_impl;
}

/* This is called when the class object is no longer used. */
void	some_object_class_final		(gpointer g_class, gpointer class_data)
{
	/* No class finalization needed. */
}

/* This is called when a instance object is created. The instance's class is passed as g_class. */
void	some_object_instance_init	(GTypeInstance *instance, gpointer g_class)
{
	SomeObject *this_object = SOME_OBJECT (instance);

	/* fill in the instance struct members */
	this_object->m_a = 42;
	this_object->m_b = NULL;
	this_object->m_c = 3.14;
}

/* Since there is no base class to derive from, base_init/finalize are NULL */
GType	some_object_get_type (void)
{
	static GType type = 0;

	if (type == 0) 
	{
		/* This is the structure that the system uses to fully describe
		how a type should be created, initialized and finalized. */

		static const GTypeInfo type_info = 
		{
			sizeof (SomeObjectClass),
			NULL,				/* base_init */
			NULL,				/* base_finalize */
			some_object_class_init,		/* class_init */
			some_object_class_final,	/* class_finalize */
			NULL,				/* class_data */
			sizeof (SomeObject),
			0,				/* n_preallocs */
			some_object_instance_init	/* instance_init */
    		};

		/* Since our type has no parent, it is considered 
		"fundamental", and we have to inform the system that our
		type is both classed (unlike say a float, int, or pointer),
		and is instantiable (the system can create instance objects.
		for example, Interfaces or Abstract classes are not 
		instantiable. */

		static const GTypeFundamentalInfo fundamental_info =
		{
			G_TYPE_FLAG_CLASSED | G_TYPE_FLAG_INSTANTIATABLE
		};	

		type = g_type_register_fundamental
		(
			g_type_fundamental_next (),	/* next available GType number */
			"SomeObjectType",		/* type name as string */
			&type_info,			/* type info as above */
			&fundamental_info,		/* fundamental info as above */
			0				/* type is not abstract */
		);
	}

	return	type;
}



/* Lets test out our code! */

int	main()
{
	SomeObject	*testobj = NULL;

	/* This gets the type system up and running. */
	g_type_init ();

	/* Create an instance object from the system. */
	testobj = SOME_OBJECT (g_type_create_instance (some_object_get_type()));

	/* Call our methods. */
	if (testobj)
	{
		g_print ("%d\n", testobj->m_a);
		some_object_method1 (testobj, 32);

		g_print ("%s\n", testobj->m_b);
		some_object_method2 (testobj, "New string.");

		g_print ("%f\n", testobj->m_c);
		some_object_method3 (testobj, 6.9);
	}

	return	0;
}
