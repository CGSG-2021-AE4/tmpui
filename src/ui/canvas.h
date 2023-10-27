#include <cassert>
#include <stack>

#include "ui_def.h"

#ifndef __canvas_h_
#define __canvas_h_

#include "entity.h"
#include "frame_render.h"

namespace ui
{
  /* UI canvas class */
  class canvas
  {
  public:

    render_2d &Render2d;

  protected:
    friend class entity;

    // Cavas values
    ivec2 Pos;      // Position
    isize2 Size;    // Size

    mask Mask;      // Mask for canvas

    // Entries' values
    entity
      *Root        {nullptr}, // Root entity - root is an entity with canvas size
      *FocusEntity  {nullptr}, // Current focused entity
      *HoverEntity  {nullptr}; // Current hovered entity

    std::stack<entity *> DrawStack;

  public:

    canvas( render_2d &NewRender2d, const ivec2 &NewPos, const isize2 &NewSize, const ::ui::layout_props &RootLayoutProps, const std::vector<entity *> &Entries ) :
      Render2d(NewRender2d),
      Pos(NewPos),
      Size(NewSize),
      Mask(Pos, Size),
      Root(new entity(entity_props { .Id = "Canvas root", .Pos = {0, 0}, .Size = Size, .LayoutProps = RootLayoutProps }, Entries, nullptr)) // Provides root isn't nullptr
    {
      // Root init
      Root->SetCanvas(this);
      Root->OnAddChild(nullptr);
      Redraw();
    } /* End of 'canvas' function */

    /* Destructor function */
    ~canvas( VOID )
    {

    } /* End of '~canvas' function */

    entity * GetRoot( VOID )
    {
      return Root;
    } /* End of 'GetRoot' function */

    VOID Resize( const isize2 &NewSize )
    {
      Size = NewSize;
      Mask = {Pos, Size};

      Root->Resize(Size);
      Redraw();
    } /* End of 'Resize' function */

    VOID Move( const ivec2 &NewPos )
    {
      Pos = NewPos;
      Mask = {Pos, Size};

      Root->Move(Pos);
      Redraw();
    } /* End of 'Move' function */

    /* Input events */

    entity * FindHoverEntity( const ivec2 &GlobalMousePos )
    {
      if (!Root->IsOver(GlobalMousePos))
        return nullptr;

      entity *CurEntity = Root;

      if (HoverEntity != nullptr)
      {
        CurEntity = HoverEntity;

        // Go up
        while (CurEntity != nullptr && !CurEntity->IsOver(GlobalMousePos))
          CurEntity = CurEntity->Parent;
      }

      if (CurEntity != nullptr)
      {
        // Go down
        while (1)
        {
          BOOL IsFound = false;

          for (entity *Child : CurEntity->Children)
            if (Child->IsOver(GlobalMousePos))
            {
              assert(Child != nullptr);
              CurEntity = Child;
              IsFound = true;
              break;
            }

          if (!IsFound)
            break;
        }
      }

      return CurEntity;
    } /* End of 'FindHoverEntity' function */

    VOID OnMouseMove( const ivec3 &Delta, const ivec2 &GlobalMousePos )
    {
      entity *NewHoverEntity = FindHoverEntity(GlobalMousePos);

      // On mouse move event
      if (HoverEntity != nullptr)
        PushToDraw(HoverEntity->OnMouseMoveEvent(Delta, GlobalMousePos - HoverEntity->GlobalPos));
      if (NewHoverEntity != nullptr)
        PushToDraw(NewHoverEntity->OnMouseMoveEvent(Delta, GlobalMousePos - NewHoverEntity->GlobalPos));

      // On hover/unhover event
      if (NewHoverEntity != HoverEntity)
      {
        if (HoverEntity != nullptr)
          PushToDraw(HoverEntity->OnUnhoverEvent(GlobalMousePos - HoverEntity->GlobalPos));
        if (NewHoverEntity != nullptr)
          PushToDraw(NewHoverEntity->OnHoverEvent(GlobalMousePos - NewHoverEntity->GlobalPos));
        HoverEntity = NewHoverEntity;
      }

      // On drag event
      if (FocusEntity != nullptr)
        PushToDraw(FocusEntity->OnDragEvent(Delta, GlobalMousePos - FocusEntity->GlobalPos));
    
    } /* End of 'OnMouseMove' function */

    VOID OnMouseDown( const ivec2 &MousePos )
    {
      if (HoverEntity != nullptr)
        PushToDraw(HoverEntity->OnMouseDownEvent(MousePos - HoverEntity->GlobalPos));

      FocusEntity = HoverEntity;
    } /* End of 'OnMouseDown' function */
  
    VOID OnMouseUp( const ivec2 &MousePos )
    {
      if (FocusEntity != nullptr)
      {
        PushToDraw(FocusEntity->OnMouseUpEvent(MousePos - FocusEntity->GlobalPos));
        if (FocusEntity == HoverEntity)
          PushToDraw(FocusEntity->OnClickEvent(MousePos - FocusEntity->GlobalPos));
      }

      FocusEntity = nullptr;
    } /* End of 'OnMouseUp' function */

    /* Draw stack functions */

    /* Push to draw stack function */
    VOID PushToDraw( entity *e )
    {
      if (e != nullptr) // Then here will be more checks
        DrawStack.push(e);
    } /* End of 'PushToDraw' function */

    /* Draw draw stack function */
    VOID Draw( VOID )
    {
      while (!DrawStack.empty())
      {
        DrawStack.top()->Draw();
        DrawStack.pop();
      }
    } /* End of 'Draw' function */

    /* Redraw whole root function */
    VOID Redraw( VOID )
    {
      PushToDraw(Root);
    } /* End of 'Redraw' function */

  }; /* End of 'canvas' class */
} /* end of 'ui' namespace */

#endif // __canvas_h_