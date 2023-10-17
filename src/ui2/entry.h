#ifndef __entry_hpp_
#define __entry_hpp_

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

    entry *Parent;                 // Parent pointer
    std::vector<entry *> Children; // Children pointers
    ivec2
      LocalPos,                 // Position in parent basis
      GlobalPos;                   // Position in canvas basis
    isize2 Size;                    // WH
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

    virtual VOID OnDraw( VOID )
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

    virtual VOID OnResize( const ivec2 &NewPos, const isize2 &NewSize )
    {
    } /* End of 'OnResize' function */

  
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

    VOID Draw( VOID )
    {
      if (!IsVisible)
        return;

      OnDraw();
      for (entry *c : Children)
        c->Draw();
    } /* End of 'draw' function */

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



#endif // __entry_hpp_