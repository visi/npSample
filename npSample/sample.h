#include "stdafx.h"

#include "npruntime.h"
#include "pluginbase.h"


bool IsStringNPIdentifier(NPIdentifier name)
{
	return *(char**)name == (char*)name + 8;
}

char* CopyNPString(NPString str)
{
	char* r = new char[str.UTF8Length + 1];

	strncpy(r, str.UTF8Characters, str.UTF8Length);
	r[str.UTF8Length] = 0;

	return r;
}

class CSample : public NPObject
{
public:
	CSample(NPP npp) : mNpp(npp)
	{
	}

	~CSample()
	{
	}

	// ******static function****** //
	static NPObject* _Creator(NPP npp, NPClass *aClass)
	{
		return new CSample(npp);
	}

	static void _Deallocate(NPObject *npobj) 
	{ 
		delete (CSample*)npobj; 
	}

	static void _Invalidate(NPObject *npobj)
	{ 
		((CSample*)npobj)->Invalidate(); 
	}

	static bool _HasMethod(NPObject *npobj, NPIdentifier name)
	{ 
		return ((CSample*)npobj)->HasMethod(name);
	}

	static bool _Invoke(NPObject *npobj, NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
	{
		return ((CSample*)npobj)->Invoke(name, args, argCount, result);
	}

	static bool _InvokeDefault(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
	{
		return ((CSample*)npobj)->InvokeDefault(args, argCount, result);
	}

	static bool _HasProperty(NPObject * npobj, NPIdentifier name)
	{
		return ((CSample*)npobj)->HasProperty(name);
	}

	static bool _GetProperty(NPObject *npobj, NPIdentifier name, NPVariant *result)
	{
		return ((CSample*)npobj)->GetProperty(name, result);
	}

	static bool _SetProperty(NPObject *npobj, NPIdentifier name, const NPVariant *value)
	{
		return ((CSample*)npobj)->SetProperty(name, value);
	}

	static bool _RemoveProperty(NPObject *npobj, NPIdentifier name)
	{
		return ((CSample*)npobj)->RemoveProperty(name);
	}

	static bool _Enumerate(NPObject *npobj, NPIdentifier **identifier, uint32_t *count)
	{
		return ((CSample*)npobj)->Enumerate(identifier, count);
	}

	static bool _Construct(NPObject *npobj, const NPVariant *args, uint32_t argCount, NPVariant *result)
	{
		return ((CSample*)npobj)->Construct(args, argCount, result);
	}


	// ******virtual function****** //
	virtual void Invalidate()
	{ 
	}

	virtual bool HasMethod(NPIdentifier name)
	{
		if(IsStringNPIdentifier(name))
		{
			char* methodName = *(char**)name;

			if(_tcscmp(methodName, TEXT("closeWnd")) == 0)
				return true;
		}

		return false;
	}

	virtual bool Invoke(NPIdentifier name, const NPVariant *args, uint32_t argCount, NPVariant *result)
	{
		if(IsStringNPIdentifier(name))
		{
			char* methodName = *(char**)name;

			if (_tcscmp(methodName, TEXT("closeWnd")) == 0 && argCount >= 1 && args[0].type == NPVariantType_String)
			{
				HWND hand;
				const char* pcClassName = args[0].value.stringValue.UTF8Characters;

				hand = FindWindow(pcClassName, NULL); // find window
				if (NULL != hand)
				{
					SendMessage(hand, WM_CLOSE, 0, 0); // close window
				}

				return true;
			}
		}

		return false;
	}

	virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount, NPVariant *result)
	{
		return true;
	}

	virtual bool HasProperty(NPIdentifier name)
	{
		return false;
	}

	virtual bool GetProperty(NPIdentifier name, NPVariant *result)
	{
		return false;
	}

	virtual bool SetProperty(NPIdentifier name, const NPVariant *value)
	{
		return false;
	}

	virtual bool RemoveProperty(NPIdentifier name)
	{
		return false;
	}

	virtual bool Enumerate(NPIdentifier **identifier, uint32_t *count)
	{
		return false;
	}

	virtual bool Construct(const NPVariant *args, uint32_t argCount, NPVariant *result)
	{
		return false;
	}

private:
	NPP	mNpp;

};

static NPClass Sample = {
	NP_CLASS_STRUCT_VERSION_CTOR,
	CSample::_Creator,
	CSample::_Deallocate,
	CSample::_Invalidate,
	CSample::_HasMethod,
	CSample::_Invoke,
	CSample::_InvokeDefault,
	CSample::_HasProperty,
	CSample::_GetProperty,
	CSample::_SetProperty,
	CSample::_RemoveProperty,
	CSample::_Enumerate,
	CSample::_Construct
};


class CPlugin : public nsPluginInstanceBase
{
public:
	CPlugin(NPP pNPInstance) : nsPluginInstanceBase(), 
		m_pNPInstance(pNPInstance), 
		m_bInitialized(FALSE), 
		m_sample(NULL)
	{
	}
	
	~CPlugin()
	{
	}

	NPBool init(NPWindow* pNPWindow)
	{
		m_bInitialized = TRUE;
		return TRUE;
	}

	void shut()
	{
		if(m_sample)
		{
			// NPN_ReleaseObject(m_sample);
			delete m_sample;
			m_sample = NULL;
		}

		m_bInitialized = FALSE;
	}

	NPBool isInitialized()
	{
		return m_bInitialized;
	}

	NPError GetValue(NPPVariable variable, void *value)
	{
		switch(variable)
		{
		case NPPVpluginNameString:
			*((char**)value) = "plugin-sample";
			break;

		case NPPVpluginDescriptionString:
			*((char**)value) = "plugin-sample for Chrome";
			break;

		case NPPVpluginScriptableNPObject:
			// if(m_sample == NULL)
			//	 m_sample = (CSample*)NPN_CreateObject(m_pNPInstance, &Sample);

			// if(m_sample != NULL)
			//	 NPN_RetainObject(m_sample);
			if(m_sample == NULL)
			{
				m_sample = new CSample(m_pNPInstance);
				m_sample->_class = &Sample;
			}

			*((NPObject**)value) = m_sample;
			break;
		}

		return nsPluginInstanceBase::GetValue(variable, value);
	}

private:
	NPP		m_pNPInstance;
	NPBool	m_bInitialized;

	CSample*	m_sample;

};
