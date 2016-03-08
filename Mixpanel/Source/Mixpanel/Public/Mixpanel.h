// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

#include "IAnalyticsProviderModule.h"
#include "IAnalyticsProvider.h"

class FMixpanelModule : public IModuleInterface
{
protected:
	bool bInitialized;
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	static inline FMixpanelModule& Get()
	{
		return FModuleManager::LoadModuleChecked< FMixpanelModule >("Mixpanel");
	}

	virtual void TrackEvent(const FString& eventName, const TArray<FAnalyticsEventAttribute>& attributes);
	virtual void SetUserID(const FString& userID);
	virtual void TrackCharge(double charge, const TArray<FAnalyticsEventAttribute>& attributes);
	virtual void StoreUserProfileData(const TArray<FAnalyticsEventAttribute>& attributes, const TArray<FAnalyticsEventAttribute>& incrementAttributes);
};