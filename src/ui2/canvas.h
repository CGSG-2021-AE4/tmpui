#ifndef __canvas_h_
#define __canvas_h_

#include <cassert>
#include "ui_def.h"
#include "entry.h"
#include "frame_render.h"

namespace ui
{
  /* UI canvas class */
  class canvas
  {
  public:

    render_2d &Render2d;

    // Cavas values
    ivec2 Pos;      // Position
    isize2 Size;    // Size

    mask Mask;      // Mask for canvas

    // Entries' values
    entry
      *Root        {nullptr}, // Root entry
      *FocusEntry  {nullptr}, // Current focused entry
      *HoverEntry  {nullptr}; // Current hovered entry

  public:

    canvas( render_2d &NewRender2d, const ivec2 &NewPos, const isize2 &NewSize, entry *NewRoot ) :
      Render2d(NewRender2d)
    {  
      SetRoot(NewRoot);
      Move(NewPos);
      Resize(NewSize);
      Mask = {Pos, Size};
    } /* End of 'canvas' function */

    VOID SetRoot( entry *NewRoot )
    {
      if (Root != nullptr)
        Root->SetCanvas(nullptr);

      Root = NewRoot; // ?
      if (Root != nullptr)
      {
        Root->SetCanvas(this);
        Root->OnAddChild(nullptr);
      }
    } /* End of 'SetRoot' function */

    VOID Resize( const isize2 &NewSize )
    {
      Size = NewSize;
      Mask = {Pos, Size};

      //if (Root != nullptr)
      //Root->OnResize(Pos, Size);

    } /* End of 'Resize' function */

    VOID Move( const ivec2 &NewPos )
    {
      Pos = NewPos;
      Mask = {Pos, Size};
    } /* End of 'Move' function */

    /* Input events */

    entry * FindHoverEntry( const ivec2 &GlobalMousePos )
    {
      if (Root == nullptr)
        return nullptr;
      if (!Root->IsOver(GlobalMousePos - Root->GlobalPos))
        return nullptr;

      entry *CurEntry = Root;
      ivec2 CurLocalMousePos = GlobalMousePos;

      if (HoverEntry != nullptr)
      {
        CurLocalMousePos = GlobalMousePos - HoverEntry->GlobalPos;
        CurEntry = HoverEntry;

        // Go up
        while (CurEntry != nullptr && !CurEntry->IsOver(CurLocalMousePos))
        {
          CurLocalMousePos += CurEntry->LocalPos;
          CurEntry = CurEntry->Parent;
        }      
      }

      if (CurEntry != nullptr)
      {
        // Go down
        while (1)
        {
          BOOL IsFound = false;

          for (entry *Child : CurEntry->Children)
            if (Child->IsOver(CurLocalMousePos + Child->LocalPos))
            {
              assert(Child != nullptr);
              CurEntry = Child;
              CurLocalMousePos -= Child->LocalPos;
              IsFound = true;
              break;
            }

          if (!IsFound)
            break;
        }
      }

      return CurEntry;
    } /* End of 'FindHoverEntry' function */

    VOID OnMouseMove( const ivec2 &Delta, const ivec2 &GlobalMousePos )
    {
      entry *NewHoverEntry = FindHoverEntry(GlobalMousePos);

      // On mouse move event
      if (HoverEntry != nullptr)
        HoverEntry->OnMouseMove(Delta, GlobalMousePos - HoverEntry->GlobalPos);
      if (NewHoverEntry != nullptr)
        NewHoverEntry->OnMouseMove(Delta, GlobalMousePos - NewHoverEntry->GlobalPos);

      // On hover/unhover event
      if (NewHoverEntry != HoverEntry)
      {
        if (HoverEntry != nullptr)
          HoverEntry->OnUnhover(GlobalMousePos - HoverEntry->GlobalPos);
        if (NewHoverEntry != nullptr)
          NewHoverEntry->OnHover(GlobalMousePos - NewHoverEntry->GlobalPos);
        HoverEntry = NewHoverEntry;
      }

      // On drag event
      if (FocusEntry != nullptr)
        FocusEntry->OnDrag(Delta, GlobalMousePos - FocusEntry->GlobalPos);
    
    } /* End of 'OnMouseMove' function */

    VOID OnMouseDown( const ivec2 &MousePos )
    {
      if (HoverEntry != nullptr)
        HoverEntry->OnMouseDown(MousePos - HoverEntry->GlobalPos);

      FocusEntry = HoverEntry;
    } /* End of 'OnMouseDown' function */
  
    VOID OnMouseUp( const ivec2 &MousePos )
    {
      if (FocusEntry != nullptr)
      {
        FocusEntry->OnMouseUp(MousePos - FocusEntry->GlobalPos);
        if (FocusEntry == HoverEntry)
          FocusEntry->OnClick(MousePos - FocusEntry->GlobalPos);
      }

      FocusEntry = nullptr;
    } /* End of 'OnMouseUp' function */

    VOID Draw( VOID )
    {
      if (Root != nullptr)
        Root->Draw();
    } /* End of 'Draw' function */

  }; /* End of 'canvas' class */
} /* end of 'ui' namespace */

#endif // __canvas_h_