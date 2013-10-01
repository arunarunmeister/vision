
#include <vx_internal.h>

static vx_bool vxIsValidThresholdType(vx_enum type)
{
    vx_bool ret = vx_false_e;
    if ((type == VX_THRESHOLD_TYPE_BINARY) ||
        (type == VX_THRESHOLD_TYPE_RANGE))
    {
        ret = vx_true_e;
    }
    return ret;
}

void vxReleaseThresholdInt(vx_threshold_t *thresh)
{
    vxReleaseReference(&thresh->base, VX_TYPE_THRESHOLD, vx_true_e, NULL);
}

void vxReleaseThreshold(vx_threshold *t)
{
    vx_threshold_t *thresh = (vx_threshold_t *)(t?*t:0);
    vxReleaseReference(&thresh->base, VX_TYPE_THRESHOLD, vx_true_e, NULL);
    if (t) *t = 0;
}

vx_threshold vxCreateThreshold(vx_context c, vx_enum type)
{
    vx_context_t *context = (vx_context_t *)c;
    vx_threshold_t *thresh = NULL;
    if (vxIsValidThresholdType(type) == vx_true_e)
    {
        if (vxIsValidContext(context) == vx_true_e)
        {
            thresh = VX_CALLOC(vx_threshold_t);
            if (thresh)
            {
                vxInitReference(&thresh->base, context, VX_TYPE_THRESHOLD);
                vxIncrementReference(&thresh->base);
                vxAddReference(thresh->base.context, (vx_reference_t *)thresh);
                thresh->type = type;
            }
        }
    }
    return (vx_threshold)thresh;
}

vx_status vxSetThresholdAttribute(vx_threshold t, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_threshold_t *thresh = (vx_threshold_t *)t;
    if (vxIsValidSpecificReference(&thresh->base, VX_TYPE_THRESHOLD) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_THRESHOLD_ATTRIBUTE_VALUE:
                if (VX_CHECK_PARAM(ptr, size, vx_uint8, 0x0) &&
                    (thresh->type == VX_THRESHOLD_TYPE_BINARY))
                {
                    thresh->value = *(vx_uint8 *)ptr;
                    vxWroteToReference(&thresh->base);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_THRESHOLD_ATTRIBUTE_LOWER:
                if (VX_CHECK_PARAM(ptr, size, vx_uint8, 0x0) &&
                    (thresh->type == VX_THRESHOLD_TYPE_RANGE))
                {
                    thresh->lower = *(vx_uint8 *)ptr;
                    vxWroteToReference(&thresh->base);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_THRESHOLD_ATTRIBUTE_UPPER:
                if (VX_CHECK_PARAM(ptr, size, vx_uint8, 0x0) &&
                    (thresh->type == VX_THRESHOLD_TYPE_RANGE))
                {
                    thresh->upper = *(vx_uint8 *)ptr;
                    vxWroteToReference(&thresh->base);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_THRESHOLD_ATTRIBUTE_TYPE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                {
                    vx_enum type = *(vx_enum *)ptr;
                    if (vxIsValidThresholdType(type) == vx_true_e)
                    {
                        thresh->type = type;
                    }
                    else
                    {
                        status = VX_ERROR_INVALID_PARAMETERS;
                    }
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            default:
                status = VX_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    VX_PRINT(VX_ZONE_API, "return %d\n", status);
    return status;
}

vx_status vxQueryThreshold(vx_threshold t, vx_enum attribute, void *ptr, vx_size size)
{
    vx_status status = VX_SUCCESS;
    vx_threshold_t *thresh = (vx_threshold_t *)t;
    if (vxIsValidSpecificReference(&thresh->base, VX_TYPE_THRESHOLD) == vx_true_e)
    {
        switch (attribute)
        {
            case VX_THRESHOLD_ATTRIBUTE_VALUE:
                if (VX_CHECK_PARAM(ptr, size, vx_uint8, 0x0) &&
                    (thresh->type == VX_THRESHOLD_TYPE_BINARY))
                {
                    *(vx_uint8 *)ptr = thresh->value;
                    vxReadFromReference(&thresh->base);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_THRESHOLD_ATTRIBUTE_LOWER:
                if (VX_CHECK_PARAM(ptr, size, vx_uint8, 0x0) &&
                    (thresh->type == VX_THRESHOLD_TYPE_RANGE))
                {
                    *(vx_uint8 *)ptr = thresh->lower;
                    vxReadFromReference(&thresh->base);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_THRESHOLD_ATTRIBUTE_UPPER:
                if (VX_CHECK_PARAM(ptr, size, vx_uint8, 0x0) &&
                    (thresh->type == VX_THRESHOLD_TYPE_RANGE))
                {
                    *(vx_uint8 *)ptr = thresh->upper;
                    vxReadFromReference(&thresh->base);
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            case VX_THRESHOLD_ATTRIBUTE_TYPE:
                if (VX_CHECK_PARAM(ptr, size, vx_enum, 0x3))
                {
                    *(vx_enum *)ptr = thresh->type;
                }
                else
                {
                    status = VX_ERROR_INVALID_PARAMETERS;
                }
                break;
            default:
                status = VX_ERROR_NOT_SUPPORTED;
                break;
        }
    }
    else
    {
        status = VX_ERROR_INVALID_REFERENCE;
    }
    VX_PRINT(VX_ZONE_API, "return %d\n", status);
    return status;
}
