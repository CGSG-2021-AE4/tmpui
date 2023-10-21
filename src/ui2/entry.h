#include "ui_def.h"

#ifndef __entry_h_
#define __entry_h_

//#include "frame_render.h"

// Some real kal
using namespace tmp;

namespace ui
{
  /* Friend classes prototypes */
  class canvas;

  enum struct entry_state
  {
    def = 0,
    hovered = 1,
    focused = 2,
    active = 3,
  }; /* End of 'entry_state' enum struct */
  
  /* UI entry class */
  class entry
  {
    /* Values */
  protected:

    ivec2
      LocalPos,                           // Position in parent basis
      GlobalPos;                          // Position in canvas basis
    isize2 Size;                          // WH

    mask
      SelfMask,                           // Self mask
      ContentMask,                        // Content mask
      SelfDrawMask,                       // Mask that is visible on the screen
      ContentDrawMask;                    // Mask for content that is visible on the screen
    
    entry *Parent {nullptr};              // Parent pointer
    canvas *Canvas {nullptr};             // Canvas pointer
    std::vector<entry *> Children;        // Children pointers

    BOOL IsVisible;                       // Is entry visible

    entry_state State {entry_state::def}; // State of entry

    /* Events */
  public:

    VOID SetVisibility( BOOL NewValue )
    {
      IsVisible = NewValue;
    } /* End of 'SetVisibility' function */

    BOOL IsOver( const ivec2 &LocalPoint )
    {
      return
        LocalPoint.X > 0 &&
        LocalPoint.Y > 0 &&
        LocalPoint.X < Size.W &&
        LocalPoint.Y < Size.H;
    } /* End of 'IsOver' function */

    virtual BOOL OnDraw( VOID )
    {
      return true;
    } /* End of 'OnDraw' function */

    virtual VOID OnHover( const ivec2 &LocalMousePos )
    {
    } /* End of 'OnHover' function */

    virtual VOID OnUnhover( const ivec2 &LocalMousePos )
    {
    } /* End of 'OnUnhover' function */

    virtual VOID OnClick( const ivec2 &LocalMousePos )
    {
    } /* End of 'OnClick' function */

    virtual VOID OnMouseDown( const ivec2 &LocalMousePos )
    {
    } /* End of 'OnMouseDown' function */

    virtual VOID OnMouseUp( const ivec2 &LocalMousePos )
    {
    } /* End of 'OnMouseUp' function */

    virtual VOID OnMouseMove( const ivec2 &Delta, const ivec2 &LocalMousePos )
    {
    } /* End of 'OnMouseMove' function */

    virtual VOID OnDrag( const ivec2 &Delta, const ivec2 &LocalMousePos )
    {
    } /* End of 'OnDrag' function */

    virtual VOID OnChange( VOID )
    {
    } /* End of 'OnChange' function */

    virtual VOID OnResize( VOID )
    {
    } /* End of 'OnResize' function */

    virtual VOID OnMove( VOID )
    {
    } /* End of 'OnResize' function */

    inline VOID OnDrawEvent( VOID )
    {
      OnDraw();
    } /* End of 'OnDrawEvent' function */

    inline VOID OnHoverEvent( const ivec2 &LocalMousePos )
    {
      State = entry_state::hovered;
      OnHover(LocalMousePos);
    } /* End of 'OnHoverEvent' function */

    inline VOID OnUnhoverEvent( const ivec2 &LocalMousePos )
    {
      State = entry_state::def;
      OnUnhover(LocalMousePos);
    } /* End of 'OnUnhoverEvent' function */

    inline VOID OnClickEvent( const ivec2 &LocalMousePos )
    {
      OnClick(LocalMousePos);
    } /* End of 'OnClickEvent' function */

    inline VOID OnMouseDownEvent( const ivec2 &LocalMousePos )
    {
      State = entry_state::active;
      OnMouseDown(LocalMousePos);
    } /* End of 'OnMouseDownEvent' function */

    inline VOID OnMouseUpEvent( const ivec2 &LocalMousePos )
    {
      State = entry_state::hovered;
      OnMouseUp(LocalMousePos);
    } /* End of 'OnMouseUpEvent' function */

    inline VOID OnMouseMoveEvent( const ivec2 &Delta, const ivec2 &LocalMousePos )
    {
      OnMouseMove(Delta, LocalMousePos);
    } /* End of 'OnMouseMoveEvent' function */

    inline VOID OnDragEvent( const ivec2 &Delta, const ivec2 &LocalMousePos )
    {
      OnDrag(Delta, LocalMousePos);
    } /* End of 'OnDragEvent' function */

    inline VOID OnChangeEvent( VOID )
    {
      OnChange();
    } /* End of 'OnChangeEvent' function */

    inline VOID OnResizeEvent( VOID )
    {
      OnResize();
    } /* End of 'OnResizeEvent' function */

    inline VOID OnMoveEvent( VOID )
    {
      OnMove();
    } /* End of 'OnResizeEvent' function */

    virtual mask GetSelfMask( VOID )
    {
      return {GlobalPos, Size};
    } /* End of 'GetSelfMask' function */

    virtual mask GetContentMask( VOID )
    {
      return {GlobalPos, Size};
    } /* End of 'GetContentMask' function */

    /* Functions */
  protected:

    friend class canvas;

    entry( const ivec2 &NewPos, const isize2 &NewSize, const std::vector<entry *> &NewChildren = {}, entry *NewParent = nullptr ) :
      Parent(nullptr),
      LocalPos(NewPos),
      Size(NewSize),
      IsVisible(true)
    {
      SetParent(NewParent);

      UpdateGlobalPos();
      UpdateMasks();

      AddChildren(NewChildren);
    } /* End of 'entry' function */

    ~entry( VOID )
    {
      // Rebind children to parent
      for (entry *c : Children)
        c->SetParent(Parent);
      // Delete self from parent
      if (Parent != nullptr)
      Parent->DeleteChild(this);
    } /* End of '~entry' function */

    VOID UpdateMasks( VOID )
    {
      SelfMask = GetSelfMask();
      ContentMask = GetContentMask();
      if (Parent != nullptr)
      {
        SelfDrawMask = SelfMask.Intersect(Parent->ContentDrawMask); // May be later add check for mask change
        ContentDrawMask = ContentMask.Intersect(SelfDrawMask);
      }
      else
      {
        SelfDrawMask = SelfMask;
        ContentDrawMask = ContentMask.Intersect(SelfDrawMask);
      }
    } /* End of 'UpdateDrawMask' function */

    /* Update entry global pos function. */
    VOID UpdateGlobalPos( VOID );

    /* Only resize function */
    VOID Resize( const isize2 &NewSize )
    {
      Size = NewSize;

      UpdateMasks();

      OnResize();
    } /* End of 'Resize' function */

    /* Only move function */
    VOID Move( const ivec2 &NewLocalPos )
    {
      LocalPos = NewLocalPos;

      UpdateGlobalPos();

      OnMove();
    } /* End of 'Resize' function */

    /* Combined move and resize function */
    VOID Reform( const ivec2 &NewLocalPos, const isize2 &NewSize )
    {
      LocalPos = NewLocalPos;
      Size = NewSize;

      UpdateGlobalPos();

      OnMove();
      OnResize();
    } /* End of 'Reform' function */

    /* Draw functions */

    VOID DrawChildren( VOID )
    {
      for (entry *c : Children)
        c->Draw();
    } /* End of 'DrawChildren' function */

    VOID Draw( VOID )
    {
      if (!IsVisible)
        return;

      if (OnDraw())
        DrawChildren();
    } /* End of 'draw' function */

    /************ Parents/Children functions ************/

    VOID SetParent( entry *NewParent )
    {
      // Rebind to parent
      if (Parent != nullptr)
        Parent->DeleteChild(this); // Delete self from parent's children

      Parent = NewParent;

      if (Parent != nullptr)
        Parent->AddChild(this); // Add self to new parent's children
      
    } /* End of 'SetParent' function */

    VOID OnAddChild( entry *NewParent )
    {
      // Rebind to parent
      if (Parent != nullptr)
        Parent->DeleteChild(this); // Delete self from parent's children

      Parent = NewParent;

      if (Parent != nullptr)
        SetCanvas(Parent->Canvas);

      UpdateGlobalPos();
    } /* End of 'OnAddChild' function */

    VOID SetCanvas( canvas *NewCanvas )
    {
      Canvas = NewCanvas;
      for (entry *Child : Children)
        Child->SetCanvas(Canvas);
    } /* End of 'SetParent' function */

    // Later there must be funtions to instert children into specific positions

    VOID AddChild( entry *NewChild )
    {
      if (NewChild == nullptr)
        return;

      for (entry *Child : Children)
        if (NewChild == Child)
          return; // We already have this child
    
      Children.push_back(NewChild);
      NewChild->OnAddChild(this);
    } /* End of 'AddChild' function */

    VOID AddChildren( const std::vector<entry *> &NewChildren )
    {
      for (entry *Child : NewChildren)
        AddChild(Child);
    } /* End of 'AddChildren' function */

    std::vector<entry *>::iterator FindChild( const entry *Child )
    {
      for (std::vector<entry *>::iterator c = Children.begin(); c != Children.end(); ++c)
        if (Child == *c)
          return c;

      return Children.end();
    } /* End of 'FindChild' function */

    VOID DeleteChild( entry *Child )
    {
      const std::vector<entry *>::iterator FoundChild = FindChild(Child);

      if (FoundChild == Children.end())
        return;
    
      Children.erase(FoundChild);
    } /* End of 'DeleteChild' function */

  }; /* End of 'entry' class */

} /* End of 'ui' namespace */



#endif // __entry_h_