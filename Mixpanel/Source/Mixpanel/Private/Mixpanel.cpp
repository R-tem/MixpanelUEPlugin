// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "MixpanelPrivatePCH.h"

#define LOCTEXT_NAMESPACE "FMixpanelModule"

void FMixpanelModule::StartupModule()
{
	bInitialized = true;

	/*FString configValue;

	GConfig->GetString(TEXT("Analytics"), TEXT("MixpanelApiKey"), configValue, GEngineIni);

	UE_LOG(LogClass, Log, TEXT("Init Mixpanel with id: %s"), *configValue);

	bInitialized = PlatformMixpanelHelper::InitMixpanel(configValue);*/
}

void FMixpanelModule::ShutdownModule()
{

}

void FMixpanelModule::TrackEvent(const FString& eventName, const TArray<FAnalyticsEventAttribute>& attributes)
{
	if (bInitialized)
	{
		PlatformMixpanelHelper::TrackEvent(eventName, attributes);
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT("FMixpanelModule::TrackEvent: Failed to track Mixpanel event, becaise initialization has been failed."));
	}
}

void FMixpanelModule::SetUserID(const FString& userID)
{
	if (bInitialized)
	{
		PlatformMixpanelHelper::SetUserID(userID);
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT("FMixpanelModule::SetUserID: Failed to track Mixpanel event, becaise initialization has been failed."));
	}
}

void FMixpanelModule::TrackCharge(double charge, const TArray<FAnalyticsEventAttribute>& attributes)
{
	if (bInitialized)
	{
		PlatformMixpanelHelper::TrackCharge(charge, attributes);
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT("FMixpanelModule::TrackCharge: Failed to track Mixpanel event, becaise initialization has been failed."));
	}
}

void FMixpanelModule::StoreUserProfileData(const TArray<FAnalyticsEventAttribute>& attributes, const TArray<FAnalyticsEventAttribute>& incrementAttributes)
{
	if (bInitialized)
	{
		PlatformMixpanelHelper::StoreUserProfileData(attributes, incrementAttributes);
	}
	else
	{
		UE_LOG(LogClass, Log, TEXT("FMixpanelModule::StoreUserProfileData: Failed to track Mixpanel event, becaise initialization has been failed."));
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMixpanelModule, Mixpanel)