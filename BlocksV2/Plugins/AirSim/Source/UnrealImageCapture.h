#pragma once

#include "CoreMinimal.h"
#include "PIPCamera.h"
#include "common/ImageCaptureBase.hpp"
#include "common/common_utils/UniqueValueMap.hpp"

class UnrealImageCapture : public msr::airlib::ImageCaptureBase
{
public:
    typedef msr::airlib::ImageCaptureBase::ImageType ImageType;

    UnrealImageCapture(const common_utils::UniqueValueMap<std::string, APIPCamera*>* cameras);
    virtual ~UnrealImageCapture();
    
    virtual void getImages(const std::vector<ImageRequest>& requests,
                                    std::vector<ImageResponse>& responses,
                                    bool use_safe_method = false,
                                    bool async = false) const override;
private:
    void getSceneCaptureImage(const std::vector<ImageRequest>& requests,
                              std::vector<ImageResponse>& responses,
                              bool use_safe_method,
                              bool async = false) const;

    void addScreenCaptureHandler(UWorld* world);
    bool getScreenshotScreen(ImageType image_type, std::vector<uint8_t>& compressedPng);

    bool updateCameraVisibility(APIPCamera* camera, const ImageRequest& request);

private:
    const common_utils::UniqueValueMap<std::string, APIPCamera*>* cameras_;
    std::vector<uint8_t> last_compressed_png_;

};
