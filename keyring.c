/*
   Copyright (C) 2011, Alexander Thomas

   This file is part of the Gnome Keyring Command Line Tool.

   This is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this tool.  If not, see <http://www.gnu.org/licenses/>.

   Author: Alexander Thomas <alexander@collab.net>
*/
#include <gnome-keyring.h>
#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "keyring.h"
#include "usage.h"
#include "config.h"


extern GMainLoop *loop;
int exit_status = FALSE;

char*
ask__password(const char *keyring, const char *password,
              char *prompt_str)
{
  char *prompt = NULL;
  char *pass = NULL;

  if (! password)
    {
      //prompt = g_strdup_printf("Enter password for '%s' keyring: ", keyring);
      prompt = g_strdup_printf(prompt_str, keyring);
      pass = getpass(prompt);

      if (prompt)
        free(prompt);

      if (pass)
        return strdup(pass);
      else
        return NULL;
    }
  else
    return strdup(password);
}


void
foreach__get_keyring(void *data, void* user_data)
{

  if (! data)
    return;

  if (g_strcasecmp((char*)user_data, "list") == 0)
    fprintf(stdout, "%s\n", (char*) data);

  return;
}


void
callback__get_list (GnomeKeyringResult result,
                    GList *list,
                    gpointer data)
{
  if (result != GNOME_KEYRING_RESULT_OK)
    {
      print_error(result, "ERROR", "\nCan't access keyring.\n");
      exit_status = FALSE;
      g_main_loop_quit(loop);
      return;
    }

  if (g_strcasecmp((char*)data, "list") == 0)
    g_list_foreach(list, foreach__get_keyring, data);

  exit_status = TRUE;
  g_main_loop_quit(loop);

  return;
}


void
callback__get_str (GnomeKeyringResult result,
                   const char *string,
                   gpointer data)
{
  if (! string)
    {
      exit_status = FALSE;
      g_main_loop_quit(loop);
      return; 
    }

  if (result != GNOME_KEYRING_RESULT_OK)
    {
      print_error(result, "ERROR", "\nCan't access keyring.\n");
      exit_status = FALSE;
      g_main_loop_quit(loop);
      return;
    }

  if (g_strcasecmp((char*)data, "getdef") == 0)
    fprintf(stdout, "%s\n", (char*) string); 

  exit_status = TRUE;
  g_main_loop_quit(loop);

  return;
}


void
callback__done (GnomeKeyringResult result,
                gpointer data)
{
  exit_status = TRUE;

  if (result != GNOME_KEYRING_RESULT_OK)
    {
      print_error(result, "ERROR", "\nCan't access keyring.\n");
      exit_status = FALSE;
    }

  g_main_loop_quit(loop);
  return;
}
#if 0
typedef struct {
	char *keyring;
	guint item_id;
	GnomeKeyringAttributeList *attributes;
	char *secret;
} GnomeKeyringFound;

struct GList {
  gpointer data;
  GList *next;
  GList *prev;
};

void callback_print_list(GnomeKeyringResult result, GList *list, gpointer data)
{
	int i;
	
	for (i=0;NULL != type_description[i].name; ++i)
		gnome_keyring_find_items
}
#endif


void
callback__get_info (GnomeKeyringResult result,
                    GnomeKeyringInfo *info,
                    gpointer data)
{
	int i;
  if (result != GNOME_KEYRING_RESULT_OK)
    {
      print_error(result, "ERROR", "\nCan't access keyring.\n");
      exit_status = FALSE;
      g_main_loop_quit(loop);
      return;
    }

  if (g_strcasecmp((char*)data, "info") == 0)
    {
      time_t create_tim;
      time_t modify_tim;
      struct tm *tmp;

      create_tim = gnome_keyring_info_get_ctime(info);
      modify_tim = gnome_keyring_info_get_mtime(info);

      fprintf (stdout, "Keyring locked : %s\n",
               (gnome_keyring_info_get_is_locked(info) ? "yes" : "no"));
      fprintf (stdout, "Auto lock on idle : %s\n",
               (gnome_keyring_info_get_lock_on_idle(info) ? "yes" : "no"));
      fprintf (stdout, "Auto lock idle timeout : %u (in sec)\n",
               gnome_keyring_info_get_lock_timeout(info));

      tmp = localtime(&create_tim);
      if (tmp != NULL)
        {
          char buf[256];
          if (strftime(buf, sizeof(buf), "%F %T %z (%a, %d %b %Y)", tmp) > 0)
           fprintf (stdout, "Keyring created on : %s\n", buf);
        }

      tmp = localtime(&modify_tim);
      if (tmp != NULL)
        {
          char buf[256];
          if (strftime(buf, sizeof(buf), "%F %T %z (%a, %d %b %Y)", tmp) > 0)
           fprintf (stdout, "Keyring modified on : %s\n", buf);
        }
    }
  exit_status = TRUE;
  g_main_loop_quit(loop);

  return;
}


int
keyring_list(void)
{
#ifndef HAVE_GNOME_KEYRING_LIST_KEYRING_NAMES
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_list_keyring_names(
     (GnomeKeyringOperationGetListCallback) callback__get_list, "list", NULL);

  g_main_loop_run(loop);

  return exit_status;
}


int
keyring_getdef(void)
{
#ifndef HAVE_GNOME_KEYRING_GET_DEFAULT_KEYRING
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_get_default_keyring(
    (GnomeKeyringOperationGetStringCallback) callback__get_str, "getdef", NULL);

  g_main_loop_run(loop);

  return exit_status;
}


int
keyring_setdef(const char *keyname)
{
#ifndef HAVE_GNOME_KEYRING_SET_DEFAULT_KEYRING
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_set_default_keyring(keyname,
    (GnomeKeyringOperationDoneCallback) callback__done, "setdef", NULL);

  g_main_loop_run(loop);

  if (exit_status)
    fprintf(stderr, "Set '%s' keyring as default.\n", keyname);
    
  return exit_status;
}


int
keyring_lock(const char *keyname)
{
#ifndef HAVE_GNOME_KEYRING_LOCK
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_lock(keyname,
    (GnomeKeyringOperationDoneCallback) callback__done, "lock", NULL);

  g_main_loop_run(loop);

  if (exit_status)
    fprintf(stderr, "Locked '%s' keyring.\n", keyname);

  return exit_status;
}


struct type_desc {
	GnomeKeyringItemType type;
	char *name;
};
struct type_desc type_description[]={
	{GNOME_KEYRING_ITEM_GENERIC_SECRET,"GENERIC_SECRET"},
	{GNOME_KEYRING_ITEM_NETWORK_PASSWORD,"NETWORK_PASSWORD"},
	{GNOME_KEYRING_ITEM_NOTE,"ITEM_NOTE"},
	{GNOME_KEYRING_ITEM_CHAINED_KEYRING_PASSWORD,"KEYRING_PASSWORD"},
	{GNOME_KEYRING_ITEM_ENCRYPTION_KEY_PASSWORD,"KEY_PASSWORD"},
	{GNOME_KEYRING_ITEM_PK_STORAGE,"PK_STORAGE"},
	{GNOME_KEYRING_ITEM_LAST_TYPE,NULL}
};

char *get_type_desc(GnomeKeyringItemType type)
{
	GnomeKeyringItemType i;
	for (i=0;NULL != type_description[i].name; ++i){
		if(type_description[i].type==type)
			return type_description[i].name;
	}
	return NULL;
}

void print_keyring_item_attributes (const char *keyname, guint32 id )
{
	GnomeKeyringAttributeList *attr;
	if(GNOME_KEYRING_RESULT_OK == gnome_keyring_item_get_attributes_sync(keyname,id,&attr)){
		guint x;
		for (x=0;x<attr->len;++x){
			GnomeKeyringAttribute *mine;
			mine=&g_array_index(attr,GnomeKeyringAttribute,x);
			printf("    %s type %s ",mine->name, get_type_desc(mine->type));
			if(GNOME_KEYRING_ATTRIBUTE_TYPE_STRING == mine->type)
				printf("=%s\n",mine->value.string);
			else
				printf("=%d\n",mine->value.integer);
		}
		 gnome_keyring_attribute_list_free(attr);
	}
}

int keyring_info(const char *keyname)
{
	GList *ids;
#ifndef HAVE_GNOME_KEYRING_GET_INFO
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_get_info(keyname,
          (GnomeKeyringOperationGetKeyringInfoCallback) callback__get_info,
           "info", NULL);

  g_main_loop_run(loop);
  if(GNOME_KEYRING_RESULT_OK == gnome_keyring_list_item_ids_sync(keyname,&ids)){
		GList *i;
		for (i=ids;NULL!=i; i=i->next){
			guint32 id=GPOINTER_TO_UINT(i->data);
			GnomeKeyringItemInfo *info;
			GnomeKeyringAttributeList *attr;
			if(GNOME_KEYRING_RESULT_OK != gnome_keyring_item_get_info_sync(keyname,id,&info))
				continue;

			printf("%d: item name '%s' : \n", id, gnome_keyring_item_info_get_display_name(info));
/*			printf("item name '%s' (secret %s): \n", gnome_keyring_item_info_get_display_name(info), gnome_keyring_item_info_get_secret(info));  */
			gnome_keyring_item_info_free (info);
			print_keyring_item_attributes(keyname,id);
			
		}
	}
  return exit_status;
}

int keyring_item_delete(const char *keyname, char *displayname)
{
	GList *ids;
	guint32 found_id=0xFFFFFFFF;
#ifndef HAVE_GNOME_KEYRING_GET_INFO
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_get_info(keyname,
          (GnomeKeyringOperationGetKeyringInfoCallback) callback__get_info,
           "info", NULL);

  g_main_loop_run(loop);
  if(GNOME_KEYRING_RESULT_OK == gnome_keyring_list_item_ids_sync(keyname,&ids)){
		GList *i;
		
		for (i=ids;NULL!=i; i=i->next){
			guint32 id=GPOINTER_TO_UINT(i->data);
			GnomeKeyringItemInfo *info;
			GnomeKeyringAttributeList *attr;
			if(GNOME_KEYRING_RESULT_OK != gnome_keyring_item_get_info_sync(keyname,id,&info))
				continue;
			if( !strcmp(gnome_keyring_item_info_get_display_name(info),displayname))
				found_id=id;
			gnome_keyring_item_info_free (info);
		}
	}
	if(0xFFFFFFFF!= found_id)
		gnome_keyring_item_delete_sync (keyname,found_id);
  return exit_status;
}
int keyring_item_delete_id(const char *keyname, int del_id)
{
	GList *ids;
	guint32 found_id=0xFFFFFFFF;
#ifndef HAVE_GNOME_KEYRING_GET_INFO
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_get_info(keyname,
          (GnomeKeyringOperationGetKeyringInfoCallback) callback__get_info,
           "info", NULL);

  g_main_loop_run(loop);
  if(GNOME_KEYRING_RESULT_OK == gnome_keyring_list_item_ids_sync(keyname,&ids)){
		GList *i;
		
		for (i=ids;NULL!=i; i=i->next){
			guint32 id=GPOINTER_TO_UINT(i->data);
			GnomeKeyringItemInfo *info;
			GnomeKeyringAttributeList *attr;
			if(GNOME_KEYRING_RESULT_OK != gnome_keyring_item_get_info_sync(keyname,id,&info))
				continue;
			if(del_id == id)
				found_id=id;
			gnome_keyring_item_info_free (info);
		}
	}
	if(0xFFFFFFFF!= found_id)
		gnome_keyring_item_delete_sync (keyname,found_id);
  return exit_status;
}

int keyring_batch(const char *keyname)
{
	GList *ids;
	gchar *old, *npw, *check;
	int i;
	if (keyname == NULL) {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
  }

  if (!gnome_keyring_is_available()) {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR", "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
  }
	old=strdup(getpass("Old Password? "));
	npw=strdup(getpass("New Password? "));
	check=getpass("Retype New ");
	if(strcmp(npw,check)){
		printf("New Password does not match. Abort\n");
		return FALSE;
	}
	if(GNOME_KEYRING_RESULT_OK == gnome_keyring_list_item_ids_sync(keyname,&ids)){
		GList *i;
		for (i=ids;NULL!=i; i=i->next){
			guint32 id=GPOINTER_TO_UINT(i->data);
			GnomeKeyringItemInfo *info;
			gchar *secret;
			if(GNOME_KEYRING_RESULT_OK != gnome_keyring_item_get_info_sync(keyname,id,&info))
				continue;
			secret=gnome_keyring_item_info_get_secret(info);
			if(strcmp(secret,old)){
				printf("No match!\n");
			}else
				printf("Changing password for:\n");
				
			print_keyring_item_attributes(keyname,id);
			gnome_keyring_item_info_set_secret(info,npw);
			if(GNOME_KEYRING_RESULT_OK !=gnome_keyring_item_set_info_sync(keyname,id,info))
				printf("Error setting item info!\n");
			gnome_keyring_item_info_free (info);
			
		}
	}
	return TRUE;
}

int
keyring_delete(const char *keyname)
{
#ifndef HAVE_GNOME_KEYRING_DELETE
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_delete(keyname,
    (GnomeKeyringOperationDoneCallback) callback__done, "delete", NULL);

  g_main_loop_run(loop);

  if (exit_status)
    fprintf(stderr, "Deleted '%s' keyring.\n", keyname);

  return exit_status;
}


int
keyring_create(const char *keyname, const char *password)
{
  char *pass = NULL;

#ifndef HAVE_GNOME_KEYRING_CREATE
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif


  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  pass = ask__password(keyname, password, "Enter password for '%s' keyring: ");
  if (! pass)
    return FALSE;

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_create(keyname, pass,
                       (GnomeKeyringOperationDoneCallback) callback__done,
                       "create", NULL);

  g_main_loop_run(loop);

  if (exit_status)
    fprintf(stderr, "Created '%s' keyring.\n", keyname);

  if (pass) free(pass);

  return exit_status;
}

int
keyring_unlock(const char *keyname,
               const char *password)
{
  char *pass = NULL;

#ifndef HAVE_GNOME_KEYRING_UNLOCK
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  pass = ask__password(keyname, password, "Enter password for '%s' keyring: ");
  if (! pass)
    return FALSE;

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_unlock(keyname, pass,
                       (GnomeKeyringOperationDoneCallback) callback__done,
                       "unlock", NULL);

  g_main_loop_run(loop);

  if (exit_status)
    fprintf(stderr, "Unlocked '%s' keyring.\n", keyname);

  if (pass) free(pass);

  return exit_status;
}


int
keyring_modify(const char *keyname,
               const char *password,
               const char *new_password)
{
  char *pass = NULL;
  char *new_pass = NULL;

#ifndef HAVE_GNOME_KEYRING_CHANGE_PASSWORD
  print_error(10, "WARNING", "\nThis feature not supported.\n");
  return TRUE; 
#endif

  if (keyname == NULL)
    {
       print_error(GNOME_KEYRING_RESULT_NO_SUCH_KEYRING, "ERROR", "\nInvalid keyring.\n");
       return FALSE;
    }

  if (!gnome_keyring_is_available())
    {
       print_error(GNOME_KEYRING_RESULT_NO_KEYRING_DAEMON, "ERROR",
                   "\nFailed to communicate with a gnome-keyring-daemon.\n");
       return FALSE;
    }

  pass = ask__password(keyname, password, "Enter password for '%s' keyring: ");
  if (! pass)
    return FALSE;

  new_pass = ask__password(keyname, new_password,
                           "Enter new password for '%s' keyring: ");
  if (! new_pass)
    {
      free(pass);
      return FALSE;
    }

  loop = g_main_loop_new(NULL, FALSE);

  gnome_keyring_change_password(keyname, pass, new_pass,
                       (GnomeKeyringOperationDoneCallback) callback__done,
                       "modify", NULL);

  g_main_loop_run(loop);

  if (exit_status)
    fprintf(stderr, "Changed '%s' keyring password.\n", keyname);

  if (pass) free(pass);
  if (new_pass) free(new_pass);

  return exit_status;
}
