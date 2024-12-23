//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ
// File        : propertyeditor.h
// Description : PropertyEditor interface
// Notes       :
//
//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef PROPERTYEDITOR_H_INCLUDED
#define PROPERTYEDITOR_H_INCLUDED

#include <editor/leditor.h>

namespace lEditor {

//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ
// PropertyEditor - Manage editing a single class of an object
//ħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħħ

class PropertyEditor : public lHasSlots<>
{
  class PropertyRollout;
  Editor              * m_editor;
  lObject             * m_object;
  lClass              * m_objectClass;
  PropertyRollout     * m_rollout;
  lArray<UITool *>      m_additionalRolloutList;

public:

  lSignal0<> SignalEditChange;

  PropertyEditor( Editor * editor, lObject * object, lClass * objectClass );
  ~PropertyEditor();

  void Init();
  void Deinit();
  void Refresh();

private:
  void OnEditChange();
};

}
#endif

