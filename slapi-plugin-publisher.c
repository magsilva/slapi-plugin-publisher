/* $OpenLDAP: pkg/ldap/contrib/slapi-plugins/addrdnvalues/addrdnvalues.c,v 1.6 2004/05/23 13:45:32 lukeh Exp $ */
/*
 * Copyright 2003-2004 PADL Software Pty Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted only as authorized by the OpenLDAP
 * Public License.
 *
 * A copy of this license is available in the file LICENSE in the
 * top-level directory of the distribution or, alternatively, at
 * <http://www.OpenLDAP.org/license.html>.
 */

#include <string.h>
#include <unistd.h>

#include <ldap.h>
#include <lber.h>

#include <slapi-plugin.h>

#include <ws/soapH.h>
#include <ws/ns3.nsmap>

static Slapi_PluginDesc modification_publisher_plugin_description = {
	"modification-publisher",
	"ideais.sf.net",
	"1.0",
	"Publishes any modification in the LDAP server"
};

static int modification_publisher_publish_modification()
{
	struct _ns1__postMsgToQueue message;
	struct _ns1__postMsgToQueueResponse response;
	struct soap *soap; // gSOAP runtime environment

	slapi_log_error(SLAPI_LOG_PLUGIN, "Publish modification", "Begin");

	message.in0 = "queue/testQueue";
	message.in1 = "teste message";

	slapi_log_error(SLAPI_LOG_PLUGIN, "Publish modification", "a");
	soap = soap_new();
	if (!soap) {
		slapi_log_error(SLAPI_LOG_PLUGIN, "Publish modification", "Could not create the SOAP handler");
	}

	slapi_log_error(SLAPI_LOG_PLUGIN, "Publish modification", "ab");
	soap_call___ns1__postMsgToQueue(soap, NULL, NULL, &message, &response);

	slapi_log_error(SLAPI_LOG_PLUGIN, "Publish modification", "abc");
	soap_destroy(soap);

	slapi_log_error(SLAPI_LOG_PLUGIN, "Publish modification", "abcd");
	soap_end(soap);

	slapi_log_error(SLAPI_LOG_PLUGIN, "Publish modification", "abcde");
	soap_done(soap);

	slapi_log_error(SLAPI_LOG_PLUGIN, "Publish modification", "Done");
}

static int modification_publisher_postop_add(Slapi_PBlock *pb)
{
	Slapi_Entry *e;
	char *dn;

	slapi_log_error(SLAPI_LOG_PLUGIN, "Post-operation add plugin", "Begin");

	if (slapi_pblock_get(pb, SLAPI_ADD_ENTRY, &e) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, (char *) __func__, "Error retrieving target entry\n");
		return -1;
	}
	
	if (slapi_pblock_get(pb, SLAPI_ADD_TARGET, &dn) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, (char *) __func__, "Error retrieving target distinguished name\n");
		return -1;
	}
	slapi_dn_normalize_case(dn);
	slapi_log_error(SLAPI_LOG_PLUGIN, (char *) __func__, "Normalized DN: %s\n", dn ); 

	modification_publisher_publish_modification();

	return LDAP_SUCCESS;
}

static int modification_publisher_postop_modify(Slapi_PBlock *pb)
{
	int rc;
	LDAPMod **mods;
	Slapi_Mod *cur;

	slapi_log_error(SLAPI_LOG_PLUGIN, "Post-operation modify plugin", "Begin");

	if (slapi_pblock_get(pb, SLAPI_MODIFY_MODS, &mods) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, (char *) __func__, "Error retrieving modifications\n");
		return -1;
	}

	modification_publisher_publish_modification();


/*
	for (cur = *mods; cur != NULL; cur++) {
		cur->mod_type; // Attribute type
		// cur->mod_values
		switch (cur->mod_op) {
			case LDAP_MOD_ADD:
				break;
			case LDAP_MOD_DELETE:
				break;
			case LDAP_MOD_REPLACE:
				break;
		}
	}
	rc = slapi_entry_add_rdn_values(e);
	if (rc != LDAP_SUCCESS) {
		slapi_send_ldap_result(pb, LDAP_OTHER, NULL,
			"Failed to parse distinguished name", 0, NULL);
		slapi_log_error(SLAPI_LOG_PLUGIN, "addrdnvalues_preop_add",
			"Failed to parse distinguished name: %s\n",
			ldap_err2string(rc));
		return -1;
	}
*/

	return 0;
}


static int modification_publisher_postop_modify_rdn(Slapi_PBlock *pb)
{
	int rc;
	Slapi_Entry *e;

	slapi_log_error(SLAPI_LOG_PLUGIN, "Post-operation add plugin", "Begin");

	if (slapi_pblock_get(pb, SLAPI_ADD_ENTRY, &e) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, "addrdnvalues_preop_add", "Error retrieving target entry\n");
		return -1;
	}

	rc = slapi_entry_add_rdn_values(e);
	if (rc != LDAP_SUCCESS) {
		slapi_send_ldap_result(pb, LDAP_OTHER, NULL,
			"Failed to parse distinguished name", 0, NULL);
		slapi_log_error(SLAPI_LOG_PLUGIN, "addrdnvalues_preop_add",
			"Failed to parse distinguished name: %s\n",
			ldap_err2string(rc));
		return -1;
	}

	return 0;
}


static int modification_publisher_postop_delete(Slapi_PBlock *pb)
{
	int rc;
	Slapi_Entry *e;

	slapi_log_error(SLAPI_LOG_PLUGIN, "Post-operation add plugin", "Begin");

	if (slapi_pblock_get(pb, SLAPI_ADD_ENTRY, &e) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, "addrdnvalues_preop_add", "Error retrieving target entry\n");
		return -1;
	}

	rc = slapi_entry_add_rdn_values(e);
	if (rc != LDAP_SUCCESS) {
		slapi_send_ldap_result(pb, LDAP_OTHER, NULL,
			"Failed to parse distinguished name", 0, NULL);
		slapi_log_error(SLAPI_LOG_PLUGIN, "addrdnvalues_preop_add",
			"Failed to parse distinguished name: %s\n",
			ldap_err2string(rc));
		return -1;
	}

	return 0;
}


int modification_publisher_init(Slapi_PBlock *pb)
{
	slapi_log_error(SLAPI_LOG_PLUGIN, "Modification publisher plugin", "Loading");


	if (slapi_pblock_set(pb, SLAPI_PLUGIN_VERSION, SLAPI_PLUGIN_VERSION_03) != 0) {
		goto error;
	}
	
	if (slapi_pblock_set(pb, SLAPI_PLUGIN_DESCRIPTION, &modification_publisher_plugin_description) != 0) {
		goto error;
	}

	if (slapi_pblock_set(pb, SLAPI_PLUGIN_POST_ADD_FN, (void *)modification_publisher_postop_add) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, "modification_postop_add", "Error registering %s\n", modification_publisher_plugin_description.spd_id);
		goto error;
	}

	if (slapi_pblock_set(pb, SLAPI_PLUGIN_POST_MODIFY_FN, (void *)modification_publisher_postop_modify) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, "modification_postop_modify", "Error registering %s\n", modification_publisher_plugin_description.spd_id);
		goto error;
	}

	if (slapi_pblock_set(pb, SLAPI_PLUGIN_POST_MODRDN_FN, (void *)modification_publisher_postop_modify_rdn) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, "modification_postop_modify_rdn", "Error registering %s\n", modification_publisher_plugin_description.spd_id);
		goto error;
	}

	if (slapi_pblock_set(pb, SLAPI_PLUGIN_POST_DELETE_FN, (void *)modification_publisher_postop_delete) != 0) {
		slapi_log_error(SLAPI_LOG_PLUGIN, "modification_postop_delete", "Error registering %s\n", modification_publisher_plugin_description.spd_id);
		goto error;
	}


	slapi_log_error(SLAPI_LOG_PLUGIN, "Modification publisher plugin", "Initialialized");
	return 0;

error:
	slapi_log_error(SLAPI_LOG_PLUGIN, "Modification publisher plugin", "Load aborted");
	return -1;
}
