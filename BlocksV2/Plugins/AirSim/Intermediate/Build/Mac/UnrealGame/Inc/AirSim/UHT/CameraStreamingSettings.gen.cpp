// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CameraStreamingSettings.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCameraStreamingSettings() {}

// Begin Cross Module References
AIRSIM_API UClass* Z_Construct_UClass_UCameraStreamingSettings();
AIRSIM_API UClass* Z_Construct_UClass_UCameraStreamingSettings_NoRegister();
DEVELOPERSETTINGS_API UClass* Z_Construct_UClass_UDeveloperSettings();
UPackage* Z_Construct_UPackage__Script_AirSim();
// End Cross Module References

// Begin Class UCameraStreamingSettings
void UCameraStreamingSettings::StaticRegisterNativesUCameraStreamingSettings()
{
}
IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCameraStreamingSettings);
UClass* Z_Construct_UClass_UCameraStreamingSettings_NoRegister()
{
	return UCameraStreamingSettings::StaticClass();
}
struct Z_Construct_UClass_UCameraStreamingSettings_Statics
{
#if WITH_METADATA
	static constexpr UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[] = {
		{ "DisplayName", "Camera Streaming Settings" },
		{ "IncludePath", "CameraStreamingSettings.h" },
		{ "ModuleRelativePath", "CameraStreamingSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_IPAddress_MetaData[] = {
		{ "Category", "Networking" },
		{ "ModuleRelativePath", "CameraStreamingSettings.h" },
	};
	static constexpr UECodeGen_Private::FMetaDataPairParam NewProp_Port_MetaData[] = {
		{ "Category", "Networking" },
		{ "ModuleRelativePath", "CameraStreamingSettings.h" },
	};
#endif // WITH_METADATA
	static const UECodeGen_Private::FStrPropertyParams NewProp_IPAddress;
	static const UECodeGen_Private::FIntPropertyParams NewProp_Port;
	static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
	static UObject* (*const DependentSingletons[])();
	static constexpr FCppClassTypeInfoStatic StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCameraStreamingSettings>::IsAbstract,
	};
	static const UECodeGen_Private::FClassParams ClassParams;
};
const UECodeGen_Private::FStrPropertyParams Z_Construct_UClass_UCameraStreamingSettings_Statics::NewProp_IPAddress = { "IPAddress", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCameraStreamingSettings, IPAddress), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_IPAddress_MetaData), NewProp_IPAddress_MetaData) };
const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_UCameraStreamingSettings_Statics::NewProp_Port = { "Port", nullptr, (EPropertyFlags)0x0010000000004001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, nullptr, nullptr, 1, STRUCT_OFFSET(UCameraStreamingSettings, Port), METADATA_PARAMS(UE_ARRAY_COUNT(NewProp_Port_MetaData), NewProp_Port_MetaData) };
const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCameraStreamingSettings_Statics::PropPointers[] = {
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCameraStreamingSettings_Statics::NewProp_IPAddress,
	(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCameraStreamingSettings_Statics::NewProp_Port,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCameraStreamingSettings_Statics::PropPointers) < 2048);
UObject* (*const Z_Construct_UClass_UCameraStreamingSettings_Statics::DependentSingletons[])() = {
	(UObject* (*)())Z_Construct_UClass_UDeveloperSettings,
	(UObject* (*)())Z_Construct_UPackage__Script_AirSim,
};
static_assert(UE_ARRAY_COUNT(Z_Construct_UClass_UCameraStreamingSettings_Statics::DependentSingletons) < 16);
const UECodeGen_Private::FClassParams Z_Construct_UClass_UCameraStreamingSettings_Statics::ClassParams = {
	&UCameraStreamingSettings::StaticClass,
	"Game",
	&StaticCppClassTypeInfo,
	DependentSingletons,
	nullptr,
	Z_Construct_UClass_UCameraStreamingSettings_Statics::PropPointers,
	nullptr,
	UE_ARRAY_COUNT(DependentSingletons),
	0,
	UE_ARRAY_COUNT(Z_Construct_UClass_UCameraStreamingSettings_Statics::PropPointers),
	0,
	0x000000A6u,
	METADATA_PARAMS(UE_ARRAY_COUNT(Z_Construct_UClass_UCameraStreamingSettings_Statics::Class_MetaDataParams), Z_Construct_UClass_UCameraStreamingSettings_Statics::Class_MetaDataParams)
};
UClass* Z_Construct_UClass_UCameraStreamingSettings()
{
	if (!Z_Registration_Info_UClass_UCameraStreamingSettings.OuterSingleton)
	{
		UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCameraStreamingSettings.OuterSingleton, Z_Construct_UClass_UCameraStreamingSettings_Statics::ClassParams);
	}
	return Z_Registration_Info_UClass_UCameraStreamingSettings.OuterSingleton;
}
template<> AIRSIM_API UClass* StaticClass<UCameraStreamingSettings>()
{
	return UCameraStreamingSettings::StaticClass();
}
UCameraStreamingSettings::UCameraStreamingSettings(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
DEFINE_VTABLE_PTR_HELPER_CTOR(UCameraStreamingSettings);
UCameraStreamingSettings::~UCameraStreamingSettings() {}
// End Class UCameraStreamingSettings

// Begin Registration
struct Z_CompiledInDeferFile_FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_Statics
{
	static constexpr FClassRegisterCompiledInInfo ClassInfo[] = {
		{ Z_Construct_UClass_UCameraStreamingSettings, UCameraStreamingSettings::StaticClass, TEXT("UCameraStreamingSettings"), &Z_Registration_Info_UClass_UCameraStreamingSettings, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCameraStreamingSettings), 3459296552U) },
	};
};
static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_1027669394(TEXT("/Script/AirSim"),
	Z_CompiledInDeferFile_FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_Statics::ClassInfo),
	nullptr, 0,
	nullptr, 0);
// End Registration
PRAGMA_ENABLE_DEPRECATION_WARNINGS
