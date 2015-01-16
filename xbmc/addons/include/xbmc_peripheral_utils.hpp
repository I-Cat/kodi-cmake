/*
 *      Copyright (C) 2014 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "xbmc_peripheral_types.h"

#include <cmath>
#include <cstring>
#include <string>
#include <vector>

#define DIGITAL_ANALOG_THRESHOLD  0.5f // TODO

#ifndef SAFE_DELETE
  #define SAFE_DELETE(x)  do { delete (x); (x) = NULL; } while (0)
#endif

#ifndef SAFE_DELETE_ARRAY
  #define SAFE_DELETE_ARRAY(x)  do { delete[] (x); (x) = NULL; } while (0)
#endif

#ifndef CONSTRAIN
  #define CONSTRAIN(min, value, max)  ((value) < (min) ? (min) : (max) < (value) ? (max) : (value))
#endif

namespace ADDON
{
  /*!
   * Utility class to manipulate arrays of peripheral types.
   */
  template <class THE_CLASS, typename THE_STRUCT>
  class PeripheralVector
  {
  public:
    static void ToStructs(const std::vector<THE_CLASS>& vecObjects, THE_STRUCT** pStructs)
    {
      if (!pStructs)
        return;

      if (vecObjects.empty())
      {
        *pStructs = NULL;
      }
      else
      {
        (*pStructs) = new THE_STRUCT[vecObjects.size()];
        for (unsigned int i = 0; i < vecObjects.size(); i++)
          vecObjects.at(i).ToStruct((*pStructs)[i]);
      }
    }

    static void ToStructs(const std::vector<THE_CLASS*>& vecObjects, THE_STRUCT** pStructs)
    {
      if (!pStructs)
        return;

      if (vecObjects.empty())
      {
        *pStructs = NULL;
      }
      else
      {
        *pStructs = new THE_STRUCT[vecObjects.size()];
        for (unsigned int i = 0; i < vecObjects.size(); i++)
          vecObjects.at(i)->ToStruct((*pStructs)[i]);
      }
    }

    static void FreeStructs(unsigned int structCount, THE_STRUCT* structs)
    {
      if (structs)
      {
        for (unsigned int i = 0; i < structCount; i++)
          THE_CLASS::FreeStruct(structs[i]);
      }
      SAFE_DELETE_ARRAY(structs);
    }
  };

  /*!
   * ADDON::Peripheral
   *
   * Wrapper class providing peripheral information. Classes can extend
   * Peripheral to inherit peripheral properties.
   */
  class Peripheral
  {
  public:
    Peripheral(PERIPHERAL_TYPE type = PERIPHERAL_TYPE_UNKNOWN, const std::string& strName = "")
    : m_type(type),
      m_strName(strName),
      m_index(0),
      m_vendorId(0),
      m_productId(0)
    {
    }

    Peripheral(PERIPHERAL_INFO& info)
    : m_type(info.type),
      m_strName(info.name ? info.name : ""),
      m_index(info.index),
      m_vendorId(info.vendor_id),
      m_productId(info.product_id)
    {
    }

    virtual ~Peripheral(void) { }

    PERIPHERAL_TYPE    Type(void) const      { return m_type; }
    const std::string& Name(void) const      { return m_strName; }
    unsigned int       Index(void) const     { return m_index; }
    unsigned int       VendorID(void) const  { return m_vendorId; }
    unsigned int       ProductID(void) const { return m_productId; }

    void SetType(PERIPHERAL_TYPE type)        { m_type      = type; }
    void SetName(const std::string& strName)  { m_strName   = strName; }
    void SetIndex(unsigned int index)         { m_index     = index; }
    void SetVendorID(unsigned int vendorId)   { m_vendorId  = vendorId; }
    void SetProductID(unsigned int productId) { m_productId = productId; }

    void ToStruct(PERIPHERAL_INFO& info) const
    {
      info.type       = m_type;
      info.name       = new char[m_strName.size() + 1];
      info.index      = m_index;
      info.vendor_id  = m_vendorId;
      info.product_id = m_productId;

      std::strcpy(info.name, m_strName.c_str());
    }

    static void FreeStruct(PERIPHERAL_INFO& info)
    {
      SAFE_DELETE_ARRAY(info.name);
    }

  private:
    PERIPHERAL_TYPE             m_type;
    std::string                 m_strName;
    unsigned int                m_index;
    unsigned int                m_vendorId;
    unsigned int                m_productId;
  };

  typedef PeripheralVector<Peripheral, PERIPHERAL_INFO> Peripherals;

  /*!
   * ADDON::JoystickButton
   *
   * Wrapper class providing button information. Classes can extend
   * JoystickButton to inherit button properties.
   */
  class JoystickButton
  {
  public:
    JoystickButton(JOYSTICK_ID          id       = JOYSTICK_ID(),
                   JOYSTICK_BUTTON_TYPE type     = JOYSTICK_BUTTON_TYPE(),
                   const std::string&   strLabel = "")
    : m_id(id),
      m_type(type),
      m_strLabel(strLabel)
    {
    }

    JoystickButton(const JOYSTICK_BUTTON& button)
    : m_id(button.id),
      m_type(button.type),
      m_strLabel(button.label ? button.label : "")
    {
    }

    JOYSTICK_ID          ID(void) const    { return m_id; }
    JOYSTICK_BUTTON_TYPE Type(void) const  { return m_type; }
    const std::string&   Label(void) const { return m_strLabel; }

    void SetID(JOYSTICK_ID id)                 { m_id = id; }
    void SetType(JOYSTICK_BUTTON_TYPE type)    { m_type = type; }
    void SetLabel(const std::string& strLabel) { m_strLabel = strLabel; }

    void ToStruct(JOYSTICK_BUTTON& button) const
    {
      button.id    = m_id;
      button.type  = m_type;
      button.label = new char[m_strLabel.size() + 1];

      std::strcpy(button.label, m_strLabel.c_str());
    }

    static void FreeStruct(JOYSTICK_BUTTON& button)
    {
      SAFE_DELETE_ARRAY(button.label);
    }

  private:
    JOYSTICK_ID          m_id;
    JOYSTICK_BUTTON_TYPE m_type;
    std::string          m_strLabel;
  };

  typedef PeripheralVector<JoystickButton, JOYSTICK_BUTTON> JoystickButtons;

  /*!
   * ADDON::Joystick
   *
   * Wrapper class providing additional joystick information not provided by
   * Peripheral. Classes can extend Joystick to inherit peripheral and joystick
   * properties.
   */
  class Joystick : public Peripheral
  {
  public:
    Joystick(const std::string& strProvider = "", const std::string& strName = "")
    : Peripheral(PERIPHERAL_TYPE_JOYSTICK, strName),
      m_strProvider(strProvider),
      m_requestedPlayer(0),
      m_buttonCount(0),
      m_hatCount(0),
      m_axisCount(0)
    {
    }

    Joystick(JOYSTICK_INFO& info)
    : Peripheral(info.peripheral_info),
      m_strProvider(info.provider ? info.provider : ""),
      m_requestedPlayer(info.requested_player_num),
      m_buttonCount(info.virtual_layout.button_count),
      m_hatCount(info.virtual_layout.hat_count),
      m_axisCount(info.virtual_layout.axis_count)
    {
      SetType(PERIPHERAL_TYPE_JOYSTICK);

      if (info.physical_layout.buttons)
      {
        for (unsigned int i = 0; i < info.physical_layout.button_count; i++)
          m_buttons.push_back(info.physical_layout.buttons[i]);
      }
    }

    virtual ~Joystick(void) { }

    const std::string& Provider(void) const        { return m_strProvider; }
    unsigned int       RequestedPlayer(void) const { return m_requestedPlayer; }
    unsigned int       ButtonCount(void) const     { return m_buttonCount; }
    unsigned int       HatCount(void) const        { return m_hatCount; }
    unsigned int       AxisCount(void) const       { return m_axisCount; }

    const std::vector<JoystickButton>& Buttons(void) const { return m_buttons; }

    void SetProvider(const std::string& strProvider)      { m_strProvider     = strProvider; }
    void SetRequestedPlayer(unsigned int requestedPlayer) { m_requestedPlayer = requestedPlayer; }
    void SetButtonCount(unsigned int buttonCount)         { m_buttonCount     = buttonCount; }
    void SetHatCount(unsigned int hatCount)               { m_hatCount        = hatCount; }
    void SetAxisCount(unsigned int axisCount)             { m_axisCount       = axisCount; }

    std::vector<JoystickButton>& Buttons(void) { return m_buttons; }

    void ToStruct(JOYSTICK_INFO& info) const
    {
      Peripheral::ToStruct(info.peripheral_info);

      info.provider = new char[m_strProvider.size() + 1];
      info.requested_player_num         = m_requestedPlayer;
      info.virtual_layout.button_count  = m_buttonCount;
      info.virtual_layout.hat_count     = m_hatCount;
      info.virtual_layout.axis_count    = m_axisCount;
      info.physical_layout.button_count = m_buttons.size();

      std::strcpy(info.provider, m_strProvider.c_str());

      JoystickButtons::ToStructs(m_buttons, &info.physical_layout.buttons);
    }

    static void FreeStruct(JOYSTICK_INFO& info)
    {
      Peripheral::FreeStruct(info.peripheral_info);

      SAFE_DELETE_ARRAY(info.provider);

      JoystickButtons::FreeStructs(info.physical_layout.button_count, info.physical_layout.buttons);
    }

  private:
    std::string                 m_strProvider;
    unsigned int                m_requestedPlayer;
    unsigned int                m_buttonCount;
    unsigned int                m_hatCount;
    unsigned int                m_axisCount;
    std::vector<JoystickButton> m_buttons;
  };

  typedef PeripheralVector<Joystick, JOYSTICK_INFO> Joysticks;

  class PeripheralEvent
  {
  public:
    PeripheralEvent(void)
    : m_type(),
      m_peripheralIndex(0),
      m_rawIndex(0),
      m_buttonState(),
      m_hatState(),
      m_axisState()
    {
    }

    PeripheralEvent(unsigned int peripheralIndex, unsigned int buttonIndex, JOYSTICK_STATE_BUTTON state)
    : m_type(JOYSTICK_EVENT_TYPE_RAW_BUTTON),
      m_peripheralIndex(peripheralIndex),
      m_rawIndex(buttonIndex),
      m_buttonState(state),
      m_hatState(),
      m_axisState()
    {
    }

    PeripheralEvent(unsigned int peripheralIndex, unsigned int hatIndex, JOYSTICK_STATE_HAT state)
    : m_type(JOYSTICK_EVENT_TYPE_RAW_HAT),
      m_peripheralIndex(peripheralIndex),
      m_rawIndex(hatIndex),
      m_buttonState(),
      m_hatState(state),
      m_axisState()
    {
    }

    PeripheralEvent(unsigned int peripheralIndex, unsigned int axisIndex, JOYSTICK_STATE_AXIS state)
    : m_type(JOYSTICK_EVENT_TYPE_RAW_AXIS),
      m_peripheralIndex(peripheralIndex),
      m_rawIndex(axisIndex),
      m_buttonState(),
      m_hatState(),
      m_axisState(state)
    {
    }

    PeripheralEvent(const PERIPHERAL_EVENT& event)
    : m_type(event.type),
      m_peripheralIndex(event.peripheral_index),
      m_rawIndex(event.raw_index),
      m_buttonState(),
      m_hatState(),
      m_axisState()
    {
      switch (m_type)
      {
        case JOYSTICK_EVENT_TYPE_RAW_BUTTON: m_buttonState = event.button_state; break;
        case JOYSTICK_EVENT_TYPE_RAW_HAT:    m_hatState    = event.hat_state;    break;
        case JOYSTICK_EVENT_TYPE_RAW_AXIS:   m_axisState   = event.axis_state;   break;
        case JOYSTICK_EVENT_TYPE_NONE:
        default: break;
      }
    }

    JOYSTICK_EVENT_TYPE   Type(void) const            { return m_type; }
    unsigned int          PeripheralIndex(void) const { return m_peripheralIndex; }
    unsigned int          RawIndex(void) const        { return m_rawIndex; }
    JOYSTICK_STATE_BUTTON ButtonState(void) const     { return m_buttonState; }
    JOYSTICK_STATE_HAT    HatState(void) const        { return m_hatState; }
    JOYSTICK_STATE_AXIS   AxisState(void) const       { return m_axisState; }

    void SetType(JOYSTICK_EVENT_TYPE type)           { m_type            = type; }
    void SetPeripheralIndex(unsigned int index)      { m_peripheralIndex = index; }
    void SetRawIndex(unsigned int index)             { m_rawIndex        = index; }
    void SetButtonState(JOYSTICK_STATE_BUTTON state) { m_buttonState     = state; }
    void SetHatState(JOYSTICK_STATE_HAT state)       { m_hatState        = state; }
    void SetAxisState(JOYSTICK_STATE_AXIS state)     { m_axisState       = state; }

    void ToStruct(PERIPHERAL_EVENT& event) const
    {
      event.type             = m_type;
      event.peripheral_index = m_peripheralIndex;
      event.raw_index        = m_rawIndex;
      switch (m_type)
      {
        case JOYSTICK_EVENT_TYPE_RAW_BUTTON: event.button_state = m_buttonState; break;
        case JOYSTICK_EVENT_TYPE_RAW_HAT:    event.hat_state    = m_hatState;    break;
        case JOYSTICK_EVENT_TYPE_RAW_AXIS:   event.axis_state   = m_axisState;   break;
        case JOYSTICK_EVENT_TYPE_NONE:
        default: break;
      }
    }

    static void FreeStruct(PERIPHERAL_EVENT& event)
    {
      (void)event;
    }

  private:
    JOYSTICK_EVENT_TYPE   m_type;
    unsigned int          m_peripheralIndex;
    unsigned int          m_rawIndex;
    JOYSTICK_STATE_BUTTON m_buttonState;
    JOYSTICK_STATE_HAT    m_hatState;
    JOYSTICK_STATE_AXIS   m_axisState;
  };

  typedef PeripheralVector<PeripheralEvent, PERIPHERAL_EVENT> PeripheralEvents;
}
