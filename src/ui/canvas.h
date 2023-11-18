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
        // Making path
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
      *Root        {nullptr},   // Root entity - root is an entity with canvas size
      *FocusEntity  {nullptr},  // Current focused entity
      *ActiveEntity  {nullptr}, // Current active entity
      *HoverEntity  {nullptr};  // Current hovered entity

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

      Root->Resize<true, false>(Size);
      //Redraw();
    } /* End of 'Resize' function */

    VOID Move( const ivec2 &NewPos )
    {
      Pos = NewPos;
      Mask = {Pos, Size};

      Root->Move<true, false>(Pos);
      //Redraw();
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
        HoverEntity->OnMouseMoveEvent(Delta, GlobalMousePos - HoverEntity->GlobalPos);
      if (NewHoverEntity != nullptr)
        NewHoverEntity->OnMouseMoveEvent(Delta, GlobalMousePos - NewHoverEntity->GlobalPos);

      // On hover/unhover event
      if (NewHoverEntity != HoverEntity)
      {
        if (HoverEntity != nullptr)
          HoverEntity->OnUnhoverEvent(GlobalMousePos - HoverEntity->GlobalPos);
        if (NewHoverEntity != nullptr)
          NewHoverEntity->OnHoverEvent(GlobalMousePos - NewHoverEntity->GlobalPos);
        HoverEntity = NewHoverEntity;
      }

      // On drag event
      if (ActiveEntity != nullptr)
        ActiveEntity->OnDragEvent(Delta, GlobalMousePos - ActiveEntity->GlobalPos);
    
    } /* End of 'OnMouseMove' function */

    VOID OnMouseDown( const ivec2 &MousePos )
    {
      if (HoverEntity != nullptr)
        HoverEntity->OnMouseDownEvent(MousePos - HoverEntity->GlobalPos);

      ActiveEntity = HoverEntity;
      if (FocusEntity != ActiveEntity)
      {
        if (FocusEntity != nullptr)
          FocusEntity->OnUnfocusEvent();
      }
      FocusEntity = ActiveEntity;
      if (FocusEntity != nullptr)
        FocusEntity->OnFocusEvent();
    } /* End of 'OnMouseDown' function */
  
    VOID OnMouseUp( const ivec2 &MousePos )
    {
      if (ActiveEntity != nullptr)
      {
        ActiveEntity->OnMouseUpEvent(MousePos - ActiveEntity->GlobalPos);
        if (ActiveEntity == HoverEntity)
          ActiveEntity->OnClickEvent(MousePos - ActiveEntity->GlobalPos);
      }

      ActiveEntity = nullptr;
    } /* End of 'OnMouseUp' function */

    /* On input event function */
    VOID OnInput( UINT Key )
    {
      if (FocusEntity != nullptr)
        FocusEntity->OnInputEvent(Key);
    } /* End of 'OnInput' function */

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

  /* Parse text class */
  class complex_text
  {
  public:

    std::string WholeStr;
    std::string_view WholeStrView;
    std::vector<std::string_view>
      Lines,
      WrappedLines;

    INT
      MaxWordLen = 0,
      MaxLineLen = 0;

    /* Setup as single line */
    VOID Single( const std::string &Str )
    {
      WholeStr = Str;
      WholeStrView = std::string_view(WholeStr);
    } /* End of 'Single' function */

    /* Parse string function */
    VOID Parse( const std::string &InStr )
    {
      WholeStr = InStr;
      WholeStrView = std::string_view(WholeStr);
      Lines.clear();

      // Parse '\n'

      std::string_view::iterator LineStart = WholeStrView.begin();
      
      for (std::string_view::iterator C = LineStart; C != WholeStrView.end(); C++)
      {
        if (*C == '\n')
        {
          // New line
          MaxLineLen = std::max(MaxLineLen, (INT)(C - LineStart));
          Lines.push_back({LineStart, C});
          LineStart = C + 1;
        }
      }
      if (LineStart != WholeStrView.end())
        Lines.push_back({LineStart, WholeStrView.end()});
    } /* End of 'Parse' function */

    VOID Wrap( const INT &Width )
    {
      // Log("Wrap str: " + WholeStr + " To width " + std::to_string(Width) + "\n"); TODO - there are several wraps of one str during init - may we should prevent it???

      INT CharsPerLine = (INT)((Width) / render_2d::FontW);
      WrappedLines.clear();

      for (auto Line : Lines)
        for (std::string_view::iterator C = Line.begin(); C < Line.end();)
        {
          while (C != Line.end() && *C == ' ') // Skip all spaces
            C++;

          std::string_view::iterator NextStrStart, LineStart, PrevWordStart, PrevWordEnd;

          LineStart = PrevWordEnd = PrevWordStart = C;
          NextStrStart = (Line.size() > CharsPerLine && LineStart <= Line.end() - CharsPerLine) ? LineStart + CharsPerLine : Line.end();

          while (C != Line.end() && (C < NextStrStart || PrevWordEnd == PrevWordStart)) // Run over line
          {
            if (*C != ' ')
            {
              if (PrevWordStart < PrevWordEnd) // Out of the word
              {
                MaxWordLen = std::max(MaxWordLen, (INT)(PrevWordEnd - PrevWordStart));
                PrevWordStart = C;
              }
            }
            else
              if (PrevWordStart >= PrevWordEnd) // In the word
                PrevWordEnd = C;
            C++;
          }

          if (C != Line.end())
          {
            if (PrevWordStart > PrevWordEnd) // In the word
            {
              WrappedLines.push_back({LineStart, PrevWordEnd});
              C = PrevWordStart;
            }
            else
              WrappedLines.push_back({LineStart, PrevWordEnd});
          }
          else
            WrappedLines.push_back({LineStart, C});
        }
    } /* End of 'Wrap' function */

    /* Constructor function */
    complex_text( const std::string &InText )
    {
      Parse(InText);
    } /* End if 'complex_text' function */

    /* Default constructor function */
    complex_text( VOID )
    {
    } /* End if 'complex_text' function */

  }; /* End of 'complex_text' class */
} /* end of 'ui' namespace */

#endif // __canvas_h_