/*
 * Copyright (c) 2012-2013 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

#include <vx_internal.h>

void vxInitLog(vx_log_t *log)
{
    log->first = -1;
    log->last = 0;
    log->count = VX_MAX_LOG_NUM_ENTRIES;
    log->enabled = vx_true_e;
    vxCreateSem(&log->lock, 1);
}

void vxAddLogEntry(vx_reference r, vx_status status, const char *message, ...)
{
    vx_reference_t *ref = (vx_reference_t *)r;
    vx_context_t *con = (vx_context_t *)r;
    vx_log_t *log = NULL;
    va_list ap;
    int c = 0;

    if (vxIsValidReference(ref) == vx_true_e)
        log = &ref->context->log;
    else if (vxIsValidContext(con) == vx_true_e)
        log = &con->log;
    else
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid reference!\n");
        return;
    }

    if (status == VX_SUCCESS)
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid status code!\n");
        return;
    }
    if (message == NULL)
    {
        VX_PRINT(VX_ZONE_ERROR, "Invalid message!\n");
        return;
    }
    if (log->enabled == vx_false_e)
        return;

    vxSemWait(&log->lock);
    vxPrintLog(log, VX_ZONE_LOG);
    va_start(ap, message);
    log->entries[log->last].reference = r;
    log->entries[log->last].status = status;
    log->entries[log->last].active = vx_true_e;
    c = vsnprintf(log->entries[log->last].message, VX_MAX_LOG_MESSAGE_LEN, message, ap);
    va_end(ap);
    VX_PRINT(VX_ZONE_LOG, "c=%d ref="VX_FMT_REF" status=%d message=%s\n", c, r, status, log->entries[log->last].message);
    if (log->first == -1)
        log->first = log->last;
    else if (log->first == log->last)
        log->first = (log->first + 1)%log->count;
    log->last = (log->last + 1)%log->count;
    vxPrintLog(log, VX_ZONE_LOG);
    vxSemPost(&log->lock);
}

void vxResetLog(vx_log_t *log)
{
    vxSemWait(&log->lock);
    log->first = -1;
    vxSemPost(&log->lock);
}

vx_status vxGetLogEntry(vx_reference r, char message[VX_MAX_LOG_MESSAGE_LEN])
{
    vx_reference_t *ref = (vx_reference_t *)r;
    vx_context_t *con = (vx_context_t *)r;
    vx_status status = VX_SUCCESS;
    vx_log_t *log = NULL;
    vx_int32  cur = 0;

    // if it's not a reference or a context, return.
    if (vxIsValidReference(ref) == vx_true_e)
        log = &ref->context->log;
    else if (vxIsValidContext(con) == vx_true_e)
        log = &con->log;
    else
        return VX_ERROR_INVALID_REFERENCE;

    vxSemWait(&log->lock);

    // if there's nothing in the log return success
    if (log->first == -1)
    {
        vxSemPost(&log->lock);
        return VX_SUCCESS;
    }
    // first, match the reference to the parameter r
    // if active mark not active and copy and return.
    // if not active move on.

    for (cur = log->first; cur != log->last; cur = (cur + 1)%log->count)
    {
        vxPrintLogEntry(log, cur, VX_ZONE_LOG);
        // if reference match or context was given
        if ((r == log->entries[cur].reference || ref->context == NULL) &&
            log->entries[cur].active == vx_true_e)
        {
            status = log->entries[cur].status;
            strncpy(message, log->entries[cur].message, VX_MAX_LOG_MESSAGE_LEN);
            log->entries[cur].active = vx_false_e;
            if (cur == log->first)
            {
                VX_PRINT(VX_ZONE_LOG, "Aged out first entry!\n");
                log->first = (log->first + 1)%log->count;
                if (log->first == log->last)
                {
                    log->first = -1;
                    VX_PRINT(VX_ZONE_LOG, "Log is now empty!\n");
                }
            }
            vxPrintLog(log, VX_ZONE_LOG);
            break;
        }
    }
    vxSemPost(&log->lock);
    return status;
}

void vxPrintLog(vx_log_t *log, vx_uint32 zone)
{
    VX_PRINT(zone, "log:%p first=%d last=%d count=%d\n", log, log->first, log->last, log->count);
}


void vxPrintLogEntry(vx_log_t *log, vx_int32 i, vx_uint32 zone)
{
    VX_PRINT(zone, "log[%d] = {ref="VX_FMT_REF" (%p) active?=%d status=%d message=%s}\n",
            i, log->entries[i].reference, log->entries[i].reference,
               log->entries[i].active,
               log->entries[i].status, log->entries[i].message);
}

void vxPrintAllLogEntries(vx_log_t *log, vx_uint32 zone)
{
    vx_int32  cur = 0;
    vxSemWait(&log->lock);
    vxPrintLog(log, zone);
    for (cur = log->first; cur != log->last; cur = (cur + 1)%log->count)
    {
        vxPrintLogEntry(log, cur, zone);
    }
    vxSemPost(&log->lock);
}
