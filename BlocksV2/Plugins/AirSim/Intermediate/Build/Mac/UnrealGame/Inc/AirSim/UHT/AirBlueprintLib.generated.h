// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "AirBlueprintLib.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AActor;
class UObject;
class UWorld;
enum class LogDebugLevel : uint8;
#ifdef AIRSIM_AirBlueprintLib_generated_h
#error "AirBlueprintLib.generated.h already included, missing '#pragma once' in AirBlueprintLib.h"
#endif
#define AIRSIM_AirBlueprintLib_generated_h

#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h_43_RPC_WRAPPERS_NO_PURE_DECLS \
	DECLARE_FUNCTION(execRunConsoleCommand); \
	DECLARE_FUNCTION(execListWorldsInRegistry); \
	DECLARE_FUNCTION(execspawnPlayer); \
	DECLARE_FUNCTION(execloadLevel); \
	DECLARE_FUNCTION(execLogMessage);


#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h_43_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUAirBlueprintLib(); \
	friend struct Z_Construct_UClass_UAirBlueprintLib_Statics; \
public: \
	DECLARE_CLASS(UAirBlueprintLib, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/AirSim"), NO_API) \
	DECLARE_SERIALIZER(UAirBlueprintLib)


#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h_43_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UAirBlueprintLib(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	UAirBlueprintLib(UAirBlueprintLib&&); \
	UAirBlueprintLib(const UAirBlueprintLib&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UAirBlueprintLib); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UAirBlueprintLib); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UAirBlueprintLib) \
	NO_API virtual ~UAirBlueprintLib();


#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h_40_PROLOG
#define FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h_43_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h_43_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h_43_INCLASS_NO_PURE_DECLS \
	FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h_43_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> AIRSIM_API UClass* StaticClass<class UAirBlueprintLib>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_mark_src_fulcrum_Colosseum_Unreal_Environments_BlocksV2_Plugins_AirSim_Source_AirBlueprintLib_h


#define FOREACH_ENUM_LOGDEBUGLEVEL(op) \
	op(LogDebugLevel::Informational) \
	op(LogDebugLevel::Success) \
	op(LogDebugLevel::Failure) \
	op(LogDebugLevel::Unimportant) 

enum class LogDebugLevel : uint8;
template<> struct TIsUEnumClass<LogDebugLevel> { enum { Value = true }; };
template<> AIRSIM_API UEnum* StaticEnum<LogDebugLevel>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
