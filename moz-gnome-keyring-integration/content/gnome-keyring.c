/** \file ******************************************************************
\verbatim
File:        gnome-keyring.c
Author:      Doug Springer
Commpany:    DNK Designs, Inc.
Date:        05/07/2015 12:53 pm
Description: 
\endverbatim
*/ /************************************************************************
*/

/**pkg-config --cflags "gnome-keyring-1"   
pkg-config --libs "glib-2.0" 
*/
#include <gnome-keyring.h>
#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/**
 * const gchar * gnome_keyring_attribute_get_string (GnomeKeyringAttribute *attribute);
 */
const gchar * gnome_keyring_attribute_get_string (GnomeKeyringAttribute *attribute)
{
	if(NULL == attribute->value.string)
		return "";
	return attribute->value.string;
}
/**
 * guint32 gnome_keyring_attribute_get_uint32 (GnomeKeyringAttribute *attribute);
 */
guint32 gnome_keyring_attribute_get_uint32 (GnomeKeyringAttribute *attribute)
{
	return attribute->value.integer;
}
/**wrapper function for opaque data type returned byt _sync  */

/***************************************************************************/
/** \details Wrap the GArray stuff with GList.
\param 
\returns 
****************************************************************************/
GnomeKeyringResult  gnome_keyring_item_get_attributes_list  (const char *keyring, guint32 id, GList **attr)
{
	GnomeKeyringResult rtn;
	GnomeKeyringAttributeList *attrlist;
	int i;
	if(NULL == attr)
		return GNOME_KEYRING_RESULT_BAD_ARGUMENTS;
	if(NULL != *attr)
		g_list_free(*attr);
	if(GNOME_KEYRING_RESULT_OK != (rtn=gnome_keyring_item_get_attributes_sync  (keyring, id, &attrlist)) )
		return rtn;
	for (i=0; i<attrlist->len; ++i){
		gpointer data=calloc(1,sizeof(GnomeKeyringAttribute));
		GnomeKeyringAttribute *element=&g_array_index(attrlist,GnomeKeyringAttribute,i);
		memcpy(data,element,sizeof(GnomeKeyringAttribute));
		*attr=g_list_prepend(*attr,data);	
	}
	g_array_free(attrlist,TRUE);
	return GNOME_KEYRING_RESULT_OK;
}

/***************************************************************************/
/** \details Delete the glist of attributes.
\param 
\returns 
****************************************************************************/
void gnome_keyring_item_get_attributes_list_free (GList **attr)
{
	GList *head;
	for (head=*attr; NULL != head; head=head->next){
		free(head->data);
		head->data=NULL;
	}
	g_list_free(*attr);
	*attr=NULL;
}


/***************************************************************************/
/** \details Manage our internal array.
\param 
\returns 
****************************************************************************/
GnomeKeyringAttributeList *gnome_keyring_attr_list_get_local ( int kill )
{
	static GnomeKeyringAttributeList * attr;
	static int init=0;	
	if( 0 == init){
    attr = g_array_new(FALSE, FALSE, sizeof (GnomeKeyringAttribute));
		init=1;
	}	
	if(kill){
		attr=NULL;
		init=0;
	}
	return attr;
}

/***************************************************************************/
/** \details Add attributes to our internal array.
\param 
\returns 
****************************************************************************/
void gnome_keyring_add_attributes (char *name, char *val)
{
	GnomeKeyringAttributeList * attr=gnome_keyring_attr_list_get_local(0);
	gnome_keyring_attribute_list_append_string(attr, name, val);
}

/***************************************************************************/
/** \details wrapper to add attributes, using our internal array
\param 
\returns 
****************************************************************************/
GnomeKeyringResult gnome_keyring_create_item_sync_ex(char *keyring, GnomeKeyringItemType type, char *displayName, char *secret, gboolean update_if_exists) 
{
	GnomeKeyringResult error;
	guint32 item_id;
	GnomeKeyringAttributeList * attr=gnome_keyring_attr_list_get_local(0);
	error=gnome_keyring_item_create_sync(keyring, type, displayName, attr, secret, update_if_exists, &item_id);
/*	if(error != GNOME_KEYRING_RESULT_OK) */
	gnome_keyring_attribute_list_free(attr);
	gnome_keyring_attr_list_get_local(1);
	return error;
};

/***************************************************************************/
/** \details Attempt to print stuff - kills the plugin.
\param 
\returns 
****************************************************************************/
void gnome_keyring_item_print_attributes(	GnomeKeyringAttributeList *attrlist)
{
	int i;
	for (i=0; i<attrlist->len; ++i){
		printf("N=%s Val =",((GnomeKeyringAttribute *)attrlist->data)[i].name);
		if( GNOME_KEYRING_ATTRIBUTE_TYPE_STRING == ((GnomeKeyringAttribute *)attrlist->data)[i].type)
			printf("%s\n",((GnomeKeyringAttribute *)attrlist->data)[i].value.string);
		else
			printf("%d\n",((GnomeKeyringAttribute *)attrlist->data)[i].value.integer);
	}
}

