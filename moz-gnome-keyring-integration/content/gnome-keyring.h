/** \file ******************************************************************
\verbatim
File:        gnome-keyring.h
Author:      Doug Springer
Commpany:    DNK Designs, Inc.
Date:        05/07/2015 12:53 pm
Description: 
\endverbatim
*/ /************************************************************************
Change Log: \n
*/
#ifndef _GNOME_KEYRING_H_
#define _GNOME_KEYRING_H_ 1
GnomeKeyringResult  gnome_keyring_item_get_attributes_list  (const char *keyring, guint32 id, GList **attr);
void gnome_keyring_item_get_attributes_list_free (GList **attr);
void gnome_keyring_item_print_attributes(	GnomeKeyringAttributeList *attrlist);
#endif
