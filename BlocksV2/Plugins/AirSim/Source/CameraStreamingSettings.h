// CameraStreamingSettings.h
#pragma once

#include "Engine/DeveloperSettings.h"
#include "CameraStreamingSettings.generated.h"

UCLASS(config=Game, defaultconfig, meta=(DisplayName="Camera Streaming Settings"))
class UCameraStreamingSettings : public UDeveloperSettings
{
    GENERATED_BODY()

public:
    UPROPERTY(config, EditAnywhere, Category = "Networking")
    FString IPAddress = "127.0.0.1";

    UPROPERTY(config, EditAnywhere, Category = "Networking")
    int32 Port = 5005;
};