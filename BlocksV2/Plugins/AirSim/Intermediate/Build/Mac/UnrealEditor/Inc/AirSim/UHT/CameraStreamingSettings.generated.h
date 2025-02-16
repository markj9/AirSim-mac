// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "CameraStreamingSettings.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef AIRSIM_CameraStreamingSettings_generated_h
#error "CameraStreamingSettings.generated.h already included, missing '#pragma once' in CameraStreamingSettings.h"
#endif
#define AIRSIM_CameraStreamingSettings_generated_h

#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_10_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUCameraStreamingSettings(); \
	friend struct Z_Construct_UClass_UCameraStreamingSettings_Statics; \
public: \
	DECLARE_CLASS(UCameraStreamingSettings, UDeveloperSettings, COMPILED_IN_FLAGS(0 | CLASS_DefaultConfig | CLASS_Config), CASTCLASS_None, TEXT("/Script/AirSim"), NO_API) \
	DECLARE_SERIALIZER(UCameraStreamingSettings) \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_10_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UCameraStreamingSettings(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UCameraStreamingSettings(UCameraStreamingSettings&&); \
	UCameraStreamingSettings(const UCameraStreamingSettings&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UCameraStreamingSettings); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCameraStreamingSettings); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCameraStreamingSettings) \
	NO_API virtual ~UCameraStreamingSettings();


#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_7_PROLOG
#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_10_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_10_INCLASS_NO_PURE_DECLS \
	FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h_10_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> AIRSIM_API UClass* StaticClass<class UCameraStreamingSettings>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_CameraStreamingSettings_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
