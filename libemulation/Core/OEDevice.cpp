
/**
 * libemulation
 * OEDevice
 * (C) 2011 by Marc S. Ressl (mressl@umich.edu)
 * Released under the GPL
 *
 * Controls a device
 */

#include "OEDevice.h"

#include "OEEmulation.h"

OEDevice::OEDevice(OEEmulation *emulation)
{
    this->emulation = emulation;
}

bool OEDevice::postMessage(OEComponent *sender, int message, void *data)
{
    switch (message)
    {
        case DEVICE_SET_LABEL:
            if (data)
                label = *((string *)data);
            
            return true;
            
        case DEVICE_GET_LABEL:
            if (data)
                *((string *)data) = label;
            
            return true;
            
        case DEVICE_SET_IMAGEPATH:
            if (data)
                imagePath = *((string *)data);
            
            return true;
            
        case DEVICE_GET_IMAGEPATH:
            if (data)
                *((string *)data) = imagePath;
            
            return true;
            
        case DEVICE_SET_LOCATIONLABEL:
            if (data)
                locationLabel = *((string *)data);
            
            return true;
            
        case DEVICE_GET_LOCATIONLABEL:
            if (data)
                *((string *)data) = locationLabel;
            
            return true;
            
        case DEVICE_SET_STATELABEL:
            if (data)
                stateLabel = *((string *)data);
            
            return true;
            
        case DEVICE_GET_STATELABEL:
            if (data)
                *((string *)data) = stateLabel;
            
            return true;
            
        case DEVICE_SET_SETTINGS:
            if (data)
                settings = *((DeviceSettings *)data);
            
            return true;
            
        case DEVICE_GET_SETTINGS:
            if (data)
                *((DeviceSettings *)data) = settings;
            
            return true;
            
        case DEVICE_CONSTRUCT_DISPLAYCANVAS:
            return constructCanvas(OECANVAS_DISPLAY, (OEComponent **)data);
            
        case DEVICE_CONSTRUCT_PAPERCANVAS:
            return constructCanvas(OECANVAS_PAPER, (OEComponent **)data);
            
        case DEVICE_CONSTRUCT_OPENGLCANVAS:
            return constructCanvas(OECANVAS_OPENGL, (OEComponent **)data);
            
        case DEVICE_DESTROY_CANVAS:
            if (emulation->destroyCanvas)
            {
                OEComponent **ref = (OEComponent **)data;
                
                OEComponents::iterator first = canvases.begin();
                OEComponents::iterator last = canvases.end();
                OEComponents::iterator i = remove(first, last, *ref);
                
                if (i != last)
                    canvases.erase(i, last);
                
                if (emulation->destroyCanvas)
                    emulation->destroyCanvas(emulation->userData, *ref);
                *ref = NULL;
                
                return true;
            }
            
            break;
            
        case DEVICE_GET_CANVASES:
            *((OEComponents *)data) = canvases;
            
            return true;
            
        case DEVICE_ADD_STORAGE:
            storages.push_back((OEComponent *)data);
            
            return true;
            
        case DEVICE_REMOVE_STORAGE:
        {
            OEComponents::iterator first = storages.begin();
            OEComponents::iterator last = storages.end();
            OEComponents::iterator i = remove(first, last, (OEComponent *)data);
            
            if (i != last)
                storages.erase(i, last);
            
            return true;
        }            
        case DEVICE_GET_STORAGES:
            *((OEComponents *)data) = storages;
            
            return true;
            
        case DEVICE_UPDATE:
            if (emulation->didUpdate)
                emulation->didUpdate(emulation->userData);
            
            return true;
            
        case DEVICE_ASSERT_ACTIVITY:
            emulation->activityCount++;
            
            return true;
            
        case DEVICE_CLEAR_ACTIVITY:
            if (emulation->activityCount <= 0)
                return false;
            
            emulation->activityCount--;
            
            return true;
            
        case DEVICE_IS_OBSERVED:
            *((bool *)data) = (observers[DEVICE_DID_CHANGE].size() != 0);
            
            return true;
    }
    
    return false;
}

bool OEDevice::constructCanvas(OECanvasType canvasType, OEComponent **ref)
{
    if (!emulation->constructCanvas)
        return false;
    
    *ref = emulation->constructCanvas(emulation->userData, this, canvasType);
    
    if (!*ref)
        return false;
    
    canvases.push_back(*ref);
    
    return true;
}