#include "MixpanelPrivatePCH.h"

#if PLATFORM_IOS

@interface MixpanelEventHandler : NSObject
{

}

+(id)StaticInit;
@end

@implementation MixpanelEventHandler

+ (id)StaticInit
{
	MixpanelEventHandler *instance = [[MixpanelEventHandler alloc] init];

	if (instance)
	{
		[[NSNotificationCenter defaultCenter] addObserver:instance
			selector : @selector(didFinishLaunchWithOptions : )
					   name:UIApplicationDidBecomeActiveNotification object : nil];
	}

	return instance;
}

-(void)didFinishLaunchWithOptions:(UIApplication *)application
{
	NSString *appID = [[NSBundle mainBundle] objectForInfoDictionaryKey:@"MixpanelApiKey"];

	if (appID)
	{
		PlatformMixpanelHelper::InitMixpanel(FString(appID));
	}
}

@end

static MixpanelEventHandler *EventHandlerStaticInitializer = [MixpanelEventHandler StaticInit];


@implementation IOSMixpanel

+(bool) InitWithID:(NSString*) appID;
{
    if(![Mixpanel class])
        return false;
    
	Mixpanel *mixpanel = [Mixpanel sharedInstanceWithToken : appID];

	return true;
}

+(void) TrackEvent:(NSString*) eventName withProperies:(NSDictionary *)properties
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		Mixpanel *mixpanel = [Mixpanel sharedInstance];

		if (mixpanel)
		{
			[mixpanel track : eventName properties : properties];
		}
	});
}

+(void )IdentifyUser:(NSString*)userID
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		Mixpanel *mixpanel = [Mixpanel sharedInstance];

		if (mixpanel)
		{
			[mixpanel identify:userID];
		}
	});
}

+(void)TrackCharge:(NSNumber*)charge withProperies : (NSDictionary *)properties
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		Mixpanel *mixpanel = [Mixpanel sharedInstance];

		if (mixpanel)
		{
			[mixpanel.people trackCharge : charge withProperties : properties];
		}
	});
}

+(void)StoreUserProfileData:(NSDictionary *)properties incrementData : (NSDictionary *)incrementProperties
{
	dispatch_async(dispatch_get_main_queue(), ^
	{
		Mixpanel *mixpanel = [Mixpanel sharedInstance];

		if (mixpanel)
		{
			if (properties && [properties count])
			{
				[mixpanel.people set : properties];
			}

			if (incrementProperties && [incrementProperties count])
			{
				[mixpanel.people increment : incrementProperties];
			}
		}
	});
}

@end
#endif // PLATFORM_IOS


bool PlatformMixpanelHelper::InitMixpanel(const FString& apiKey)
{
#if PLATFORM_IOS
    return [IOSMixpanel InitWithID:[NSString stringWithFString: apiKey]];
#endif // PLATFORM_IOS

	return false;
}

void PlatformMixpanelHelper::TrackEvent(const FString& eventName, const TArray<FAnalyticsEventAttribute>& attributes)
{
#if PLATFORM_IOS
    if(![IOSMixpanel class])
        return;
    
	NSString* convertedEventName = [NSString stringWithFString : eventName];
	const int32 attrCount = attributes.Num();
	if (attrCount > 0)
	{
		NSDictionary* attributesDict = [NSMutableDictionary dictionaryWithCapacity : attrCount];
		for (auto attr : attributes)
		{
			NSString* attrName = [NSString stringWithFString : attr.AttrName];
			NSString* attrValue = [NSString stringWithFString : attr.AttrValue];
			[attributesDict setValue : attrValue forKey : attrName];
		}
		
		[IOSMixpanel TrackEvent : convertedEventName withProperies : attributesDict];
	}
#endif // PLATFORM_IOS
}

void PlatformMixpanelHelper::SetUserID(const FString& userID)
{
#if PLATFORM_IOS
	if (![IOSMixpanel class])
		return;

	NSString* convertedUserID = [NSString stringWithFString : userID];
	
	[IOSMixpanel IdentifyUser : convertedUserID];
#endif // PLATFORM_IOS
}

void PlatformMixpanelHelper::TrackCharge(double charge, const TArray<FAnalyticsEventAttribute>& attributes)
{
#if PLATFORM_IOS
	if (![IOSMixpanel class])
		return;

	const int32 attrCount = attributes.Num();
	if (attrCount > 0)
	{
		NSDictionary* attributesDict = [NSMutableDictionary dictionaryWithCapacity : attrCount];
		for (auto attr : attributes)
		{
			NSString* attrName = [NSString stringWithFString : attr.AttrName];
			NSString* attrValue = [NSString stringWithFString : attr.AttrValue];
			[attributesDict setValue : attrValue forKey : attrName];
		}

		[IOSMixpanel TrackCharge : [NSNumber numberWithDouble : charge] withProperies : attributesDict];
	}
#endif // PLATFORM_IOS
}

void PlatformMixpanelHelper::StoreUserProfileData(const TArray<FAnalyticsEventAttribute>& attributes, const TArray<FAnalyticsEventAttribute>& incrementAttributes)
{
#if PLATFORM_IOS
	if (![IOSMixpanel class])
		return;

	int32 attrCount = attributes.Num();
	NSDictionary* attributesDict = nil;
	NSDictionary* incrementAttributesDict = nil;

	if (attrCount > 0)
	{
		attributesDict = [NSMutableDictionary dictionaryWithCapacity : attrCount];
		for (auto attr : attributes)
		{
			NSString* attrName = [NSString stringWithFString : attr.AttrName];
			NSString* attrValue = [NSString stringWithFString : attr.AttrValue];
			[attributesDict setValue : attrValue forKey : attrName];
		}
	}

	attrCount = incrementAttributes.Num();
	if (attrCount > 0)
	{
		incrementAttributesDict = [NSMutableDictionary dictionaryWithCapacity : attrCount];
		for (auto attr : attributes)
		{
			NSString* attrName = [NSString stringWithFString : attr.AttrName];
			NSString* attrValue = [NSString stringWithFString : attr.AttrValue];

			NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
			formatter.numberStyle = NSNumberFormatterDecimalStyle;
			NSNumber *numberValue = [formatter numberFromString : attrValue];
			[incrementAttributesDict setValue : numberValue forKey : attrName];
		}
	}

	[IOSMixpanel StoreUserProfileData : attributesDict incrementData : incrementAttributesDict];
#endif // PLATFORM_IOS
}