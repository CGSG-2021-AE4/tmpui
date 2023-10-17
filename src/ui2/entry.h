#ifndef __entry_h_
#define __entry_h_

#include "ui_def.h"

// Some real kal
using namespace tmp;

namespace ui
{
  /* Friend classes prototypes */
  class canvas;

  /* UI entry class */
  class entry
  {
    /* Values */
  private:

    ivec2
      LocalPos,                    // Position in parent basis
      GlobalPos;                   // Position in canvas basis
    isize2 Size;                   // WH

    mask
      Mask,                        // Source mask
      DrawMask;                    // Mask that is visible on the screen
    
    entry *Parent;                 // Parent pointer
    std::vector<entry *> Children; // Children pointers

    BOOL IsVisible;                // Is entry visible

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

    virtual VOID OnDraw( renderer &Rnd )
    {
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

    virtual mask GetMask( VOID )
    {
      return {GlobalPos, Size};
    } /* End of 'GetMask' function */

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
      AddChildren(NewChildren);

      UpdateGlobalPos();
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

    VOID UpdateDrawMask( const mask &ParentMask )
    {
      DrawMask = Mask.Intersect(ParentMask); // May be later add check for mask change
    } /* End of 'UpdateDrawMask' function */

    VOID UpdateGlobalPos( VOID )
    {
      // Don't know is it right
      if (Parent != nullptr)
        GlobalPos = Parent->GlobalPos + LocalPos;
      else
        GlobalPos = LocalPos;
    } /* End of 'UpdateGlobalPos' function */

    /* Only resize function */
    VOID Resize( const isize2 &NewSize, const mask &ParentMask )
    {
      Size = NewSize;

      UpdateDrawMask(ParentMask);

      OnResize();
    } /* End of 'Resize' function */

    /* Only move function */
    VOID Move( const ivec2 &NewLocalPos, const mask &ParentMask )
    {
      LocalPos = NewLocalPos;

      UpdateGlobalPos();
      UpdateDrawMask(ParentMask);

      OnMove();
    } /* End of 'Resize' function */

    /* Combined move and resize function */
    VOID Reform( const ivec2 &NewLocalPos, const isize2 &NewSize, const mask &ParentMask )
    {
      LocalPos = NewLocalPos;
      Size = NewSize;

      UpdateGlobalPos();
      UpdateDrawMask(ParentMask);

      OnMove();
      OnResize();
    } /* End of 'Reform' function */

    /* Draw function. */
    VOID Draw( renderer &Rnd )
    {
      if (!IsVisible)
        return;

      OnDraw(Rnd);
      for (entry *c : Children)
        c->Draw(Rnd);
    } /* End of 'draw' function */

    /************ Parents/Children functions ************/

    VOID SetParent( entry *NewParent )
    {
      // Rebind to parent
      if (Parent != nullptr)
        Parent->DeleteChild(this); // Delete self from parent's children
      if (NewParent != nullptr)
        NewParent->AddChild(this); // Add self to new parent's children
      Parent = NewParent;
    
      // Recalculate global pos
      if (Parent != nullptr)
        GlobalPos = Parent->GlobalPos + LocalPos;
    } /* End of 'SetParent' function */

    // Later there must be funtions to instert children into specific positions

    VOID AddChild( entry *NewChild )
    {
      for (entry *Child : Children)
        if (NewChild == Child)
          return; // We already have this child
    
      Children.push_back(NewChild);
    } /* End of 'AddChild' function */

    VOID AddChildren( const std::vector<entry *> &NewChildren )
    {
      for (entry *Child : Children)
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