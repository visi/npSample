// closeWnd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "sample.h"

NPError NS_PluginInitialize()
{
	return NPERR_NO_ERROR;
}

void NS_PluginShutdown()
{
}

nsPluginInstanceBase * NS_NewPluginInstance(nsPluginCreateData * aCreateDataStruct)
{
	if(!aCreateDataStruct)
		return NULL;

	CPlugin* plugin = new CPlugin(aCreateDataStruct->instance);

	return plugin;
}

void NS_DestroyPluginInstance(nsPluginInstanceBase * aPlugin)
{
	if(aPlugin)
		delete (CPlugin *)aPlugin;
}
