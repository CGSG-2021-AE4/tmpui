#include <cassert>
#include <stack>
#include <chrono>
#include <map>
#include <optional>

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
    /* Draw tree node class */
    class node
    {
      entity *E {nullptr};
      std::map<entity *, node> Children;

    public:

      /* Constructor function */
      node( entity *NewE ) :
        E(NewE)
      {
      } /* End of 'node' function */

      /* Def constructor function */
      node( VOID ) :
        E(nullptr)
      {
      } /* End of 'node' function */

      /* Draw recursive function */
      template<BOOL DrawE = true>
        VOID DrawRec( VOID )
        {
          if (Children.empty())
          {
            if constexpr (DrawE)
            {
              E->Draw();
              Log("Draw entity " + E->Id);
            }
            return;
          }

          for (auto &n : Children)
            n.second.DrawRec();
          Children.clear();
        } /* End of 'DrawRec' function */

      /* Add to tree recuresive function */
      VOID AddRec( std::stack<entity *> &PathStack, BOOL IsNew )
      {
        if (!PathStack.empty())
        {
          if (!IsNew && Children.empty())
            return;

          entity *E = PathStack.top();
          if (Children.find(E) == Children.end())
          {
            Children.emplace(E, node(E));
            PathStack.pop();
            Children[E].AddRec(PathStack, 1); // Like new node
          }
          else
          {
            PathStack.pop();
            Children[E].AddRec(PathStack, 0); // Like not new node
          }
        }
        else
          Children.clear();
      } /* End of 'AddRec' function */

    }; /* End of 'node' class */
      
    /* Draw manager class */
    class draw_manager
    {
    private:

      render_2d &Render2d;
      node RootNode;

    public:

      /* Construct function */
      draw_manager( render_2d &NewR ) :
        Render2d(NewR)
      {
      } /* End of 'draw_manager' function */

      /* Push to entity to draw function */
      VOID PushToDraw( entity *Entity )
      {
        // Making Path
        std::stack<entity *> Path;
        for (entity *e = Entity; e != nullptr; e = e->Parent)
          Path.push(e);

        RootNode.AddRec(Path, 1);
      } /* End of 'PushToDraw' function */

      /* Draw draw tree function */
      VOID Draw( VOID )
      {
        // Log("-- Draw manager start --");
        RootNode.DrawRec<false>();
        // Log("-- Draw manager end --");
      } /* End of 'Draw' function */

    }; /* End of 'draw_manager' class */
  
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
    draw_manager DrawManager;

  public:

    canvas( render_2d &NewRender2d, const ivec2 &NewPos, const isize2 &NewSize, const ::ui::layout_type &RootLayoutType, const std::vector<entity *> &Entities ) :
      Render2d(NewRender2d),
      Pos(NewPos),
      Size(NewSize),
      Mask(Pos, Size),
      Root(new entity(entity_props<VOID>{ .Id = "Canvas root", .Pos = {0, 0}, .Size = Size, .LayoutType = RootLayoutType })), // Provides root isn't nullptr
      DrawManager(Render2d)
    {
      // Root init
      Root->SetCanvas(this);
      Root->OnAddChild(nullptr);
      Root->AddChildren(Entities);
      Redraw();
    } /* End of 'canvas' function */

    /* Destructor function */
    ~canvas( VOID )
    {
      delete Root;
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
      if (e != nullptr)
        DrawManager.PushToDraw(e);
    } /* End of 'PushToDraw' function */

    /* Draw draw stack function */
    VOID Draw( VOID )
    {
      DrawManager.Draw();
    } /* End of 'Draw' function */

    /* Redraw whole root function */
    VOID Redraw( VOID )
    {
      PushToDraw(Root);
    } /* End of 'Redraw' function */

  }; /* End of 'canvas' class */
} /* end of 'ui' namespace */

#endif // __canvas_h_