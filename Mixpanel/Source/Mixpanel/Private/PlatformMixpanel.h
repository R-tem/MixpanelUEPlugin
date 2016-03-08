#ifndef PlatformMixpanel_h
#define PlatformMixpanel_h

#include "MixpanelPrivatePCH.h"

#if PLATFORM_IOS
#import <Foundation/Foundation.h>
#import <Mixpanel/Mixpanel.h>

@interface IOSMixpanel : NSObject

+(bool) InitWithID:(NSString*) appID;
+(void) TrackEvent:(NSString*) eventName withProperies:(NSDictionary *)properties;
+(void) IdentifyUser:(NSString*)userID;
+(void) TrackCharge:(NSNumber*)charge withProperies : (NSDictionary *)properties;
+(void) StoreUserProfileData:(NSDictionary *)properties incrementData: (NSDictionary *)incrementProperties;

@end
#endif // PLATFORM_IOS

class PlatformMixpanelHelper
{
protected:

    PlatformMixpanelHelper();
public:
	virtual ~PlatformMixpanelHelper() {};
    
    static bool InitMixpanel(const FString& apiKey);
    static void TrackEvent(const FString& eventName, const TArray<FAnalyticsEventAttribute>& attributes);
	static void SetUserID(const FString& userID);
	static void TrackCharge(double charge, const TArray<FAnalyticsEventAttribute>& attributes);
	static void StoreUserProfileData(const TArray<FAnalyticsEventAttribute>& attributes, const TArray<FAnalyticsEventAttribute>& incrementAttributes);
};

#endif /* PlatformMixpanel_h */
