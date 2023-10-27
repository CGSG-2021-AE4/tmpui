#include <algorithm>

#include "ui_def.h"

#ifndef __entry_h_
#define __entry_h_

//#define ENABLE_PATH_LOG

// Some real kal
using namespace tmp;

namespace ui
{
  /* Friend classes prototypes */
  class canvas;

  /* Layout type enum struct */
  enum struct layout_type
  {
    eBlock,
    eFlexRow,
    eFlexColumn,
  }; /* End of 'layout_type' enum struct */

  /* Layout props struct */
  struct layout_props
  {
    layout_type Type = layout_type::eBlock; // Type of entry layout
    FLT Flex         = 0;                   // Flex coef
    isize2 MinSize   = {0, 0};              // Min size
    isize2 MaxSize   = {-1, -1};            // Max size
    BOOL IsScrollable = 0;                  // Can the entry be scrolled
  }; /* End of 'layout_props' struct */

  /* Box props */
  struct box_props
  {
    INT
      MarginW = 0,  // Margin width
      BorderW = 0,  // Border width
      PaddingW = 0; // Padding width
  }; /* End of 'box_props' struct */

  /* Entry props structure */
  struct entry_props
  {
    std::string Id {""};
    ivec2 Pos {0};
    isize2 Size {0};
    layout_props LayoutProps {};
    box_props BoxProps {};
  }; /* End of 'entry_props' struct */

  /* Physical entry state */
  enum struct entry_state
  {
    eDef     = 0,
    eHovered = 1,
    eFocused = 2,
    eActive  = 3,
  }; /* End of 'entry_state' enum struct */

  /* UI entry class */
  class entry
  {
    /* Values */
  protected:

    // Info

    std::string Id {""};                   // Entries id

    // Position

    ivec2
      LocalPos {0},                        // Position in parent basis
      GlobalPos {0},                       // Position in canvas basis
      GlobalContentPos {0},                // Position in canvas basis
      ContentOffset {0};                   // Content offset

    // Sizes

    isize2
      Size {0},                            // Size
      BorderSize {0},                      // Border size
      ContentSize {0},                     // Content size
      CompContentSize {0},                 // Computed content size
      MarginSize {0};                      // Margin size

    // Masks

    mask
      SelfMask,                            // Self mask
      ContentMask,                         // Content mask
      SelfDrawMask,                        // Mask that is visible on the screen
      ContentDrawMask;                     // Mask for content that is visible on the screen
    
    // Neighbours pointers

    entry *Parent {nullptr};               // Parent pointer
    canvas *Canvas {nullptr};              // Canvas pointer
    std::vector<entry *> Children;         // Children pointers

    // Dynamic state

    entry_state
      State{ entry_state::eDef };          // State of entry

    // Style stuff

    BOOL IsVisible;                        // Is entry visible

    layout_props LayoutProps {};           // Props of entry's layout
    box_props BoxProps {};                 // Props of entry's box
    FLT ChildrenFlexSum = 0;               // Sum of children's flex values (is used only with FlexRow/FlexColumn flex type)
    isize2 ChildrenFlex0Size = 0;          // Sum of min sizes of children with flex 0
    BOOL IsScrollable = 0;                 // Can the entry be scrolled

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

    virtual BOOL OnClick( const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnClick' function */

    virtual VOID OnMouseDown( const ivec2 &LocalMousePos )
    {
    } /* End of 'OnMouseDown' function */

    virtual VOID OnMouseUp( const ivec2 &LocalMousePos )
    {
    } /* End of 'OnMouseUp' function */

    virtual BOOL OnMouseMove( const ivec3 &Delta, const ivec2 &LocalMousePos )
    {
      return false;
    } /* End of 'OnMouseMove' function */

    virtual BOOL OnDrag( const ivec3 &Delta, const ivec2 &LocalMousePos )
    {
      return false;
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

    virtual mask GetSelfMask( const ivec2 &GlobalPos, const isize2 &Size )
    {
      return {GlobalPos, Size};
    } /* End of 'GetSelfMask' function */

    virtual mask GetContentMask( const ivec2 &GlobalContentPos, const isize2 &ContentSize )
    {
      return {GlobalContentPos, ContentSize};
    } /* End of 'GetContentMask' function */

    /******* Inline event functions (interlayer) *******/

    inline VOID OnHoverEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("Entry {} OnHover event.", Id));
      State = entry_state::eHovered;
      OnHover(LocalMousePos);
    } /* End of 'OnHoverEvent' function */

    inline VOID OnUnhoverEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("Entry {} OnUnhover event.", Id));
      State = entry_state::eDef;
      OnUnhover(LocalMousePos);
    } /* End of 'OnUnhoverEvent' function */

    inline BOOL OnClickEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("Entry {} OnClick event.", Id));
      return OnClick(LocalMousePos) ? true : Parent != nullptr ? Parent->OnClickEvent(LocalMousePos + LocalPos) : false;
    } /* End of 'OnClickEvent' function */

    inline VOID OnMouseDownEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("Entry {} OnMouseDown event.", Id));
      State = entry_state::eActive;
      OnMouseDown(LocalMousePos);
    } /* End of 'OnMouseDownEvent' function */

    inline VOID OnMouseUpEvent( const ivec2 &LocalMousePos )
    {
      Log(std::format("Entry {} OnMouseUp event.", Id));
      State = entry_state::eHovered;
      OnMouseUp(LocalMousePos);
    } /* End of 'OnMouseUpEvent' function */

    inline BOOL OnMouseMoveEvent( const ivec3 &Delta, const ivec2 &LocalMousePos )
    {
      //Log(std::format("${} OnMouseMove event.", Id));
      if (LayoutProps.IsScrollable && Delta.Z != 0) // May be later I'l add a check with the content mask
      {
        // Scroll response
        ivec2 DeltaMove = {0, Delta.Z}; // Some temp shit

        // Clamping
        ContentOffset = {std::min(std::max(ContentOffset.X + DeltaMove.X + CompContentSize.W, ContentSize.W) - CompContentSize.W, 0),
                         std::min(std::max(ContentOffset.Y + DeltaMove.Y + CompContentSize.H, ContentSize.H) - CompContentSize.H, 0)};
        for (auto *c : Children)
        {
          c->UpdateGlobalPosRec();
          c->UpdateMasksRec();
        }
        Draw(); // DOTO I think this is shit and  I will remove it later
        return true;
      }

      return OnMouseMove(Delta, LocalMousePos) ? true : Parent != nullptr ? Parent->OnMouseMoveEvent(Delta, LocalMousePos + LocalPos) : false;
    } /* End of 'OnMouseMoveEvent' function */

    inline BOOL OnDragEvent( const ivec3 &Delta, const ivec2 &LocalMousePos )
    {
      Log(std::format("Entry {} OnDrag event.", Id));
      return OnDrag(Delta, LocalMousePos) ? true : Parent != nullptr ? Parent->OnDragEvent(Delta, LocalMousePos + LocalPos) : false;
    } /* End of 'OnDragEvent' function */

    inline VOID OnChangeEvent( VOID )
    {
      Log(std::format("Entry {} OnChange event.", Id));
      OnChange();
    } /* End of 'OnChangeEvent' function */

    inline VOID OnResizeEvent( VOID )
    {
      Log(std::format("Entry {} OnResize event.", Id));
      OnResize();
    } /* End of 'OnResizeEvent' function */

    inline VOID OnMoveEvent( VOID )
    {
      Log(std::format("Entry {} OnMove event.", Id));
      OnMove();
    } /* End of 'OnResizeEvent' function */

  protected:
  public: // !!! TMP DEBUG SHIT

    friend class canvas;

    /* Entry with id mentioned constructor function.
     * ARGUMENTS:
     *   - id string:
     *       const std::string &NewId;
     *   - pos:
     *       const ivec2 &NewPos;
     *   - size:
     *       const isize2 &NewSize;
     *   - props:
     *       const props_type &Props;
     *   - children:
     *       const std::vector<entry *> &NewChildren;
     *   - parent:
     *       entry *NewParent;
     */
    template<typename props_type>
      entry( const props_type &Props, const std::vector<entry *> &NewChildren = {}, entry *NewParent = nullptr ) :
        Parent(NewParent),
        IsVisible(true)
      {
#ifdef ENABLE_PATH_LOG
        Log(std::format("Entry {} constructor.", Id));
#endif // ENABLE_PATH_LOG

        // Setup info from props

        if constexpr (requires { Props.Id; })
          Id = Props.Id;
        if constexpr (requires { Props.Pos; })
          SetPos(Props.Pos);
        if constexpr (requires { Props.Size; })
          SetSize(Props.Size);
        if constexpr (requires { Props.LayoutProps; })
          LayoutProps = Props.LayoutProps;
        if constexpr (requires { Props.BoxProps; })
          BoxProps = Props.BoxProps;

        SetParent(NewParent);
        AddChildren(NewChildren);
        UpdateShape();
      } /* End of 'entry' function */

    /* Entry desctrictor function */
    ~entry( VOID )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entry {} Destructor.", Id));
#endif // ENABLE_PATH_LOG

      // SOME REAL SHIT
      if (Parent == nullptr)
        for (entry *c : Children)
          delete c;

      // Rebind children to parent
      for (entry *c : Children)
        c->SetParent(Parent);
      // Delete self from parent
      Parent->DeleteChild(this);
    } /* End of '~entry' function */

  private:

    /* Update children layout function */
    VOID UpdateChildrenLayout( VOID )
    {
      if (LayoutProps.Type == layout_type::eFlexRow)
      {
        INT ContentFlexSizeW = ContentSize.W - ChildrenFlex0Size.W;

        CompContentSize = {};
        if (ChildrenFlexSum == 0)
        {
          INT Offset = 0;
          isize2 NewSize;


          for (entry *c : Children)
          {
            NewSize = Clamp({c->LayoutProps.MinSize.W, ContentSize.H}, c->LayoutProps.MinSize, c->LayoutProps.MaxSize);
            c->Reform({Offset, 0}, NewSize);
            Offset += NewSize.W;
            CompContentSize.W += NewSize.W;
            CompContentSize.H = std::max(CompContentSize.H, NewSize.H);
          }
        }
        else
        {
          INT Offset = 0;
          isize2 NewSize;

          for (entry *c : Children)
          {
            NewSize = Clamp({(INT)(c->LayoutProps.Flex / ChildrenFlexSum * ContentFlexSizeW), ContentSize.H}, c->LayoutProps.MinSize, c->LayoutProps.MaxSize);
            c->Reform({Offset, 0}, NewSize);
            Offset += NewSize.W;
            CompContentSize.W += NewSize.W;
            CompContentSize.H = std::max(CompContentSize.H, NewSize.H);
          }
        }
      }
      else if (LayoutProps.Type == layout_type::eFlexColumn)
      {
        CompContentSize = {};

        INT ContentFlexSizeH = ContentSize.H - ChildrenFlex0Size.H;
        if (ChildrenFlexSum == 0)
        {
          INT Offset = 0;
          isize2 NewSize;

          for (entry *c : Children)
          {
            NewSize = Clamp({ContentSize.W, c->LayoutProps.MinSize.H}, c->LayoutProps.MinSize, c->LayoutProps.MaxSize);
            c->Reform({0, Offset}, NewSize);
            Offset += NewSize.H;
            CompContentSize.H += NewSize.H;
            CompContentSize.W = std::max(CompContentSize.W, NewSize.W);
          }
        }
        else
        {
          INT Offset = 0;
          isize2 NewSize;


          for (entry *c : Children)
          {
            NewSize = Clamp({ContentSize.W, (INT)(c->LayoutProps.Flex / ChildrenFlexSum * ContentFlexSizeH)}, c->LayoutProps.MinSize, c->LayoutProps.MaxSize);
            c->Reform({0, Offset}, NewSize);
            Offset += NewSize.H;
            CompContentSize.H += NewSize.H;
            CompContentSize.W = std::max(CompContentSize.W, NewSize.W);
          }
        }
      }
      // if (LayoutProps.Type == layout_type::eBlock)
      // return;
    } /* End of 'UpdateChildrenLayout' function */

    /* Update self and content masks function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID UpdateMasks( VOID )
    {
      SelfMask = GetSelfMask(GlobalPos, Size);
      ContentMask = GetContentMask(GlobalPos + ivec2(BoxProps.BorderW), BorderSize);
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

    /* Update self and childrens masks function (recursive) */
    VOID UpdateMasksRec( VOID )
    {
      UpdateMasks();
      for (entry *c : Children)
        c->UpdateMasksRec();
    } /* End of 'UpdateMasksRec' function */

    /* Update entry global pos function */
    VOID UpdateGlobalPos( VOID );

    /* Update entry global pos recursive function (+ update children) */
    VOID UpdateGlobalPosRec( VOID )
    {
      UpdateGlobalPos();
      for (entry *c : Children)
        c->UpdateGlobalPosRec();
    } /* End of 'UpdateGlobalPos' function */

    /* Update shape function */
    VOID UpdateShape( VOID )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entry {} update shape.", Id));
#endif // ENABLE_PATH_LOG

      UpdateGlobalPos();
      UpdateMasks();
      UpdateChildrenLayout();
    } /* End of 'UpdateShape' function */

    /* Set position function */
    inline VOID SetPos( const ivec2 &NewLocalPos )
    {
      LocalPos = NewLocalPos + ivec2(BoxProps.MarginW);
    } /* End of 'SetPos' function */

    /* Set size function */
    inline VOID SetSize( const isize2 &NewMarginSize )
    {
      MarginSize = NewMarginSize;
      Size = MarginSize - isize2(BoxProps.MarginW) * 2;
      BorderSize = Size - isize2(BoxProps.BorderW) * 2;
      ContentSize = BorderSize - isize2(BoxProps.PaddingW) * 2;
    } /* End of 'SetSize' function */

  public:

    /* Resize function.
     * ARGUMENTS:
     *   - new size:
     *       const isize2 &NewSize;
     * RETURNS: None.
     */
    VOID Resize( const isize2 &NewSize )
    {
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entry {} Resize.", Id));
#endif // ENABLE_PATH_LOG

      // Update sizes
      SetSize(NewSize);
      
      UpdateShape();
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
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entry {} Move.", Id));
#endif // ENABLE_PATH_LOG
      SetPos(NewLocalPos);

      UpdateGlobalPosRec();
      UpdateMasksRec();
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
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entry {} Reform.", Id));
#endif // ENABLE_PATH_LOG
      SetPos(NewLocalPos);
      SetSize(NewSize);

      UpdateShape();
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
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entry {} Draw.", Id));
#endif // ENABLE_PATH_LOG
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
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entry {} SetParent.", Id));
#endif // ENABLE_PATH_LOG
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
#ifdef ENABLE_PATH_LOG
      Log(std::format("Entry {} OnAddChild.", Id));
#endif // ENABLE_PATH_LOG
      // Rebind to parent
      if (Parent != nullptr)
        Parent->DeleteChild(this); // Delete self from parent's children

      Parent = NewParent;

      if (Parent != nullptr)
        SetCanvas(Parent->Canvas);

      UpdateShape();
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

  private:

    /* Add child private function.
     * ARGUMENTS:
     *   - adding child:
     *       entry *NewChild;
     * RETURNS: None.
     */
    VOID AddChildUnsafe( entry *NewChild )
    {
      if (NewChild == nullptr)
        return;

      for (entry *Child : Children)
        if (NewChild == Child)
          return; // We already have this child
    
      Children.push_back(NewChild);
      NewChild->OnAddChild(this);
      ChildrenFlexSum += NewChild->LayoutProps.Flex;
      if (NewChild->LayoutProps.Flex == 0)
        ChildrenFlex0Size += NewChild->LayoutProps.MinSize;
    } /* End of 'AddChildUnsafe' function */

  public:

    /* Add child public function.
     * ARGUMENTS:
     *   - adding child:
     *       entry *NewChild;
     * RETURNS: None.
     */
    VOID AddChild( entry *NewChild )
    {
      AddChildUnsafe(NewChild);
      UpdateChildrenLayout();
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

      UpdateChildrenLayout();
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
    
      ChildrenFlexSum -= (*FoundChild)->LayoutProps.Flex;
      if ((*FoundChild)->LayoutProps.Flex == 0)
        ChildrenFlex0Size -= (*FoundChild)->LayoutProps.MinSize;
      Children.erase(FoundChild);
      UpdateChildrenLayout();
    } /* End of 'DeleteChild' function */

  }; /* End of 'entry' class */

} /* End of 'ui' namespace */

#endif // __entry_h_