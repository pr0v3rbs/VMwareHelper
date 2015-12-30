#include "VirtualDesktopManager.h"

VirtualDesktopManager::VirtualDesktopManager() :
	desktopManagerInternal_(nullptr)
{
}

VirtualDesktopManager::~VirtualDesktopManager()
{
	Uninit();
}

HRESULT VirtualDesktopManager::Init()
{
	::CoInitialize(NULL);

	IServiceProvider* pServiceProvider = nullptr;
	HRESULT result = ::CoCreateInstance(CLSID_ImmersiveShell, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IServiceProvider), (PVOID*)&pServiceProvider);

	if (SUCCEEDED(result))
	{
		result = pServiceProvider->QueryService(CLSID_VirtualDesktopAPI_Unknown, &desktopManagerInternal_);
		pServiceProvider->Release();
	}

	return result;
}

void VirtualDesktopManager::Uninit()
{
	if (desktopManagerInternal_ != nullptr)
	{
		desktopManagerInternal_->Release();
	}
}

HRESULT VirtualDesktopManager::SwitchLeft()
{
	IVirtualDesktop *pDesktop = nullptr;
	HRESULT result = desktopManagerInternal_->GetCurrentDesktop(&pDesktop);

	if (SUCCEEDED(result))
	{
		IVirtualDesktop *pAdjacentDesktop = nullptr;

		result = desktopManagerInternal_->GetAdjacentDesktop(pDesktop, AdjacentDesktop::LeftDirection, &pAdjacentDesktop);
		if (SUCCEEDED(result))
		{
			result = desktopManagerInternal_->SwitchDesktop(pAdjacentDesktop);

			pAdjacentDesktop->Release();
		}

		pDesktop->Release();
	}

	return result;
}

HRESULT VirtualDesktopManager::SwitchRight()
{
	IVirtualDesktop *pDesktop = nullptr;
	HRESULT result = desktopManagerInternal_->GetCurrentDesktop(&pDesktop);

	if (SUCCEEDED(result))
	{
		IVirtualDesktop *pAdjacentDesktop = nullptr;

		result = desktopManagerInternal_->GetAdjacentDesktop(pDesktop, AdjacentDesktop::RightDirection, &pAdjacentDesktop);
		if (SUCCEEDED(result))
		{
			result = desktopManagerInternal_->SwitchDesktop(pAdjacentDesktop);

			pAdjacentDesktop->Release();
		}

		pDesktop->Release();
	}

	return result;
}