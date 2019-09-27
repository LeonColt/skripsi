#pragma once
#ifndef POCO_NO_UNWINDOWS
#define POCO_NO_UNWINDOWS
#endif // !POCO_NO_UNWINDOWS
#include <exception>
#include <Poco\JSON\JSONException.h>
#include "CustomInternetJSONAsyncTask.h"
#include "Registry.h"
class RegistryGetBenutzerID : public CustomInternetJSONAsyncTask
{
protected:
	LPVOID doInBackground(LPVOID input);
	void onMainPostExecute(Poco::JSON::Object::Ptr& json);
	void onPostPostExecute();
public:
	RegistryGetBenutzerID();
	~RegistryGetBenutzerID();
};

