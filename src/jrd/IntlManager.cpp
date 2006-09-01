/*
 *	PROGRAM:	JRD International support
 *	MODULE:		IntlManager.cpp
 *	DESCRIPTION:	INTL Manager
 *
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2004 Adriano dos Santos Fernandes <adrianosf@uol.com.br>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/IntlManager.h"
#include "../jrd/os/mod_loader.h"
#include "../jrd/intlobj_new.h"
#include "../jrd/intl_proto.h"
#include "../common/config/config.h"
#include "../common/classes/GenericMap.h"
#include "../common/classes/objects_array.h"
#include "../common/classes/fb_string.h"
#include "../common/classes/init.h"

#include "../config/ConfigFile.h"
#include "../config/ConfObj.h"
#include "../config/ConfObject.h"
#include "../config/Element.h"
#include "../config/ScanDir.h"
#include "../config/AdminException.h"
#include "../vulcan/JString.h"

#define STRINGIZE_AUX(x)	#x
#define STRINGIZE(x)		STRINGIZE_AUX(x)


namespace Jrd {

using namespace Firebird;


struct ExternalInfo
{
	ExternalInfo(const PathName& a_moduleName, const string& a_name)
		: moduleName(a_moduleName),
		  name(a_name)
	{
	}

	ExternalInfo(MemoryPool& p, const ExternalInfo& externalInfo)
		: moduleName(p, externalInfo.moduleName),
		  name(p, externalInfo.name)
	{
	}

	ExternalInfo()
	{
	}

	PathName moduleName;
	string name;
};


static Firebird::InitInstance<GenericMap<Pair<Left<PathName, ModuleLoader::Module*> > > > modules;
static Firebird::InitInstance<GenericMap<Pair<Full<string, ExternalInfo> > > > charSetCollations;


bool IntlManager::initialize()
{
	bool ok = true;
	ObjectsArray<string> conflicts;

	//// TODO: intlnames.h
	registerCharSetCollation("NONE:NONE", "", "NONE");
	registerCharSetCollation("OCTETS:OCTETS", "", "OCTETS");
	registerCharSetCollation("ASCII:ASCII", "", "ASCII");
	registerCharSetCollation("UNICODE_FSS:UNICODE_FSS", "", "UNICODE_FSS");
	registerCharSetCollation("UTF8:UTF8", "", "UTF8");
	registerCharSetCollation("UTF8:UCS_BASIC", "", "UCS_BASIC");
	registerCharSetCollation("UTF8:UNICODE", "", "UNICODE");

	registerCharSetCollation("UTF16:UTF16", "", "UTF16");
#ifdef FB_NEW_INTL_ALLOW_NOT_READY
	registerCharSetCollation("UTF16:UCS_BASIC", "", "UCS_BASIC");
	registerCharSetCollation("UTF32:UTF32", "", "UTF32");
	registerCharSetCollation("UTF32:UCS_BASIC", "", "UCS_BASIC");
#endif

	Firebird::PathName intlPath;
	PathUtils::concatPath(intlPath, Config::getRootDirectory(), "intl");

	ScanDir dir(intlPath.c_str(), "*.conf");

	try
	{
		while (dir.next())
		{
			ConfigFile configFile(dir.getFilePath(), 0);

			for (Element* el = configFile.objects->children; el; el = el->sibling)
			{
				if (el->name == "charset")
				{
					string charSetName = el->getAttributeName(0);
					PathName filename;

					Element* module = el->findChild("intl_module");
					if (module)
					{
						JString moduleName(module->getAttributeName(0));
						ConfObj objModule = configFile.findObject("intl_module", moduleName);
						filename = objModule->getValue("filename", "");

						if (!modules().exist(filename))
						{
							ModuleLoader::Module* mod =
								ModuleLoader::loadModule(filename);
							if (!mod)
							{
								ModuleLoader::doctorModuleExtention(filename);
								mod = ModuleLoader::loadModule(filename);
							}
							if (mod)
							{
								modules().put(filename, mod);
							}
							else
							{
								gds__log((string("Can't load INTL module '") +
									filename.c_str() + "'").c_str());
								ok = false;
							}
						}
					}
					
					for (Element* el2 = el->children; el2; el2 = el2->sibling)
					{
						if (el2->name == "collation")
						{
							string collationName = (const char*)el2->getAttributeName(0);
							string charSetCollation = charSetName + ":" + collationName;
							const char* externalName = (const char*)el2->getAttributeName(1);

							if (!registerCharSetCollation(charSetCollation, filename,
								(externalName ? externalName : collationName)))
							{
								conflicts.add(charSetCollation);
								ok = false;
							}
						}
					}
				}
			}
		}
	}
	catch (AdminException& ex)
	{
		gds__log((string("Error in INTL plugin config file '") + dir.getFilePath() + "': " + ex.getText()).c_str());
		ok = false;
	}

	for (ObjectsArray<string>::const_iterator name(conflicts.begin()); name != conflicts.end(); ++name)
		charSetCollations().remove(*name);

	return ok;
}


bool IntlManager::collationInstalled(const Firebird::string& collationName,
									 const Firebird::string& charSetName)
{
	return charSetCollations().exist(charSetName + ":" + collationName);
}


bool IntlManager::lookupCharSet(const Firebird::string& charSetName, charset* cs)
{
	ExternalInfo externalInfo;

	if (charSetCollations().get(charSetName + ":" + charSetName, externalInfo))
	{
		if (externalInfo.moduleName.isEmpty())
			return INTL_builtin_lookup_charset(cs, externalInfo.name.c_str());

#ifdef INTL_BUILTIN
		return LD_lookup_charset(cs, externalInfo.name.c_str());
#else
		ModuleLoader::Module* module;
		
		if (modules().get(externalInfo.moduleName, module) && module)
		{
			pfn_INTL_lookup_charset lookupFunction =
				(pfn_INTL_lookup_charset)module->findSymbol(STRINGIZE(CHARSET_ENTRYPOINT));

			if (lookupFunction && (*lookupFunction)(cs, externalInfo.name.c_str()))
			{
				return validateCharSet(charSetName, cs);
			}
		}
#endif
	}

	return false;
}


bool IntlManager::lookupCollation(const Firebird::string& collationName,
								  const Firebird::string& charSetName,
								  USHORT attributes, const UCHAR* specificAttributes,
								  ULONG specificAttributesLen, bool ignoreAttributes,
								  texttype* tt)
{
	ExternalInfo charSetExternalInfo;
	ExternalInfo collationExternalInfo;

	if (charSetCollations().get(charSetName + ":" + charSetName, charSetExternalInfo) &&
		charSetCollations().get(charSetName + ":" + collationName, collationExternalInfo))
	{
		if (collationExternalInfo.moduleName.isEmpty())
		{
			return INTL_builtin_lookup_texttype(tt, collationExternalInfo.name.c_str(),
				charSetExternalInfo.name.c_str(), attributes, specificAttributes,
				specificAttributesLen, ignoreAttributes);
		}


#ifdef INTL_BUILTIN
		return LD_lookup_texttype(tt, collationExternalInfo.name.c_str(),
			charSetExternalInfo.name.c_str(), attributes, specificAttributes,
			specificAttributesLen, ignoreAttributes);
#else
		ModuleLoader::Module* module;
		
		if (modules().get(collationExternalInfo.moduleName, module) && module)
		{
			pfn_INTL_lookup_texttype lookupFunction =
				(pfn_INTL_lookup_texttype)module->findSymbol(STRINGIZE(TEXTTYPE_ENTRYPOINT));

			if (lookupFunction &&
				(*lookupFunction)(tt, collationExternalInfo.name.c_str(), charSetExternalInfo.name.c_str(),
								  attributes, specificAttributes,
								  specificAttributesLen, ignoreAttributes))
			{
				return true;
			}
		}
#endif
	}

	return false;
}


bool IntlManager::registerCharSetCollation(const Firebird::string& name, const Firebird::PathName& filename,
										   const Firebird::string& externalName)
{
	ExternalInfo conflict;

	if (charSetCollations().get(name, conflict))
	{
		gds__log((string("INTL plugin conflict: ") + name + " defined in " +
			(conflict.moduleName.isEmpty() ? "<builtin>" : conflict.moduleName.c_str()) +
			" and " + filename.c_str()).c_str());
		return false;
	}

	charSetCollations().put(name, ExternalInfo(filename, externalName));
	return true;
}


bool IntlManager::validateCharSet(const Firebird::string& charSetName, charset* cs)
{
	bool valid = true;
	string s;

	string unsupportedMsg;
	unsupportedMsg.printf("Unsupported character set %s.", charSetName.c_str());

	if (!(cs->charset_flags & CHARSET_ASCII_BASED))
	{
		valid = false;
		s.printf("%s. Only ASCII-based character sets are supported yet.",
			unsupportedMsg.c_str());
		gds__log(s.c_str());
	}

	if (cs->charset_min_bytes_per_char != 1)
	{
		valid = false;
		s.printf("%s. Wide character sets are not supported yet.",
			unsupportedMsg.c_str());
		gds__log(s.c_str());
	}

	if (cs->charset_space_length != 1 || *cs->charset_space_character != ' ')
	{
		valid = false;
		s.printf("%s. Only ASCII space is supported in charset_space_character yet.",
			unsupportedMsg.c_str());
		gds__log(s.c_str());
	}

	return valid;
}


}	// namespace Jrd
