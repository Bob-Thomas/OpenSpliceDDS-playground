#pragma once
#include "wsseapi.h"
#include "wsaapi.h"
#include <src/onvif/OnvifDevice.hpp>
#include <soapMediaBindingProxy.h>

class OnvifMedia
{
  private:
    OnvifDevice &device;
    MediaBindingProxy media;

  public:
    OnvifMedia(OnvifDevice &device) : device(device)
    {
        if (device.hasMedia())
        {
            media.soap_endpoint = device.getEndpoint();
            soap_register_plugin(media.soap, soap_wsse);
        }
        else
        {
            throw "Camera does not support media";
        }
    }

    int createProfile(std::string name, std::string token, _trt__CreateProfileResponse response)
    {
        if (!device.isAuthenticated(media.soap))
        {
            std::cerr << "Connection not authenticated\n";
            return -1;
        }

        _trt__CreateProfile request;
        request.Name = name;
        request.Token = &token;

        int result = media.CreateProfile(&request, response);
        std::cout << *soap_faultstring(media.soap);
        soap_destroy(device.getSoap());
        soap_end(device.getSoap());
        return result;
    }

    int getProfiles(_trt__GetProfilesResponse response)
    {
        if (!device.isAuthenticated(media.soap))
        {
            std::cerr << "Connection not authenticated\n";
            return -1;
        }

        _trt__GetProfiles request;
        int result = media.GetProfiles(&request, response);
        soap_destroy(device.getSoap());
        soap_end(device.getSoap());
        return result;
    }

    int getProfile(std::string token, _trt__GetProfileResponse response)
    {
        if (!device.isAuthenticated(media.soap))
        {
            std::cerr << "Connection not authenticated\n";
            return -1;
        }

        _trt__GetProfile request;
        request.ProfileToken = token;

        int result = media.GetProfile(&request, response);
        // std::cout << *soap_faultstring(media.soap);

        soap_destroy(device.getSoap());
        soap_end(device.getSoap());
        return result;
    }

    int addPtzConfiguration(std::string profileToken, std::string configurationToken, _trt__AddPTZConfigurationResponse response)
    {
        if (!device.isAuthenticated(media.soap))
        {
            std::cerr << "Connection not authenticated\n";
            return -1;
        }

        _trt__AddPTZConfiguration request;
        request.ProfileToken = profileToken;
        request.ConfigurationToken = configurationToken;

        int result = media.AddPTZConfiguration(&request, response);
        soap_destroy(device.getSoap());
        soap_end(device.getSoap());
        return result;
    }
};