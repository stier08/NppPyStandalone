#ifndef NPP_PYSCRIPT_IMPORTEXPORT_H_GUARD
#define NPP_PYSCRIPT_IMPORTEXPORT_H_GUARD

#ifdef _WIN32
	#ifdef NPP_STANDALONE
		#define NPP_PYSCRIPT_DOCKING_API
	#else
		#ifdef NPP_PYSCRIPT_DOCKING_EXPORTS
			#define NPP_PYSCRIPT_DOCKING_API	__declspec(dllexport)
		#else
			#define NPP_PYSCRIPT_DOCKING_API	__declspec(dllimport)
		#endif
	#endif
#endif

#endif // NPP_PYSCRIPT_IMPORTEXPORT_H_GUARD
