/* Insert GPL header. */

#include <glib-object.h>
#include "gobject-tutorial-example2.h"

/* This is called when the class object is created. */
void	some_object_class_init		(gpointer g_class, gpointer class_data)
{
	/* Cast the class pointer and data pointer into something useful, and do initialization. */
}

/* This is called when the class object is no longer used. */
void	some_object_class_final		(gpointer g_class, gpointer class_data)
{
	/* Cast the class pointer and data pointer into something useful, and do finalization. */
}

/* This is called when a instance object is created. */
void	some_object_instance_init	(GTypeInstance *instance, gpointer g_class)
{
	/* Cast the instance pointer and data pointer into something useful, and do initialization. */
}

/* If you change the parent type, you must rewrite all the type casts in the constructor. */
GObject* some_object_constructor (GType self_type, guint n_properties, GObjectConstructParam *properties)
{
	SomeObjectClass	*this_class;
	GObjectClass	*parent_class;
	GObject		*constructed_obj;

	/* Get the proper vtables from the type system. */
	this_class 	= SOME_OBJECT_CLASS (g_type_class_peek (SOME_OBJECT_TYPE));
	parent_class	= G_OBJECT_CLASS (g_type_class_peek_parent (this_class));
	
	/* Must first invoke parent's constructor. */
	constructed_obj = parent_class->constructor (self_type, n_properties, properties);

	/* There is not much left to do here. */

	return constructed_obj;
}

/*  */
void	some_object_dispose (GObject *self)
{
}

/*  */
void	some_object_finalize (GObject *self)
{
}

/*  */
void	some_object_get_property (GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
}

/*  */
void	some_object_set_property (GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
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

		type = g_type_register_static
		(
			G_OBJECT,	/* next available GType number */
			"SomeObjectType",		/* type name as string */
			&type_info,			/* type info as above */
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
