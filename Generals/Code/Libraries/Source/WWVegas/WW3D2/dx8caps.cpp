/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : dx8 caps                                                     *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/ww3d2/dx8caps.cpp                            $*
 *                                                                                             *
 *              Original Author:: Hector Yee                                                   *
 *                                                                                             *
 *                       Author : Kenny Mitchell                                               * 
 *                                                                                             * 
 *                     $Modtime:: 06/27/02 1:27p                                              $*
 *                                                                                             *
 *                    $Revision:: 31                                                          $*
 *                                                                                             *
 * 06/27/02 KM Z Format support																						*
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "dx8caps.h"
#include "dx8wrapper.h"
#include "formconv.h"
#pragma warning (disable : 4201)		// nonstandard extension - nameless struct
#include <windows.h>
#include <mmsystem.h>

static StringClass CapsWorkString;


enum {
	VENDOR_ID_NVIDIA=0x10de,
	VENROD_ID_ATI=0x1002
};


static const char* VendorNames[]={
	"Unknown",
	"NVidia",
	"ATI",
	"Intel",
	"S3",
	"PowerVR",
	"Matrox",
	"3Dfx",
	"3DLabs",
	"CirrusLogic",
	"Rendition"
};

DX8Caps::VendorIdType DX8Caps::Define_Vendor(unsigned vendor_id)
{
	return VENDOR_UNKNOWN;
}

static const char* DeviceNamesNVidia[]={
	"Unknown NVidia device",
	"GeForce3",
	"Quadro2 PRO",
	"GeForce2 Go",
	"GeForce2 ULTRA",
	"GeForce2 GTS",
	"Quadro",
	"GeForce DDR",
	"GeForce 256",
	"TNT2 Aladdin",
	"TNT2",
	"TNT2 ULTRA",
	"TNT2 Vanta",
	"TNT2 M64",
	"TNT",
	"RIVA 128",
	"TNT Vanta",
	"NV1",
	"GeForce2 MX"
	"GeForce4 Ti 4600",
	"GeForce4 Ti 4400",
	"GeForce4 Ti",
	"GeForce4 Ti 4200",
	"GeForce4 MX 460",
	"GeForce4 MX 440",
	"GeForce4 MX 420",
	"GeForce4",
	"GeForce4 Go 440",
	"GeForce4 Go 420",
	"GeForce4 Go 420 32M",
	"GeForce4 Go 440 64M",
	"GeForce4 Go",
	"GeForce3 Ti 500",
	"GeForce3 Ti 200",
	"GeForce2 Integrated",
	"GeForce2 Ti",
	"Quadro2 MXR//EX//GO",
	"GeFORCE2_MX 100//200",
	"GeFORCE2_MX 400",
	"Quadro DCC"
};

static const char* DeviceNamesATI[]={
	"Unknown ATI Device",
	"Rage II",
	"Rage II+",
	"Rage IIc PCI",
	"Rage IIc AGP",
	"Rage 128 Mobility",
	"Rage 128 Mobility M3",
	"Rage 128 Mobility M4",
	"Rage 128 PRO ULTRA",
	"Rage 128 4X",
	"Rage 128 PRO GL",
	"Rage 128 PRO VR",
	"Rage 128 GL",
	"Rage 128 VR",
	"Rage PRO",
	"Rage PRO Mobility",
	"Mobility Radeon",
	"Mobility Radeon VE(M6)",
	"Radeon VE",
	"Radeon DDR",
	"Radeon",
	"Mobility R7500",
	"R7500",
	"R8500"
};

static const char* DeviceNames3DLabs[]={
	"Unknown 3DLabs Device",
	"Permedia",
	"300SX",
	"500TX",
	"Delta",
	"MX",
	"Gamma",
	"Permedia2S (ST)",
	"Permedia3",
	"R3",
	"Permedia4",
	"R4",
	"G2",
	"Oxygen VX1",
	"TI P1",
	"Permedia2"
};

static const char* DeviceNames3Dfx[]={
	"Unknown 3Dfx Device",
	"Voodoo 5500 AGP",
	"Voodoo 3",
	"Banshee",
	"Voodoo 2",
	"Voodoo Graphics",
	"Voodoo Rush"
};

static const char* DeviceNamesMatrox[]={
	"Unknown Matrox Device",
	"G550",
	"G400",
	"G200 AGP",
	"G200 PCI",
	"G100 PCI",
	"G100 AGP",
	"Millennium II AGP",
	"Millennium II PCI",
	"Mystique",
	"Millennium",
	"Parhelia",
	"Parhelia AGP 8X"
};

static const char* DeviceNamesPowerVR[]={
	"Unknown PowerVR Device",
	"Kyro"
};

static const char* DeviceNamesS3[]={
	"Unknown S3 Device",
	"Savage MX",
	"Savage 4",
	"Savage 200"
};

static const char* DeviceNamesIntel[]={
	"Unknown Intel Device",
	"i810",
	"i810e",
	"i815"
};

DX8Caps::DeviceTypeATI DX8Caps::Get_ATI_Device(unsigned device_id)
{
	return DEVICE_ATI_UNKNOWN;
}
DX8Caps::DeviceType3DLabs DX8Caps::Get_3DLabs_Device(unsigned device_id)
{
	return DEVICE_3DLABS_UNKNOWN;
}
DX8Caps::DeviceTypeNVidia DX8Caps::Get_NVidia_Device(unsigned device_id)
{
	return DEVICE_NVIDIA_UNKNOWN;
}
DX8Caps::DeviceType3Dfx DX8Caps::Get_3Dfx_Device(unsigned device_id)
{
	return DEVICE_3DFX_UNKNOWN;
}

DX8Caps::DeviceTypeMatrox DX8Caps::Get_Matrox_Device(unsigned device_id)
{
	return DEVICE_MATROX_UNKNOWN;
}
 
DX8Caps::DeviceTypePowerVR DX8Caps::Get_PowerVR_Device(unsigned device_id)
{
	return DEVICE_POWERVR_UNKNOWN;
}

DX8Caps::DeviceTypeS3 DX8Caps::Get_S3_Device(unsigned device_id)
{
	return DEVICE_S3_UNKNOWN;
}

DX8Caps::DeviceTypeIntel DX8Caps::Get_Intel_Device(unsigned device_id)
{
	return DEVICE_INTEL_UNKNOWN;
}

DX8Caps::DX8Caps(
	IDirect3D8* direct3d,
	IDirect3DDevice8* D3DDevice, 
	WW3DFormat display_format, 
	const D3DADAPTER_IDENTIFIER8& adapter_id)
	:
	Direct3D(direct3d),
	MaxDisplayWidth(0),
	MaxDisplayHeight(0)
{
	Init_Caps(D3DDevice);
	Compute_Caps(display_format, adapter_id);
}

//Don't really need this but I added this function to free static variables so
//they don't show up in our memory manager as a leak. -MW 7-22-03
void DX8Caps::Shutdown(void)
{
	CapsWorkString.Release_Resources();
}

// ----------------------------------------------------------------------------
//
// Init the caps structure
//
// ----------------------------------------------------------------------------

void DX8Caps::Init_Caps(IDirect3DDevice8* D3DDevice)
{
	D3DDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING,TRUE);
	DX8CALL(GetDeviceCaps(&swVPCaps));

	if ((swVPCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)==D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		SupportTnL=true;

		D3DDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING,FALSE);
		DX8CALL(GetDeviceCaps(&hwVPCaps));	
	} else {
		SupportTnL=false;			
	}
}

// ----------------------------------------------------------------------------
//
// Compute the caps bits
//
// ----------------------------------------------------------------------------
void DX8Caps::Compute_Caps(WW3DFormat display_format, const D3DADAPTER_IDENTIFIER8& adapter_id)
{
//	Init_Caps(D3DDevice);

	const D3DCAPS8& caps=Get_DX8_Caps();

	if ((caps.DevCaps&D3DDEVCAPS_NPATCHES)==D3DDEVCAPS_NPATCHES) {
		SupportNPatches=true;
	} else {
		SupportNPatches=false;
	}

	if ((caps.TextureOpCaps&D3DTEXOPCAPS_DOTPRODUCT3)==D3DTEXOPCAPS_DOTPRODUCT3) 
	{
		SupportDot3=true;
	} else {
		SupportDot3=false;
	}

	supportGamma=((swVPCaps.Caps2&D3DCAPS2_FULLSCREENGAMMA)==D3DCAPS2_FULLSCREENGAMMA);

	MaxTexturesPerPass=MAX_TEXTURE_STAGES;

	Check_Texture_Format_Support(display_format,caps);
	Check_Texture_Compression_Support(caps);
	Check_Bumpmap_Support(caps);
	Check_Shader_Support(caps);
	Check_Maximum_Texture_Support(caps);
	Vendor_Specific_Hacks(adapter_id);
	CapsWorkString="";
}

// ----------------------------------------------------------------------------
//
// Check bump map texture support
//
// ----------------------------------------------------------------------------

void DX8Caps::Check_Bumpmap_Support(const D3DCAPS8& caps)
{
	SupportBumpEnvmap = TRUE;
	SupportBumpEnvmapLuminance = TRUE;
}

// ----------------------------------------------------------------------------
//
// Check compressed texture support
//
// ----------------------------------------------------------------------------

void DX8Caps::Check_Texture_Compression_Support(const D3DCAPS8& caps)
{
	SupportDXTC = TRUE;
}

void DX8Caps::Check_Texture_Format_Support(WW3DFormat display_format,const D3DCAPS8& caps)
{
	if (display_format==WW3D_FORMAT_UNKNOWN) {
		for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i)
			SupportTextureFormat[i]=false;
		return;
	}
	D3DFORMAT d3d_display_format=WW3DFormat_To_D3DFormat(display_format);
	for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
		if (i==WW3D_FORMAT_UNKNOWN) {
			SupportTextureFormat[i]=false;
		}
		else {
			WW3DFormat format=(WW3DFormat)i;
			SupportTextureFormat[i]=SUCCEEDED(
				Direct3D->CheckDeviceFormat(
					caps.AdapterOrdinal,
					caps.DeviceType,
					d3d_display_format,
					0,
					D3DRTYPE_TEXTURE,
					WW3DFormat_To_D3DFormat(format)));
		}
	}
}

void DX8Caps::Check_Render_To_Texture_Support(WW3DFormat display_format,const D3DCAPS8& caps)
{
	if (display_format==WW3D_FORMAT_UNKNOWN) {
		for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
			SupportRenderToTextureFormat[i]=false;
		}
		return;
	}
	D3DFORMAT d3d_display_format=WW3DFormat_To_D3DFormat(display_format);
	for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
		if (i==WW3D_FORMAT_UNKNOWN) {
			SupportRenderToTextureFormat[i]=false;
		}
		else {
			WW3DFormat format=(WW3DFormat)i;
			SupportRenderToTextureFormat[i]=SUCCEEDED(
				Direct3D->CheckDeviceFormat(
					caps.AdapterOrdinal,
					caps.DeviceType,
					d3d_display_format,
					D3DUSAGE_RENDERTARGET,
					D3DRTYPE_TEXTURE,
					WW3DFormat_To_D3DFormat(format)));
		}
	}
}

//**********************************************************************************************
//! Check Depth Stencil Format Support
/*! KJM
*/
void DX8Caps::Check_Depth_Stencil_Support(WW3DFormat display_format, const D3DCAPS8& caps)
{
	if (display_format==WW3D_FORMAT_UNKNOWN) 
	{
		for (unsigned i=0;i<WW3D_ZFORMAT_COUNT;++i) 
		{
			SupportDepthStencilFormat[i]=false;
		}
		return;
	}

	D3DFORMAT d3d_display_format=WW3DFormat_To_D3DFormat(display_format);
	
	for (unsigned i=0;i<WW3D_ZFORMAT_COUNT;++i) 
	{
		if (i==WW3D_ZFORMAT_UNKNOWN) 
		{
			SupportDepthStencilFormat[i]=false;
		}
		else 
		{
			WW3DZFormat format=(WW3DZFormat)i;
			SupportDepthStencilFormat[i]=SUCCEEDED
			(
				Direct3D->CheckDeviceFormat
				(
					caps.AdapterOrdinal,
					caps.DeviceType,
					d3d_display_format,
					D3DUSAGE_DEPTHSTENCIL,
					D3DRTYPE_TEXTURE,
					WW3DZFormat_To_D3DFormat(format)
				)
			);
		}
	}
}

void DX8Caps::Check_Maximum_Texture_Support(const D3DCAPS8& caps)
{
	MaxSimultaneousTextures=caps.MaxSimultaneousTextures;
}

void DX8Caps::Check_Shader_Support(const D3DCAPS8& caps)
{
	VertexShaderVersion=caps.VertexShaderVersion;
	PixelShaderVersion=caps.PixelShaderVersion;
}

// ----------------------------------------------------------------------------
//
// Implement some vendor-specific hacks to fix certain driver bugs that can't be
// avoided otherwise.
//
// ----------------------------------------------------------------------------

void DX8Caps::Vendor_Specific_Hacks(const D3DADAPTER_IDENTIFIER8& adapter_id)
{
	if (adapter_id.VendorId==VENDOR_ID_NVIDIA) {
		SupportNPatches = false;	// Driver incorrectly report N-Patch support
		SupportTextureFormat[WW3D_FORMAT_DXT1] = false;			// DXT1 is broken on NVidia hardware
		SupportDXTC=
			SupportTextureFormat[WW3D_FORMAT_DXT1]|
			SupportTextureFormat[WW3D_FORMAT_DXT2]|
			SupportTextureFormat[WW3D_FORMAT_DXT3]|
			SupportTextureFormat[WW3D_FORMAT_DXT4]|
			SupportTextureFormat[WW3D_FORMAT_DXT5];
	}

//	SupportDXTC=false;

}

