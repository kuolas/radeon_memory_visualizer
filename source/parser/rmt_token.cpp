//=============================================================================
// Copyright (c) 2019-2022 Advanced Micro Devices, Inc. All rights reserved.
/// @author AMD Developer Tools Team
/// @file
/// @brief  Implementation of functions related to RMT token structures.
//=============================================================================

#include "rmt_assert.h"
#include "rmt_token.h"

#include <vector>

// The array of user data allocations, so they can be cleared up later.
static std::vector<uint8_t*> allocs;

uint8_t* RmtTokenAllocatePayloadCache(size_t size)
{
    uint8_t* memory = new uint8_t[size];
    allocs.push_back(memory);
    return memory;
}

void RmtTokenClearPayloadCaches()
{
    for (auto it : allocs)
    {
        delete[] (it);
    }
    allocs.clear();
}

void RmtTokenCopy(RmtToken* dest, const RmtToken* src)
{
    if (src == dest)
    {
        return;
    }

    dest->type = src->type;

    memcpy(&dest->common, &src->common, sizeof(RmtTokenCommon));

    switch (src->type)
    {
    case RmtTokenType::kRmtTokenTypeCpuMap:
        memcpy(&dest->cpu_map_token, &src->cpu_map_token, sizeof(RmtTokenCpuMap));
        break;

    case RmtTokenType::kRmtTokenTypeMisc:
        memcpy(&dest->misc_token, &src->misc_token, sizeof(RmtTokenMisc));
        break;

    case RmtTokenType::kRmtTokenTypePageReference:
        memcpy(&dest->page_reference_token, &src->page_reference_token, sizeof(RmtTokenPageReference));
        break;

    case RmtTokenType::kRmtTokenTypePageTableUpdate:
        memcpy(&dest->page_table_update_token, &src->page_table_update_token, sizeof(RmtTokenPageTableUpdate));
        break;

    case RmtTokenType::kRmtTokenTypeProcessEvent:
        memcpy(&dest->process_event_token, &src->process_event_token, sizeof(RmtTokenProcessEvent));
        break;

    case RmtTokenType::kRmtTokenTypeResourceBind:
        memcpy(&dest->resource_bind_token, &src->resource_bind_token, sizeof(RmtTokenResourceBind));
        break;

    case RmtTokenType::kRmtTokenTypeResourceCreate:
        memcpy(&dest->resource_create_token, &src->resource_create_token, sizeof(RmtTokenResourceCreate));
        break;

    case RmtTokenType::kRmtTokenTypeResourceDestroy:
        memcpy(&dest->resource_destroy_token, &src->resource_destroy_token, sizeof(RmtTokenResourceDestroy));
        break;

    case RmtTokenType::kRmtTokenTypeResourceReference:
        memcpy(&dest->resource_reference, &src->resource_reference, sizeof(RmtTokenResourceReference));
        break;

    case RmtTokenType::kRmtTokenTypeTimeDelta:
        memcpy(&dest->time_delta_token, &src->time_delta_token, sizeof(RmtTokenTimeDelta));
        break;

    case RmtTokenType::kRmtTokenTypeTimestamp:
        memcpy(&dest->timestamp_token, &src->timestamp_token, sizeof(RmtTokenTimestamp));
        break;

    case RmtTokenType::kRmtTokenTypeUserdata:
        dest->userdata_token.resource_identifier          = src->userdata_token.resource_identifier;
        dest->userdata_token.correlation_identifier       = src->userdata_token.correlation_identifier;
        dest->userdata_token.original_resource_identifier = src->userdata_token.original_resource_identifier;
        dest->userdata_token.size_in_bytes                = src->userdata_token.size_in_bytes;
        dest->userdata_token.userdata_type                = src->userdata_token.userdata_type;

        if ((dest->userdata_token.userdata_type == kRmtUserdataTypeName) && (dest->userdata_token.size_in_bytes > 0))
        {
            dest->userdata_token.payload_cache = RmtTokenAllocatePayloadCache(static_cast<size_t>(dest->userdata_token.size_in_bytes));
            memcpy(dest->userdata_token.payload_cache, src->userdata_token.payload_cache, dest->userdata_token.size_in_bytes);
            dest->userdata_token.payload_cache[dest->userdata_token.size_in_bytes - 1] = 0;
        }
        break;

    case RmtTokenType::kRmtTokenTypeVirtualAllocate:
        memcpy(&dest->virtual_allocate_token, &src->virtual_allocate_token, sizeof(RmtTokenVirtualAllocate));
        break;

    case RmtTokenType::kRmtTokenTypeVirtualFree:
        memcpy(&dest->virtual_free_token, &src->virtual_free_token, sizeof(RmtTokenVirtualFree));
        break;

    default:
        break;
    }
}