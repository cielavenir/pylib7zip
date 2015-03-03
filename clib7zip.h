#ifndef CLIB7ZIP_H
#define CLIB7ZIP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "7zip/PropID.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef HRESULT (*_GetNumberOfFormats)(uint32_t*);
typedef HRESULT (*_GetNumberOfMethods)(uint32_t *);
typedef HRESULT (*_GetMethodProperty)(uint32_t index, uint32_t propID, PROPVARIANT * value);
typedef HRESULT (*_GetHandlerProperty2)(uint32_t, uint32_t propID, PROPVARIANT *);
typedef HRESULT (*_CreateObject)(const GUID *, const GUID *, void **);

//PROPVARIANT
PROPVARIANT* create_propvariant();
void destroy_propvariant(PROPVARIANT*);

//-

typedef struct IInArchive IInArchive;
typedef struct IInStream IInStream;

//IInStream
typedef HRESULT (*_stream_read_callback)
    (void* self, void *data, uint32_t size, uint32_t *processedSize);
typedef HRESULT (*_stream_seek_callback)
    (void* self, uint64_t offset, int32_t seekOrigin, uint64_t *newPosition);
typedef HRESULT (*_stream_get_size_callback)(void* self, uint64_t *size);

IInStream* create_instream_from_file(FILE* file);
/*
IInStream* create_instream_from_callbacks(void* data,
    _stream_read_callback read_cb,
    _stream_seek_callback seek_cb,
    _stream_get_size_callback get_size_cb);
*/

//IArchiveOpenCallback
typedef HRESULT (*_set_total_callback)
    (void* self, const uint64_t * files, const uint64_t * bytes);
typedef HRESULT (*_set_completed_callback)
    (void* self, const uint64_t * files, const uint64_t * bytes);
typedef HRESULT (*_get_password_callback)(void* self, wchar_t** password);

//IInArchive
IInArchive* create_archive(const wchar_t* type);
void archive_release(IInArchive* archive);
HRESULT archive_open(
    IInArchive* archive,
    IInStream* in_stream,
    void* data /* optional - passed to callbacks as first arg */,
    wchar_t* password, /* optional */
    _get_password_callback get_password_callback, /* optional - takes precedence over password */
    _set_total_callback set_total_callback, /* optional */
    _set_completed_callback set_completed_callback /* optional */);

HRESULT archive_get_num_items(IInArchive* archive, uint32_t* num_items);
HRESULT archive_get_item_property_pvar(
    IInArchive* archive, uint32_t index, uint32_t prop, PROPVARIANT* pvar);
HRESULT archive_get_item_property_str(
    IInArchive* archive, uint32_t index, uint32_t prop, wchar_t buf[], size_t buf_size);
HRESULT archive_get_item_property_uint64(
    IInArchive* archive, uint32_t index, uint32_t prop_id, uint64_t* out);

HRESULT archive_close(IInArchive*);

#ifdef __cplusplus
}
#endif
#endif
