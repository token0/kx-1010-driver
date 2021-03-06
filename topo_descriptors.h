/* 1010 PCI/PCIe Audio Driver
   Copyright (c) Eugene Gavrilov, 2002-2016

   * This program is free software; you can redistribute it and/or modify
   * it under the terms of the GNU General Public License as published by
   * the Free Software Foundation; either version 2 of the License, or
   * (at your option) any later version.
   * 
   * This program is distributed in the hope that it will be useful,
   * but WITHOUT ANY WARRANTY; without even the implied warranty of
   * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   * GNU General Public License for more details.
   *
   * You should have received a copy of the GNU General Public License
   * along with this program; if not, write to the Free Software
   * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#pragma once

static KSDATARANGE PinDataRangesBridge[] =
{
    {
            sizeof(KSDATARANGE),
            0,
            0,
            0,
            STATICGUIDOF(KSDATAFORMAT_TYPE_AUDIO),
            STATICGUIDOF(KSDATAFORMAT_SUBTYPE_ANALOG),
            STATICGUIDOF(KSDATAFORMAT_SPECIFIER_NONE)
    }
};

static PKSDATARANGE PinDataRangePointersBridge[] =
{
    (PKSDATARANGE)&PinDataRangesBridge[0]
};

static PCPIN_DESCRIPTOR MiniportPins[] =
{
    // TOPO_WAVEOUT_SOURCE
    {
        0,0,0,  // InstanceCount
        NULL,   // AutomationTable
        {       // KsPinDescriptor
            0,                                          // InterfacesCount
            NULL,                                       // Interfaces
            0,                                          // MediumsCount
            NULL,                                       // Mediums
            SIZEOF_ARRAY(PinDataRangePointersBridge),   // DataRangesCount
            PinDataRangePointersBridge,                 // DataRanges
            KSPIN_DATAFLOW_IN,                          // DataFlow
            KSPIN_COMMUNICATION_NONE,                   // Communication
            &KSCATEGORY_AUDIO,							// Category
            NULL,				 						// Name
            0                                           // Reserved
        }
    },

    // TOPO_WAVEIN_SOURCE
    {
        0,0,0,  // InstanceCount
        NULL,   // AutomationTable
        {       // KsPinDescriptor
            0,                                          // InterfacesCount
            NULL,                                       // Interfaces
            0,                                          // MediumsCount
            NULL,                                       // Mediums
            SIZEOF_ARRAY(PinDataRangePointersBridge),   // DataRangesCount
            PinDataRangePointersBridge,                 // DataRanges
            KSPIN_DATAFLOW_IN,                          // DataFlow
            KSPIN_COMMUNICATION_NONE,                 	// Communication
            &KSNODETYPE_SPDIF_INTERFACE,	// or: &KSNODETYPE_LINE_CONNECTOR,                 // Category
            NULL,				                		// Name
            0                                           // Reserved
        }
    },

    // TOPO_WAVEOUT_DEST = play_control
    {
        0,0,0,  // InstanceCount
        NULL,   // AutomationTable
        {       // KsPinDescriptor
            0,                                          // InterfacesCount
            NULL,                                       // Interfaces
            0,                                          // MediumsCount
            NULL,                                       // Mediums
            SIZEOF_ARRAY(PinDataRangePointersBridge),   // DataRangesCount
            PinDataRangePointersBridge,                 // DataRanges
            KSPIN_DATAFLOW_OUT,                         // DataFlow
            KSPIN_COMMUNICATION_NONE,                 	// Communication
            &KSNODETYPE_SPDIF_INTERFACE,	// or: &KSNODETYPE_LINE_CONNECTOR,					// Category
            NULL,				                  		// Name (this name shows up as
                                                        // the playback panel name in SoundVol)
            0                                           // Reserved
        }
    },
    // TOPO_WAVEIN_DEST = recording_control
    {
        0,0,0,  // InstanceCount
        NULL,   // AutomationTable
        {       // KsPinDescriptor
            0,                                          // InterfacesCount
            NULL,                                       // Interfaces
            0,                                          // MediumsCount
            NULL,                                       // Mediums
            SIZEOF_ARRAY(PinDataRangePointersBridge),   // DataRangesCount
            PinDataRangePointersBridge,                 // DataRanges
            KSPIN_DATAFLOW_OUT,                         // DataFlow
            KSPIN_COMMUNICATION_NONE,                   // Communication
            &KSCATEGORY_AUDIO,							// Category
            NULL,										// Name
            0                                           // Reserved
        }
    }
};

static PCPROPERTY_ITEM NodeProperties[] =
{
    { // filter/pin
        &KSPROPSETID_Audio, 
        KSPROPERTY_AUDIO_VOLUMELEVEL,
        KSPROPERTY_TYPE_GET | KSPROPERTY_TYPE_SET | KSPROPERTY_TYPE_BASICSUPPORT,
        Property::PropertyVolume
    },
    { // pin
        &KSPROPSETID_Audio, 
        KSPROPERTY_AUDIO_MUTE,
        KSPROPERTY_TYPE_GET | KSPROPERTY_TYPE_SET | KSPROPERTY_TYPE_BASICSUPPORT,
        Property::PropertyMute
    },
    { // filter
        &KSPROPSETID_Audio,
        KSPROPERTY_AUDIO_CPU_RESOURCES,
        KSPROPERTY_TYPE_GET | KSPROPERTY_TYPE_BASICSUPPORT,
        Property::PropertyCpuResources
    }
};

static PCPROPERTY_ITEM FilterProperties[] =
{
    { //
        &KSPROPSETID_DRIVER_Private,
        KSPROPERTY_DRIVER_TOPOLOGY,
        KSPROPERTY_TYPE_GET|KSPROPERTY_TYPE_SET,
        Topology::PropertyPrivate
    }
};

/*
static PCEVENT_ITEM NodeEvent[] =
{
    // This is a generic event for nearly every node property.
    {
        &KSEVENTSETID_AudioControlChange,   // Something changed!
        KSEVENT_CONTROL_CHANGE,             // The only event-property defined.
        KSEVENT_TYPE_ENABLE | KSEVENT_TYPE_BASICSUPPORT,
        CMiniportTopology::EventHandler
    }
};
*/
DEFINE_PCAUTOMATION_TABLE_PROP (NodeAutomation,NodeProperties);
DEFINE_PCAUTOMATION_TABLE_PROP (FilterAutomation,FilterProperties);
// DEFINE_PCAUTOMATION_TABLE_PROP_EVENT (AutomationWithEvent, TopologyProperties, NodeEvent);

// TopologyNodes
// List of node identifiers.

enum
{
 TOPO_WAVEOUT_VOLUME=0,
 TOPO_WAVEOUT_MUTE
};

static PCNODE_DESCRIPTOR MiniportNodes[] =
{
    // TOPO_WAVEOUT_VOLUME
    {
        0,                      // Flags
        &NodeAutomation,    	// AutomationTable
        &KSNODETYPE_VOLUME,     // Type
        &KSAUDFNAME_WAVE_VOLUME // Name
    },
    // TOPO_WAVEOUT_MUTE
    {
        0,                      // Flags
        &NodeAutomation,    	// AutomationTable with event support
        &KSNODETYPE_MUTE,     	// Type
        &KSAUDFNAME_WAVE_MUTE 	// Name
    }
};

static PCCONNECTION_DESCRIPTOR MiniportConnections[] =
{   //  FromNode,               FromPin,          		ToNode,                 	ToPin
    {   PCFILTER_NODE,          TOPO_WAVEOUT_SOURCE, 	TOPO_WAVEOUT_VOLUME,        1             		},
    {   TOPO_WAVEOUT_VOLUME,    0,                  	TOPO_WAVEOUT_MUTE,	      	1,		    		},
    {   TOPO_WAVEOUT_MUTE,		0,		     			PCFILTER_NODE,          	TOPO_WAVEOUT_DEST 	},

    {   PCFILTER_NODE,          TOPO_WAVEIN_SOURCE,   	PCFILTER_NODE,          	TOPO_WAVEIN_DEST   	}
};

// MiniportFilterDescription
static PCFILTER_DESCRIPTOR MiniportFilterDescriptor =
{
    0,                                  // Version
    &FilterAutomation,				    // AutomationTable
    sizeof(PCPIN_DESCRIPTOR),           // PinSize
    SIZEOF_ARRAY(MiniportPins),         // PinCount
    MiniportPins,                       // Pins
    sizeof(PCNODE_DESCRIPTOR),          // NodeSize
    SIZEOF_ARRAY(MiniportNodes),        // NodeCount
    MiniportNodes,                      // Nodes
    SIZEOF_ARRAY(MiniportConnections),  // ConnectionCount
    MiniportConnections,                // Connections
    0,                                  // CategoryCount
    NULL                                // Categories
};
