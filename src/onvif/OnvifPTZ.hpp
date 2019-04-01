#pragma once
#include "wsseapi.h"
#include "wsaapi.h"
#include <src/onvif/OnvifDevice.hpp>
#include <soapPTZBindingProxy.h>

class OnvifPTZ
{
  private:
    OnvifDevice &device;
    PTZBindingProxy ptz;
    std::string profileToken;
    bool configurationLoaded = false;
    float panLimit[2] = {0, 0};
    float tiltLimit[2] = {0, 0};
    float zoomLimit[2] = {0, 0};

  public:
    OnvifPTZ(OnvifDevice &device, std::string profileToken, std::string configurationToken) : device(device), profileToken(profileToken)
    {
        if (device.hasPtz())
        {
            ptz.soap_endpoint = device.getEndpoint();
            soap_register_plugin(ptz.soap, soap_wsse);

            _tptz__GetConfiguration getConfiguration;
            _tptz__GetConfigurationResponse getConfigurationResponse;
            getConfiguration.PTZConfigurationToken = configurationToken;

            if (!device.isAuthenticated(ptz.soap))
            {
                std::cerr << "Connection not authenticated\n";
                throw "Camera is not authenticated\n";
            }

            if (SOAP_OK == ptz.GetConfiguration(&getConfiguration, getConfigurationResponse))
            {
                panLimit[0] = getConfigurationResponse.PTZConfiguration->PanTiltLimits->Range->XRange->Min;
                panLimit[1] = getConfigurationResponse.PTZConfiguration->PanTiltLimits->Range->XRange->Max;

                tiltLimit[0] = getConfigurationResponse.PTZConfiguration->PanTiltLimits->Range->YRange->Min;
                tiltLimit[1] = getConfigurationResponse.PTZConfiguration->PanTiltLimits->Range->YRange->Max;

                zoomLimit[0] = getConfigurationResponse.PTZConfiguration->ZoomLimits->Range->XRange->Min;
                zoomLimit[1] = getConfigurationResponse.PTZConfiguration->ZoomLimits->Range->XRange->Max;

                configurationLoaded = true;
                std::cout << "PAN LIMITS - " << panLimit[0] << " : " << panLimit[1] << "\n";
                std::cout << "TILT LIMITS - " << tiltLimit[0] << " : " << tiltLimit[1] << "\n";
            }
            else
            {
                std::cerr << *soap_faultstring(ptz.soap) << "\n";
                std::cerr << "Failed to load ptz config\n";
            }
            soap_destroy(device.getSoap());
            soap_end(device.getSoap());
        }
        else
        {
            throw "Camera does not support PTZ";
        }
    }

    int continuousMove(float pan, float tilt, float zoom, _tptz__ContinuousMoveResponse response)
    {
        if (!configurationLoaded)
        {
            std::cerr << "PTZ configuration not loaded\n";
            return -1;
        }
        if (pan < panLimit[0] || pan > panLimit[1])
        {
            std::cerr << "Pan speed is not within set limits\n";
            return -1;
        }
        if (tilt < tiltLimit[0] || tilt > tiltLimit[1])
        {
            std::cerr << "tilt speed is not within set limits\n";
            return -1;
        }
        if (zoom < zoomLimit[0] || zoom > zoomLimit[1])
        {
            std::cerr << "tilt speed is not within set limits\n";
            return -1;
        }

        _tptz__ContinuousMove request;
        request.Velocity = new tt__PTZSpeed();
        request.Velocity->PanTilt = new tt__Vector2D();
        request.Velocity->PanTilt->x = pan;
        request.Velocity->PanTilt->y = tilt;
        request.Velocity->Zoom = new tt__Vector1D();
        request.Velocity->Zoom->x = zoom;

        request.ProfileToken = profileToken;

        if (!device.isAuthenticated(ptz.soap))
        {
            std::cerr << "Connection not authenticated\n";
            return -1;
        }

        int result = ptz.ContinuousMove(&request, response);
        std::cerr << *soap_faultstring(ptz.soap) << "\n";
        soap_destroy(device.getSoap());
        soap_end(device.getSoap());

        return result;
    }
};
