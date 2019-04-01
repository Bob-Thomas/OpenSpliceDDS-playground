#pragma once
#include <string>
#include "wsseapi.h"
#include "wsaapi.h"
#include "soapDeviceBindingProxy.h"

class OnvifDevice
{
  private:
    DeviceBindingProxy proxyDevice;
    std::string username;
    std::string password;
    std::string endpoint;
    bool ptzEnabled = false;
    bool mediaEnabled = false;
    struct soap *soap;

  public:
    OnvifDevice(std::string username, std::string password, std::string ip) : username(username), password(password), endpoint(
                                                                                                                          "http://" + ip + "/onvif/device_service")
    {
        proxyDevice.soap_endpoint = endpoint.c_str();
        soap_register_plugin(proxyDevice.soap, soap_wsse);
        soap = soap_new();

        if (!this->isAuthenticated(proxyDevice.soap))
        {
            std::cerr << "Authentication failed\n";
        }
        std::cout << "Connection succesful retrieving information and capabilities\n";

        _tds__GetDeviceInformation getDeviceInformation;
        _tds__GetDeviceInformationResponse getDeviceInformationResponse;

        if (SOAP_OK == proxyDevice.GetDeviceInformation(&getDeviceInformation, getDeviceInformationResponse))
        {
            std::cout << "Manufacturer: " << getDeviceInformationResponse.Manufacturer << "\n";
            std::cout << "Device model: " << getDeviceInformationResponse.Model << "\n";
        }
        else
        {
            std::cerr << "Failed to retrieve information";
        }
        soap_destroy(soap);
        soap_end(soap);

        if (!this->isAuthenticated(proxyDevice.soap))
        {
            std::cerr << "Authentication failed\n";
        }
        _tds__GetCapabilities capabilities;
        _tds__GetCapabilitiesResponse capabilitiesResponse;
        std::cout << "Capabilities:\n";
        if (SOAP_OK == proxyDevice.GetCapabilities(&capabilities, capabilitiesResponse))
        {
            if (capabilitiesResponse.Capabilities->Media != NULL)
            {
                mediaEnabled = true;
            }
            if (capabilitiesResponse.Capabilities->PTZ != NULL)
            {
                ptzEnabled = true;
            }
        }
        else
        {
            std::cerr << "Failed to retrieve capabilities";
        }
        std::cout << "\t Media enabled:" << ((mediaEnabled) ? "TRUE" : "FALSE") << "\n";
        std::cout << "\t PTZ enabled:" << ((ptzEnabled) ? "TRUE" : "FALSE") << "\n";

        soap_destroy(soap);
        soap_end(soap);
    }
    std::string getUserName()
    {
        return username;
    }

    std::string getPassword()
    {
        return password;
    }

    const char *getEndpoint()
    {
        return endpoint.c_str();
    }

    struct soap *getSoap()
    {
        return soap;
    }

    bool isAuthenticated(struct soap *soap)
    {
        if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(soap, NULL, username.c_str(), password.c_str()))
        {
            return -1;
        }

        if (SOAP_OK != soap_wsse_add_Timestamp(soap, "Time", 10))
        {
            return -1;
        }
        return 1;
    }

    bool hasMedia()
    {
        return mediaEnabled;
    }

    bool hasPtz()
    {
        return ptzEnabled;
    }
};
