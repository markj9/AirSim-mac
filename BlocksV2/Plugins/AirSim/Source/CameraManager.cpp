#include "CameraManager.h"
#include "GameFramework/PlayerController.h"
#include "AirBlueprintLib.h"
#include "FFmpegEncoder.h"
#include "Common/UdpSocketBuilder.h"
extern "C" {
    #include <libavcodec/avcodec.h>
}


ACameraManager::ACameraManager()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create a spring arm component for our chase camera
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));
    SpringArm->SetWorldRotation(FRotator(-20.0f, 0.0f, 0.0f));
    SpringArm->TargetArmLength = 125.0f;
    SpringArm->bEnableCameraLag = false;
    SpringArm->bEnableCameraRotationLag = false;
    SpringArm->CameraRotationLagSpeed = 10.0f;
    SpringArm->bInheritPitch = true;
    SpringArm->bInheritYaw = true;
    SpringArm->bInheritRoll = true;
}

void ACameraManager::BeginPlay()
{
    Super::BeginPlay();
    // Define the interval in seconds
    // constexpr float IntervalInSeconds = 0.041666f; //  24fps
    constexpr float IntervalInSeconds = 0.066666f; // 15fps
    //constexpr float IntervalInSeconds = 0.016; // 60fps
    // Schedule the task to run at regular intervals
    GetWorld()->GetTimerManager().SetTimer(
        RenderTaskTimerHandle,                // Timer handle
        this,                                 // Object to call the function on
        &ACameraManager::StreamCameraData,          // Function to call
        IntervalInSeconds,                    // Interval between calls
        true                                  // Loop indefinitely
    );
}

void ACameraManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (mode_ == ECameraDirectorMode::CAMERA_DIRECTOR_MODE_MANUAL) {
        manual_pose_controller_->updateActorPose(DeltaTime);
    }
    else if (mode_ == ECameraDirectorMode::CAMERA_DIRECTOR_MODE_SPRINGARM_CHASE) {
        //do nothing, spring arm is pulling the camera with it
    }
    else if (mode_ == ECameraDirectorMode::CAMERA_DIRECTOR_MODE_NODISPLAY) {
        //do nothing, we have camera turned off
    }
    else { //make camera move in desired way
        UAirBlueprintLib::FollowActor(ExternalCamera, follow_actor_, initial_ground_obs_offset_, ext_obs_fixed_z_);
    }

    
}

// Function to convert uint32 to big-endian byte array
void WriteUInt32BE(FArchive& Ar, uint32 Value)
{
    uint8 Bytes[4];
    Bytes[0] = (Value >> 24) & 0xFF;
    Bytes[1] = (Value >> 16) & 0xFF;
    Bytes[2] = (Value >> 8) & 0xFF;
    Bytes[3] = Value & 0xFF;
    Ar.Serialize(Bytes, 4);
}


void ACameraManager::StreamCameraData()
{
    if (this->is_capturing_frame_)
        return;
    
    // Initialize the encoder if not already initialized.
    if (!this->encoder_.IsValid()) {
        this->encoder_ = MakeShared<FFmpegEncoder>();
        // auto initialized = this->encoder_->InitializeEncoder(640, 360, 15, 500000, AV_CODEC_ID_H264);
        auto initialized = this->encoder_->InitializeEncoder(640, 360, 15, 500000, AV_CODEC_ID_MPEG2VIDEO);
        this->encoder_->OnPacketEncodedCallback = [](const uint8* PacketData, int32 PacketSize, const void* state)
        {
            const CameraStream* stream = static_cast<const CameraStream*>(state);
            if (stream)
            {
                int32 bytes_sent = 0;
                // Now you can use CameraStreamPtr to access CameraStream8's members.
                auto addr = stream->remote_ip_addr.ToInternetAddr();
                stream->socket->SendTo(PacketData, PacketSize, bytes_sent, *addr);
            }
        };
    }
    
    for (auto stream : this->streams_) {
        this->is_capturing_frame_ = true;
        std::vector<msr::airlib::ImageCaptureBase::ImageRequest> requests{
            stream.request
        };
        std::vector<msr::airlib::ImageCaptureBase::ImageResponse> responses;
        stream.image_capture->getImages(
            requests,
            responses,
            true,
            false);
        
        this->is_capturing_frame_ = false;
        if (responses.size() > 0) {
            std::vector<uint8_t> image_data = responses[0].image_data_uint8;
            auto success = this->encoder_->EncodeFrame(image_data.data(), image_data.size(), &stream);
            if (success) {
                // Success!
            }
        }
        else {
            UE_LOG(LogTemp, Warning, TEXT("No image data to send."));
        }
    }
}

ECameraDirectorMode ACameraManager::getMode()
{
    return mode_;
}

void ACameraManager::initializeForBeginPlay(ECameraDirectorMode view_mode,
                                             AActor* follow_actor, APIPCamera* fpv_camera, APIPCamera* front_camera, APIPCamera* back_camera)
{
    manual_pose_controller_ = NewObject<UManualPoseController>(this, "CameraDirector_ManualPoseController");
    manual_pose_controller_->initializeForPlay();

    setupInputBindings();

    mode_ = view_mode;

    follow_actor_ = follow_actor;
    fpv_camera_ = fpv_camera;
    front_camera_ = front_camera;
    backup_camera_ = back_camera;
    camera_start_location_ = ExternalCamera->GetActorLocation();
    camera_start_rotation_ = ExternalCamera->GetActorRotation();
    initial_ground_obs_offset_ = camera_start_location_ -
                                 (follow_actor_ ? follow_actor_->GetActorLocation() : FVector::ZeroVector);

    //set initial view mode
    switch (mode_) {
    case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_FLY_WITH_ME:
        inputEventFlyWithView();
        break;
    case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_FPV:
        inputEventFpvView();
        break;
    case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_GROUND_OBSERVER:
        inputEventGroundView();
        break;
    case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_MANUAL:
        inputEventManualView();
        break;
    case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_SPRINGARM_CHASE:
        inputEventSpringArmChaseView();
        break;
    case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_BACKUP:
        inputEventBackupView();
        break;
    case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_NODISPLAY:
        inputEventNoDisplayView();
        break;
    case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_FRONT:
        inputEventFrontView();
        break;
    default:
        throw std::out_of_range("Unsupported view mode specified in CameraDirector::initializeForBeginPlay");
    }
}

void ACameraManager::attachSpringArm(bool attach)
{
    if (attach) {
        //If we do have actor to follow AND don't have sprint arm attached to that actor, we will attach it
        if (follow_actor_ && ExternalCamera->GetRootComponent()->GetAttachParent() != SpringArm) {
            //For car, we want a bit of camera lag, as that is customary of racing video games
            //If the lag is missing, the camera will also occasionally shake.
            //But, lag is not desired when piloting a drone
            SpringArm->bEnableCameraRotationLag = camera_rotation_lag_enabled_;

            //attach spring arm to actor
            SpringArm->AttachToComponent(follow_actor_->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
            SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 34.0f));

            //remember current parent for external camera. Later when we remove external
            //camera from spring arm, we will attach it back to its last parent
            last_parent_ = ExternalCamera->GetRootComponent()->GetAttachParent();
            ExternalCamera->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
            //now attach camera to spring arm
            ExternalCamera->AttachToComponent(SpringArm, FAttachmentTransformRules::KeepRelativeTransform);
        }

        //For car, we need to move the camera back a little more than for a drone.
        //Otherwise, the camera will be stuck inside the car
        ExternalCamera->SetActorRelativeLocation(FVector(follow_distance_ * 100.0f, 0.0f, 0.0f));
        ExternalCamera->SetActorRelativeRotation(FRotator(10.0f, 0.0f, 0.0f));
        //ExternalCamera->bUsePawnControlRotation = false;
    }
    else { //detach
        if (last_parent_ && ExternalCamera->GetRootComponent()->GetAttachParent() == SpringArm) {
            ExternalCamera->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
            ExternalCamera->AttachToComponent(last_parent_, FAttachmentTransformRules::KeepRelativeTransform);
        }
    }
}

void ACameraManager::setMode(ECameraDirectorMode mode)
{
    { //first remove any settings done by previous mode

        //detach spring arm
        if (mode_ == ECameraDirectorMode::CAMERA_DIRECTOR_MODE_SPRINGARM_CHASE &&
            mode != ECameraDirectorMode::CAMERA_DIRECTOR_MODE_SPRINGARM_CHASE) {
            attachSpringArm(false);
        }

        // Re-enable rendering
        if (mode_ == ECameraDirectorMode::CAMERA_DIRECTOR_MODE_NODISPLAY &&
            mode != ECameraDirectorMode::CAMERA_DIRECTOR_MODE_NODISPLAY) {
            UAirBlueprintLib::enableViewportRendering(this, true);
        }

        //Remove any existing key bindings for manual mode
        if (mode != ECameraDirectorMode::CAMERA_DIRECTOR_MODE_MANUAL) {
            if (ExternalCamera != nullptr && manual_pose_controller_->getActor() == ExternalCamera) {

                manual_pose_controller_->setActor(nullptr);
            }
            //else someone else is bound to manual pose controller, leave it alone
        }
    }

    { //perform any settings to enter in to this mode

        switch (mode) {
        case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_MANUAL:
            //if new mode is manual mode then add key bindings
            manual_pose_controller_->setActor(ExternalCamera);
            break;
        case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_SPRINGARM_CHASE:
            //if we switched to spring arm mode then attach to spring arm (detachment was done earlier in method)
            attachSpringArm(true);
            break;
        case ECameraDirectorMode::CAMERA_DIRECTOR_MODE_NODISPLAY:
            UAirBlueprintLib::enableViewportRendering(this, false);
            break;
        default:
            //other modes don't need special setup
            break;
        }
    }

    //make switch official
    mode_ = mode;
}

void ACameraManager::setupInputBindings()
{
    UAirBlueprintLib::EnableInput(this);

    UAirBlueprintLib::BindActionToKey("inputEventFpvView", EKeys::F, this, &ACameraManager::inputEventFpvView);
    UAirBlueprintLib::BindActionToKey("inputEventFlyWithView", EKeys::B, this, &ACameraManager::inputEventFlyWithView);
    UAirBlueprintLib::BindActionToKey("inputEventGroundView", EKeys::Backslash, this, &ACameraManager::inputEventGroundView);
    UAirBlueprintLib::BindActionToKey("inputEventManualView", EKeys::M, this, &ACameraManager::inputEventManualView);
    UAirBlueprintLib::BindActionToKey("inputEventSpringArmChaseView", EKeys::Slash, this, &ACameraManager::inputEventSpringArmChaseView);
    UAirBlueprintLib::BindActionToKey("inputEventBackupView", EKeys::K, this, &ACameraManager::inputEventBackupView);
    UAirBlueprintLib::BindActionToKey("inputEventNoDisplayView", EKeys::Hyphen, this, &ACameraManager::inputEventNoDisplayView);
    UAirBlueprintLib::BindActionToKey("inputEventFrontView", EKeys::I, this, &ACameraManager::inputEventFrontView);
}

void ACameraManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    manual_pose_controller_ = nullptr;
    SpringArm = nullptr;
    ExternalCamera = nullptr;
}

APIPCamera* ACameraManager::getFpvCamera() const
{
    return fpv_camera_;
}

APIPCamera* ACameraManager::getExternalCamera() const
{
    return ExternalCamera;
}

APIPCamera* ACameraManager::getBackupCamera() const
{
    return backup_camera_;
}

int ACameraManager::connectCameraStream(const std::string& ip_addr, int port, const UnrealImageCapture *image_capture, const msr::airlib::ImageCaptureBase::ImageRequest request)
{
    const ACameraManager::CameraStream stream = ACameraManager::CameraStream(ip_addr, port, image_capture, request);
    this->streams_.push_back(stream);
    return port;
    // const auto& response = getImages(request, camera_details.vehicle_name, camera_details.external);
    // if (response.size() > 0)
    //     return response.at(0).image_data_uint8;
    // else
    //     return std::vector<uint8_t>();
}



void ACameraManager::inputEventSpringArmChaseView()
{
    if (ExternalCamera) {
        setMode(ECameraDirectorMode::CAMERA_DIRECTOR_MODE_SPRINGARM_CHASE);
        ExternalCamera->showToScreen();
        disableCameras(true, true, false, true);
    }
    else
        UAirBlueprintLib::LogMessageString("Camera is not available: ", "ExternalCamera", LogDebugLevel::Failure);

    notifyViewModeChanged();
}

void ACameraManager::inputEventGroundView()
{
    if (ExternalCamera) {
        setMode(ECameraDirectorMode::CAMERA_DIRECTOR_MODE_GROUND_OBSERVER);
        ExternalCamera->showToScreen();
        disableCameras(true, true, false, true);
        ext_obs_fixed_z_ = true;
    }
    else
        UAirBlueprintLib::LogMessageString("Camera is not available: ", "ExternalCamera", LogDebugLevel::Failure);

    notifyViewModeChanged();
}

void ACameraManager::inputEventManualView()
{
    if (ExternalCamera) {
        setMode(ECameraDirectorMode::CAMERA_DIRECTOR_MODE_MANUAL);
        ExternalCamera->showToScreen();
        disableCameras(true, true, false, true);
    }
    else
        UAirBlueprintLib::LogMessageString("Camera is not available: ", "ExternalCamera", LogDebugLevel::Failure);

    notifyViewModeChanged();
}

void ACameraManager::inputEventNoDisplayView()
{
    if (ExternalCamera) {
        setMode(ECameraDirectorMode::CAMERA_DIRECTOR_MODE_NODISPLAY);
        disableCameras(true, true, true, true);
    }
    else
        UAirBlueprintLib::LogMessageString("Camera is not available: ", "ExternalCamera", LogDebugLevel::Failure);

    notifyViewModeChanged();
}

void ACameraManager::inputEventBackupView()
{
    if (backup_camera_) {
        setMode(ECameraDirectorMode::CAMERA_DIRECTOR_MODE_BACKUP);
        backup_camera_->showToScreen();
        disableCameras(true, false, true, true);
    }
    else
        UAirBlueprintLib::LogMessageString("Camera is not available: ", "backup_camera", LogDebugLevel::Failure);

    notifyViewModeChanged();
}

void ACameraManager::inputEventFrontView()
{
    if (front_camera_) {
        setMode(ECameraDirectorMode::CAMERA_DIRECTOR_MODE_FRONT);
        front_camera_->showToScreen();
        disableCameras(true, true, true, false);
    }
    else
        UAirBlueprintLib::LogMessageString("Camera is not available: ", "backup_camera", LogDebugLevel::Failure);

    notifyViewModeChanged();
}

void ACameraManager::inputEventFlyWithView()
{
    if (ExternalCamera) {
        setMode(ECameraDirectorMode::CAMERA_DIRECTOR_MODE_FLY_WITH_ME);
        ExternalCamera->showToScreen();

        if (follow_actor_)
            ExternalCamera->SetActorLocationAndRotation(
                follow_actor_->GetActorLocation() + initial_ground_obs_offset_, camera_start_rotation_);
        disableCameras(true, true, false, true);
        ext_obs_fixed_z_ = false;
    }
    else
        UAirBlueprintLib::LogMessageString("Camera is not available: ", "ExternalCamera", LogDebugLevel::Failure);

    notifyViewModeChanged();
}

void ACameraManager::inputEventFpvView()
{
    if (fpv_camera_) {
        setMode(ECameraDirectorMode::CAMERA_DIRECTOR_MODE_FPV);
        fpv_camera_->showToScreen();
        disableCameras(false, true, true, true);
    }
    else
        UAirBlueprintLib::LogMessageString("Camera is not available: ", "fpv_camera", LogDebugLevel::Failure);

    notifyViewModeChanged();
}

void ACameraManager::disableCameras(bool fpv, bool backup, bool external, bool front)
{
    if (fpv && fpv_camera_)
        fpv_camera_->disableMain();
    if (backup && backup_camera_)
        backup_camera_->disableMain();
    if (external && ExternalCamera)
        ExternalCamera->disableMain();
    if (front && front_camera_)
        front_camera_->disableMain();
}

void ACameraManager::notifyViewModeChanged()
{
    bool nodisplay = ECameraDirectorMode::CAMERA_DIRECTOR_MODE_NODISPLAY == mode_;

    UWorld* world = GetWorld();
    UGameViewportClient* gameViewport = world->GetGameViewport();
    gameViewport->bDisableWorldRendering = nodisplay;
}
