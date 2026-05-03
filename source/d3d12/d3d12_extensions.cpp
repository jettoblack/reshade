/*
 * Copyright (C) 2026 Patrick Mours
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if RESHADE_ADDON >= 2

#include "d3d12_device.hpp"
#include "d3d12_extensions.hpp"
#include "com_utils.hpp"
#include "dll_log.hpp"
#include "hook_manager.hpp"

HRESULT ID3D12DeviceExt_GetCudaTextureObject(IUnknown *device_ext, D3D12_CPU_DESCRIPTOR_HANDLE srv_handle, D3D12_CPU_DESCRIPTOR_HANDLE sampler_handle, UINT32 *cuda_texture_handle)
{
	com_ptr<ID3D12Device> device;
	device_ext->QueryInterface(IID_PPV_ARGS(&device));
	assert(device != nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE orig_srv = srv_handle;
	D3D12_CPU_DESCRIPTOR_HANDLE orig_smp = sampler_handle;

	if (const auto device_proxy = get_private_pointer_d3dx<D3D12Device>(device.get()))
	{
		srv_handle = device_proxy->convert_to_original_cpu_descriptor_handle(srv_handle);
		sampler_handle = device_proxy->convert_to_original_cpu_descriptor_handle(sampler_handle);
	}

	/* DEBUG: log handle translation for GetCudaTextureObject */
	reshade::log::message(reshade::log::level::warning,
	    "RESHADE_DBG:GetCudaTextureObject thr=%lu srv_in=0x%zx srv_out=0x%zx smp_in=0x%zx smp_out=0x%zx",
	    (unsigned long)GetCurrentThreadId(), orig_srv.ptr, srv_handle.ptr, orig_smp.ptr, sampler_handle.ptr);

	return reshade::hooks::call(ID3D12DeviceExt_GetCudaTextureObject, reshade::hooks::vtable_from_instance(device_ext) + 7)(device_ext, srv_handle, sampler_handle, cuda_texture_handle);
}

HRESULT ID3D12DeviceExt_GetCudaSurfaceObject(IUnknown *device_ext, D3D12_CPU_DESCRIPTOR_HANDLE uav_handle, UINT32 *cuda_surface_handle)
{
	com_ptr<ID3D12Device> device;
	device_ext->QueryInterface(IID_PPV_ARGS(&device));
	assert(device != nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE orig_uav = uav_handle;

	if (const auto device_proxy = get_private_pointer_d3dx<D3D12Device>(device.get()))
	{
		uav_handle = device_proxy->convert_to_original_cpu_descriptor_handle(uav_handle);
	}

	/* DEBUG: log handle translation for GetCudaSurfaceObject */
	reshade::log::message(reshade::log::level::warning,
	    "RESHADE_DBG:GetCudaSurfaceObject thr=%lu uav_in=0x%zx uav_out=0x%zx",
	    (unsigned long)GetCurrentThreadId(), orig_uav.ptr, uav_handle.ptr);

	return reshade::hooks::call(ID3D12DeviceExt_GetCudaSurfaceObject, reshade::hooks::vtable_from_instance(device_ext) + 8)(device_ext, uav_handle, cuda_surface_handle);
}

HRESULT ID3D12DeviceExt2_GetCudaMergedTextureSamplerObject(IUnknown *device_ext, D3D12_GET_CUDA_MERGED_TEXTURE_SAMPLER_OBJECT_PARAMS *params)
{
	com_ptr<ID3D12Device> device;
	device_ext->QueryInterface(IID_PPV_ARGS(&device));
	assert(device != nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE orig_tex = params->texDesc;
	D3D12_CPU_DESCRIPTOR_HANDLE orig_smp = params->smpDesc;

	if (const auto device_proxy = get_private_pointer_d3dx<D3D12Device>(device.get()))
	{
		params->texDesc = device_proxy->convert_to_original_cpu_descriptor_handle(params->texDesc);
		params->smpDesc = device_proxy->convert_to_original_cpu_descriptor_handle(params->smpDesc);
	}

	/* DEBUG: log handle translation for GetCudaMergedTextureSamplerObject */
	reshade::log::message(reshade::log::level::warning,
	    "RESHADE_DBG:GetCudaMergedTextureSamplerObject thr=%lu tex_in=0x%zx tex_out=0x%zx smp_in=0x%zx smp_out=0x%zx",
	    (unsigned long)GetCurrentThreadId(), orig_tex.ptr, params->texDesc.ptr, orig_smp.ptr, params->smpDesc.ptr);

	return reshade::hooks::call(ID3D12DeviceExt2_GetCudaMergedTextureSamplerObject, reshade::hooks::vtable_from_instance(device_ext) + 14)(device_ext, params);
}

HRESULT ID3D12DeviceExt2_GetCudaIndependentDescriptorObject(IUnknown *device_ext, D3D12_GET_CUDA_INDEPENDENT_DESCRIPTOR_OBJECT_PARAMS *params)
{
	com_ptr<ID3D12Device> device;
	device_ext->QueryInterface(IID_PPV_ARGS(&device));
	assert(device != nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE orig_desc = params->desc;
	HRESULT hr = E_FAIL;

	if (const auto device_proxy = get_private_pointer_d3dx<D3D12Device>(device.get()))
	{
		params->desc = device_proxy->convert_to_original_cpu_descriptor_handle(params->desc);
	}

	hr = reshade::hooks::call(ID3D12DeviceExt2_GetCudaIndependentDescriptorObject, reshade::hooks::vtable_from_instance(device_ext) + 15)(device_ext, params);

	/* DEBUG: log full params and result */
	reshade::log::message(reshade::log::level::warning,
	    "RESHADE_DBG:GetCudaIndependentDescriptorObject thr=%lu desc_in=0x%zx desc_out=0x%zx type=%u hr=%#x handle=0x%llx",
	    (unsigned long)GetCurrentThreadId(), orig_desc.ptr, params->desc.ptr,
	    params->type, (unsigned)hr, (unsigned long long)params->handle);

	return hr;
}

#endif
