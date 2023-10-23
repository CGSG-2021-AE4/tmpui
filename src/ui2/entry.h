#include "ui_def.h"

#ifndef __entry_h_
#define __entry_h_


// Some real kal
using namespace tmp;

namespace ui
{
  /* Friend classes prototypes */
  class canvas;

  /* Physical entry state */
  enum struct entry_state
  {
    def     = 0,
    hovered = 1,
    focused = 2,
    active  = 3,
  }; /* End of 'entry_state' enum struct */
  
  /* UI entry class */
  class entry
  {
    /* Values */
  protected:

    std::string Id {""};                  // Entries id

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

    /* Set visibility function */
    VOID SetVisibility( BOOL NewValue )
    {
      IsVisible = NewValue;
    } /* End of 'SetVisibility' function */

    /* Is point over entry function */
    BOOL IsOver( const ivec2 &GlobalPoint )
    {
      return SelfMask(GlobalPoint);
    } /* End of 'IsOver' function */

    /******* Virtual event functions for overriding by user *******/

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

    virtual VOID OnResize( VOID )
    {
    } /* End of 'OnResize' function */

    virtual VOID OnMove( VOID )
    {
    } /* End of 'OnResize' function */

    virtual mask GetSelfMask( VOID )
    {
      return {GlobalPos, Size};
    } /* End of 'GetSelfMask' function */

    virtual mask GetContentMask( VOID )
    {
      return {GlobalPos, Size};
    } /* End of 'GetContentMask' function */

    /******* Inline event functions (interlayer) *******/

    inline VOID OnHoverEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("{} OnHover event.", Id));
      State = entry_state::hovered;
      OnHover(LocalMousePos);
    } /* End of 'OnHoverEvent' function */

    inline VOID OnUnhoverEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("{} OnUnhover event.", Id));
      State = entry_state::def;
      OnUnhover(LocalMousePos);
    } /* End of 'OnUnhoverEvent' function */

    inline VOID OnClickEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("{} OnClick event.", Id));
      OnClick(LocalMousePos);
    } /* End of 'OnClickEvent' function */

    inline VOID OnMouseDownEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("{} OnMouseDown event.", Id));
      State = entry_state::active;
      OnMouseDown(LocalMousePos);
    } /* End of 'OnMouseDownEvent' function */

    inline VOID OnMouseUpEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("{} OnMouseUp event.", Id));
      State = entry_state::hovered;
      OnMouseUp(LocalMousePos);
    } /* End of 'OnMouseUpEvent' function */

    inline VOID OnMouseMoveEvent( const ivec2 &Delta, const ivec2 &LocalMousePos )
    {
      //Log(std::format("${} OnMouseMove event.", Id));
      OnMouseMove(Delta, LocalMousePos);
    } /* End of 'OnMouseMoveEvent' function */

    inline VOID OnDragEvent( const ivec2 &Delta, const ivec2 &LocalMousePos )
    {
      Log(std::format("{} OnDrag event.", Id));
      OnDrag(Delta, LocalMousePos);
    } /* End of 'OnDragEvent' function */

    inline VOID OnChangeEvent( VOID )
    {
      Log(std::format("{} OnChange event.", Id));
      OnChange();
    } /* End of 'OnChangeEvent' function */

    inline VOID OnResizeEvent( VOID )
    {
      Log(std::format("{} OnResize event.", Id));
      OnResize();
    } /* End of 'OnResizeEvent' function */

    inline VOID OnMoveEvent( VOID )
    {
      Log(std::format("{} OnMove event.", Id));
      OnMove();
    } /* End of 'OnResizeEvent' function */

  protected:

    friend class canvas;

    /* Entry constructor function.
     * ARGUMENTS:
     *   - pos:
     *       const ivec2 &NewPos;
     *   - size:
     *       const isize2 &NewSize;
     *   - children:
     *       const std::vector<entry *> &NewChildren;
     *   - parent:
     *       entry *NewParent;
     */
    entry( const ivec2 &NewPos, const isize2 &NewSize, const std::vector<entry *> &NewChildren = {}, entry *NewParent = nullptr ) :
      Parent(nullptr),
      LocalPos(NewPos),
      Size(NewSize),
      IsVisible(true)
    {
      SetParent(NewParent);

      UpdateGlobalPos();

      AddChildren(NewChildren);
    } /* End of 'entry' function */

    /* Entry with id mentioned constructor function.
     * ARGUMENTS:
     *   - id string:
     *       const std::string &NewId;
     *   - pos:
     *       const ivec2 &NewPos;
     *   - size:
     *       const isize2 &NewSize;
     *   - children:
     *       const std::vector<entry *> &NewChildren;
     *   - parent:
     *       entry *NewParent;
     */
    entry( const std::string &NewId, const ivec2 &NewPos, const isize2 &NewSize, const std::vector<entry *> &NewChildren = {}, entry *NewParent = nullptr ) :
      Id(NewId),
      Parent(nullptr),
      LocalPos(NewPos),
      Size(NewSize),
      IsVisible(true)
    {
      SetParent(NewParent);

      UpdateGlobalPos();

      AddChildren(NewChildren);
    } /* End of 'entry' function */

    /* Entry desctrictor function */
    ~entry( VOID )
    {
      // Rebind children to parent
      for (entry *c : Children)
        c->SetParent(Parent);
      // Delete self from parent
      if (Parent != nullptr)
      Parent->DeleteChild(this);
    } /* End of '~entry' function */

    /* Update self and content masks function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
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

    /* Update entry global pos function */
    VOID UpdateGlobalPos( VOID );

    /* Resize function.
     * ARGUMENTS:
     *   - new size:
     *       const isize2 &NewSize;
     * RETURNS: None.
     */
    VOID Resize( const isize2 &NewSize )
    {
      Size = NewSize;

      UpdateMasks();

      OnResize();
    } /* End of 'Resize' function */

    /* Move function.
     * ARGUMENTS:
     *   - new local pos:
     *       const ivec2 &NewLocalPos;
     * RETURNS: None.
     */
    VOID Move( const ivec2 &NewLocalPos )
    {
      LocalPos = NewLocalPos;

      UpdateGlobalPos();

      OnMove();
    } /* End of 'Resize' function */

    /* Combined move and resize function.
     * ARGUMENTS:
     *   - new local pos:
     *       const ivec2 &NewLocalPos;
     *   - new size:
     *       const isize2 &NewSize;
     * RETURNS: None.
     */
    VOID Reform( const ivec2 &NewLocalPos, const isize2 &NewSize )
    {
      LocalPos = NewLocalPos;
      Size = NewSize;

      UpdateGlobalPos();

      OnMove();
      OnResize();
    } /* End of 'Reform' function */

    /* Draw entry's children function */
    VOID DrawChildren( VOID )
    {
      for (entry *c : Children)
        c->Draw();
    } /* End of 'DrawChildren' function */

    /* Draw entry function */
    VOID Draw( VOID )
    {
      if (!IsVisible)
        return;

      OnDraw();
      DrawChildren();
    } /* End of 'draw' function */

    /************ Parents/Children operations functions ************/

    /* Set entry's parent function.
     * ARGUMENTS:
     *   - new parent:
     *       entry *NewParent;
     * RETURNS: None.
     */
    VOID SetParent( entry *NewParent )
    {
      // Rebind to parent
      if (Parent != nullptr)
        Parent->DeleteChild(this); // Delete self from parent's children

      Parent = NewParent;

      if (Parent != nullptr)
        Parent->AddChild(this); // Add self to new parent's children
      
    } /* End of 'SetParent' function */

    /* On add entry as a child to one parent function.
     * ARGUMENTS:
     *   - new entry's parent:
     *       entry *NewParent;
     * RETURNS: None.
     */
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

    /* Set canvas function.
     * ARGUMENTS:
     *   - new canvas:
     *       canvas *NewCanvas;
     * RETURNS: None.
     */
    VOID SetCanvas( canvas *NewCanvas )
    {
      Canvas = NewCanvas;
      for (entry *Child : Children)
        Child->SetCanvas(Canvas);
    } /* End of 'SetParent' function */

    // Later there must be funtions to instert children into specific positions

    /* Add child function.
     * ARGUMENTS:
     *   - adding child:
     *       entry *NewChild;
     * RETURNS: None.
     */
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

    /* Add children function.
     * ARGUMENTS:
     *   - adding children:
     *       const std::vector<entry *> &NewChildren;
     * RETURNS: None.
     */
    VOID AddChildren( const std::vector<entry *> &NewChildren )
    {
      for (entry *Child : NewChildren)
        AddChild(Child);
    } /* End of 'AddChildren' function */

    /* Find a child function.
     * ARGUMENTS:
     *   - child's pointer:
     *       const entry *Child;
     * RETURNS:
     *   (std::vector<entry *>::iterator) - iterator to the child or end().
     */
    std::vector<entry *>::iterator FindChild( const entry *Child )
    {
      for (std::vector<entry *>::iterator c = Children.begin(); c != Children.end(); ++c)
        if (Child == *c)
          return c;

      return Children.end();
    } /* End of 'FindChild' function */

    /* Delete child function.
     * ARGUMENTS:
     *   - child's pointer:
     *       const entry *Child;
     * RETURNS: None.
     */
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