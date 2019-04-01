/**
 * Quick example original -> https://github.com/Sufi-Al-Hussaini/onvif-gsoap-by-example
 * Editted to be more barebone will be replaced by own impl
 * ./ipconvif -cIp '192.168.1.53' -cUsr 'admin' -cPwd 'admin' -fIp '192.168.1.51:21' -fUsr 'ftpuser' -fPwd 'Ftpftp123
 *
 */

#include "DeviceBinding.nsmap"
#include <iostream>
#include "wsseapi.h"
#include "wsaapi.h"
#include <stdarg.h> // For va_start, etc.
#include <algorithm>
#include <chrono>
#include <thread> // std::thread, std::this_thread::sleep_for
#include <src/onvif/OnvifDevice.hpp>
#include <src/onvif/OnvifMedia.hpp>
#include <src/onvif/OnvifPTZ.hpp>
#include <src/cameraDDS.hpp>

char *getCmdOption(char **begin, char **end, const std::string &option)
{
	char **itr = std::find(begin, end, option);
	if (itr != end && ++itr != end)
	{
		return *itr;
	}
	return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
	return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[])
{

	// Proxy declarations
	DeviceBindingProxy proxyDevice;
	PTZBindingProxy proxyPTZ;
	// RemoteDiscoveryBindingProxy proxyDiscovery;
	MediaBindingProxy proxyMedia;

	if (!(cmdOptionExists(argv, argv + argc, "-cIp") && cmdOptionExists(argv, argv + argc, "-cUsr") && cmdOptionExists(argv, argv + argc, "-cPwd")))
	{
		std::cout << "usage: ./ipconvif -cIp [<camera-ip>:<port>] -cUsr <cam-id> -cPwd <cam-pwd>\n";
		return -1;
	}

	char *camIp = getCmdOption(argv, argv + argc, "-cIp");
	char *camUsr = getCmdOption(argv, argv + argc, "-cUsr");
	char *camPwd = getCmdOption(argv, argv + argc, "-cPwd");

	std::string ptzConfigurationToken = "0";
	std::string profile = "PTZ_ONVIF1";
	std::string profileToken = "PTZ_ONVIF_TOKEN1";

	OnvifDevice device(camUsr, camPwd, camIp);
	OnvifMedia media(device);

	_trt__GetProfileResponse getProfileResponse;
	if (SOAP_OK == media.getProfile(profileToken, getProfileResponse))
	{
		std::cout << "retrieved profile\n";
	}
	else
	{
		std::cerr << "error happened retrieving profile\nCreating new one\n";

		_trt__CreateProfileResponse createProfileResponse;
		if (SOAP_OK == media.createProfile(profile, profileToken, createProfileResponse))
		{
			_trt__AddPTZConfigurationResponse addPTZConfigurationResponse;
			if (SOAP_OK == media.addPtzConfiguration(profileToken, ptzConfigurationToken, addPTZConfigurationResponse))
			{
			}
			else
			{
				std::cout << "Failed to create ptz configuration\n";
			}
		}
		else
		{
			std::cout << "Failed to create profile\n";
			return -1;
		}
	}

	OnvifPTZ ptz(device, profileToken, ptzConfigurationToken);

	CameraDDS dds;

	dds.setCameraUpdateCallback([&](float pan, float tilt, float zoom){
		_tptz__ContinuousMoveResponse response;
		ptz.continuousMove(pan, tilt, zoom, response);
	});

	while (true)
	{
		dds.update();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}

	return 0;
}
